
#include "Shader_Defines.hpp"

cbuffer	LightDesc
{
	vector g_vLightDir;
    vector g_vLightPos;
    float  g_fLightRange;
    float  g_fLightAngle;
    vector g_vLightDiffuse;
    vector g_vLightAmbient;
    vector g_vLightSpecular;
};

cbuffer MaterialDesc
{
    vector g_vMtrlDiffuse;
    vector g_vMtrlAmbient = vector(1.f, 1.f, 1.f, 1.f);
    vector g_vMtrlSpecular;
};

cbuffer CameraDesc
{
    vector g_vCamPosition;
};

cbuffer MatricesInv
{
    matrix g_ViewMatrixInv;
    matrix g_ProjMatrixInv;
};

// Need Specular texture
Texture2D		g_NormalTexture;
Texture2D       g_DepthTexture;
Texture2D		g_DiffuseTexture;
Texture2D       g_SpecularTexture;
Texture2D		g_ShadeTexture;
Texture2D       g_BloomOriTexture;
Texture2D       g_BloomTexture;
Texture2D       g_BloomTexture2;
Texture2D       g_BloomTexture3;
Texture2D       g_SSAOTexture;

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

VS_OUT VS_MAIN_VIEWPORT(VS_IN In)
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

vector	PS_MAIN(PS_IN In) : SV_TARGET
{
	vector		vColor = (vector)0;

	vColor = g_DiffuseTexture.Sample(g_DefaultSampler, In.vTexUV);
   
    return vColor;
}

struct PS_OUT_LIGHTACC
{
	vector vShade : SV_TARGET0;
    vector vSpecular : SV_TARGET1;
};

PS_OUT_LIGHTACC PS_MAIN_DIRECTION(PS_IN In)
{
	PS_OUT_LIGHTACC		Out = (PS_OUT_LIGHTACC)1;

	vector		vNormalDesc = g_NormalTexture.Sample(g_DefaultSampler, In.vTexUV);
    /* 0 ~ 1 */
	/* -1 ~ 1 */
	vector		vNormal = vector(vNormalDesc.xyz * 2.f - 1.f, 0.f);
    vector      vDepthDesc = g_DepthTexture.Sample(g_DefaultSampler, In.vTexUV);
    float fViewZ = vDepthDesc.x * 300.f; // far로 다시 곱한다
    float4 lightDir = -(-1.f, -1.f, -1.f, 0.f);

    Out.vShade.xyz = (g_vLightDiffuse * max(dot(normalize(g_vLightDir) * -1.f, vNormal), 0.f) + (g_vLightAmbient * g_vMtrlAmbient)).xyz;
    //Out.vShade.xyz = (g_vLightDiffuse * max(dot(normalize(lightDir) * -1.f, vNormal), 0.f) + (g_vLightAmbient * g_vMtrlAmbient)).xyz;
	
    vector vRefect = reflect(normalize(g_vLightDir), vNormal);
    //vector vRelfect = reflect(normalize(lightDir), vNormal);

    vector vWorldPos;
    vWorldPos.x = In.vTexUV.x * 2.f - 1.f;
    vWorldPos.y = In.vTexUV.y * -2.f + 1.f;
    vWorldPos.z = vDepthDesc.y; // In.vPosition.z 이 될 수 있나?
    vWorldPos.w = 1.f;

    /* 뷰스페이스상에서 튜영핼렬까지 곱해놓은 상태 */
    vWorldPos = vWorldPos * fViewZ;

    /* 뷰스페이스 */
    vWorldPos = mul(vWorldPos, g_ProjMatrixInv);

    /* 월드 */
    vWorldPos = mul(vWorldPos, g_ViewMatrixInv);

    vector vLook = vWorldPos - g_vCamPosition;

    /* 너무 세게 들어가니 pow로 조절 */    
    vector vSpecular = g_SpecularTexture.Sample(g_DefaultSampler, In.vTexUV);
    float4 vSpecularColor = (float4) 1.f * vSpecular.b;

    vSpecularColor *= pow(max(dot(normalize(vRefect) * -1.f, normalize(vLook)), 0.f), 30.f);
    Out.vSpecular = vSpecularColor;
    Out.vSpecular.a = 0;

	return Out;
}

