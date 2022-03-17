
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

cbuffer Dissolve
{
	float g_fDissolve;
};

struct MeshBoneMatrices
{
	matrix		BoneMatrices[128];
};

MeshBoneMatrices		g_BoneMatrices;

Texture2D g_DiffuseTexture;
Texture2D g_NormalTexture;
Texture2D g_DissolveTexture;
Texture2D g_DepthTexture;
Texture2D depthMapTexture0;
Texture2D depthMapTexture1;
Texture2D depthMapTexture2;
Texture2D depthMapTexture3;
Texture2D depthMapTexture4;
Texture2D depthMapTexture5;

SamplerState	g_DiffuseSampler
{
	AddressU = wrap;
	AddressV = wrap;
};

SamplerState g_SamplerClamp
{
	Filter = min_mag_mip_point;

	AddressU = clamp;
	AddressV = clamp;
};

struct VS_IN
{
	float3	vPosition : POSITION; /* 로컬스페이스 */
	float2	vTexUV : TEXCOORD0;
	float3	vNormal : NORMAL;
	float3	vTangent : TANGENT;
	float3 vBiNormal : BINORMAL;

	float4 vRight : INSTANCE0;
	float4 vUp : INSTANCE1;
	float4 vLook : INSTANCE2;
	float4 vTranslation : INSTANCE3;
};

struct VS_OUT
{
	float4	vPosition : SV_POSITION;
	float2	vTexUV : TEXCOORD0;
	float3	vNormal : NORMAL;
	float3 vTangent : TANGENT;
	float3 vBiNormal : BINORMAL;
	float4 lightViewPosition0 : TEXCOORD1;
	float3 lightPos0 : TEXCOORD2;
	float4 lightViewPosition1 : TEXCOORD3;
	float3 lightPos1 : TEXCOORD4;
	float4 lightViewPosition2 : TEXCOORD5;
	float3 lightPos2 : TEXCOORD6;
	float4 lightViewPosition3 : TEXCOORD7;
	float3 lightPos3 : TEXCOORD8;
	float4 lightViewPosition4 : TEXCOORD9;
	float3 lightPos4 : TEXCOORD10;
	float4 lightViewPosition5 : TEXCOORD11;
	float3 lightPos5 : TEXCOORD12;

	float4 vProjPos : TEXCOORD13;
};

struct VS_OUT_LIGHT_DEPTH
{
	float4 vPosition : SV_POSITION;
	float4 vLightDepthPosition : TEXCOORD0;
	float2 vTexUV : TEXCOORD1;
	//float4 vPosition : SV_POSITION;
	//float4 vNormal : NORMAL;
	//float4 vLightDepthPosition : TEXCOORD0;
	//float2 vTexUV : TEXCOORD1;
};

/* 정점의 스페이스 변환. (월드, 뷰, 투영행렬의 곱.)*/
VS_OUT VS_MAIN(VS_IN In)
{
	VS_OUT		Out = (VS_OUT)0;

	matrix		matWV, matWVP;
	float4x4 WorldMatirx;

	WorldMatirx = float4x4(In.vRight, In.vUp, In.vLook, In.vTranslation);

	matWV = mul(WorldMatirx, g_ViewMatrix);
	matWVP = mul(matWV, g_ProjMatrix);

	/* 가시점길찾기. */

	Out.vPosition = mul(vector(In.vPosition, 1.f), matWVP);
	Out.vProjPos = Out.vPosition;
	Out.vTexUV = In.vTexUV;
	Out.vNormal = normalize(mul(vector(In.vNormal, 0.f), WorldMatirx)).xyz;
	Out.vTangent = normalize(mul(vector(In.vTangent, 0.f), WorldMatirx)).xyz;
	Out.vBiNormal = normalize(mul(vector(In.vBiNormal, 0.f), WorldMatirx)).xyz;
	//Out.vNormal = mul(vector(In.vNormal, 0.f), WorldMatirx);
	//   Out.vNormal = normalize(Out.vNormal);

	Out.lightViewPosition0 = mul(vector(In.vPosition, 1.f), WorldMatirx);
	Out.lightViewPosition0 = mul(Out.lightViewPosition0, g_LightViewMatrix0);
	Out.lightViewPosition0 = mul(Out.lightViewPosition0, g_LightProjMatrix0);

	Out.lightViewPosition1 = mul(vector(In.vPosition, 1.f), WorldMatirx);
	Out.lightViewPosition1 = mul(Out.lightViewPosition1, g_LightViewMatrix1);
	Out.lightViewPosition1 = mul(Out.lightViewPosition1, g_LightProjMatrix1);

	Out.lightViewPosition2 = mul(vector(In.vPosition, 1.f), WorldMatirx);
	Out.lightViewPosition2 = mul(Out.lightViewPosition2, g_LightViewMatrix2);
	Out.lightViewPosition2 = mul(Out.lightViewPosition2, g_LightProjMatrix2);

	Out.lightViewPosition3 = mul(vector(In.vPosition, 1.f), WorldMatirx);
	Out.lightViewPosition3 = mul(Out.lightViewPosition3, g_LightViewMatrix3);
	Out.lightViewPosition3 = mul(Out.lightViewPosition3, g_LightProjMatrix3);

	Out.lightViewPosition4 = mul(vector(In.vPosition, 1.f), WorldMatirx);
	Out.lightViewPosition4 = mul(Out.lightViewPosition4, g_LightViewMatrix4);
	Out.lightViewPosition4 = mul(Out.lightViewPosition4, g_LightProjMatrix4);

	Out.lightViewPosition5 = mul(vector(In.vPosition, 1.f), WorldMatirx);
	Out.lightViewPosition5 = mul(Out.lightViewPosition5, g_LightViewMatrix5);
	Out.lightViewPosition5 = mul(Out.lightViewPosition5, g_LightProjMatrix5);

	float4 worldPosition = mul(vector(In.vPosition, 1.f), WorldMatirx);
	Out.lightPos0 = lightPosition0.xyz - worldPosition.xyz;
	Out.lightPos1 = lightPosition1.xyz - worldPosition.xyz;
	Out.lightPos2 = lightPosition2.xyz - worldPosition.xyz;
	Out.lightPos3 = lightPosition3.xyz - worldPosition.xyz;
	Out.lightPos4 = lightPosition4.xyz - worldPosition.xyz;
	Out.lightPos5 = lightPosition5.xyz - worldPosition.xyz;

	return Out;
}

