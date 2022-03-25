
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
	float3	vPosition : POSITION; /* 로컬스페이스 */
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

/* 정점의 스페이스 변환. (월드, 뷰, 투영행렬의 곱.)*/
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

/* VS_OUT 구조체의 SV_POSITION이라는 시멘틱을 가진 데이터에 한해서만  */
/* W값으로 XYZW모두를 나눈다. */
/* 뷰포트로 정점의 위치를 변환한다. */
/* 래스터라이즈 : 정점으로 둘러쌓여진 픽셀의 정보를 생성하낟.(둘러쌓여진 정점의 정보를 기반으로하여)  */

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