PS_OUT_LIGHTACC PS_MAIN_POINT(PS_IN In)
{
    PS_OUT_LIGHTACC Out = (PS_OUT_LIGHTACC) 1;

    vector vNormalDesc = g_NormalTexture.Sample(g_DefaultSampler, In.vTexUV);
    vector vNormal = vector(vNormalDesc.xyz * 2.f - 1.f, 0.f);

    vector vDepthDesc = g_DepthTexture.Sample(g_DefaultSampler, In.vTexUV);
    float fViewZ = vDepthDesc.r * 300.f;

    vector vWorldPos;

    vWorldPos.x = In.vTexUV.x * 2.f - 1.f;
    vWorldPos.y = In.vTexUV.y * -2.f + 1.f;
    vWorldPos.z = vDepthDesc.y;
    vWorldPos.w = 1.f;

	/* 뷰스페이스 상에서 투영행렬까지 곱핸놓은 상태. */
    vWorldPos = vWorldPos * fViewZ;

	/* 뷰스페이스 */
    vWorldPos = mul(vWorldPos, g_ProjMatrixInv);

	/* 월드 */
    vWorldPos = mul(vWorldPos, g_ViewMatrixInv);

    vector vLook = vWorldPos - g_vCamPosition;
    vector vLightDir = vWorldPos - g_vLightPos;
    float fLength = length(vLightDir);

    float fAtt = max((g_fLightRange - fLength), 0.f) / g_fLightRange;

    Out.vShade.xyz = ((g_vLightDiffuse * max(dot(normalize(vLightDir) * -1.f, vNormal), 0.f) + (g_vLightAmbient * g_vMtrlAmbient)).xyz) * fAtt;

    
    vector vReflect = reflect(normalize(vLightDir), vNormal);

    vector vSpecular = g_SpecularTexture.Sample(g_DefaultSampler, In.vTexUV);
    float4 vSpecularColor = (float4) 1.f * vSpecular.b;

    vSpecularColor *= pow(max(dot(normalize(vReflect) * -1.f, normalize(vLook)), 0.f), 30.f) * fAtt;


    Out.vSpecular = vSpecularColor;
    Out.vSpecular.a = 0.f;

    return Out;
}


PS_OUT_LIGHTACC PS_MAIN_SPOT(PS_IN In)
{
    PS_OUT_LIGHTACC Out = (PS_OUT_LIGHTACC) 1;

    vector vNormalDesc = g_NormalTexture.Sample(g_DefaultSampler, In.vTexUV);
    vector vNormal = vector(vNormalDesc.xyz * 2.f - 1.f, 0.f);

    vector vDepthDesc = g_DepthTexture.Sample(g_DefaultSampler, In.vTexUV);
    float fViewZ = vDepthDesc.r * 300.f;

    vector vWorldPos;

    vWorldPos.x = In.vTexUV.x * 2.f - 1.f;
    vWorldPos.y = In.vTexUV.y * -2.f + 1.f;
    vWorldPos.z = vDepthDesc.y;
    vWorldPos.w = 1.f;

	/* 뷰스페이스 상에서 투영행렬까지 곱핸놓은 상태. */
    vWorldPos = vWorldPos * fViewZ;

	/* 뷰스페이스 */
    vWorldPos = mul(vWorldPos, g_ProjMatrixInv);

	/* 월드 */
    vWorldPos = mul(vWorldPos, g_ViewMatrixInv);

    vector vLightDir = vWorldPos - g_vLightPos;
    float fLength = length(vLightDir);

    float fAtt = max((g_fLightRange - fLength), 0.f) / g_fLightRange;

    Out.vShade.xyz = ((g_vLightDiffuse * max(dot(normalize(vLightDir) * -1.f, vNormal), 0.f) + (g_vLightAmbient * g_vMtrlAmbient)).xyz) * fAtt;
    
    Out.vShade.xyz *= pow(max(dot(normalize(vLightDir), g_vLightDir), 0.f), g_fLightAngle);

    vector vReflect = reflect(normalize(vLightDir), vNormal);


    vector vLook = vWorldPos - g_vCamPosition;

    Out.vSpecular = pow(max(dot(normalize(vReflect) * -1.f, normalize(vLook)), 0.f), 30.f) * (fAtt * g_fLightAngle);
    Out.vSpecular.rgb /= 10.f;
    Out.vSpecular.a = 0.f;

    return Out;
}

float4 PS_MAIN_BLEND_TOOL(PS_IN In) : SV_TARGET0
{
    float4 Out = (float4) 0;

	vector		vDiffuseColor = g_DiffuseTexture.Sample(g_DefaultSampler, In.vTexUV);
	vector		vShade = g_ShadeTexture.Sample(g_DefaultSampler, In.vTexUV);
    vector      vSpecular = g_SpecularTexture.Sample(g_DefaultSampler, In.vTexUV);
      
    Out = vDiffuseColor * vShade + vSpecular;
    if (Out.a <= 0.0f)
        discard;

	return Out;
}

float4 PS_MAIN_BLEND_CLIENT(PS_IN In) : SV_TARGET0
{
    float4 Out = (float4) 0;

    vector vDiffuseColor = g_DiffuseTexture.Sample(g_DefaultSampler, In.vTexUV);
    vector vShade = g_ShadeTexture.Sample(g_DefaultSampler, In.vTexUV);
    vector vSpecular = g_SpecularTexture.Sample(g_DefaultSampler, In.vTexUV);
    vector vSSAO = g_SSAOTexture.Sample(g_DefaultSampler, In.vTexUV);
   
    Out = vDiffuseColor * vShade * vSSAO + vSpecular;

    if (vDiffuseColor.a <= 0.0f)
        discard;

    return Out;
}

