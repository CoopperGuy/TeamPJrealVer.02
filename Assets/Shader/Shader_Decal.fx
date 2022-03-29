
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

float g_fFadeAlpha;
float4 g_vOffsetColor;

Texture2D   g_DiffuseTexture;
Texture2D   g_MaskTexture;
Texture2D   g_DepthTexture;

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
    float4 vMask = (float4) 0.f;

    float2 uv = In.vProj.xy / In.vProj.w;
    uv = uv * float2(0.5f, -0.5f) + 0.5f;
    float4 vPixelDepth = g_DepthTexture.Sample(g_DefaultSampler, uv.xy);
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
    vMask = g_MaskTexture.Sample(g_DefaultSampler, decaluv);
    vColor = g_DiffuseTexture.Sample(g_DefaultSampler, decaluv);
    
    vMask.a = (vMask.r + vMask.g + vMask.b) / 3.f;
    if (vMask.a <= 0.1f)
        discard;

    vColor.rgb += g_vOffsetColor.rgb;
    vColor.a = vMask.a + g_vOffsetColor.a;
    vColor.a *= g_fFadeAlpha;    
    
    if (vColor.a <= 0.f)
        discard;

    if (vMask.r > 0.99f)
        vColor.rgb += 0.5f;

    return vColor;
}

vector PS_MAIN_REDUP(PS_IN In) : SV_TARGET
{
    float4 vColor = (float4) 0.f;
    float4 vMask = (float4) 0.f;

    float2 uv = In.vProj.xy / In.vProj.w;
    uv = uv * float2(0.5f, -0.5f) + 0.5f;
    float4 vPixelDepth = g_DepthTexture.Sample(g_DefaultSampler, uv.xy);
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
    vMask = g_MaskTexture.Sample(g_DefaultSampler, decaluv);
    vColor = g_DiffuseTexture.Sample(g_DefaultSampler, decaluv);

    vColor.a = (vMask.r + vMask.g + vMask.b) / 3.f;
    if (vColor.a <= 0.1f)
        discard;

    vColor.a = vColor.a * g_fFadeAlpha;
    vColor.r = 1.f;
    vColor.gb = vMask.g;
    

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
  
    pass RedUpPass
    {
        SetRasterizerState(Rasterizer_Solid);
        SetDepthStencilState(DepthStecil_Default, 0);
        SetBlendState(Blend_None, vector(0.f, 0.f, 0.f, 0.f), 0xffffffff);

        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN_REDUP();
    }

}