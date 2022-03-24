
#include "Shader_Defines.hpp"

static const float Weight[13] =
{
    0.0561, 0.1353, 0.278, 0.4868, 0.7261, 0.9231,
    1, 0.9231, 0.7261, 0.4868, 0.278, 0.1353, 0.0561
};

static const float Total = 6.2108;

Texture2D g_DiffuseTexture;

struct VS_IN
{
    float3 vPosition : POSITION; /* 로컬스페이스 */
    float2 vTexUV : TEXCOORD0;
};

struct VS_OUT
{
    float4 vPosition : SV_POSITION;
    float2 vTexUV : TEXCOORD0;
};

VS_OUT VS_MAIN(VS_IN In)
{
    VS_OUT Out = (VS_OUT) 0;

    Out.vPosition = vector(In.vPosition, 1.f);
    Out.vTexUV = In.vTexUV;

    return Out;
}

struct VS_OUT_BLUR
{
    float4 vPosition : SV_POSITION;
    float2 vTexUV : TEXCOORD0;
    float2 texCoord1 : TEXCOORD1;
    float2 texCoord2 : TEXCOORD2;
    float2 texCoord3 : TEXCOORD3;
    float2 texCoord4 : TEXCOORD4;
    float2 texCoord5 : TEXCOORD5;
    float2 texCoord6 : TEXCOORD6;
    float2 texCoord7 : TEXCOORD7;
    float2 texCoord8 : TEXCOORD8;
    float2 texCoord9 : TEXCOORD9;
};

VS_OUT_BLUR VS_MAIN_BLURX(VS_IN In)
{
    VS_OUT_BLUR Out = (VS_OUT_BLUR) 0;

    Out.vPosition = vector(In.vPosition, 1.f);
    Out.vTexUV = In.vTexUV;

    float texelSize = 1.f / 1280.f / 4.f;

    Out.texCoord1 = In.vTexUV + float2(texelSize * -4.0f, 0.0f);
    Out.texCoord2 = In.vTexUV + float2(texelSize * -3.0f, 0.0f);
    Out.texCoord3 = In.vTexUV + float2(texelSize * -2.0f, 0.0f);
    Out.texCoord4 = In.vTexUV + float2(texelSize * -1.0f, 0.0f);
    Out.texCoord5 = In.vTexUV + float2(texelSize * 0.0f, 0.0f);
    Out.texCoord6 = In.vTexUV + float2(texelSize * 1.0f, 0.0f);
    Out.texCoord7 = In.vTexUV + float2(texelSize * 2.0f, 0.0f);
    Out.texCoord8 = In.vTexUV + float2(texelSize * 3.0f, 0.0f);
    Out.texCoord9 = In.vTexUV + float2(texelSize * 4.0f, 0.0f);

    return Out;
}

VS_OUT_BLUR VS_MAIN_BLURY(VS_IN In)
{
    VS_OUT_BLUR Out = (VS_OUT_BLUR) 0;

    Out.vPosition = vector(In.vPosition, 1.f);
    Out.vTexUV = In.vTexUV;

    float texelSize = 1.f / 720.f / 4.f;

    Out.texCoord1 = In.vTexUV + float2(0.0f, texelSize * -4.0f);
    Out.texCoord2 = In.vTexUV + float2(0.0f, texelSize * -3.0f);
    Out.texCoord3 = In.vTexUV + float2(0.0f, texelSize * -2.0f);
    Out.texCoord4 = In.vTexUV + float2(0.0f, texelSize * -1.0f);
    Out.texCoord5 = In.vTexUV + float2(0.0f, texelSize * 0.0f);
    Out.texCoord6 = In.vTexUV + float2(0.0f, texelSize * 1.0f);
    Out.texCoord7 = In.vTexUV + float2(0.0f, texelSize * 2.0f);
    Out.texCoord8 = In.vTexUV + float2(0.0f, texelSize * 3.0f);
    Out.texCoord9 = In.vTexUV + float2(0.0f, texelSize * 4.0f);

    return Out;
}

struct PS_IN
{
    float4 vPosition : SV_POSITION;
    float2 vTexUV : TEXCOORD0;
};

struct PS_OUT
{
    float4 vHDR : SV_TARGET0;
    //float4 vHDR2 : SV_TARGET1;
    //float4 vHDR3 : SV_TARGET2;    
};

float4 PS_MAIN(PS_IN In) : SV_TARGET
{  
    float4 Color = (float4)0.f;

    Color = g_DiffuseTexture.Sample(g_DefaultSampler, In.vTexUV);   

    return Color;
}