VS_OUT_LIGHT_DEPTH VS_MAIN_LIGHT_DEPTH(VS_IN In)
{
	VS_OUT_LIGHT_DEPTH Out = (VS_OUT_LIGHT_DEPTH)0;

	matrix matLightWV, matLightWVP;

	float4x4 WorldMatirx;
	WorldMatirx = float4x4(In.vRight, In.vUp, In.vLook, In.vTranslation);

	matLightWV = mul(WorldMatirx, g_LightViewMatrix0);
	matLightWVP = mul(matLightWV, g_LightProjMatrix0);
	Out.vPosition = mul(vector(In.vPosition, 1.f), matLightWVP);

	Out.vLightDepthPosition = Out.vPosition;
	Out.vTexUV = In.vTexUV;
	return Out;
}

struct PS_IN
{
	float4	vPosition : SV_POSITION;
	float2	vTexUV : TEXCOORD0;
	float3	vNormal : NORMAL;
	float3 vTangent : TANGENT;
	float3 vBiNormal : BINORMAL;
	float4 lightViewPosition0 : TEXCOORD1;
	float3 lightPos0 : TEXCOORD2;
	float4 lightViewPosition1 : TEXCOORD3;
	float3 lightPos1 : TEXCOORD4;
	float4 lightViewPosition2 : TEXCOORD5;
	float3 lightPos2 : TEXCOORD6;
	float4 lightViewPosition3 : TEXCOORD7;
	float3 lightPos3 : TEXCOORD8;
	float4 lightViewPosition4 : TEXCOORD9;
	float3 lightPos4 : TEXCOORD10;
	float4 lightViewPosition5 : TEXCOORD11;
	float3 lightPos5 : TEXCOORD12;



	float4 vProjPos : TEXCOORD13;
};
struct PS_IN_LIGHT_DEPTH
{
	float4 vPosition : SV_POSITION;
	float4 vLightDepthPosition : TEXCOORD0;
	float2 vTexUV : TEXCOORD1;
};
struct PS_OUT
{
	vector	vDiffuse : SV_TARGET0;
	vector	vNormal : SV_TARGET1;
	vector vDepth : SV_TARGET2;

};

struct PS_OUT_LIGHT_DEPTH
{
	vector vLightDepth : SV_TARGET0;
};

struct PS_OUT_BEHIND_WALL
{
	vector vDiffuse : SV_TARGET0;

};


