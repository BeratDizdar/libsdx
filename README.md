```c
#include "swl.h"
#include "sdx.h"
#include <d3d9.h>

#include "vs_bytecode.h"
#include "ps_bytecode.h"

typedef struct { float x, y, z; float nx, ny, nz; unsigned long color; float u, v; } Vertex;

D3DVERTEXELEMENT9 vertex_elements[] = {
    {0, 0,  D3DDECLTYPE_FLOAT3,   D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0}, // x, y, z
    {0, 12, D3DDECLTYPE_FLOAT3,   D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_NORMAL,   0}, // nx, ny, nz
    {0, 24, D3DDECLTYPE_D3DCOLOR, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_COLOR,    0}, // color
    {0, 28, D3DDECLTYPE_FLOAT2,   D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0}, // u, v
    D3DDECL_END()
};

Vertex v_data[] = {
    { -0.5f,  0.5f, 0.5f,  0.0f, 0.0f, -1.0f,  0xFFFFFFFF,  0.0f, 0.0f },
    {  0.5f,  0.5f, 0.5f,  0.0f, 0.0f, -1.0f,  0xFFFFFFFF,  1.0f, 0.0f },
    {  0.5f, -0.5f, 0.5f,  0.0f, 0.0f, -1.0f,  0xFFFFFFFF,  1.0f, 1.0f },
    { -0.5f, -0.5f, 0.5f,  0.0f, 0.0f, -1.0f,  0xFFFFFFFF,  0.0f, 1.0f },
};
unsigned short i_data[] = { 0, 1, 2, 0, 2, 3 };

IDirect3D9* gobj;
IDirect3DDevice9* device;
IDirect3DVertexBuffer9* v_buffer;
IDirect3DIndexBuffer9* i_buffer;
IDirect3DTexture9* my_tex;
IDirect3DVertexShader9* v_shader;
IDirect3DPixelShader9* p_shader;
IDirect3DVertexDeclaration9* v_decl;
IDirect3DStateBlock9* state;

float acc, tick = 1.0f/35.0f;

int loop(float dt) {
    if (swl_IsKeyDown(27)) swl_SendQuitEvent();

    acc += dt;
    for (; acc >= tick;) { // 35 FPS

        device->lpVtbl->Clear(device, 0, NULL, D3DCLEAR_TARGET, 0xFF202020, 1.0f, 0);
        device->lpVtbl->BeginScene(device);

        sdx_Apply(state);
        device->lpVtbl->DrawIndexedPrimitive(device, D3DPT_TRIANGLELIST, 0, 0, 4, 0, 2);

        device->lpVtbl->EndScene(device);
        device->lpVtbl->Present(device, NULL, NULL, NULL, NULL);
        
        acc -= tick;
    }
    return 0;
}

int main() {
    swl_CreateWindow("X", 800, 600);
    
    gobj = sdx_CreateID3D9();
    device = sdx_CreateID3DDevice9(gobj, swl_GetWindowPtr());
    device->lpVtbl->SetRenderState(device, D3DRS_SPECULARENABLE, 0);
    device->lpVtbl->SetRenderState(device, D3DRS_ALPHABLENDENABLE, TRUE);
    device->lpVtbl->CreateVertexShader(device, (DWORD*)g_vs20_VS_Main, &v_shader);
    device->lpVtbl->CreatePixelShader(device, (DWORD*)g_ps20_PS_Main, &p_shader);
    device->lpVtbl->CreateVertexDeclaration(device, vertex_elements, &v_decl);
    v_buffer = sdx_CreateID3DVertexBuffer9(device, 4*sizeof(Vertex), v_data, 0, 1);
    i_buffer = sdx_CreateID3DIndexBuffer9(device, 6*sizeof(short), i_data, 1);
    my_tex = sdx_CreateID3DTexture9FromPath(device, "t.png");
    device->lpVtbl->SetSamplerState(device, 0, D3DSAMP_MAGFILTER, D3DTEXF_POINT);
    device->lpVtbl->SetSamplerState(device, 0, D3DSAMP_MINFILTER, D3DTEXF_POINT);

    sdx_BeginStateBlock(device);
        device->lpVtbl->SetVertexShader(device, v_shader);
        device->lpVtbl->SetPixelShader(device, p_shader);
        device->lpVtbl->SetTexture(device, 0, (IDirect3DBaseTexture9*)my_tex);
        device->lpVtbl->SetVertexDeclaration(device, v_decl);
        device->lpVtbl->SetStreamSource(device, 0, v_buffer, 0, sizeof(Vertex));
        device->lpVtbl->SetIndices(device, i_buffer);
    sdx_EndStateBlock(device, &state);

    swl_Mainloop(loop);

    state->lpVtbl->Release(state);
    my_tex->lpVtbl->Release(my_tex);
    v_decl->lpVtbl->Release(v_decl);
    i_buffer->lpVtbl->Release(i_buffer);
    v_buffer->lpVtbl->Release(v_buffer);
    p_shader->lpVtbl->Release(p_shader);
    v_shader->lpVtbl->Release(v_shader);
    device->lpVtbl->Release(device);
    gobj->lpVtbl->Release(gobj);
    
    swl_CloseWindow();
    return 0;
}
```