struct PS_IN_BLUR
{
    float4 vPosition : SV_POSITION;
    float2 vTexUV : TEXCOORD0;
    float2 texCoord1 : TEXCOORD1;
    float2 texCoord2 : TEXCOORD2;
    float2 texCoord3 : TEXCOORD3;
    float2 texCoord4 : TEXCOORD4;
    float2 texCoord5 : TEXCOORD5;
    float2 texCoord6 : TEXCOORD6;
    float2 texCoord7 : TEXCOORD7;
    float2 texCoord8 : TEXCOORD8;
    float2 texCoord9 : TEXCOORD9;
};

struct PS_OUT_BLUR
{
    vector vColor : SV_TARGET0;
};

vector PS_MAIN_TEST(PS_IN In) : SV_TARGET
{
    vector vColor = (vector) 0;

    float uv = 0;

    float tu = 1.f / 1280.f;

    for (int i = -6; i < 6; ++i)
    {
        uv = In.vTexUV + float2(tu * i, 0);
        vColor += mul(Weight[6 + i], g_DiffuseTexture.Sample(g_DefaultSampler, uv));
    }
    
    vColor = vColor / Total;

    return vColor;
}

vector PS_MAIN_TEST2(PS_IN In) : SV_TARGET
{
    vector vColor = (vector) 0;

    float uv = 0;

    float tu = 1.f / 720.f;

    for (int i = -6; i < 6; ++i)
    {
        uv = In.vTexUV + float2(0, tu * i);
        vColor += mul(Weight[6 + i], g_DiffuseTexture.Sample(g_DefaultSampler, uv));
    }
    
    vColor = vColor / Total;

    return vColor;
}

PS_OUT_BLUR PS_MAIN_BLUR(PS_IN_BLUR In)
{
    PS_OUT_BLUR Out = (PS_OUT_BLUR) 0;
        
    float weight0, weight1, weight2, weight3, weight4;
    float normalization;
    //float4 color;

    weight0 = 1.0f;
    weight1 = 0.9f;
    weight2 = 0.55f;
    weight3 = 0.18f;
    weight4 = 0.1f;

    normalization = (weight0 + 2.0f * (weight1 + weight2 + weight3 + weight4));
   
    weight0 = weight0 / normalization;
    weight1 = weight1 / normalization;
    weight2 = weight2 / normalization;
    weight3 = weight3 / normalization;
    weight4 = weight4 / normalization;

    Out.vColor = float4(0.0f, 0.0f, 0.0f, 0.0f);

    Out.vColor += g_DiffuseTexture.Sample(g_DefaultSampler, In.texCoord1) * weight4;
    Out.vColor += g_DiffuseTexture.Sample(g_DefaultSampler, In.texCoord2) * weight3;
    Out.vColor += g_DiffuseTexture.Sample(g_DefaultSampler, In.texCoord3) * weight2;
    Out.vColor += g_DiffuseTexture.Sample(g_DefaultSampler, In.texCoord4) * weight1;
    Out.vColor += g_DiffuseTexture.Sample(g_DefaultSampler, In.texCoord5) * weight0;
    Out.vColor += g_DiffuseTexture.Sample(g_DefaultSampler, In.texCoord6) * weight1;
    Out.vColor += g_DiffuseTexture.Sample(g_DefaultSampler, In.texCoord7) * weight2;
    Out.vColor += g_DiffuseTexture.Sample(g_DefaultSampler, In.texCoord8) * weight3;
    Out.vColor += g_DiffuseTexture.Sample(g_DefaultSampler, In.texCoord9) * weight4;
    
    //Out.vColor.a = 1.0f;
   

    return Out;
};

technique11 DefaultDevice
{
    pass Default
    {
        SetRasterizerState(Rasterizer_Solid);
        SetDepthStencilState(DepthStecil_NotZTest, 0);
        SetBlendState(Blend_None, vector(0.f, 0.f, 0.f, 0.f), 0xffffffff);

        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN();
    }

    pass BlurX
    {
        SetRasterizerState(Rasterizer_Solid);
        SetDepthStencilState(DepthStecil_NotZTest, 0);
        SetBlendState(Blend_None, vector(0.f, 0.f, 0.f, 0.f), 0xffffffff);

        VertexShader = compile vs_5_0 VS_MAIN_BLURX();
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN_BLUR();
    }

    pass BlurY
    {
        SetRasterizerState(Rasterizer_Solid);
        SetDepthStencilState(DepthStecil_NotZTest, 0);
        SetBlendState(Blend_None, vector(0.f, 0.f, 0.f, 0.f), 0xffffffff);

        VertexShader = compile vs_5_0 VS_MAIN_BLURY();
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN_BLUR();
    }
}