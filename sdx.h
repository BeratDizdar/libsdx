#pragma once

void* sdx_CreateID3D9();
void* sdx_CreateID3DDevice9(void* d3d9, void* hwnd);
void* sdx_CreateID3DVertexBuffer9(void* device, int size, unsigned int FVF);    // dinamik -- static ayır ya da
void* sdx_CreateID3DIndexBuffer9(void* device, int size);                       // yoksa komple dinamik mi olsa