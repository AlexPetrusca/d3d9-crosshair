#pragma once

#include <d3d9.h>

extern void* d3d9VTable[119]; // d3d9 virtual table

void** hookD3D9Device(LPDIRECT3DDEVICE9 hD3D9DummyDevice);