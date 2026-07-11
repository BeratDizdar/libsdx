#pragma once

void* sdx_CreateID3D9();
void* sdx_CreateID3DDevice9(void* d3d9, void* hwnd);
void* sdx_CreateID3DVertexBuffer9(void* device, int size, void* data, unsigned int FVF, unsigned int managed);
void* sdx_CreateID3DIndexBuffer9(void* device, int size, void* data, unsigned int managed);
void* sdx_CreateID3DTexture9FromPath(void* device, const char* filepath);
void* sdx_CreateRenderTargetTexture(void* device, int width, int height);
void* sdx_CreateID3DVertexDeclaration9(void* device, const void* elements);
void  sdx_BeginStateBlock(void* device);
void  sdx_EndStateBlock(void* device, void** state_block);
void  sdx_Apply(void* state_block);
