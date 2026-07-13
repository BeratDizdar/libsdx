#include"sdx.h"
#include<d3d9.h>
#define STBI_NO_SIMD
#define STBI_ONLY_PNG
#define STB_IMAGE_IMPLEMENTATION
#include"depen/stb_image.h"
#include<stdarg.h>
#define SDX __declspec(dllexport)

//---------------------------------------------------------------------
//
// Create Functions
//
//---------------------------------------------------------------------

SDX void sdx_CreateContext(void** d3d9){*d3d9 = Direct3DCreate9(D3D_SDK_VERSION);}
SDX void sdx_CreateDevice(void* d3d9, void* hwnd, int pure, void** device){
    D3DPRESENT_PARAMETERS pp = {0};
    pp.Windowed = TRUE;
    pp.SwapEffect = D3DSWAPEFFECT_DISCARD;
    pp.BackBufferFormat = D3DFMT_UNKNOWN;
    pp.hDeviceWindow = hwnd;
    ((IDirect3D9*)d3d9)->lpVtbl->CreateDevice(((IDirect3D9*)d3d9),
        D3DADAPTER_DEFAULT,D3DDEVTYPE_HAL,hwnd,
        D3DCREATE_HARDWARE_VERTEXPROCESSING | (D3DCREATE_PUREDEVICE*pure),
        &pp,(IDirect3DDevice9**)device
    );
}
SDX void sdx_CreateVertexBuffer(void* device, int size, void* data, unsigned int FVF, unsigned int managed, void** vb) {
    DWORD pool = (managed)? D3DPOOL_MANAGED: D3DPOOL_DEFAULT;
    DWORD usage = (!managed)? D3DUSAGE_DYNAMIC|D3DUSAGE_WRITEONLY: 0;    
    ((IDirect3DDevice9*)device)->lpVtbl->CreateVertexBuffer(((IDirect3DDevice9*)device), size, usage, FVF, pool, (IDirect3DVertexBuffer9**)vb, NULL);

    if (*vb && data) {
        void* v;
        DWORD lock_flag = (!managed) ? D3DLOCK_DISCARD : 0;
        ((IDirect3DVertexBuffer9*)(*vb))->lpVtbl->Lock(((IDirect3DVertexBuffer9*)(*vb)), 0, 0, (void**)&v, lock_flag);
        memcpy(v, data, size);
        ((IDirect3DVertexBuffer9*)(*vb))->lpVtbl->Unlock(((IDirect3DVertexBuffer9*)(*vb)));
    }
}

SDX void sdx_CreateIndexBuffer(void* device, int size, void* data, unsigned int managed, void** ib) {
    DWORD pool = (managed)? D3DPOOL_MANAGED: D3DPOOL_DEFAULT;
    DWORD usage = (!managed)? D3DUSAGE_DYNAMIC|D3DUSAGE_WRITEONLY: 0;
    ((IDirect3DDevice9*)device)->lpVtbl->CreateIndexBuffer(((IDirect3DDevice9*)device), size, usage, D3DFMT_INDEX16, pool, (IDirect3DIndexBuffer9**)ib, NULL);
    
    if (*ib && data) {
        void* i;
        DWORD lock_flag = (!managed) ? D3DLOCK_DISCARD : 0;
        ((IDirect3DIndexBuffer9*)(*ib))->lpVtbl->Lock(((IDirect3DIndexBuffer9*)(*ib)), 0, 0, (void**)&i, lock_flag);
        memcpy(i, data, size);
        ((IDirect3DIndexBuffer9*)(*ib))->lpVtbl->Unlock(((IDirect3DIndexBuffer9*)(*ib)));
    }
}

