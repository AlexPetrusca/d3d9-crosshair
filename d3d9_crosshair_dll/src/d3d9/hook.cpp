#include "pch.h"
#include "hook.h"
#include "../crosshair.h"

typedef HRESULT (APIENTRY*tEndScene)(LPDIRECT3DDEVICE9 pDevice);

void* d3d9VTable[119]; // d3d9 virtual table
tEndScene oEndScene = nullptr; // real d3d9 endScene function

HRESULT APIENTRY hkEndScene(LPDIRECT3DDEVICE9 pDevice) {
    renderCrosshair(pDevice);
    return oEndScene(pDevice);
}

bool Hook(char* src, char* dst, int len) {
    if (len < 5) return false;

    // change protection level of bytes we are going to overwrite in `src` (the function we are hooking)
    DWORD curProtection;
    VirtualProtect(src, len, PAGE_EXECUTE_READWRITE, &curProtection);

    // overwrite first `len` bytes in `src` with NOPs
    memset(src, 0x90, len);
    
    // Add JMP to `dst` address at the start of `src`
    // 0xE9 is "JMP rel32 - Jump near, relative, displacement relative to next instruction"
    *src = (char) 0xE9;
    *(uintptr_t*)(src + 1) = (uintptr_t) (dst - src - 5);

    // revert protection level of bytes we  overwrote
    DWORD temp;
    VirtualProtect(src, len, curProtection, &temp);

    return true;
}

char* TrampHook(char* src, char* dst, unsigned int len) {
    if (len < 5) return 0;

    // Create the gateway (`len` accounts for the overwritten bytes & `5` accounts for the JMP)
    char* gateway = (char*) VirtualAlloc(0, len + 5, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);

    // Put the bytes that will be overwritten in the gateway
    memcpy(gateway, src, len);
    
    // Add JMP to `src` address at the end of the gateway
    // 0xE9 is "JMP rel32 - Jump near, relative, displacement relative to next instruction"
    *(gateway + len) = (char) 0xE9; // 1 byte opcode
    *(uintptr_t*)(gateway + len + 1) = (uintptr_t)(src - gateway - 5); // 4 byte relative offset

    // Place the hook at the destination
    if (Hook(src, dst, len)) {
        return gateway;
    }
    return nullptr;
}

void** hookD3D9Device(LPDIRECT3DDEVICE9 hD3D9DummyDevice) {
    memcpy(d3d9VTable, *reinterpret_cast<void***>(hD3D9DummyDevice), sizeof(d3d9VTable));
    oEndScene = (tEndScene) TrampHook((char*)d3d9VTable[42], (char*)hkEndScene, 7);
    return d3d9VTable;
}
