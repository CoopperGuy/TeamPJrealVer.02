
#include "Shader_Defines.hpp"

cbuffer Matrices
{
    matrix g_WorldMatrix;
    matrix g_ViewMatrix;
    matrix g_ProjMatrix;
}

cbuffer CameraDesc
{
    vector g_vCamPosition;
}

Texture2D g_DiffuseTexture;
SamplerState g_DiffuseSampler
{
    AddressU = mirror;
    AddressV = mirror;
};
struct PointWorldPosition
{
    vector PointWorldPosition[128];
};

PointWorldPosition g_PointWorldPosition;
//--------------------------------------------------------------------------------------

struct VS_IN
{
    float3 vPosition : POSITION;
    float2 vSize : PSIZE;
    uint vIndex : BLENDINDICES;
};

struct VS_OUT
{
    float4 vPosition : POSITION;
    float2 vSize : PSIZE;
};

cbuffer LineDesc
{
    float4  g_vColor : COLOR0;
    float   g_fThickness : PSIZE;
};


//--------------------------------------------------------------------------------------
// Vertex Shader
//--------------------------------------------------------------------------------------
VS_OUT VS_MAIN(VS_IN In)
{
    VS_OUT Out = (VS_OUT) 0;

    Out.vPosition = g_PointWorldPosition.PointWorldPosition[In.vIndex];
    Out.vSize = In.vSize;

    return Out;
}



struct GS_OUT
{
    float4 vPosition : SV_POSITION;
    float2 vTexUV : TEXCOORD0;
};

[maxvertexcount(6)]
void GS_MAIN( /*입력*/line VS_OUT points[2], /*출력*/inout TriangleStream<GS_OUT> Out)
{
    float3 inVertex1Pos = points[0].vPosition.xyz;
    float3 inVertex2Pos = points[1].vPosition.xyz;


    float3 lineVec = normalize(inVertex1Pos - inVertex2Pos);

    float3 lineCenter = (inVertex1Pos + inVertex2Pos) / 2.f;
    float3 towardsCamVec = normalize(g_vCamPosition.xyz - lineCenter);

    float3 localUpVec = normalize(cross(lineVec, towardsCamVec));


    GS_OUT v[6];
    float LineThinkness = g_fThickness;
    v[0].vPosition.xyz = inVertex1Pos + LineThinkness * localUpVec;
    v[0].vTexUV = float2(0.0f, 0.0f);

    v[1].vPosition.xyz = inVertex2Pos + LineThinkness * localUpVec;
    v[1].vTexUV = float2(1.0f, 0.0f);

    v[2].vPosition.xyz = inVertex1Pos - LineThinkness * localUpVec;
    v[2].vTexUV = float2(0.0f, 1.0f);

    v[3] = v[2];
    v[4] = v[1];
    v[5].vPosition.xyz = inVertex2Pos - LineThinkness * localUpVec;
    v[5].vTexUV = float2(1.0f, 1.0f);
    
    matrix matVP = mul(g_ViewMatrix, g_ProjMatrix);

    for (int i = 0; i < 6; i++)
    {
        v[i].vPosition.w = 1.f;
        v[i].vPosition = mul(v[i].vPosition, matVP);
    }

    Out.Append(v[0]);
    Out.Append(v[1]);
    Out.Append(v[2]);
    Out.RestartStrip();
    Out.Append(v[3]);
    Out.Append(v[4]);
    Out.Append(v[5]);
    Out.RestartStrip();

    //float4 p0 = points[0].vPosition;
    //float4 p1 = points[1].vPosition;

    //float w0 = p0.w;
    //float w1 = p1.w;

    //p0.xyz /= p0.w;
    //p1.xyz /= p1.w;

    //float3 line01 = p1 - p0;
    //float3 dir = normalize(line01);

    //float2 RenderTargetSize = { 1920, 1280 };
    //float3 ratio = float3(RenderTargetSize.y, RenderTargetSize.x, 0);
    //ratio = normalize(ratio);

    //float3 unit_z = normalize(float3(0, 0, -1));

    //float3 normal = normalize(cross(unit_z, dir) * ratio);

    //float width = 0.01;

    //GS_OUT v[4];

    //float3 dir_offset = dir * ratio * width;
    //float3 normal_scaled = normal * ratio * width;

    //float3 p0_ex = p0.xyz - dir_offset;
    //float3 p1_ex = p1.xyz + dir_offset;

    //v[0].vPosition = float4(p0_ex - normal_scaled, 1) * w0;
    //v[0].vTexUV = float2(0, 0);

    //v[1].vPosition = float4(p0_ex + normal_scaled, 1) * w0;
    //v[1].vTexUV = float2(0, 0);

    //v[2].vPosition = float4(p1_ex + normal_scaled, 1) * w1;
    //v[2].vTexUV = float2(0, 0);

    //v[3].vPosition = float4(p1_ex - normal_scaled, 1) * w1;
    //v[3].vTexUV = float2(0, 0);

    //Out.Append(v[2]);
    //Out.Append(v[1]);
    //Out.Append(v[0]);

    //Out.RestartStrip();

    //Out.Append(v[3]);
    //Out.Append(v[2]);
    //Out.Append(v[0]);

    //Out.RestartStrip();
}


struct PS_IN
{
    float4 vPosition : SV_POSITION;
    float2 vTexUV : TEXCOORD0;
};

vector PS_MAIN(PS_IN In) : SV_TARGET
{
    //vector vColor = (vector) 0;
    
    //vColor.r = 1;
    //vColor.a = 0.6;

    //return vColor;

    vector vColor = (vector) 0;
    vColor = g_DiffuseTexture.Sample(g_DiffuseSampler, In.vTexUV);
    vector red = { 1, 0, 0, 1 };
    vector green = { 0, 1, 0, 1 };
    vColor *= green;
    if (vColor.a < 0.3f)
        discard;

    return vColor;
}


vector PS_MAIN_SIGHT_INDICATOR(PS_IN In) : SV_TARGET
{
    return g_vColor;
}


technique11 DefaultDevice
{
    pass DefaultPass
    {
        SetRasterizerState(Rasterizer_Sky);
        SetDepthStencilState(DepthStecil_Default, 0);
        SetBlendState(Blend_Alpha, vector(0.f, 0.f, 0.f, 0.f), 0xffffffff);

        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = compile gs_5_0 GS_MAIN();
        PixelShader = compile ps_5_0 PS_MAIN();
    }
    pass SightIndicator
    {
        SetRasterizerState(Rasterizer_Sky);
        SetDepthStencilState(DepthStecil_Default, 0);
        SetBlendState(Blend_Alpha, vector(0.f, 0.f, 0.f, 0.f), 0xffffffff);

        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = compile gs_5_0 GS_MAIN();
        PixelShader = compile ps_5_0 PS_MAIN_SIGHT_INDICATOR();
    }
}