SDX void sdx_CreateTextureFromPath(void* device, const char* filepath, void** tex) {
    int width, height, channels;
    unsigned char *img = stbi_load(filepath, &width, &height, &channels, 4);
    if (!img) { *tex = NULL; return; }

    ((IDirect3DDevice9*)device)->lpVtbl->CreateTexture(((IDirect3DDevice9*)device), 
        width, height, 1, 0, D3DFMT_A8R8G8B8, D3DPOOL_MANAGED, (IDirect3DTexture9**)tex, NULL);
    if (*tex) {
        D3DLOCKED_RECT rect;
        ((IDirect3DTexture9*)(*tex))->lpVtbl->LockRect(((IDirect3DTexture9*)(*tex)), 0, &rect, NULL, 0);
        unsigned char* dest = (unsigned char*)rect.pBits;
        for (int y = 0; y < height; ++y) {
            unsigned char* src_row = img + (y * width * 4);
            unsigned char* dst_row = dest + (y * rect.Pitch);
            
            for (int x = 0; x < width; ++x) {
                dst_row[x*4 + 0] = src_row[x*4 + 2];
                dst_row[x*4 + 1] = src_row[x*4 + 1];
                dst_row[x*4 + 2] = src_row[x*4 + 0];
                dst_row[x*4 + 3] = src_row[x*4 + 3];
            }
        }
        ((IDirect3DTexture9*)(*tex))->lpVtbl->UnlockRect(((IDirect3DTexture9*)(*tex)), 0);
    }   
    stbi_image_free(img);
}
SDX void sdx_CreateRenderTargetTexture(void* device, int width, int height, void** tex) {
    ((IDirect3DDevice9*)device)->lpVtbl->CreateTexture(
        ((IDirect3DDevice9*)device),
        width, height, 1, 
        D3DUSAGE_RENDERTARGET, 
        D3DFMT_A8R8G8B8, 
        D3DPOOL_DEFAULT, 
        (IDirect3DTexture9**)tex, NULL
    );
}
SDX void sdx_CreateVertexDeclaration(void* device, const int* layout, void** decl) {
    D3DVERTEXELEMENT9 d3d_elements[32]; // maks 32 yaptım şimdilik
    short offset = 0;
    int i = 0;

    while (layout[i] != SDX_LAYOUT_END) {
        d3d_elements[i].Stream = 0;
        d3d_elements[i].Offset = offset;
        d3d_elements[i].Method = D3DDECLMETHOD_DEFAULT;
        d3d_elements[i].UsageIndex = 0;

        switch (layout[i]) {
            case SDX_LAYOUT_POS3:
                d3d_elements[i].Type = D3DDECLTYPE_FLOAT3;
                d3d_elements[i].Usage = D3DDECLUSAGE_POSITION;
                offset += 12;
                break;
            case SDX_LAYOUT_POS2:
                d3d_elements[i].Type = D3DDECLTYPE_FLOAT2;
                d3d_elements[i].Usage = D3DDECLUSAGE_POSITION;
                offset += 8;
                break;
            case SDX_LAYOUT_NORM3:
                d3d_elements[i].Type = D3DDECLTYPE_FLOAT3;
                d3d_elements[i].Usage = D3DDECLUSAGE_NORMAL;
                offset += 12;
                break;
            case SDX_LAYOUT_COLOR4:
                d3d_elements[i].Type = D3DDECLTYPE_D3DCOLOR;
                d3d_elements[i].Usage = D3DDECLUSAGE_COLOR;
                offset += 4;
                break;
            case SDX_LAYOUT_UV2:
                d3d_elements[i].Type = D3DDECLTYPE_FLOAT2;
                d3d_elements[i].Usage = D3DDECLUSAGE_TEXCOORD;
                offset += 8;
                break;
        }i++;
    }
    d3d_elements[i].Stream = 0xFF; 
    d3d_elements[i].Offset = 0;
    d3d_elements[i].Type = D3DDECLTYPE_UNUSED;
    d3d_elements[i].Method = 0;
    d3d_elements[i].Usage = 0;
    d3d_elements[i].UsageIndex = 0;
    ((IDirect3DDevice9*)device)->lpVtbl->CreateVertexDeclaration(
        ((IDirect3DDevice9*)device), 
        (const D3DVERTEXELEMENT9*)d3d_elements, 
        (IDirect3DVertexDeclaration9**)decl
    );
}
SDX void sdx_CreateVertexShader(void *device, unsigned long *data, void **shader) {
    HRESULT hr=((IDirect3DDevice9*)device)->lpVtbl->CreateVertexShader((IDirect3DDevice9*)device, data, shader);
    if (FAILED(hr)) {
        printf("BINGO! D3D9 bu Bytecode'u reddetti!\n");
    }
}

