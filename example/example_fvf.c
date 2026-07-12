#include "swl.h"
#include "sdx.h"
#include <d3d9.h>

#define FVF_2D (D3DFVF_XYZRHW | D3DFVF_TEX1)

typedef struct {
    float x, y, z, rhw;
    float u, v;
} Vertex;

void* ctx = 0;
void* dev = 0;
void* tex = 0;
void* vb  = 0;

float px = 300.0f, py = 200.0f;
float w = 128.0f, h = 128.0f;

int game_loop(float dt) {
    if (swl_IsKeyDown(27)) swl_SendQuitEvent();

    float speed = 400.0f;
    if (swl_IsKeyDown('W')) py -= speed * dt;
    if (swl_IsKeyDown('S')) py += speed * dt;
    if (swl_IsKeyDown('A')) px -= speed * dt;
    if (swl_IsKeyDown('D')) px += speed * dt;

    int mx, my;
    swl_GetMousePos(&mx, &my);
    if (swl_IsKeyDown(1)) {
        px = mx - (w / 2.0f);
        py = my - (h / 2.0f);
    }

    Vertex quad[4] = {
        { px,     py,     0.0f, 1.0f,  0.0f, 0.0f },
        { px + w, py,     0.0f, 1.0f,  1.0f, 0.0f },
        { px,     py + h, 0.0f, 1.0f,  0.0f, 1.0f },
        { px + w, py + h, 0.0f, 1.0f,  1.0f, 1.0f }
    };
    
    sdx_UpdateVertexBuffer(vb, sizeof(quad), 0, quad);

    sdx_ClearTarget(dev, 0xFF050A05, 1.0f); 
    sdx_BeginScene(dev);
    sdx_SetRenderState(dev, D3DRS_ALPHABLENDENABLE, 1);
    sdx_SetRenderState(dev, D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
    sdx_SetRenderState(dev, D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
    sdx_SetFVF(dev, FVF_2D);
    
    sdx_SetStreamSource(dev, 0, vb, 0, sizeof(Vertex));
    sdx_SetTexture(dev, 0, tex);
    
    sdx_DrawPrimitive(dev, D3DPT_TRIANGLESTRIP, 0, 2);
    
    sdx_EndScene(dev);
    sdx_Present(dev);
    
    return 0;
}

int main() {
    swl_CreateWindow("FVF Demo", 800, 600);
    sdx_CreateContext(&ctx);
    sdx_CreateDevice(ctx, swl_GetWindowPtr(), &dev);
    sdx_CreateTextureFromPath(dev, "t.png", &tex);
    sdx_CreateVertexBuffer(dev, sizeof(Vertex) * 4, 0, FVF_2D, 0, &vb);

    swl_Mainloop(game_loop);
    
    sdx_ReleaseArgs(vb, tex, dev, ctx, NULL);
    swl_CloseWindow();
    return 0;
}