
#include "Shader_Defines.hpp"

cbuffer Matrices
{
	matrix		g_WorldMatrix;
	matrix		g_ViewMatrix;
	matrix		g_ProjMatrix;
}

cbuffer Color
{
    float4 g_Color;
    float g_Alpha;
};

Texture2D		g_DiffuseTexture;
Texture2D       g_MaskTexture;

struct VS_IN
{
	float3	vPosition : POSITION; /* ���ý����̽� */
	float2	vTexUV : TEXCOORD0;

	float4	vRight : INSTANCE0;
    float4 vUp : INSTANCE1;
    float4 vLook : INSTANCE2;
    float4 vTranslation : INSTANCE3;
    int iRenderEnable : INSTANCE4;
};

struct VS_OUT
{
	float4	vPosition : SV_POSITION;
	float2	vTexUV : TEXCOORD0;
    int iRenderEnable : TEXCOORD1;
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
    Out.iRenderEnable = In.iRenderEnable;

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
    int iRenderEnable : TEXCOORD1;
};

vector	PS_MAIN(PS_IN In) : SV_TARGET
{
	vector		vColor = (vector)0;

   if (In.iRenderEnable == 0)
       discard;

    vector vMaskColor = g_MaskTexture.Sample(g_DefaultSampler, In.vTexUV);

    vColor.a = vMaskColor.r * g_Alpha;
    vColor.rgb = vMaskColor.rgb * g_Color.rgb;
    if (vColor.r > 0.99)
        vColor.rgb += 1.f;

	if (vColor.a < 0.5f)
		discard;

	return vColor;
}



vector PS_MAINLERP(PS_IN In) : SV_TARGET
{
    vector vColor = (vector) 0;

    if (In.iRenderEnable == 0)
        discard;
    
    vector vMaskColor = g_MaskTexture.Sample(g_DefaultSampler, In.vTexUV);
    vColor.a = vMaskColor.r * g_Alpha;

    vColor.rgb = (g_Color.rgb + 1.f) * vMaskColor.r;
    if (vColor.a < 0.1f)
        discard;

    return vColor;
}



technique11		DefaultDevice
{
	pass DefaultPass
	{
        SetRasterizerState(Rasterizer_NoneCull);
		SetDepthStencilState(DepthStecil_Default, 0);
		SetBlendState(Blend_Alpha, vector(0.f, 0.f, 0.f, 0.f), 0xffffffff);

		VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN();
    }	
    pass LerpPass
    {
        SetRasterizerState(Rasterizer_NoneCull);
        SetDepthStencilState(DepthStecil_Default, 0);
        SetBlendState(Blend_Alpha, vector(0.f, 0.f, 0.f, 0.f), 0xffffffff);

        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_MAINLERP();
    }
}