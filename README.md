```c
// shader.hlsl
// fxc.exe /T vs_2_0 /E VS /Fh vs.h shader.hlsl
// fxc.exe /T ps_2_0 /E PS /Fh ps.h shader.hlsl
float4x4 orthoMat : register(c0);
sampler2D tex0 : register(s0);

struct VOut {
    float4 pos : POSITION;
    float2 uv : TEXCOORD0;
};

VOut VS(float2 pos : POSITION, float2 uv : TEXCOORD0) {
    VOut Out;
    Out.pos = mul(float4(pos, 0.0f, 1.0f), orthoMat);
    Out.uv = uv;
    return Out;
}

float4 PS(VOut In) : COLOR {
    return tex2D(tex0, In.uv);
}

// ##################################################################

// sdx.h
#pragma once

enum { // vertex declaration layout
    SDX_LAYOUT_END = 0,
    SDX_LAYOUT_POS2,
    SDX_LAYOUT_POS3,
    SDX_LAYOUT_COLOR4,
    SDX_LAYOUT_NORM3,
    SDX_LAYOUT_UV2,
};

void sdx_CreateContext(void** ctx);
void sdx_CreateDevice(void* ctx, void* hwnd, void** device);
void sdx_CreateVertexBuffer(void* device, int size, void* data, unsigned int FVF, unsigned int managed, void** vb);
void sdx_CreateIndexBuffer(void* device, int size, void* data, unsigned int managed, void** ib);
void sdx_CreateTextureFromPath(void* device, const char* filepath, void** tex);
void sdx_CreateRenderTargetTexture(void* device, int width, int height, void** tex);
void sdx_CreateVertexDeclaration(void* device, const int* layout, void** decl);
void sdx_CreateVertexShader(void* device, unsigned long* data, void** shader);
void sdx_CreatePixelShader(void* device, unsigned long* data, void** shader);

void sdx_ReleaseArgs(void* obj, ...); // put NULL at ending

// do not use with state block
void sdx_SetOrtho(void* device, int shader_register, float width, float height);
// ---------------------------

void sdx_SetVertexShaderConstantF(void* device, int start, const float* data, int count);
void sdx_SetPixelShaderConstantF(void* device, int start, const float* data, int count);
void sdx_SetViewport(void* device, int x, int y, int width, int height);
void sdx_SetScissorRect(void* device, int left, int top, int right, int bottom);
void sdx_SetRenderState(void* device, unsigned int state, unsigned int value);
void sdx_SetRenderTarget(void* device, int index, void* surface);
void sdx_SetSamplerFilter(void* device, int sampler, int mag, int min, int mip);
void sdx_SetTexture(void* device, int stage, void* tex);
void sdx_SetVertexShader(void* device, void* shader);
void sdx_SetPixelShader(void* device, void* shader);
void sdx_SetVertexDeclaration(void* device, void* decl);
void sdx_SetStreamSource(void* device, int stream, void* vbuf, int offset, int stride);
void sdx_SetFVF(void* device, int FVF);
void sdx_BeginStateBlock(void* device);
void sdx_EndStateBlock(void* device, void** state_block);
void sdx_Apply(void* state_block);

void sdx_GetRenderTarget(void* device, int index, void** surface);
void sdx_GetSurfaceLevel(void* texture, int level, void** surface);

void sdx_UpdateVertexBuffer(void* vb, int size, int offset, const void* data);
void sdx_UpdateIndexBuffer(void* ib, int size, int offset, const void* data);

void sdx_ClearTarget(void* device, unsigned int color, float z);
void sdx_DrawPrimitive(void* device, int type, int start, int count);
void sdx_Present(void* device);
void sdx_BeginScene(void* device);
void sdx_EndScene(void* device);
```
