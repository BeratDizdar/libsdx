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