SDX void sdx_CreatePixelShader(void *device, unsigned long *data, void **shader) {
    HRESULT hr=((IDirect3DDevice9*)device)->lpVtbl->CreatePixelShader((IDirect3DDevice9*)device, data, shader);
    if (FAILED(hr)) {
        printf("BINGO! D3D9 bu Bytecode'u reddetti!\n");
    }
}

//---------------------------------------------------------------------
//
// RELEASE
//
//---------------------------------------------------------------------

SDX void sdx_ReleaseArgs(void* first_obj, ...) {
    va_list ap;
    va_start(ap, first_obj);
    for (void* obj = first_obj; obj != NULL; obj = va_arg(ap, void*)) {
        IUnknown* com_obj = (IUnknown*)obj;
        com_obj->lpVtbl->Release(com_obj);
    }
    va_end(ap);
}

//---------------------------------------------------------------------
//
// State Functions
//
//---------------------------------------------------------------------

SDX void sdx_SetOrtho(void* device, int shader_register, float width, float height) {
    float mat[16] = {
        2.0f / width,  0.0f,          0.0f, -1.0f,
        0.0f,         -2.0f / height, 0.0f,  1.0f,
        0.0f,          0.0f,          1.0f,  0.0f,
        0.0f,          0.0f,          0.0f,  1.0f
    };
    sdx_SetVertexShaderConstantF(device, shader_register, mat, 4);
}
SDX void sdx_SetVertexShaderConstantF(void *device, int start, const float *data, int count) {
    ((IDirect3DDevice9*)device)->lpVtbl->SetVertexShaderConstantF(device, start, data, count);
}
SDX void sdx_SetPixelShaderConstantF(void *device, int start, const float *data, int count) {
    ((IDirect3DDevice9*)device)->lpVtbl->SetPixelShaderConstantF(device, start, data, count);
}
SDX void sdx_SetViewport(void* device, int x, int y, int width, int height) {
    ((IDirect3DDevice9*)device)->lpVtbl->SetViewport(device, &(D3DVIEWPORT9){x,y,width,height,0,1});
}
SDX void sdx_SetScissorRect(void* device, int left, int top, int right, int bottom) {
    ((IDirect3DDevice9*)device)->lpVtbl->SetScissorRect(device, &(RECT){left, top, right, bottom});
}
SDX void sdx_SetRenderState(void *device, unsigned int state, unsigned int value) {
    ((IDirect3DDevice9*)device)->lpVtbl->SetRenderState(device, state, value);
}
SDX void sdx_SetRenderTarget(void *device, int index, void *surface) {
    ((IDirect3DDevice9*)device)->lpVtbl->SetRenderTarget(device, index, surface);
}
SDX void sdx_SetSamplerFilter(void *device, int sampler, int mag, int min, int mip) {
    ((IDirect3DDevice9*)device)->lpVtbl->SetSamplerState(device, sampler, D3DSAMP_MAGFILTER, mag);
    ((IDirect3DDevice9*)device)->lpVtbl->SetSamplerState(device, sampler, D3DSAMP_MINFILTER, min);
    ((IDirect3DDevice9*)device)->lpVtbl->SetSamplerState(device, sampler, D3DSAMP_MIPFILTER, mip);
}
SDX void sdx_SetTexture(void *device, int stage, void *tex) {
    ((IDirect3DDevice9*)device)->lpVtbl->SetTexture((IDirect3DDevice9*)device, stage, tex);
}
SDX void sdx_SetVertexShader(void *device, void *shader) {
    ((IDirect3DDevice9*)device)->lpVtbl->SetVertexShader((IDirect3DDevice9*)device, shader);
}
SDX void sdx_SetPixelShader(void *device, void *shader) {
    ((IDirect3DDevice9*)device)->lpVtbl->SetPixelShader((IDirect3DDevice9*)device, shader);
}
SDX void sdx_SetVertexDeclaration(void *device, void *decl) {
    ((IDirect3DDevice9*)device)->lpVtbl->SetVertexDeclaration((IDirect3DDevice9*)device, decl);
}
SDX void sdx_SetStreamSource(void *device, int stream, void *vbuf, int offset, int stride) {
    ((IDirect3DDevice9*)device)->lpVtbl->SetStreamSource(device, stream, vbuf, offset, stride);
}
SDX void sdx_BeginStateBlock(void *device) {
    ((IDirect3DDevice9*)device)->lpVtbl->BeginStateBlock(device);
}
SDX void sdx_EndStateBlock(void* device, void** state_block) {
    ((IDirect3DDevice9*)device)->lpVtbl->EndStateBlock(device, state_block);
}
SDX void sdx_ApplyStateBlock(void* state_block) {
    ((IDirect3DStateBlock9*)state_block)->lpVtbl->Apply(state_block);
}

