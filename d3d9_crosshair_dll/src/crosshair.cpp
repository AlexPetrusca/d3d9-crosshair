#include "pch.h"
#include "crosshair.h"

#include <complex>

LPDIRECT3DDEVICE9 pD3DDevice; // real d3d9 device
D3DVIEWPORT9 pViewport; // real d3d9 viewport
bool bInit; // set true when real d3d9 device has been hijacked

int NUM_TYPES = 3; // number of types of crosshairs
int NUM_COLORS = 14; // number of colors for crosshair
int type = 0; // selects the type of crosshair
int color = 0; // selects the color of the crosshair
int size = 7; // size of the crosshair

D3DCOLOR GetColor() {
    D3DCOLOR d3dColor;
    if (color == 0) {
        d3dColor = D3DCOLOR_RGBA(0xFF, 0x00, 0x00, 0xFF); // red
    } else if (color == 1) {
        d3dColor = D3DCOLOR_RGBA(0x00, 0xFF, 0x00, 0xFF); // green
    } else if (color == 2) {
        d3dColor = D3DCOLOR_RGBA(0x00, 0x00, 0xFF, 0xFF); // blue
    } else if (color == 3) {
        d3dColor = D3DCOLOR_RGBA(0x00, 0xFF, 0xFF, 0xFF); // cyan
    } else if (color == 4) {
        d3dColor = D3DCOLOR_RGBA(0xFF, 0x00, 0xFF, 0xFF); // magenta
    } else if (color == 5) {
        d3dColor = D3DCOLOR_RGBA(0xFF, 0xFF, 0x00, 0xFF); // yellow
    } else if (color == 6) {
        d3dColor = D3DCOLOR_RGBA(0xFF, 0xFF, 0xFF, 0xFF); // white
    } else if (color == 7) {
        d3dColor = D3DCOLOR_RGBA(0x00, 0x00, 0x00, 0xFF); // black
    } else if (color == 8) {
        d3dColor = D3DCOLOR_RGBA(0x80, 0x00, 0x80, 0xFF); // purple
    } else if (color == 9) {
        d3dColor = D3DCOLOR_RGBA(0xFF, 0x45, 0x00, 0xFF); // orange
    } else if (color == 10) {
        d3dColor = D3DCOLOR_RGBA(0xFF, 0x14, 0x93, 0xFF); // hot pink
    } else if (color == 11) {
        d3dColor = D3DCOLOR_RGBA(0x1E, 0x90, 0xFF, 0xFF); // standard blue
    } else if (color == 12) {
        d3dColor = D3DCOLOR_RGBA(0xD2, 0xB4, 0x8C, 0xFF); // cream
    } else {
        d3dColor = D3DCOLOR_RGBA(0x77, 0x88, 0x99, 0xFF); // gray-blue
    }
    return d3dColor;
}

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

void DrawCrosshair() {
    int x = pViewport.Width / 2 - size / 2;
    int y = pViewport.Height / 2 - size / 2;
    D3DCOLOR d3dColor = GetColor();
    if (type == 0) {
        DrawFilledCircle(x, y, size, size, d3dColor);
    } else if (type == 1) {
        DrawFilledRect(x, y, size, size, d3dColor);
    } else {
        DrawFilledTriangle(x, y, size, size, d3dColor);
    }
}

void incrementType() {
    type = (type + 1) % NUM_TYPES;
}

void decrementType() {
    type = (type + NUM_TYPES - 1) % NUM_TYPES;
}

void incrementColor() {
    color = (color + 1) % NUM_COLORS;
}

void decrementColor() {
    color = (color + NUM_COLORS - 1) % NUM_COLORS;
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
    DrawCrosshair();
}
