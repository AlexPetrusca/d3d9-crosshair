#include "pch.h"
#include "crosshair.h"

#include <complex>

LPDIRECT3DDEVICE9 pD3DDevice; // real d3d9 device
D3DVIEWPORT9 pViewport; // real d3d9 viewport
bool bInit; // set true when real d3d9 device has been hijacked

int NUM_TYPES = 3; // count of types of crosshairs
int type = 0; // selects the type of crosshair
int size = 6; // size of the crosshair

void DrawFilledRect(int x, int y, int w, int h, D3DCOLOR color) {
    D3DRECT BarRect = {x, y, x + w, y + h};
    pD3DDevice->Clear(1, &BarRect, D3DCLEAR_TARGET, color, 0, 0);
}

void DrawFilledTriangle(int x, int y, int w, int h, D3DCOLOR color) {
    D3DRECT* BarRects = new D3DRECT[h];
    for (int i = 0; i < h; i++) {
        int wH = i;
        int xH = x + w / 2 - wH / 2;
        int yH = y + i;
        BarRects[i] = {xH, yH, xH + wH, yH + 1};
    }
    pD3DDevice->Clear(h, BarRects, D3DCLEAR_TARGET, color, 0, 0);
    delete BarRects;
}

void DrawFilledCircle(int x, int y, int w, int h, D3DCOLOR color) {
    D3DRECT* BarRects = new D3DRECT[h];
    for (int i = 0; i < h; i++) {
        float delta = std::abs((float)i - (float)h / 2);
        float wH = 2 * std::sqrt((float)(w * w) / 4 - delta * delta);
        float xH = (float)x + (float)w / 2 - wH / 2;
        float yH = (float)y + i;
        BarRects[i] = {(int)xH, (int)yH, (int)(xH + wH), (int)yH + 1};
    }
    pD3DDevice->Clear(h, BarRects, D3DCLEAR_TARGET, color, 0, 0);
    delete BarRects;
}

void drawCrosshair() {
    if (type == 0) {
        DrawFilledCircle(
            pViewport.Width / 2 - size / 2,
            pViewport.Height / 2 - size / 2,
            size,
            size,
            D3DCOLOR_RGBA(255, 0, 0, 255)
        );
    } else if (type == 1) {
        DrawFilledRect(
            pViewport.Width / 2 - size / 2,
            pViewport.Height / 2 - size / 2,
            size,
            size,
            D3DCOLOR_RGBA(255, 0, 0, 255)
        );
    } else {
        DrawFilledTriangle(
            pViewport.Width / 2 - size / 2,
            pViewport.Height / 2 - size / 2,
            size,
            size,
            D3DCOLOR_RGBA(255, 0, 0, 255)
        );
    }
}

void incrementType() {
    type = (type + 1) % NUM_TYPES;
}

void decrementType() {
    type = (type + NUM_TYPES - 1) % NUM_TYPES;
}

void expandCrosshair() {
    size++;
}

void shrinkCrosshair() {
    if (size > 0) {
        size--;
    }
}

void renderCrosshair(LPDIRECT3DDEVICE9 pDevice) {
    if (!bInit) {
        pD3DDevice = pDevice;
        pD3DDevice->GetViewport(&pViewport);
        bInit = true;
    }
    drawCrosshair();
}
