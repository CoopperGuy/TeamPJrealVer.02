
#include "Shader_Defines.hpp"

cbuffer Matrices
{
	matrix		g_WorldMatrix;
	matrix		g_ViewMatrix;
	matrix		g_ProjMatrix;	
}

cbuffer MatricesInv
{
    matrix g_WorldMatrixInv;
    matrix g_ViewMatrixInv;
    matrix g_ProjMatrixInv;
};

Texture2D   g_DiffuseTexture;
Texture2D   g_DepthTexture;

SamplerState	g_DiffuseSampler
{
	AddressU = wrap;
	AddressV = wrap;
};

struct VS_IN
{
	float3 vPosition : POSITION; /* 로컬스페이스 */
	float3 vTexUV : TEXCOORD0;

};

struct VS_OUT
{
	float4	vPosition : SV_POSITION;
	float3	vTexUV : TEXCOORD0;
    float4 vProj : TEXCOORD1;
};

/* 정점의 스페이스 변환. (월드, 뷰, 투영행렬의 곱.)*/
VS_OUT VS_MAIN(VS_IN In)
{
	VS_OUT		Out = (VS_OUT)0;

	matrix		matWV, matWVP;

	matWV = mul(g_WorldMatrix, g_ViewMatrix);
	matWVP = mul(matWV, g_ProjMatrix);

	Out.vPosition = mul(vector(In.vPosition, 1.f), matWVP);
    Out.vProj = Out.vPosition;
	Out.vTexUV = In.vTexUV;

	return Out;
}


struct PS_IN
{
	float4	vPosition : SV_POSITION;
	float3	vTexUV : TEXCOORD0;
    float4 vProj : TEXCOORD1;

};

//float2 GetPixelUV()
//{

//    float4 vWVP = _In.wvp;

//    float2 uv = vWVP.xy / vWVP.w;
//    uv = uv * float2(0.5, -0.5) + 0.5;

//    return uv;

//}

vector	PS_MAIN(PS_IN In) : SV_TARGET
{
    float4 vColor = (float4) 0.f;

    float2 uv = In.vProj.xy / In.vProj.w;
    uv = uv * float2(0.5f, -0.5f) + 0.5f;
    float4 vPixelDepth = g_DepthTexture.Sample(g_DiffuseSampler, uv.xy);
    float fViewZ = vPixelDepth.x * 300.f;
    
    vector vLocalPos = (vector) 0.f;

    vLocalPos.x = uv.x * 2.f - 1.f;
    vLocalPos.y = uv.y * -2.f + 1.f;
    vLocalPos.z = vPixelDepth.y;
    vLocalPos.w = 1.f;

    vLocalPos = vLocalPos * fViewZ;
    vLocalPos = mul(vLocalPos, g_ProjMatrixInv);
    vLocalPos = mul(vLocalPos, g_ViewMatrixInv);
    vLocalPos = mul(vLocalPos, g_WorldMatrixInv);

    float3 ObjectAbsPos = abs(vLocalPos.xyz);
    clip(0.5f - ObjectAbsPos);

    float2 decaluv = vLocalPos.xz + 0.5f;
    vColor = g_DiffuseTexture.Sample(g_DiffuseSampler, decaluv);
    
    if (vColor.a <= 0.f)
        discard;

    return vColor;
}



technique11		DefaultDevice
{
	pass DefaultPass
	{
        SetRasterizerState(Rasterizer_Solid);
        SetDepthStencilState(DepthStecil_Default, 0);
		SetBlendState(Blend_None, vector(0.f, 0.f, 0.f, 0.f), 0xffffffff);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN();
	}	
}