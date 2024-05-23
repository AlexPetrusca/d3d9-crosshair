// crosshair_dll.cpp: DLL that loads a crosshair overlay into a D3D9 application.
//
// Once injected into an application, it operates as follows:
//  - Create a "dummy" d3d9 device. This requires creating a new window and d3d9 interface object.
//  - Extract the vtable of the "dummy" d3d9 device.
//      - Now that we have the vtable, we have pointers to all the methods on the d3d9 device class.
//      - These methods are shared across all object instances of the d3d9 device class.
//      - This means we can intercept method calls from the real d3d9 device object.
//  - Hook the EndScene method on the d3d9 device vtable.
//      - This involves overriding the first few opcodes of the method with a jump to our code.
//      - We want to keep the old method callable, so we have to do a bit more work.
//          - We save the opcodes to be overriden in an executable memory buffer.
//          - At the end of this buffer, we add a jump that redirects to a point after the stolen opcodes.
//          - Now we can execute this memory buffer; it will run the stolen opcodes and then jump to run
//            rest of the original method's opcodes. Ta-da! It's like we never touched a thing!
//      - Now that we've hooked EndScene, any call to EndScene from the real d3d9 device will jump to
//        our designated callback function first.
//  - Hijack the real d3d9 device.
//      - The d3d9 device is the first parameter of every method, so we can simply "steal" the pointer
//        to the real d3d9 device in our EndScene callback.
//  - Render the crosshair overlay in our EndScene callback.
//      - Using the hijacked d3d9 device, we can finally render anything we want on top of the rendered screen.
//      - We make sure to call the old EndScene method at the end to keep everything running as normal.

#include "pch.h"
#include "crosshair_dll.h"
#include "src/d3d9/dummy.h"
#include "src/d3d9/hook.h"
#include "src/keylogger.h"

#include <d3d9.h>
#include <iostream>

void printLog(const char* message) {
    std::cout << "[" << "\033[31m" << "D3D9_CROSSHAIR" << "\033[0m" << "]: " << message << "\n";
}

BOOL existsD3D9() {
    char exePath[MAX_PATH];
    GetModuleFileNameA(nullptr, exePath, MAX_PATH);
    char* exeName = strrchr(exePath, '\\') + 1;
    *strrchr(exeName, '.') = '\0';
    return strcmp(exeName, "d3d9_crosshair") != 0 && GetModuleHandleA("d3d9.dll") != nullptr;
}

DWORD WINAPI init(HMODULE hModule) {
    printLog("Attaching...");
    if (existsD3D9()) {
        printLog("Setting Keylogger...");
        attachKeylogger(hModule);
        printLog("Creating Dummy D3D9Device...");
        createD3D9Device(hModule);
        printLog("Hijacking D3D9Device VTable...");
        hookD3D9Device(hD3D9DummyDevice);
        printLog("Successfully Hooked D3D9 Device!");
    }
    return 0;
}

void onInit(HMODULE hModule) {
    CloseHandle(CreateThread(nullptr, 0, (LPTHREAD_START_ROUTINE)init, hModule, 0, nullptr));
}

void onDestroy() {
    if (hD3D9Interface) {
        hD3D9Interface->Release();
        hD3D9Interface = nullptr;
        printLog("Destroyed D3D9Interface!");
    }
    if (hWindow) {
        DestroyWindow(hWindow);
        hWindow = nullptr;
        printLog("Destroyed Window!");
    }
    if (hKlgHook) {
        UnhookWindowsHookEx(hKlgHook);
        hKlgHook = nullptr;
        printLog("Detached Keylogger!");
    }
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved) {
    switch (ul_reason_for_call) {
    case DLL_PROCESS_ATTACH:
        onInit(hModule);
        break;
    case DLL_PROCESS_DETACH:
        onDestroy();
        break;
    default:
        break;
    }
    return TRUE;
}

LRESULT CALLBACK crosshairWinHook(int nCode, WPARAM wParam, LPARAM lParam) {
    if (nCode == HCBT_ACTIVATE || nCode == HCBT_SETFOCUS) {
        if (existsD3D9()) {
            MessageBeep(0xFFFFFFFF);
        }
    }
    return CallNextHookEx(nullptr, nCode, wParam, lParam);
}