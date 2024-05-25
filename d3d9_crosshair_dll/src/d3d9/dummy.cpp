#include "pch.h"
#include "dummy.h"

HWND hWindow; // window handle
LPDIRECT3D9 hD3D9Interface; // d3d9 interface
LPDIRECT3DDEVICE9 hD3D9DummyDevice; // dummy d3d9 device

LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

HWND createWindow(HMODULE hModule) {
    // Register window class
    const wchar_t CLASS_NAME[] = TEXT("WindowClassAP");
    WNDCLASSEX wc;
    ZeroMemory(&wc, sizeof(WNDCLASSEX));
    wc.cbSize = sizeof(WNDCLASSEX);
    wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hModule;
    wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
    wc.lpszClassName = CLASS_NAME;
    RegisterClassEx(&wc);

    // Create the window
    hWindow = CreateWindowEx(
        0, // Optional window styles
        CLASS_NAME, // Window class
        TEXT("WindowAP"), // Window text
        WS_OVERLAPPEDWINDOW, // Window style
        0, 0, 800, 600, // Position and Size
        nullptr, // Parent window    
        nullptr, // Menu
        hModule, // Instance handle
        nullptr // Additional application data
    );

    if (hWindow == nullptr) {
        MessageBox(nullptr, TEXT("hWnd creation failed"), TEXT("hWnd creation failed"), MB_OK);
        return nullptr;
    }
    ShowWindow(hWindow, SW_HIDE);
    return hWindow;
}

LPDIRECT3DDEVICE9 createD3D9Device(HMODULE hModule) {
    HWND hWnd = createWindow(hModule);
    
    hD3D9Interface = Direct3DCreate9(D3D_SDK_VERSION);
    if (hD3D9Interface == nullptr) {
        MessageBox(nullptr, TEXT("hD3D9Interface creation failed"), TEXT("hD3D9Interface creation failed"), MB_OK);
        return nullptr;
    }

    D3DPRESENT_PARAMETERS d3dpp;
    ZeroMemory(&d3dpp, sizeof(d3dpp));
    d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
    d3dpp.hDeviceWindow = hWnd;
    d3dpp.Windowed = true;

    HRESULT hr = hD3D9Interface->CreateDevice(
        D3DADAPTER_DEFAULT, // Adapter
        D3DDEVTYPE_REF, // HAL
        hWnd, // Focus Window
        D3DCREATE_SOFTWARE_VERTEXPROCESSING, // Behavior Flag
        &d3dpp, // Presentation Parameters
        &hD3D9DummyDevice // Device Object Pointer Address
    );
    if (FAILED(hr)) {
        MessageBox(nullptr, TEXT("D3DDevice creation failed"), TEXT("D3DDevice creation failed"), MB_OK);
    }
    return hD3D9DummyDevice;
}