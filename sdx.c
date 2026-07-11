#include "sdx.h"
#include<d3d9.h>
#define STBI_NO_SIMD
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
// https://learn.microsoft.com/en-us/windows/win32/api/d3d9/nf-d3d9-idirect3ddevice9-beginstateblock

void* sdx_CreateID3D9(){return Direct3DCreate9(D3D_SDK_VERSION);}
void* sdx_CreateID3DDevice9(void* d3d9, void* hwnd){
    IDirect3DDevice9* device;
    D3DPRESENT_PARAMETERS pp = {0};
    pp.Windowed = TRUE;
    pp.SwapEffect = D3DSWAPEFFECT_DISCARD;
    pp.BackBufferFormat = D3DFMT_UNKNOWN;
    pp.hDeviceWindow = hwnd;
    ((IDirect3D9*)d3d9)->lpVtbl->CreateDevice(((IDirect3D9*)d3d9),
        D3DADAPTER_DEFAULT,D3DDEVTYPE_HAL,hwnd,
        D3DCREATE_HARDWARE_VERTEXPROCESSING | D3DCREATE_PUREDEVICE,
        &pp,&device);
    return device;
}
void *sdx_CreateID3DVertexBuffer9(void* device, int size, void* data, unsigned int FVF, unsigned int managed){
    DWORD pool = (managed)? D3DPOOL_MANAGED: D3DPOOL_DEFAULT;
    DWORD usage = (!managed)? D3DUSAGE_DYNAMIC|D3DUSAGE_WRITEONLY: 0;
    IDirect3DVertexBuffer9* vertex;
    ((IDirect3DDevice9*)device)->lpVtbl->CreateVertexBuffer(((IDirect3DDevice9*)device), size, usage, FVF, pool, &vertex, NULL);
    if (data) {
        void* v;
        DWORD lock_flag = (!managed) ? D3DLOCK_DISCARD : 0;
        vertex->lpVtbl->Lock(vertex, 0, 0, (void**)&v, 0);
        memcpy(v, data, size);
        vertex->lpVtbl->Unlock(vertex);
    }
    return vertex;
}
void *sdx_CreateID3DIndexBuffer9(void* device, int size, void* data, unsigned int managed){
    DWORD pool = (managed)? D3DPOOL_MANAGED: D3DPOOL_DEFAULT;
    DWORD usage = (!managed)? D3DUSAGE_DYNAMIC|D3DUSAGE_WRITEONLY: 0;
    IDirect3DIndexBuffer9* index;
    ((IDirect3DDevice9*)device)->lpVtbl->CreateIndexBuffer(((IDirect3DDevice9*)device), size, usage, D3DFMT_INDEX16, pool, &index, NULL);
    if (data) {
        void* i;
        DWORD lock_flag = (!managed) ? D3DLOCK_DISCARD : 0;
        index->lpVtbl->Lock(index, 0, 0, (void**)&i, lock_flag);
        memcpy(i, data, size);
        index->lpVtbl->Unlock(index);
    }
    return index;
}
void* sdx_CreateID3DTexture9FromPath(void* device, const char* filepath) {
    int width, height, channels;
    unsigned char *img = stbi_load(filepath, &width, &height, &channels, 4);
    if (!img) return NULL;

    IDirect3DTexture9* tex;
    ((IDirect3DDevice9*)device)->lpVtbl->CreateTexture(((IDirect3DDevice9*)device), 
        width, height, 1, 0, D3DFMT_A8R8G8B8, D3DPOOL_MANAGED, &tex, NULL);
    D3DLOCKED_RECT rect;
    tex->lpVtbl->LockRect(tex, 0, &rect, NULL, 0);

    unsigned char* dest = (unsigned char*)rect.pBits;
    for (int y = 0; y < height; ++y) {
        unsigned char* src_row = img + (y * width * 4);
        unsigned char* dst_row = dest + (y * rect.Pitch);
        
        for (int x = 0; x < width; ++x) {
            dst_row[x*4 + 0] = src_row[x*4 + 2]; // Blue  <- Red
            dst_row[x*4 + 1] = src_row[x*4 + 1]; // Green <- Green
            dst_row[x*4 + 2] = src_row[x*4 + 0]; // Red   <- Blue
            dst_row[x*4 + 3] = src_row[x*4 + 3]; // Alpha <- Alpha
        }
    }
    tex->lpVtbl->UnlockRect(tex, 0);
    stbi_image_free(img);

    return tex;
}
void* sdx_CreateRenderTargetTexture(void* device, int width, int height) {
    IDirect3DTexture9* tex = NULL;
    ((IDirect3DDevice9*)device)->lpVtbl->CreateTexture(
        ((IDirect3DDevice9*)device),
        width, height, 1, 
        D3DUSAGE_RENDERTARGET, 
        D3DFMT_A8R8G8B8, 
        D3DPOOL_DEFAULT, 
        &tex, NULL
    );
    return (void*)tex;
}
void* sdx_CreateID3DVertexDeclaration9(void* device, const void* elements) {
    IDirect3DVertexDeclaration9* decl = NULL;
    ((IDirect3DDevice9*)device)->lpVtbl->CreateVertexDeclaration(
        ((IDirect3DDevice9*)device), 
        (const D3DVERTEXELEMENT9*)elements, 
        &decl
    );
    return decl;
}
void sdx_BeginStateBlock(void* device) {
    ((IDirect3DDevice9*)device)->lpVtbl->BeginStateBlock(device);
}
void sdx_EndStateBlock(void* device, void** state_block) {
    ((IDirect3DDevice9*)device)->lpVtbl->EndStateBlock(device, state_block);
}
void sdx_Apply(void* state_block) {
    ((IDirect3DStateBlock9*)state_block)->lpVtbl->Apply(state_block);
}