PS_OUT	PS_MAIN(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;

	float bias;
	float4 color;
	float4 diffuseColor;
	float2 projectTexCoord;
	float depthValue;
	float lightDepthValue;
	float lightIntensity;
	float4 textureColor;
	float3 lightDir;

	bias = 0.00001f;

	color = vector(0.1f, 0.1f, 0.1f, 1.f);
	diffuseColor = vector(0.4f, 0.4f, 0.4f, 1.f);
	lightDir = -(-1.f, -1.f, -1.f);


	projectTexCoord.x = In.lightViewPosition0.x / In.lightViewPosition0.w / 2.f + 0.5f;
	projectTexCoord.y = -In.lightViewPosition0.y / In.lightViewPosition0.w / 2.f + 0.5f;

	if ((saturate(projectTexCoord.x) == projectTexCoord.x) && (saturate(projectTexCoord.y) == projectTexCoord.y))
	{
		depthValue = depthMapTexture0.Sample(g_SamplerClamp, projectTexCoord).x;

		lightDepthValue = In.lightViewPosition0.z / In.lightViewPosition0.w;
		lightDepthValue = lightDepthValue - bias;

		color = vector(0.7, 0.7f, 0.7f, 1.f);

		if (lightDepthValue < depthValue)
		{
			color = vector(1.f, 1.f, 1.f, 1.f);

			//// 이 픽셀의 빛의 양을 계산합니다.
			//lightIntensity = saturate(dot(In.vNormal, lightDir));

			//if (lightIntensity > 0.0f)
			//{
			//    // 확산 색과 광 강도의 양에 따라 최종 확산 색을 결정합니다.
			//    color += (diffuseColor * lightIntensity);

			//    // 최종 빛의 색상을 채웁니다.
			//    color = saturate(color);
			//}
		}

	}
	else
	{
		color = vector(1.f, 1.f, 1.f, 1.f);
		//lightIntensity = saturate(dot(In.vNormal, lightDir));
		//if (lightIntensity > 0.0f)
		//{
		//        // 확산 색과 광 강도의 양에 따라 최종 확산 색을 결정합니다.
		//    color += (diffuseColor * lightIntensity);
		//    color = saturate(color);
		//}
	}

	color = saturate(color);
	// 이 텍스처 좌표 위치에서 샘플러를 사용하여 텍스처에서 픽셀 색상을 샘플링합니다.
	textureColor = g_DiffuseTexture.Sample(g_DiffuseSampler, In.vTexUV);

	color = color * textureColor;


	if (color.a < 0.8f)
		discard;


	Out.vDiffuse = color;

	vector vNormalDesc = g_NormalTexture.Sample(g_DiffuseSampler, In.vTexUV);
	float3 vNormal = vNormalDesc.xyz * 2.f - 1.f;
	float3x3 TBN = float3x3(In.vTangent, In.vBiNormal, In.vNormal);
	vNormal = mul(vNormal, TBN);

	Out.vNormal = vector(In.vNormal.xyz * 0.5f + 0.5f, 1.0f);
	Out.vDepth = vector(In.vProjPos.w / 300.f, In.vProjPos.z / In.vProjPos.w, 0.f, 0.f);

	//Out.vDiffuse = g_DiffuseTexture.Sample(g_DiffuseSampler, In.vTexUV);	

	//   Out.vDiffuse.a = (Out.vDiffuse.r + Out.vDiffuse.g + Out.vDiffuse.b / 3.f) * 0.5f;
	//   if (Out.vDiffuse.a==0)
	//       discard;

	///* In.vNormal.xyz : -1 ~ 1 */ 
	///* Out.vNormal.xyz : 0 ~ 1 */

	//Out.vNormal = vector(In.vNormal * 0.5f + 0.5f, 0.f);

	return Out;
}

PS_OUT_LIGHT_DEPTH PS_MAIN_LIGHT_DEPTH(PS_IN_LIGHT_DEPTH In)
{
	PS_OUT_LIGHT_DEPTH Out = (PS_OUT_LIGHT_DEPTH)0;

	//float4 textureColor = g_DiffuseTexture.Sample(g_DiffuseSampler, In.vTexUV);
	//if (textureColor.a < 1.f)
	//    Out.vLightDepth = vector(1.f, 0.f, 0.f, 0.f);
	//else
	Out.vLightDepth = vector(In.vLightDepthPosition.z / In.vLightDepthPosition.w, 0.f, 0.f, 1.f);
	return Out;
}


technique11		DefaultDevice
{
	pass NonAnimation
	{
		SetRasterizerState(Rasterizer_Solid);
		SetDepthStencilState(DepthStecil_Default, 0);
		SetBlendState(Blend_None, vector(0.f, 0.f, 0.f, 0.f), 0xffffffff);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN();
	}

	pass LightDepth
	{
		SetRasterizerState(Rasterizer_Solid);
		SetDepthStencilState(DepthStecil_Default, 0);
		SetBlendState(Blend_None, vector(0.f, 0.f, 0.f, 0.f), 0xffffffff);

		VertexShader = compile vs_5_0 VS_MAIN_LIGHT_DEPTH();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN_LIGHT_DEPTH();
	}
}