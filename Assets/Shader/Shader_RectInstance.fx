
#include "Shader_Defines.hpp"

cbuffer Matrices
{
	matrix		g_WorldMatrix;
	matrix		g_ViewMatrix;
	matrix		g_ProjMatrix;

	matrix g_LightViewMatrix0;
	matrix g_LightProjMatrix0;
	matrix g_LightViewMatrix1;
	matrix g_LightProjMatrix1;
	matrix g_LightViewMatrix2;
	matrix g_LightProjMatrix2;
	matrix g_LightViewMatrix3;
	matrix g_LightProjMatrix3;
	matrix g_LightViewMatrix4;
	matrix g_LightProjMatrix4;
	matrix g_LightViewMatrix5;
	matrix g_LightProjMatrix5;
}

cbuffer LightBuffer
{
	float3 lightPosition0;
	float3 lightPosition1;
	float3 lightPosition2;
	float3 lightPosition3;
	float3 lightPosition4;
	float3 lightPosition5;

	float3 lightDir0;
	float3 lightDir1;
	float3 lightDir2;
	float3 lightDir3;
	float3 lightDir4;
	float3 lightDir5;

	float lightAngle0;
	float lightAngle1;
	float lightAngle2;
	float lightAngle3;
	float lightAngle4;
	float lightAngle5;
};

Texture2D		g_DiffuseTexture;

SamplerState	g_DiffuseSampler
{
	AddressU = mirror;
	AddressV = mirror;
};

struct VS_IN
{
	float3	vPosition : POSITION; /* ���ý����̽� */
	float2	vTexUV : TEXCOORD0;

	float4	vRight : INSTANCE0;
    float4 vUp : INSTANCE1;
    float4 vLook : INSTANCE2;
    float4 vTranslation : INSTANCE3;
};

struct VS_OUT
{
	float4	vPosition : SV_POSITION;
	float2	vTexUV : TEXCOORD0;
	float4	vViewPortPos : TEXCOORD1;
};

/* ������ �����̽� ��ȯ. (����, ��, ��������� ��.)*/
VS_OUT VS_MAIN(VS_IN In)
{
	VS_OUT		Out = (VS_OUT)0;

	matrix		matWV, matWVP;

	matrix		WorldMatrix = float4x4(In.vRight, In.vUp, In.vLook, In.vTranslation);

	matWV = mul(WorldMatrix, g_ViewMatrix);
	matWVP = mul(matWV, g_ProjMatrix);

	Out.vPosition = mul(vector(In.vPosition, 1.f), matWVP);
	Out.vTexUV = In.vTexUV;

	return Out;
}

/* VS_OUT ����ü�� SV_POSITION�̶�� �ø�ƽ�� ���� �����Ϳ� ���ؼ���  */
/* W������ XYZW��θ� ������. */
/* ����Ʈ�� ������ ��ġ�� ��ȯ�Ѵ�. */
/* �����Ͷ����� : �������� �ѷ��׿��� �ȼ��� ������ �����ϳ�.(�ѷ��׿��� ������ ������ ��������Ͽ�)  */

struct PS_IN
{
	float4	vPosition : SV_POSITION;
	float2	vTexUV : TEXCOORD0;
	float4	vViewPortPos : TEXCOORD1;
};

vector	PS_MAIN(PS_IN In) : SV_TARGET
{
	vector		vColor = (vector)0;

	vColor = g_DiffuseTexture.Sample(g_DiffuseSampler, In.vTexUV);

	//if (vColor.a < 0.3f)
	//	discard;

	return vColor;
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
}