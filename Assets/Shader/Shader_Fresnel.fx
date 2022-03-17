#include "Shader_Defines.hpp"

cbuffer Matrices
{
	matrix		g_WorldMatrix;
	matrix		g_ViewMatrix;
	matrix		g_ProjMatrix;	
}

cbuffer Color
{
    float4 vColor;
    float fPower;
};

cbuffer CameraDesc
{
    vector g_vCamPosition;
}

Texture2D g_DiffuseTexture;
SamplerState g_DiffuseSampler
{
    AddressU = mirror;
    AddressV = mirror;
};
TextureCube Reflection;
SamplerState g_CubemapSampler
{
    Filter = MIN_MAG_MIP_LINEAR;
    AddressU = Wrap;
    AddressV = Wrap;
};

//TextureCube Reflection;
//sampler ReflectionSampler = sampler_state
//{
//    Texture = <Reflection>;
//};

float Reflectivity = 50.0f;
float Rzero = 1.0f;

struct VS_IN
{
    float3 vPosition : POSITION; /* 로컬스페이스 */
    float3 vNormal : NORMAL;
    float2 vTexUV : TEXCOORD0;
    float3 vTangent : TANGENT;
    uint4 vBlendIndex : BLENDINDEX;
    float4 vBlendWeight : BLENDWEIGHT;
};

struct VS_OUT
{
	float4	vPosition : SV_POSITION;
    float3 vNormal : NORMAL;
	float2	vTexUV : TEXCOORD0;
    float3 ViewVec : TANGENT;
};

/* 정점의 스페이스 변환. (월드, 뷰, 투영행렬의 곱.)*/
VS_OUT VS_MAIN(VS_IN In)
{
	VS_OUT		Out = (VS_OUT)0;
    
    matrix matWV, matWVP;

    matWV = mul(g_WorldMatrix, g_ViewMatrix);
    matWVP = mul(matWV, g_ProjMatrix);


    Out.vPosition = mul(vector(In.vPosition, 1.f), matWVP);
    Out.vNormal = mul(vector(In.vNormal, 0.f), g_WorldMatrix).xyz;
   // Out.ViewVec = -normalize(mul(vector(In.vPosition, 1.f), matWV).xyz);
    // Out.ViewVec = mul(vector(In.vPosition, 1.f), matWV).xyz;
    vector vWorldPos = mul(vector(In.vPosition, 1.f), g_WorldMatrix);
    Out.ViewVec = -normalize(g_vCamPosition - vWorldPos).xyz;
    Out.vTexUV = In.vTexUV;


	return Out;
}

/* VS_OUT 구조체의 SV_POSITION이라는 시멘틱을 가진 데이터에 한해서만  */
/* W값으로 XYZW모두를 나눈다. */
/* 뷰포트로 정점의 위치를 변환한다. */
/* 래스터라이즈 : 정점으로 둘러쌓여진 픽셀의 정보를 생성하낟.(둘러쌓여진 정점의 정보를 기반으로하여)  */

struct PS_IN
{
    float4 vPosition : SV_POSITION;
    float3 vNormal : NORMAL;
    float2 vTexUV : TEXCOORD0;
    float3 ViewVec : TANGENT;
};

vector	PS_MAIN(PS_IN In) : SV_TARGET
{
    In.vNormal = normalize(In.vNormal);
    In.ViewVec = normalize(In.ViewVec);

    float3 reflVec = reflect(In.ViewVec, In.vNormal);
    // float4 reflection = texCUBE(ReflectionSampler, reflVec);
   //float4 reflection = Reflection.Sample(g_CubemapSampler, reflVec);
    float4 reflection = { 0.1f, 0.4f, 0.1f, 1.f };
    reflection = mul(reflection, vector(reflVec, 0.f));


    // vector t = g_DiffuseTexture.Sample(g_DiffuseSampler, In.vTexUV);
    vector t = vector(vColor.xyz, 0.2f);

   //  float4 fresnel = /*Rzero + (1.0f - Rzero) **/pow(abs(1.0f + dot(In.ViewVec, In.vNormal)), 0.03f);
   // float4 result = /*t + */lerp(t, reflection, fresnel);

    float fresnel = pow(saturate(dot(normalize(In.ViewVec), normalize(reflVec))), fPower);
    fresnel = clamp(fresnel, 0.f, 0.8f);
    float4 result = t;
    result.a += fresnel;
    // result.a = 1;
    return result;
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