float4 PS_MAIN_BLOOM(PS_IN In) : SV_TARGET0
{
    float4 Out = (float4)0.f;

    vector vDiffuseColor = g_DiffuseTexture.Sample(g_DefaultSampler, In.vTexUV);
    vector vBloomColor = g_BloomTexture.Sample(g_DefaultSampler, In.vTexUV);
    vector vBloomColor2 = g_BloomTexture2.Sample(g_DefaultSampler, In.vTexUV);
    vector vBloomColor3 = g_BloomTexture3.Sample(g_DefaultSampler, In.vTexUV);
    vector vBloomOri = g_BloomOriTexture.Sample(g_DefaultSampler, In.vTexUV);
    
    //if (vDiffuseColor.a == 0.f)
        //discard;

    //return vDiffuseColor;

    float4 vBloom = pow(pow(abs(vBloomColor), 2.2f) + pow(abs(vBloomOri), 2.2f), 1.f / 2.2f);
    float4 vBloom2 = pow(pow(abs(vBloomColor2), 2.2f) + pow(abs(vBloomOri), 2.2f), 1.f / 2.2f);
    float4 vBloom3 = pow(pow(abs(vBloomColor3), 2.2f) + pow(abs(vBloomOri), 2.2f), 1.f / 2.2f);
    float4 vOut = vDiffuseColor;

    vOut = pow(abs(vOut), 2.2f);
    vBloom = pow(abs(vBloom), 2.2f);
    vBloom2 = pow(abs(vBloom), 2.2f);
    vBloom3 = pow(abs(vBloom), 2.2f);
    
    vOut += vBloom + vBloom2 + vBloom3;
    
    if(vOut.a <= 0.f)
        discard;

    return pow(abs(vOut), 1.f / 2.2f);
}


//BlendState Blend_None
//{
//	BlendEnable[0] = false;
//};

//DepthStencilState  DepthStecil_NotZTest
//{
//	DepthEnable = false;
//	DepthWriteMask = all;
//	DepthFunc = always;
//};

//RasterizerState Rasterizer_Solid
//{
//	FillMode = Solid;
//	CullMode = Back;
//	FrontCounterClockwise = false;
//};

technique11		DefaultDevice
{
	pass Debug
	{
		SetRasterizerState(Rasterizer_Solid);
        SetDepthStencilState(DepthStecil_NotZTest, 0);
		SetBlendState(Blend_None, vector(0.f, 0.f, 0.f, 0.f), 0xffffffff);

		VertexShader = compile vs_5_0 VS_MAIN_VIEWPORT();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN();
	}

	pass Directional
	{
		SetRasterizerState(Rasterizer_Solid);
		SetDepthStencilState(DepthStecil_NotZTest, 0);
        SetBlendState(Blend_One, vector(0.f, 0.f, 0.f, 0.f), 0xffffffff);

		VertexShader = compile vs_5_0 VS_MAIN_VIEWPORT();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN_DIRECTION();
	}

	pass Point
	{
		SetRasterizerState(Rasterizer_Solid);
		SetDepthStencilState(DepthStecil_NotZTest, 0);
		SetBlendState(Blend_One, vector(0.f, 0.f, 0.f, 0.f), 0xffffffff);

		VertexShader = compile vs_5_0 VS_MAIN_VIEWPORT();
		GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN_POINT();
    }
    pass Spot
    {
        SetRasterizerState(Rasterizer_Solid);
        SetDepthStencilState(DepthStecil_NotZTest, 0);
        SetBlendState(Blend_One, vector(0.f, 0.f, 0.f, 0.f), 0xffffffff);

        VertexShader = compile vs_5_0 VS_MAIN_VIEWPORT();
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN_SPOT();
    }

	pass Blend_Tool
	{
		SetRasterizerState(Rasterizer_Solid);
		SetDepthStencilState(DepthStecil_NotZTest, 0);
		SetBlendState(Blend_None, vector(0.f, 0.f, 0.f, 0.f), 0xffffffff);

		VertexShader = compile vs_5_0 VS_MAIN_VIEWPORT();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN_BLEND_TOOL();
	}

    pass Bloom
    {
        SetRasterizerState(Rasterizer_Solid);
        SetDepthStencilState(DepthStecil_NotZTest, 0);
        SetBlendState(Blend_None, vector(0.f, 0.f, 0.f, 0.f), 0xffffffff);

        VertexShader = compile vs_5_0 VS_MAIN_VIEWPORT();
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN_BLOOM();
    }

    pass Blend_Client
    {
        SetRasterizerState(Rasterizer_Solid);
        SetDepthStencilState(DepthStecil_NotZTest, 0);
        SetBlendState(Blend_None, vector(0.f, 0.f, 0.f, 0.f), 0xffffffff);

        VertexShader = compile vs_5_0 VS_MAIN_VIEWPORT();
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN_BLEND_CLIENT();
    }
}