// d3d9_crosshair.cpp: Executable that injects d3d9_crosshair.dll into all applications spawned after it.
//
// It operates by registering a windows hook which injects d3d9_crosshair.dll into an application based
// on the WH_CBT policy. The system calls a WH_CBT hook procedure before activating, creating, destroying,
// minimizing, maximizing, moving, or sizing a window.

#define WIN32_LEAN_AND_MEAN 
#include <windows.h> 
#include <iostream>
#include <cstdlib> 
#include "crosshair_dll.h" 

HHOOK winHookHandle;

void pauseExit() {
    std::cout << std::endl; 
    system("pause");
    exit(EXIT_SUCCESS);
}

void attachHook() {
    HINSTANCE crosshairDllHandle = GetModuleHandleA("d3d9_crosshair.dll");
    winHookHandle = SetWindowsHookExA(WH_CBT, &crosshairWinHook, crosshairDllHandle, 0);
}

void onExit() {
    UnhookWindowsHookEx(winHookHandle);
} 
  
int main() {
    std::atexit(onExit);
    attachHook();
    pauseExit();
}
 