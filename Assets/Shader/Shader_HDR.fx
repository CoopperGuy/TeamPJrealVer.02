
#include "Shader_Defines.hpp"

static const float fMiddleGray = 0.18f;
static const float fWhiteCutoff = 0.9f;

static const float Weight[13] =
{
    0.0561, 0.1353, 0.278, 0.4868, 0.7261, 0.9231,
    1, 0.9231, 0.7261, 0.4868, 0.278, 0.1353, 0.0561
};

static const float Total = 6.2108;
float g_BrightIntensity;

Texture2D		g_DiffuseTexture;

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
	VS_OUT		Out = (VS_OUT)0;

	Out.vPosition = vector(In.vPosition, 1.f);
	Out.vTexUV = In.vTexUV;

	return Out;
}

struct PS_IN
{
	float4	vPosition : SV_POSITION;
	float2	vTexUV : TEXCOORD0;
};

struct PS_OUT
{
    float4 vHDR : SV_TARGET0;
    //float4 vHDR2 : SV_TARGET1;
    //float4 vHDR3 : SV_TARGET2;    
};

PS_OUT PS_MAIN(PS_IN In)
{
    PS_OUT Out = (PS_OUT) 0;

    float4 vColor = g_DiffuseTexture.Sample(g_ClampSampler, In.vTexUV);
    if(vColor.a == 0.f)
        discard;

    float3 HDR;       
    float Luminance = 0.08f;
    
    HDR = pow(g_DiffuseTexture.Sample(g_ClampSampler, In.vTexUV), 2.2f) * fMiddleGray / (Luminance + 0.001f);
    
    HDR *= (1.f + (HDR / (fWhiteCutoff * fWhiteCutoff)));

    HDR /= (1.f + HDR);
       
    Out.vHDR = float4(pow(HDR, 1.f / 2.2f), 1.f);
    
    return Out;
}

float4 PS_MAIN_BIRGHT(PS_IN In) : SV_TARGET
{
    float4 BrightColor = (float)0.f;
    float4 FragColor = g_DiffuseTexture.Sample(g_ClampSampler, In.vTexUV);

    float BirghtNess = dot(FragColor.rgb, float3(0.2126f, 0.7152f, 0.0722f));
    if (BirghtNess > g_BrightIntensity)
       BrightColor = float4(FragColor.rgb, 1.f);
    
    return BrightColor;
}

technique11		DefaultDevice
{   
	pass Hdr
	{
		SetRasterizerState(Rasterizer_Solid);
		SetDepthStencilState(DepthStecil_NotZTest, 0);
		SetBlendState(Blend_None, vector(0.f, 0.f, 0.f, 0.f), 0xffffffff);

        VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN();
	}

    pass BirghtFilter
    {
        SetRasterizerState(Rasterizer_Solid);
        SetDepthStencilState(DepthStecil_NotZTest, 0);
        SetBlendState(Blend_None, vector(0.f, 0.f, 0.f, 0.f), 0xffffffff);

        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN_BIRGHT();
    }
}