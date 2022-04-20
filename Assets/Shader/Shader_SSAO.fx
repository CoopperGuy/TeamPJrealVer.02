
#include "Shader_Defines.hpp"

float g_fRadius = 0.001f;
float g_fFar = 300.f;
float g_fFalloff = 0.000002f;
float g_fStrength = 0.0007f;
float g_fTotStrength = 1.38f;
float g_fInvSamples = 1.f / 16.f;

Texture2D		g_DepthTexture;
Texture2D       g_NormalTexture;

float3 g_vRandom[16] =
{
    float3(0.2024537f, 0.841204f, -0.9060141f),
    float3(-0.2200423f, 0.6282339f, -0.8275437f),
    float3(0.3677659f, 0.1086345f, -0.4466777f),
    float3(0.8775856f, 0.4617546f, -0.6427765f),
    float3(0.7867433f, -0.141479f, -0.1567597f),
    float3(0.4839356f, -0.8253108f, -0.1563844f),
    float3(0.4401554f, -0.4228428f, -0.3300118f),
    float3(0.0019193f, -0.8048455f, 0.0726584f),
    float3(-0.7578573f, -0.5583301f, 0.2347527f),
    float3(-0.4540417f, -0.252365f, 0.0694318f),
    float3(-0.0483353f, -0.2527294f, 0.5924745f),
    float3(-0.4192392f, 0.2084218f, -0.3672943f),
    float3(-0.8433938f, 0.1451271f, 0.2202872f),
    float3(-0.4037157f, -0.8263387f, 0.4698132f),
    float3(-0.6657394f, 0.6298575f, 0.6342437f),
    float3(-0.0001783f, 0.2834622f, 0.8343929f)
};

struct tagSSAO_In
{
    float2 vUV;
    float fDepth;
    float fViewZ;
    float3 vNormal;
};

struct tagSSAO_Out
{
    float4 vAmbient;
};

float3 randomNormal(float2 tex)
{
    float noiseX = (frac(sin(dot(tex, float2(15.8989f, 76.132f) * 1.f)) * 46336.23745f));
    float noiseY = (frac(sin(dot(tex, float2(11.9899f, 62.223f) * 2.f)) * 34748.34744f));
    float noiseZ = (frac(sin(dot(tex, float2(13.3238f, 63.122f) * 3.f)) * 59998.47362f));
    return normalize(float3(noiseX, noiseY, noiseZ));

}

tagSSAO_Out Get_SSAO(tagSSAO_In In)
{
    tagSSAO_Out Out = (tagSSAO_Out) 0.f;

    float3 vRay;
    float3 vReflect;
    float2 vRandomUV;
    float fOccNorm;

    int iColor = 0;

    [enroll] for (int i = 0; i < 16; ++i)
    {
        vRay = reflect(randomNormal(In.vUV), g_vRandom[i]);
        vReflect = normalize(reflect(vRay, In.vNormal)) * g_fRadius;
        vReflect.x *= -1.f;
        vRandomUV = In.vUV + vReflect.xy;
        fOccNorm = g_DepthTexture.Sample(g_DefaultSampler, vRandomUV).g * g_fFar * In.fViewZ;

        if (fOccNorm <= In.fDepth + 0.003f)
            ++iColor;
    }

    Out.vAmbient = abs((iColor / 16.f) - 1);
    return Out;
}

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

float4 PS_MAIN(PS_IN In) : SV_Target
{
    float4 vDepth = g_DepthTexture.Sample(g_DefaultSampler, In.vTexUV);
    float4 vNormal = g_NormalTexture.Sample(g_DefaultSampler, In.vTexUV);

    if(vNormal.a != 0.f)
        return float4(1.f, 1.f, 1.f, 1.f);

    vNormal = normalize(vNormal * 2.f - 1.f);

    tagSSAO_In SSAO_In = (tagSSAO_In) 0;
    SSAO_In.vUV = In.vTexUV;
    SSAO_In.vNormal = vNormal.rgb;
    SSAO_In.fViewZ = vDepth.r * g_fFar;
    SSAO_In.fDepth = vDepth.g * g_fFar * SSAO_In.fViewZ;

    tagSSAO_Out Out = Get_SSAO(SSAO_In);

    return (1.f - Out.vAmbient);
}


technique11		DefaultDevice
{   
	pass SSAO
	{
		SetRasterizerState(Rasterizer_Solid);
		SetDepthStencilState(DepthStecil_NotZTest, 0);
		SetBlendState(Blend_None, vector(0.f, 0.f, 0.f, 0.f), 0xffffffff);

        VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN();
	}
}