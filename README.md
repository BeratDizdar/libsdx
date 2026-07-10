```c
#include "swl.h"
#include "sdx.h"
#include <d3d9.h>

typedef struct { float x, y, z; unsigned long color; } Vertex;
#define D3DFVF_VERTEX (D3DFVF_XYZ | D3DFVF_DIFFUSE)

Vertex v_data[] = {
    { -0.5f,  0.5f, 0.5f, 0xFFFF0000 },
    {  0.5f,  0.5f, 0.5f, 0xFF00FF00 },
    {  0.5f, -0.5f, 0.5f, 0xFF0000FF },
    { -0.5f, -0.5f, 0.5f, 0xFFFFFFFF },
};
unsigned short i_data[] = { 0, 1, 2, 0, 2, 3 };

IDirect3D9* gobj;
IDirect3DDevice9* device;
IDirect3DVertexBuffer9* v_buffer;
IDirect3DIndexBuffer9* i_buffer;

float acc, tick = 1.0/35.0;
int loop(float dt) {
    if (swl_IsKeyDown(27)) swl_SendQuitEvent();

    acc+=dt;
    for (;acc>=tick;){ // 35 FPS
        device->lpVtbl->Clear(device, 0, NULL, D3DCLEAR_TARGET, 0xFF202020, 1.0f, 0);
        device->lpVtbl->BeginScene(device);
    
        device->lpVtbl->SetFVF(device, D3DFVF_VERTEX);
        device->lpVtbl->SetStreamSource(device, 0, v_buffer, 0, sizeof(Vertex));
        device->lpVtbl->SetIndices(device, i_buffer);
        device->lpVtbl->DrawIndexedPrimitive(device, D3DPT_TRIANGLELIST, 0, 0, 4, 0, 2);
    
        device->lpVtbl->EndScene(device);
        device->lpVtbl->Present(device, NULL, NULL, NULL, NULL);
        
        acc-=tick;
    }
    
}

int main() {
    swl_CreateWindow("X", 400, 300);
    gobj = sdx_CreateID3D9();
    device = sdx_CreateID3DDevice9(gobj, swl_GetWindowPtr());
    device->lpVtbl->SetRenderState(device, D3DRS_SPECULARENABLE, 0);
    device->lpVtbl->SetRenderState(device, D3DRS_ZENABLE, 0);

    v_buffer = sdx_CreateID3DVertexBuffer9(device, 4*sizeof(Vertex), D3DFVF_VERTEX);
    Vertex* v;
    v_buffer->lpVtbl->Lock(v_buffer, 0, 0, (void**)&v, 0);
    memcpy(v, v_data, 4 * sizeof(Vertex));
    v_buffer->lpVtbl->Unlock(v_buffer);

    i_buffer = sdx_CreateID3DIndexBuffer9(device, 6*sizeof(short));
    unsigned short* i;
    i_buffer->lpVtbl->Lock(i_buffer, 0, 0, (void**)&i, 0);
    memcpy(i, i_data, 6 * sizeof(unsigned short));
    i_buffer->lpVtbl->Unlock(i_buffer);

    swl_Mainloop(loop);

    i_buffer->lpVtbl->Release(i_buffer);
    v_buffer->lpVtbl->Release(v_buffer);
    device->lpVtbl->Release(device);
    gobj->lpVtbl->Release(gobj);
    swl_CloseWindow();
    return 0;
}
```
