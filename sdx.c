#include "sdx.h"
#include<d3d9.h>

void* sdx_CreateID3D9(){return Direct3DCreate9(D3D_SDK_VERSION);}
void* sdx_CreateID3DDevice9(void* d3d9, void* hwnd){
    IDirect3DDevice9* device;
    D3DPRESENT_PARAMETERS pp = {0};
    pp.Windowed = TRUE;
    pp.SwapEffect = D3DSWAPEFFECT_DISCARD;
    pp.BackBufferFormat = D3DFMT_UNKNOWN;
    pp.hDeviceWindow = hwnd;
    ((IDirect3D9*)d3d9)->lpVtbl->CreateDevice(((IDirect3D9*)d3d9),
        D3DADAPTER_DEFAULT,
        D3DDEVTYPE_HAL,
        hwnd,
        D3DCREATE_HARDWARE_VERTEXPROCESSING | D3DCREATE_PUREDEVICE,
        &pp,
        &device
    );
    return device;
}
void *sdx_CreateID3DVertexBuffer9(void* device, int size, unsigned int FVF){
    IDirect3DVertexBuffer9* vertex;
    ((IDirect3DDevice9*)device)->lpVtbl->CreateVertexBuffer(((IDirect3DDevice9*)device), size, 0, FVF, D3DPOOL_MANAGED, &vertex, NULL);
    return vertex;
}
void *sdx_CreateID3DIndexBuffer9(void* device, int size){
    IDirect3DIndexBuffer9* index;
    ((IDirect3DDevice9*)device)->lpVtbl->CreateIndexBuffer(((IDirect3DDevice9*)device), size, 0, D3DFMT_INDEX16, D3DPOOL_MANAGED, &index, NULL);
    return index;
}