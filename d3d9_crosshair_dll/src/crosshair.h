#pragma once

#include "d3d9.h"

void incrementType();
void decrementType();

void incrementColor();
void decrementColor();

void expandCrosshair();
void shrinkCrosshair();

void renderCrosshair(LPDIRECT3DDEVICE9 pDevice);