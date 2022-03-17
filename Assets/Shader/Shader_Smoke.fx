
#include "Shader_Defines.hpp"

cbuffer Matrices
{
	matrix		g_WorldMatrix;
	matrix		g_ViewMatrix;
	matrix		g_ProjMatrix;	
}
cbuffer UVCoords
{
    uint i_indexUV;
};

Texture2D		g_Texture;

SamplerState	g_DiffuseSampler
{
	AddressU = mirror;
	AddressV = mirror;
};

struct VS_IN
{
	float3	vPosition : POSITION; /* ���ý����̽� */
    float2 vTexUV : TEXCOORD0;

	float4	vRight : INSTANCE0;
    float4 vUp : INSTANCE1;
    float4 vLook : INSTANCE2;
    float4 vTranslation : INSTANCE3;
    float  iStartFrame : INSTANCE4;
};

struct VS_OUT
{
	float4	vPosition : SV_POSITION;
    float2 vTexUV : TEXCOORD0;
};

/* ������ �����̽� ��ȯ. (����, ��, ��������� ��.)*/
VS_OUT VS_MAIN(VS_IN In)
{
	VS_OUT		Out = (VS_OUT)0;

	matrix		matWV, matWVP;

	// matrix		WorldMatrix = float4x4(In.vRight, In.vUp, In.vLook, In.vTranslation);
    matrix WorldMatrix = float4x4(In.vRight, In.vUp, In.vLook, In.vTranslation);

    matWV = mul(WorldMatrix, g_ViewMatrix);
	matWVP = mul(matWV, g_ProjMatrix);

    uint uvFactor = 8;
	Out.vPosition = mul(vector(In.vPosition, 1.f), matWVP);
    float2 tileSize = float2(1.0 / uvFactor, 1.0 / uvFactor);
    float2 tileXY = float2(((uint) In.iStartFrame) % uvFactor, ((uint) In.iStartFrame) / uvFactor);

  // Scale texture coordinates and add offset.
    float2 tileUV = In.vTexUV * tileSize + tileXY * tileSize;
    Out.vTexUV = tileUV;
   //  Out.vTexUV.y = In.vTexUV.y + (In.iStartFrame * 0.05f);

	return Out;
}

VS_OUT VS_MAIN66(VS_IN In)
{
    VS_OUT Out = (VS_OUT) 0;

    matrix matWV, matWVP;

	// matrix		WorldMatrix = float4x4(In.vRight, In.vUp, In.vLook, In.vTranslation);
    matrix WorldMatrix = float4x4(In.vRight, In.vUp, In.vLook, In.vTranslation);

    matWV = mul(g_WorldMatrix, g_ViewMatrix);
    matWVP = mul(matWV, g_ProjMatrix);

    uint uvFactor = 6;
    Out.vPosition = mul(vector(In.vPosition, 1.f), matWVP);
    float2 tileSize = float2(1.0 / uvFactor, 1.0 / uvFactor);
    float2 tileXY = float2(((uint) In.iStartFrame) % uvFactor, ((uint) In.iStartFrame) / uvFactor);

  // Scale texture coordinates and add offset.
    float2 tileUV = In.vTexUV * tileSize + tileXY * tileSize;
    Out.vTexUV = tileUV;
   //  Out.vTexUV.y = In.vTexUV.y + (In.iStartFrame * 0.05f);

    return Out;
}


/* VS_OUT ����ü�� SV_POSITION�̶�� �ø�ƽ�� ���� �����Ϳ� ���ؼ���  */
/* W������ XYZW��θ� ������. */
/* ����Ʈ�� ������ ��ġ�� ��ȯ�Ѵ�. */
/* �����Ͷ����� : �������� �ѷ��׿��� �ȼ��� ������ �����ϳ�.(�ѷ��׿��� ������ ������ ��������Ͽ�)  */

struct PS_IN
{
    float4 vPosition : SV_POSITION;
    float2 vTexUV : TEXCOORD0;
};

vector	PS_MAIN(PS_IN In) : SV_TARGET
{
	vector		vColor = (vector)0;
    vColor = g_Texture.Sample(g_DiffuseSampler, In.vTexUV);

	return vColor;
}

vector PS_MAIN_IMPACT(PS_IN In) : SV_TARGET
{
    vector vColor = (vector) 0;
    vColor = g_Texture.Sample(g_DiffuseSampler, In.vTexUV);
    vColor.a -= 0.9f;
    vColor.rgb += 0.6f;
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
    pass Impact
    {
        SetRasterizerState(Rasterizer_Solid);
        SetDepthStencilState(DepthStecil_Default, 0);
        SetBlendState(Blend_Alpha, vector(0.f, 0.f, 0.f, 0.f), 0xffffffff);

        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN_IMPACT();
    }
    pass Muzzle
    {
        SetRasterizerState(Rasterizer_Sky);
        SetDepthStencilState(DepthStecil_Default, 0);
        SetBlendState(Blend_Alpha, vector(0.f, 0.f, 0.f, 0.f), 0xffffffff);

        VertexShader = compile vs_5_0 VS_MAIN66();
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN();
    }
}