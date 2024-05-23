#include "pch.h"
#include "keylogger.h"

#include <iostream>

#include "crosshair.h"

HHOOK hKlgHook; // keylogger hook handle

LRESULT CALLBACK crosshairKeyHook(int nCode, WPARAM wParam, LPARAM lParam) {
    static bool shiftModifier;
    if (nCode == HC_ACTION) {
        PKBDLLHOOKSTRUCT p = (PKBDLLHOOKSTRUCT) lParam;
        if (wParam == WM_KEYDOWN || wParam == WM_SYSKEYDOWN) {
            // std::cout << ">>>>>>>> KEY PRESSED: " << code << std::endl;
            if (p->vkCode == VK_LSHIFT || p->vkCode == VK_RSHIFT) {
                shiftModifier = true;
            }
            if (shiftModifier) {
                if (p->vkCode == VK_OEM_PLUS) {
                    incrementType();
                } else if (p->vkCode == VK_OEM_MINUS) {
                    decrementType();
                }
            } else {
                if (p->vkCode == VK_OEM_PLUS) {
                    expandCrosshair();
                } else if (p->vkCode == VK_OEM_MINUS) { 
                    shrinkCrosshair();
                }
            }
        } else if (wParam == WM_KEYUP || wParam == WM_SYSKEYUP) {
            if (p->vkCode == VK_LSHIFT || p->vkCode == VK_RSHIFT) {
                shiftModifier = false;
            }
        }
    }	
    return CallNextHookEx(nullptr, nCode, wParam, lParam);
}

void keyloggerMain(HMODULE hModule) {
    hKlgHook = SetWindowsHookEx(WH_KEYBOARD_LL, crosshairKeyHook, hModule, 0);
    MSG msg;
    while (!GetMessage(&msg, nullptr, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
}

void attachKeylogger(HMODULE hModule) {
    CloseHandle(CreateThread(nullptr, 0, (LPTHREAD_START_ROUTINE)keyloggerMain, hModule, 0, nullptr));
}
