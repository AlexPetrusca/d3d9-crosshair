#pragma once

#include <d3d9.h>

extern HWND hWindow; // window handle
extern LPDIRECT3D9 hD3D9Interface; // d3d9 interface
extern LPDIRECT3DDEVICE9 hD3D9DummyDevice; // dummy d3d9 device

LPDIRECT3DDEVICE9 createD3D9Device(HMODULE hModule);