//---------------------------------------------------------------------
//
// Reference Functions
//
//---------------------------------------------------------------------

SDX void sdx_GetRenderTarget(void* device, int index, void** surface) {
    ((IDirect3DDevice9*)device)->lpVtbl->GetRenderTarget(device, index, surface);
}
SDX void sdx_GetSurfaceLevel(void* texture, int level, void** surface) {
    ((IDirect3DTexture9*)texture)->lpVtbl->GetSurfaceLevel(texture, level, surface);
}

//---------------------------------------------------------------------
//
// Memory Functions
//
//---------------------------------------------------------------------

SDX void sdx_UpdateVertexBuffer(void* vb, int size, int offset, const void* data) {
    if (!vb || !data) return;
    
    void* pVoid;
    ((IDirect3DVertexBuffer9*)vb)->lpVtbl->Lock(((IDirect3DVertexBuffer9*)vb), offset, size, (void**)&pVoid, D3DLOCK_DISCARD);
    memcpy(pVoid, data, size);
    ((IDirect3DVertexBuffer9*)vb)->lpVtbl->Unlock(((IDirect3DVertexBuffer9*)vb));
}
SDX void sdx_UpdateIndexBuffer(void* ib, int size, int offset, const void* data) {
    if (!ib || !data) return;
    
    void* pVoid;
    ((IDirect3DIndexBuffer9*)ib)->lpVtbl->Lock(((IDirect3DIndexBuffer9*)ib), offset, size, (void**)&pVoid, D3DLOCK_DISCARD);
    memcpy(pVoid, data, size);
    ((IDirect3DIndexBuffer9*)ib)->lpVtbl->Unlock(((IDirect3DIndexBuffer9*)ib));
}

//---------------------------------------------------------------------
//
// Surface Functions
//
//---------------------------------------------------------------------

SDX void sdx_ClearTarget(void *device, unsigned int color, float z) {
    ((IDirect3DDevice9*)device)->lpVtbl->Clear((IDirect3DDevice9*)device, 0, NULL, D3DCLEAR_TARGET, color, z, 0);
}
SDX void sdx_DrawPrimitive(void *device, int type, int start, int count) {
    ((IDirect3DDevice9*)device)->lpVtbl->DrawPrimitive(device, type, start, count);
}
SDX void sdx_DrawPrimitiveUP(void *device, int type, int count, void *data, int stride) {
    ((IDirect3DDevice9*)device)->lpVtbl->DrawPrimitiveUP(device, type, count, data, stride);
}
SDX void sdx_Present(void *device) {
    ((IDirect3DDevice9*)device)->lpVtbl->Present(device, NULL, NULL, NULL, NULL);
}
SDX void sdx_BeginScene(void *device) {
    ((IDirect3DDevice9*)device)->lpVtbl->BeginScene(device);
}
SDX void sdx_EndScene(void *device) {
    ((IDirect3DDevice9*)device)->lpVtbl->EndScene(device);
}
SDX void sdx_SetFVF(void *device, int FVF) {
    ((IDirect3DDevice9*)device)->lpVtbl->SetFVF(device, FVF);
}
SDX void sdx_SetTextureStageState(void *device, int stage, int type, int value) {
    ((IDirect3DDevice9*)device)->lpVtbl->SetTextureStageState(device, stage, type, value);
}