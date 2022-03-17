#include "Shader_Defines.hpp"

cbuffer Matrices
{
	matrix		g_WorldMatrix;
	matrix		g_ViewMatrix;
	matrix		g_ProjMatrix;	
}

cbuffer UiInformation
{
    float g_Percentage = 1.f;
    float g_Back = 1.f;
    float g_Time = 0.f;
    bool g_isHover = false;
    bool g_isSelected = false;
};

cbuffer Color
{
    float4 vColor;
};

SamplerState Sampler
{
    AddressU = mirror;
    AddressV = mirror;
};


Texture2D Diffuse;

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

	matrix		matWV, matWVP;

	matWV = mul(g_WorldMatrix, g_ViewMatrix);
	matWVP = mul(matWV, g_ProjMatrix);

	Out.vPosition = mul(vector(In.vPosition, 1.f), matWVP);
	Out.vTexUV = In.vTexUV;
	return Out;
}

struct PS_IN
{
	float4	vPosition : SV_POSITION;
	float2	vTexUV : TEXCOORD0;
};

vector	PS_MAIN(PS_IN In) : SV_TARGET
{
    float4 diffuse = Diffuse.Sample(Sampler, In.vTexUV);
    if(diffuse.a < 0.1f)
        discard;
    float4 color = diffuse;
    return color;
}

vector PS_MAIN_HPBAR(PS_IN In) : SV_TARGET
{
    float4 color = Diffuse.Sample(Sampler, In.vTexUV);
    clip(color.a < 0.1f ? -1 : 1);
    if (In.vTexUV.x > g_Percentage && In.vTexUV.x < g_Back)
    {
        color.rgb += 0.3f;
    }
    else if (In.vTexUV.x > g_Percentage)
    {
        discard;
    }
    // color += vColor;
    return color;
}

technique11		DefaultDevice
{
	pass DefaultPass
	{
        SetRasterizerState(Rasterizer_Solid);
        SetDepthStencilState(DepthStecil_Default, 0);
        SetBlendState(Blend_Alpha, vector(0.f, 0.f, 0.f, 0.f), 0xffffffff);

		VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN();
    }
    pass HpBar
    {
        SetRasterizerState(Rasterizer_Solid);
        SetDepthStencilState(DepthStecil_Default, 0);
        SetBlendState(Blend_Alpha, vector(0.f, 0.f, 0.f, 0.f), 0xffffffff);

        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN_HPBAR();
    }

}