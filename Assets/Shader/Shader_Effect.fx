#include "Shader_Defines.hpp"

cbuffer Trail
{
    float g_AlphaSet;
    float g_ProcessTime;
    float g_UVSpd = 1.f;
};

cbuffer Matrices
{
    matrix g_WorldMatrix;
    matrix g_ViewMatrix;
    matrix g_ProjMatrix;
}

cbuffer NoiseBuffer
{
    float g_fFrameTime;
    float3 g_vScrollSpeedX;
    float3 g_vScrollSpeedY;
    float3 g_vScale;
    float g_fPadding;
};

cbuffer DistortionBuffer
{
    float2 g_vDistortion[3];
    float g_fDistortionScale;
    float g_fDistortionBias;
};

cbuffer EffectBuffer
{
    float4 g_vOffsetColor;
    float g_fFadeAlpha;
	float g_OneSideTime;
    uint g_iSpriteNum;
	uint g_iSpriteNumX;
	uint g_iSpriteNumY;
};

Texture2D g_DiffuseTexture;
Texture2D g_MaskTexture;
Texture2D g_NoiseTexture;
Texture2D g_NormalTexture;
Texture2D g_HDRTexture;

struct VS_IN
{
    float3 vPosition : POSITION;
    float2 vTexUV : TEXCOORD0;
};

struct VS_OUT
{
    float4 vPosition : SV_POSITION;
    float2 vTexUV : TEXCOORD0;
};

struct VS_OUT_SPRITE
{
    float4 vPosition : SV_POSITION;
    float2 vTexUV : TEXCOORD0;
    float2 vMaskUV : TEXCOORD1;
};

struct VS_OUT_TEST
{
    float4 vPosition : SV_POSITION;
    float2 vTexUV : TEXCOORD0;
    float2 vTexCoord1 : TEXCOORD1;
    float2 vTexCoord2 : TEXCOORD2;
    float2 vTexCoord3 : TEXCOORD3;
};

struct VS_OUT_TRAIL
{
    float4 vPosition : SV_POSITION;
    float2 vTexUV : TEXCOORD0;
    float2 vTexCoord1 : TEXCOORD1;
    float2 vTexCoord2 : TEXCOORD2;
    float2 vTexCoord3 : TEXCOORD3;
    float4 vProj : TEXCOORD4;
};

VS_OUT VS_MAIN(VS_IN In)
{
    VS_OUT Out = (VS_OUT) 0;

    matrix matWV, matWVP;

    matWV = mul(g_WorldMatrix, g_ViewMatrix);
    matWVP = mul(matWV, g_ProjMatrix);

    Out.vPosition = mul(vector(In.vPosition, 1.f), matWVP);
    Out.vTexUV = In.vTexUV;
    return Out;
}

VS_OUT_TEST VS_MAIN_TEST(VS_IN In)
{
    VS_OUT_TEST Out = (VS_OUT_TEST) 0;

    matrix matWV, matWVP;

    matWV = mul(g_WorldMatrix, g_ViewMatrix);
    matWVP = mul(matWV, g_ProjMatrix);

    Out.vPosition = mul(vector(In.vPosition, 1.f), matWVP);

    /* TexUV */    
    Out.vTexUV = In.vTexUV;

    Out.vTexCoord1 = In.vTexUV * g_vScale.x;
    Out.vTexCoord1.x = Out.vTexCoord1.x + (g_fFrameTime * g_vScrollSpeedX.x);
    Out.vTexCoord1.y = Out.vTexCoord1.y + (g_fFrameTime * g_vScrollSpeedY.x);

    Out.vTexCoord2 = In.vTexUV * g_vScale.y;
    Out.vTexCoord2.x = Out.vTexCoord2.x + (g_fFrameTime * g_vScrollSpeedX.y);
    Out.vTexCoord2.y = Out.vTexCoord2.y + (g_fFrameTime * g_vScrollSpeedY.y);

    Out.vTexCoord3 = In.vTexUV * g_vScale.z;
    Out.vTexCoord3.x = Out.vTexCoord3.x + (g_fFrameTime * g_vScrollSpeedX.z);
    Out.vTexCoord3.y = Out.vTexCoord3.y + (g_fFrameTime * g_vScrollSpeedY.z);

    return Out;
}


VS_OUT_TEST VS_MAIN_TESTUVMOVE(VS_IN In)
{
    VS_OUT_TEST Out = (VS_OUT_TEST) 0;

    matrix matWV, matWVP;

    matWV = mul(g_WorldMatrix, g_ViewMatrix);
    matWVP = mul(matWV, g_ProjMatrix);

    Out.vPosition = mul(vector(In.vPosition, 1.f), matWVP);

    /* TexUV */    
    Out.vTexUV.x = In.vTexUV.x;
    Out.vTexUV.y = In.vTexUV.y + cos(g_ProcessTime * g_UVSpd);

    Out.vTexCoord1 = In.vTexUV * g_vScale.x;
    Out.vTexCoord1.x = Out.vTexCoord1.x + (g_fFrameTime * g_vScrollSpeedX.x);
    Out.vTexCoord1.y = Out.vTexCoord1.y + (g_fFrameTime * g_vScrollSpeedY.x);

    Out.vTexCoord2 = In.vTexUV * g_vScale.y;
    Out.vTexCoord2.x = Out.vTexCoord2.x + (g_fFrameTime * g_vScrollSpeedX.y);
    Out.vTexCoord2.y = Out.vTexCoord2.y + (g_fFrameTime * g_vScrollSpeedY.y);

    Out.vTexCoord3 = In.vTexUV * g_vScale.z;
    Out.vTexCoord3.x = Out.vTexCoord3.x + (g_fFrameTime * g_vScrollSpeedX.z);
    Out.vTexCoord3.y = Out.vTexCoord3.y + (g_fFrameTime * g_vScrollSpeedY.z);

    return Out;
}

VS_OUT_TEST VS_MAIN_TESTUVMOVEX(VS_IN In)
{
    VS_OUT_TEST Out = (VS_OUT_TEST) 0;

    matrix matWV, matWVP;

    matWV = mul(g_WorldMatrix, g_ViewMatrix);
    matWVP = mul(matWV, g_ProjMatrix);

    Out.vPosition = mul(vector(In.vPosition, 1.f), matWVP);

    /* TexUV */    
    Out.vTexUV.x = In.vTexUV.x + cos(g_ProcessTime * g_UVSpd);
    Out.vTexUV.y = In.vTexUV.y;

    Out.vTexCoord1 = In.vTexUV * g_vScale.x;
    Out.vTexCoord1.x = Out.vTexCoord1.x + (g_fFrameTime * g_vScrollSpeedX.x);
    Out.vTexCoord1.y = Out.vTexCoord1.y + (g_fFrameTime * g_vScrollSpeedY.x);

    Out.vTexCoord2 = In.vTexUV * g_vScale.y;
    Out.vTexCoord2.x = Out.vTexCoord2.x + (g_fFrameTime * g_vScrollSpeedX.y);
    Out.vTexCoord2.y = Out.vTexCoord2.y + (g_fFrameTime * g_vScrollSpeedY.y);

    Out.vTexCoord3 = In.vTexUV * g_vScale.z;
    Out.vTexCoord3.x = Out.vTexCoord3.x + (g_fFrameTime * g_vScrollSpeedX.z);
    Out.vTexCoord3.y = Out.vTexCoord3.y + (g_fFrameTime * g_vScrollSpeedY.z);

    return Out;
}

VS_OUT_TEST VS_MAIN_TESTUVMOVEYSpeed(VS_IN In)
{
	VS_OUT_TEST Out = (VS_OUT_TEST)0;

	matrix matWV, matWVP;

	matWV = mul(g_WorldMatrix, g_ViewMatrix);
	matWVP = mul(matWV, g_ProjMatrix);

	Out.vPosition = mul(vector(In.vPosition, 1.f), matWVP);

	/* TexUV */
	Out.vTexUV.x = In.vTexUV.x;
	Out.vTexUV.y = In.vTexUV.y + (cos(g_ProcessTime * g_UVSpd) * (float)g_iSpriteNumX);

	Out.vTexCoord1 = In.vTexUV * g_vScale.x;
	Out.vTexCoord1.x = Out.vTexCoord1.x + (g_fFrameTime * g_vScrollSpeedX.x);
	Out.vTexCoord1.y = Out.vTexCoord1.y + (g_fFrameTime * g_vScrollSpeedY.x);

	Out.vTexCoord2 = In.vTexUV * g_vScale.y;
	Out.vTexCoord2.x = Out.vTexCoord2.x + (g_fFrameTime * g_vScrollSpeedX.y);
	Out.vTexCoord2.y = Out.vTexCoord2.y + (g_fFrameTime * g_vScrollSpeedY.y);

	Out.vTexCoord3 = In.vTexUV * g_vScale.z;
	Out.vTexCoord3.x = Out.vTexCoord3.x + (g_fFrameTime * g_vScrollSpeedX.z);
	Out.vTexCoord3.y = Out.vTexCoord3.y + (g_fFrameTime * g_vScrollSpeedY.z);

	return Out;
}

VS_OUT_TEST VS_MAIN_TESTUVMOVEY(VS_IN In)
{
	VS_OUT_TEST Out = (VS_OUT_TEST)0;

	matrix matWV, matWVP;

	matWV = mul(g_WorldMatrix, g_ViewMatrix);
	matWVP = mul(matWV, g_ProjMatrix);

	Out.vPosition = mul(vector(In.vPosition, 1.f), matWVP);
	
	/* TexUV */
	Out.vTexUV.x = In.vTexUV.x;
	Out.vTexUV.y = In.vTexUV.y + g_OneSideTime;

	Out.vTexCoord1 = In.vTexUV * g_vScale.x;
	Out.vTexCoord1.x = Out.vTexCoord1.x + (g_fFrameTime * g_vScrollSpeedX.x);
	Out.vTexCoord1.y = Out.vTexCoord1.y + (g_fFrameTime * g_vScrollSpeedY.x);

	Out.vTexCoord2 = In.vTexUV * g_vScale.y;
	Out.vTexCoord2.x = Out.vTexCoord2.x + (g_fFrameTime * g_vScrollSpeedX.y);
	Out.vTexCoord2.y = Out.vTexCoord2.y + (g_fFrameTime * g_vScrollSpeedY.y);

	Out.vTexCoord3 = In.vTexUV * g_vScale.z;
	Out.vTexCoord3.x = Out.vTexCoord3.x + (g_fFrameTime * g_vScrollSpeedX.z);
	Out.vTexCoord3.y = Out.vTexCoord3.y + (g_fFrameTime * g_vScrollSpeedY.z);

	return Out;
}

VS_OUT_TEST VS_MAIN_UVHALF(VS_IN In)
{
    VS_OUT_TEST Out = (VS_OUT_TEST) 0;

    matrix matWV, matWVP;

    matWV = mul(g_WorldMatrix, g_ViewMatrix);
    matWVP = mul(matWV, g_ProjMatrix);

    Out.vPosition = mul(vector(In.vPosition, 1.f), matWVP);

    /* TexUV */    
    Out.vTexUV.x = In.vTexUV.x;
    Out.vTexUV.y = In.vTexUV.y * 0.5f - 0.5f;

    Out.vTexCoord1 = In.vTexUV * g_vScale.x;
    Out.vTexCoord1.x = Out.vTexCoord1.x + (g_fFrameTime * g_vScrollSpeedX.x);
    Out.vTexCoord1.y = Out.vTexCoord1.y + (g_fFrameTime * g_vScrollSpeedY.x);

    Out.vTexCoord2 = In.vTexUV * g_vScale.y;
    Out.vTexCoord2.x = Out.vTexCoord2.x + (g_fFrameTime * g_vScrollSpeedX.y);
    Out.vTexCoord2.y = Out.vTexCoord2.y + (g_fFrameTime * g_vScrollSpeedY.y);

    Out.vTexCoord3 = In.vTexUV * g_vScale.z;
    Out.vTexCoord3.x = Out.vTexCoord3.x + (g_fFrameTime * g_vScrollSpeedX.z);
    Out.vTexCoord3.y = Out.vTexCoord3.y + (g_fFrameTime * g_vScrollSpeedY.z);

    return Out;
}

VS_OUT_TEST VS_MAIN_FlogasUVMove(VS_IN In)
{
	VS_OUT_TEST Out = (VS_OUT_TEST)0;

	matrix matWV, matWVP;

	matWV = mul(g_WorldMatrix, g_ViewMatrix);
	matWVP = mul(matWV, g_ProjMatrix);

	Out.vPosition = mul(vector(In.vPosition, 1.f), matWVP);

	/* TexUV */
	Out.vTexUV.x = In.vTexUV.x + sin(g_fFrameTime);
	Out.vTexUV.y = In.vTexUV.y;

	Out.vTexCoord1 = In.vTexUV * g_vScale.x;
	Out.vTexCoord1.x = Out.vTexCoord1.x + (g_fFrameTime * g_vScrollSpeedX.x);
	Out.vTexCoord1.y = Out.vTexCoord1.y + (g_fFrameTime * g_vScrollSpeedY.x);

	Out.vTexCoord2 = In.vTexUV * g_vScale.y;
	Out.vTexCoord2.x = Out.vTexCoord2.x + (g_fFrameTime * g_vScrollSpeedX.y);
	Out.vTexCoord2.y = Out.vTexCoord2.y + (g_fFrameTime * g_vScrollSpeedY.y);

	Out.vTexCoord3 = In.vTexUV * g_vScale.z;
	Out.vTexCoord3.x = Out.vTexCoord3.x + (g_fFrameTime * g_vScrollSpeedX.z);
	Out.vTexCoord3.y = Out.vTexCoord3.y + (g_fFrameTime * g_vScrollSpeedY.z);

	return Out;
}

VS_OUT_TRAIL VS_MAIN_TRAIL(VS_IN In)
{
    VS_OUT_TRAIL Out = (VS_OUT_TRAIL) 0;

    //matrix matVP;

    //matVP = mul(g_ViewMatrix, g_ProjMatrix);

    //Out.vPosition = mul(vector(In.vPosition, 1.f), matVP);
    //Out.vTexUV = In.vTexUV;

    matrix matVP;

    matVP = mul(g_ViewMatrix, g_ProjMatrix);

    Out.vPosition = mul(vector(In.vPosition, 1.f), matVP);

    /* TexUV */    
    Out.vTexUV = In.vTexUV;

    Out.vTexCoord1 = In.vTexUV * g_vScale.x;
    Out.vTexCoord1.x = Out.vTexCoord1.x + (g_fFrameTime * g_vScrollSpeedX.x);
    Out.vTexCoord1.y = Out.vTexCoord1.y + (g_fFrameTime * g_vScrollSpeedY.x);

    Out.vTexCoord2 = In.vTexUV * g_vScale.y;
    Out.vTexCoord2.x = Out.vTexCoord2.x + (g_fFrameTime * g_vScrollSpeedX.y);
    Out.vTexCoord2.y = Out.vTexCoord2.y + (g_fFrameTime * g_vScrollSpeedY.y);

    Out.vTexCoord3 = In.vTexUV * g_vScale.z;
    Out.vTexCoord3.x = Out.vTexCoord3.x + (g_fFrameTime * g_vScrollSpeedX.z);
    Out.vTexCoord3.y = Out.vTexCoord3.y + (g_fFrameTime * g_vScrollSpeedY.z);

    Out.vProj = Out.vPosition;

    return Out;
}

VS_OUT_SPRITE VS_MAIN_SPRITE(VS_IN In)
{
    VS_OUT_SPRITE Out = (VS_OUT_SPRITE) 0;

    /* Postion */
    matrix matWV, matWVP;

    matWV = mul(g_WorldMatrix, g_ViewMatrix);
    matWVP = mul(matWV, g_ProjMatrix);

    Out.vPosition = mul(vector(In.vPosition, 1.f), matWVP);
    Out.vTexUV = In.vTexUV;
  //  Out.vMaskUV = In.vTexUV;

    uint UVx = 0;
    uint UVy = 0;

    UVx = g_iSpriteNum % g_iSpriteNumX;
    UVy = g_iSpriteNum / g_iSpriteNumX;


    Out.vTexUV.x = ((In.vTexUV.x + UVx) / (float)g_iSpriteNumX);
    Out.vTexUV.y = ((In.vTexUV.y + UVy) / (float)g_iSpriteNumY);
   
    return Out;
}

VS_OUT_SPRITE VS_MAIN_SPRITEMASK(VS_IN In)
{
	VS_OUT_SPRITE Out = (VS_OUT_SPRITE)0;

	/* Postion */
	matrix matWV, matWVP;

	matWV = mul(g_WorldMatrix, g_ViewMatrix);
	matWVP = mul(matWV, g_ProjMatrix);

	Out.vPosition = mul(vector(In.vPosition, 1.f), matWVP);
	Out.vTexUV = In.vTexUV;
	Out.vMaskUV = In.vTexUV;

	uint UVx = 0;
	uint UVy = 0;

	UVx = g_iSpriteNum % g_iSpriteNumX;
	UVy = g_iSpriteNum / g_iSpriteNumX;


	Out.vTexUV.x = ((In.vTexUV.x + UVx) / (float)g_iSpriteNumX);
	Out.vTexUV.y = ((In.vTexUV.y + UVy) / (float)g_iSpriteNumY);

	return Out;
}

struct PS_IN
{
    float4 vPosition : SV_POSITION;
    float2 vTexUV : TEXCOORD0;
};

struct PS_IN_SPRITE
{
    float4 vPosition : SV_POSITION;
    float2 vTexUV : TEXCOORD0;
    float2 vMaskUV : TEXCOORD1;
};

struct PS_IN_TEST
{
    float4 vPosition : SV_POSITION;
    float2 vTexUV : TEXCOORD0;
    float2 vTexCoord1 : TEXCOORD1;
    float2 vTexCoord2 : TEXCOORD2;
    float2 vTexCoord3 : TEXCOORD3;
};

struct PS_IN_TRAIL
{
    float4 vPosition : SV_POSITION;
    float2 vTexUV : TEXCOORD0;
    float2 vTexCoord1 : TEXCOORD1;
    float2 vTexCoord2 : TEXCOORD2;
    float2 vTexCoord3 : TEXCOORD3;
    float4 vProj : TEXCOORD4;
};

vector PS_MAIN(PS_IN In) : SV_TARGET
{    
    float4 DiffuseColor = float4(0.f, 0.f, 0.f, 0.f);
    DiffuseColor = g_DiffuseTexture.Sample(g_DefaultSampler, In.vTexUV);
    if (DiffuseColor.a == 0.f)
        discard;

    return DiffuseColor;
}

vector PS_MAIN_FIRE(PS_IN_TEST In) : SV_TARGET
{
    float4 vNoise[3];
    float4 vFinalNoise;
    float fPerturb;
    float2 vNoiseCoord;
    float4 vDiffuseColor;
    float4 vAlpha;

    vNoise[0] = g_NoiseTexture.Sample(g_DefaultSampler, In.vTexCoord1);
    vNoise[1] = g_NoiseTexture.Sample(g_DefaultSampler, In.vTexCoord2);
    vNoise[2] = g_NoiseTexture.Sample(g_DefaultSampler, In.vTexCoord3);
     
    vNoise[0] = (vNoise[0] - 0.5f) * 2.0f;
    vNoise[1] = (vNoise[1] - 0.5f) * 2.0f;
    vNoise[2] = (vNoise[2] - 0.5f) * 2.0f;

    vNoise[0].xy = vNoise[0].xy * g_vDistortion[0].xy;
    vNoise[1].xy = vNoise[1].xy * g_vDistortion[1].xy;
    vNoise[2].xy = vNoise[2].xy * g_vDistortion[2].xy;
    
    vFinalNoise = vNoise[0] + vNoise[1] + vNoise[2];
    fPerturb = ((1.f - In.vTexUV.y) * g_fDistortionScale) + g_fDistortionBias;
    vNoiseCoord = (vFinalNoise.xy * fPerturb) + In.vTexUV.xy;
    
    vDiffuseColor = g_DiffuseTexture.Sample(g_ClampSampler, vNoiseCoord.xy);
    vAlpha = g_MaskTexture.Sample(g_ClampSampler, vNoiseCoord.xy);
    
    vAlpha.a = ((vAlpha.r + vAlpha.g + vAlpha.b) / 3.f);
    if (vAlpha.a <= 0.f)
        discard;

    vDiffuseColor.rgb += g_vOffsetColor.rgb;
    vDiffuseColor.a = vAlpha.a + g_vOffsetColor.a;
    vDiffuseColor.a *= g_fFadeAlpha;

    if (vDiffuseColor.a <= 0.f)
        discard;
        
   
    return vDiffuseColor;
}

vector PS_MAIN_RectTexture(PS_IN_TEST In) : SV_TARGET
{
	float4 vNoise[3];
	float4 vFinalNoise;
	float fPerturb;
	float2 vNoiseCoord;
	float4 vDiffuseColor;
	float4 vAlpha;

	vNoise[0] = g_NoiseTexture.Sample(g_DefaultSampler, In.vTexCoord1);
	vNoise[1] = g_NoiseTexture.Sample(g_DefaultSampler, In.vTexCoord2);
	vNoise[2] = g_NoiseTexture.Sample(g_DefaultSampler, In.vTexCoord3);

	vNoise[0] = (vNoise[0] - 0.5f) * 2.0f;
	vNoise[1] = (vNoise[1] - 0.5f) * 2.0f;
	vNoise[2] = (vNoise[2] - 0.5f) * 2.0f;

	vNoise[0].xy = vNoise[0].xy * g_vDistortion[0].xy;
	vNoise[1].xy = vNoise[1].xy * g_vDistortion[1].xy;
	vNoise[2].xy = vNoise[2].xy * g_vDistortion[2].xy;

	vFinalNoise = vNoise[0] + vNoise[1] + vNoise[2];

	fPerturb = ((1.f - In.vTexUV.y) * g_fDistortionScale) + g_fDistortionBias;

	vNoiseCoord = (vFinalNoise.xy * fPerturb) + In.vTexUV.xy;

	vDiffuseColor = g_DiffuseTexture.Sample(g_BorderSampler, vNoiseCoord.xy);

	vAlpha = g_MaskTexture.Sample(g_BorderSampler, vNoiseCoord.xy);
	
	vAlpha.a = (vAlpha.r + vAlpha.g + vAlpha.b) / 3;
   
	vDiffuseColor.a = vAlpha.a * g_fFadeAlpha;
	if (vDiffuseColor.a <= 0.1f)
		discard;

	return vDiffuseColor;
}

vector PS_MAIN_MESH(PS_IN_TEST In) : SV_TARGET
{
    float4 vNoise[3];
    float4 vFinalNoise;
    float fPerturb;
    float2 vNoiseCoord;
    float4 vDiffuseColor;
    float4 vAlpha;

    vNoise[0] = g_NoiseTexture.Sample(g_DefaultSampler, In.vTexCoord1);
    vNoise[1] = g_NoiseTexture.Sample(g_DefaultSampler, In.vTexCoord2);
    vNoise[2] = g_NoiseTexture.Sample(g_DefaultSampler, In.vTexCoord3);
     
    vNoise[0] = (vNoise[0] - 0.5f) * 2.0f;
    vNoise[1] = (vNoise[1] - 0.5f) * 2.0f;
    vNoise[2] = (vNoise[2] - 0.5f) * 2.0f;

    vNoise[0].xy = vNoise[0].xy * g_vDistortion[0].xy;
    vNoise[1].xy = vNoise[1].xy * g_vDistortion[1].xy;
    vNoise[2].xy = vNoise[2].xy * g_vDistortion[2].xy;

    //vNoise[0].xy = vNoise[0].xy * float2(0.1f, 0.2f);
    //vNoise[1].xy = vNoise[1].xy * float2(0.1f, 0.3f);
    //vNoise[2].xy = vNoise[2].xy * float2(0.1f, 0.1f);

    vFinalNoise = vNoise[0] + vNoise[1] + vNoise[2];

    fPerturb = ((1.f - In.vTexUV.y) * g_fDistortionScale) + g_fDistortionBias;

    vNoiseCoord = (vFinalNoise.xy * fPerturb) + In.vTexUV.xy;

    vDiffuseColor = g_DiffuseTexture.Sample(g_DefaultSampler, vNoiseCoord.xy);

    vAlpha = g_MaskTexture.Sample(g_DefaultSampler, vNoiseCoord.xy);
    //if (vAlpha.a <= 0.1f)
       //discard;
    vAlpha.a = (vAlpha.r + vAlpha.g + vAlpha.b) / 3;
   // vDiffuseColor.a = vAlpha.a;     
    vDiffuseColor.rgb += g_vOffsetColor.rgb;
    vDiffuseColor.a = vAlpha.a + g_vOffsetColor.a;
    vDiffuseColor.a *= g_fFadeAlpha;
    if (vDiffuseColor.a <= 0.1f)
        discard;

    return vDiffuseColor;
}

vector PS_MAIN_MESH_BOADER(PS_IN_TEST In) : SV_TARGET
{
	float4 vNoise[3];
	float4 vFinalNoise;
	float fPerturb;
	float2 vNoiseCoord;
	float4 vDiffuseColor;
	float4 vAlpha;

	vNoise[0] = g_NoiseTexture.Sample(g_DefaultSampler, In.vTexCoord1);
	vNoise[1] = g_NoiseTexture.Sample(g_DefaultSampler, In.vTexCoord2);
	vNoise[2] = g_NoiseTexture.Sample(g_DefaultSampler, In.vTexCoord3);

	vNoise[0] = (vNoise[0] - 0.5f) * 2.0f;
	vNoise[1] = (vNoise[1] - 0.5f) * 2.0f;
	vNoise[2] = (vNoise[2] - 0.5f) * 2.0f;

	vNoise[0].xy = vNoise[0].xy * g_vDistortion[0].xy;
	vNoise[1].xy = vNoise[1].xy * g_vDistortion[1].xy;
	vNoise[2].xy = vNoise[2].xy * g_vDistortion[2].xy;

	//vNoise[0].xy = vNoise[0].xy * float2(0.1f, 0.2f);
	//vNoise[1].xy = vNoise[1].xy * float2(0.1f, 0.3f);
	//vNoise[2].xy = vNoise[2].xy * float2(0.1f, 0.1f);

	vFinalNoise = vNoise[0] + vNoise[1] + vNoise[2];

	fPerturb = ((1.f - In.vTexUV.y) * g_fDistortionScale) + g_fDistortionBias;

	vNoiseCoord = (vFinalNoise.xy * fPerturb) + In.vTexUV.xy;

	vDiffuseColor = g_DiffuseTexture.Sample(g_DefaultSampler, vNoiseCoord.xy);

	vAlpha = g_MaskTexture.Sample(g_BorderSampler, vNoiseCoord.xy);
	//if (vAlpha.a <= 0.1f)
	//discard;
	vAlpha.a = (vAlpha.r + vAlpha.g + vAlpha.b) / 3;
	// vDiffuseColor.a = vAlpha.a;     
	vDiffuseColor.a = vAlpha.a * g_fFadeAlpha;
	if (vDiffuseColor.a <= 0.1f)
	    discard;

	return vDiffuseColor;
}

vector PS_MAIN_SPRITE(PS_IN_SPRITE In) : SV_TARGET
{
    float4 vDiffuseColor;
    float4 vMask;

	//vMask = g_MaskTexture.Sample(g_DefaultSampler, In.vMaskUV);
    vDiffuseColor = g_DiffuseTexture.Sample(g_DefaultSampler, In.vTexUV);
	vMask = g_DiffuseTexture.Sample(g_DefaultSampler, In.vTexUV);
    vMask.a = (vMask.r + vMask.g + vMask.b) / 3;

    if (vMask.a <= 0.2f)
        discard;
    
    vDiffuseColor.rgb += g_vOffsetColor.rgb;
    vDiffuseColor.a = vMask.a + g_vOffsetColor.a;
    vDiffuseColor.a *= g_fFadeAlpha;
    
    if (vDiffuseColor.a <= 0.2f)
        discard;

    return vDiffuseColor;
}


vector PS_MAIN_SPRITEALPHAR(PS_IN_SPRITE In) : SV_TARGET
{
    float4 vDiffuseColor;
    float4 vMask;

	//vMask = g_MaskTexture.Sample(g_DefaultSampler, In.vMaskUV);
    vDiffuseColor = g_DiffuseTexture.Sample(g_DefaultSampler, In.vTexUV);
    vMask = g_DiffuseTexture.Sample(g_DefaultSampler, In.vTexUV);
    vMask.a = vMask.r ;

    if (vMask.a <= 0.f)
        discard;
    
    vDiffuseColor.rgb += g_vOffsetColor.rgb;
    vDiffuseColor.a = vMask.a + g_vOffsetColor.a;
    vDiffuseColor.a *= g_fFadeAlpha;
    
    if (vDiffuseColor.a <= 0.2f)
        discard;

    return vDiffuseColor;
}

vector PS_MAIN_SPRITEMASK(PS_IN_SPRITE In) : SV_TARGET
{
    float4 vDiffuseColor;
    float4 vMask;

	vMask = g_MaskTexture.Sample(g_DefaultSampler, In.vMaskUV);
	vMask.a = ((vMask.r + vMask.g + vMask.b) / 3);

    vDiffuseColor = g_DiffuseTexture.Sample(g_DefaultSampler, In.vTexUV);

	vDiffuseColor.a = vMask.a;

	if (vDiffuseColor.a <= 0.1f)
		discard;

	return vDiffuseColor;
}

vector PS_MAIN_MaskAlsoSPRITE(PS_IN_SPRITE In) : SV_TARGET
{
	float4 vDiffuseColor;
	float4 vMask;

	vMask = g_MaskTexture.Sample(g_DefaultSampler, In.vTexUV);
	vMask.a = (vMask.r + vMask.g + vMask.b) / 3;

    if (vMask.a <= 0.f)
        discard;

	vDiffuseColor = g_DiffuseTexture.Sample(g_DefaultSampler, In.vTexUV);

    vDiffuseColor.rgb += g_vOffsetColor.rgb;
    vDiffuseColor.a = vMask.a + g_vOffsetColor.a;
    vDiffuseColor.a *= g_fFadeAlpha;

	if (vDiffuseColor.a <= 0.3f)
	discard;

	return vDiffuseColor;
}

vector PS_MAIN_TESTSPRITE(PS_IN_SPRITE In) : SV_TARGET
{
    float4 vDiffuseColor;
    float4 vMask;

    vMask = g_MaskTexture.Sample(g_DefaultSampler, In.vTexUV);
    vMask.a = (vMask.r + vMask.g + vMask.b) / 3;

    if (vMask.a <= 0.f)
        discard;

    vDiffuseColor = g_DiffuseTexture.Sample(g_DefaultSampler, In.vTexUV);

    vDiffuseColor.rgb *= g_vOffsetColor.rgb;
    vDiffuseColor.a = vMask.a + g_vOffsetColor.a;
    vDiffuseColor.a *= g_fFadeAlpha;

    if (vDiffuseColor.a <= 0.2f)
        discard;

    return vDiffuseColor;
}

vector PS_MAIN_TRAIL(PS_IN_TRAIL In) : SV_TARGET
{
    float2 uv = In.vProj.xy / In.vProj.w;
    uv = uv * float2(0.5f, -0.5f) + 0.5f;
    
    float4 vNoise = g_NoiseTexture.Sample(g_DefaultSampler, In.vTexUV);

    uv.x += vNoise.x * 0.1f;
    uv.y += vNoise.y * 0.1f;

    float4 vDiffuse = g_HDRTexture.Sample(g_ClampSampler, uv/* + 0.01f*/);
    float4 vColor = g_MaskTexture.Sample(g_DefaultSampler, In.vTexUV);
      
    vColor.a = vColor.r;
    vColor.rgb += g_vOffsetColor.rgb;
    //vColor.gb = vColor.g;

    if (vColor.a > 0.f)
        vDiffuse += vColor;
    
    return vDiffuse;
}



vector PS_MAIN_MESHALPHA(PS_IN_TEST In) : SV_TARGET
{
    float4 vNoise[3];
    float4 vFinalNoise;
    float fPerturb;
    float2 vNoiseCoord;
    float4 vDiffuseColor;
    float4 vAlpha;

    vNoise[0] = g_NoiseTexture.Sample(g_DefaultSampler, In.vTexCoord1);
    vNoise[1] = g_NoiseTexture.Sample(g_DefaultSampler, In.vTexCoord2);
    vNoise[2] = g_NoiseTexture.Sample(g_DefaultSampler, In.vTexCoord3);
     
    vNoise[0] = (vNoise[0] - 0.5f) * 2.0f;
    vNoise[1] = (vNoise[1] - 0.5f) * 2.0f;
    vNoise[2] = (vNoise[2] - 0.5f) * 2.0f;

    vNoise[0].xy = vNoise[0].xy * g_vDistortion[0].xy;
    vNoise[1].xy = vNoise[1].xy * g_vDistortion[1].xy;
    vNoise[2].xy = vNoise[2].xy * g_vDistortion[2].xy;

    //vNoise[0].xy = vNoise[0].xy * float2(0.1f, 0.2f);
    //vNoise[1].xy = vNoise[1].xy * float2(0.1f, 0.3f);
    //vNoise[2].xy = vNoise[2].xy * float2(0.1f, 0.1f);

    vFinalNoise = vNoise[0] + vNoise[1] + vNoise[2];

    fPerturb = ((1.f - In.vTexUV.y) * g_fDistortionScale) + g_fDistortionBias;

    vNoiseCoord = (vFinalNoise.xy * fPerturb) + In.vTexUV.xy;

    vDiffuseColor = g_DiffuseTexture.Sample(g_DefaultSampler, vNoiseCoord.xy);

    vAlpha = g_MaskTexture.Sample(g_DefaultSampler, vNoiseCoord.xy);
    //if (vAlpha.a <= 0.1f)
       //discard;
    vAlpha.a = (vAlpha.r + vAlpha.g + vAlpha.b) / 3;
   // vDiffuseColor.a = vAlpha.a;     
    vDiffuseColor.rgb = vDiffuseColor.rgb * vAlpha.a;
    vDiffuseColor.rgb += g_vOffsetColor.rgb;
    vDiffuseColor.a = vAlpha.a + g_vOffsetColor.a;
    vDiffuseColor.a *= g_fFadeAlpha;

    if (vDiffuseColor.a <= 0.1f)
        discard;

    return vDiffuseColor;
}


vector PS_MAIN_MESHUP(PS_IN_TEST In) : SV_TARGET
{
    float4 vNoise[3];
    float4 vFinalNoise;
    float fPerturb;
    float2 vNoiseCoord;
    float4 vDiffuseColor;
    float4 vAlpha;

    vNoise[0] = g_NoiseTexture.Sample(g_DefaultSampler, In.vTexCoord1);
    vNoise[1] = g_NoiseTexture.Sample(g_DefaultSampler, In.vTexCoord2);
    vNoise[2] = g_NoiseTexture.Sample(g_DefaultSampler, In.vTexCoord3);
     
    vNoise[0] = (vNoise[0] - 0.5f) * 2.0f;
    vNoise[1] = (vNoise[1] - 0.5f) * 2.0f;
    vNoise[2] = (vNoise[2] - 0.5f) * 2.0f;

    vNoise[0].xy = vNoise[0].xy * g_vDistortion[0].xy;
    vNoise[1].xy = vNoise[1].xy * g_vDistortion[1].xy;
    vNoise[2].xy = vNoise[2].xy * g_vDistortion[2].xy;

    vFinalNoise = vNoise[0] + vNoise[1] + vNoise[2];

    fPerturb = ((1.f - In.vTexUV.y) * g_fDistortionScale) + g_fDistortionBias;

    vNoiseCoord = (vFinalNoise.xy * fPerturb) + In.vTexUV.xy;

    vDiffuseColor = g_DiffuseTexture.Sample(g_DefaultSampler, vNoiseCoord.xy);

    vAlpha = g_MaskTexture.Sample(g_BorderSampler, vNoiseCoord.xy);
    
    vAlpha.a = (vAlpha.r + vAlpha.g + vAlpha.b) * 1.5f;

    vDiffuseColor.a = vAlpha.a * g_fFadeAlpha;
    if (vDiffuseColor.a <= 0.1f)
        discard;

    return vDiffuseColor;
}


vector PS_MAIN_MESHREDUP(PS_IN_TEST In) : SV_TARGET
{
    float4 vNoise[3];
    float4 vFinalNoise;
    float fPerturb;
    float2 vNoiseCoord;
    float4 vDiffuseColor;
    float4 vAlpha;

    vNoise[0] = g_NoiseTexture.Sample(g_DefaultSampler, In.vTexCoord1);
    vNoise[1] = g_NoiseTexture.Sample(g_DefaultSampler, In.vTexCoord2);
    vNoise[2] = g_NoiseTexture.Sample(g_DefaultSampler, In.vTexCoord3);
     
    vNoise[0] = (vNoise[0] - 0.5f) * 2.0f;
    vNoise[1] = (vNoise[1] - 0.5f) * 2.0f;
    vNoise[2] = (vNoise[2] - 0.5f) * 2.0f;

    vNoise[0].xy = vNoise[0].xy * g_vDistortion[0].xy;
    vNoise[1].xy = vNoise[1].xy * g_vDistortion[1].xy;
    vNoise[2].xy = vNoise[2].xy * g_vDistortion[2].xy;

    vFinalNoise = vNoise[0] + vNoise[1] + vNoise[2];

    fPerturb = ((1.f - In.vTexUV.y) * g_fDistortionScale) + g_fDistortionBias;

    vNoiseCoord = (vFinalNoise.xy * fPerturb) + In.vTexUV.xy;

    vDiffuseColor = g_DiffuseTexture.Sample(g_DefaultSampler, vNoiseCoord.xy);

    vAlpha = g_MaskTexture.Sample(g_BorderSampler, vNoiseCoord.xy);
    
    vAlpha.a = (vAlpha.r + vAlpha.g + vAlpha.b) * 1.5f;

    vDiffuseColor.a = vAlpha.a * g_fFadeAlpha;
    vDiffuseColor.r = 1.f;
    vDiffuseColor.gb = vAlpha.g;
    if (vDiffuseColor.a <= 0.1f)
        discard;

    return vDiffuseColor;
}

vector PS_MAIN_MESHREDUPOffset(PS_IN_TEST In) : SV_TARGET
{
	float4 vNoise[3];
	float4 vFinalNoise;
	float fPerturb;
	float2 vNoiseCoord;
	float4 vDiffuseColor;
	float4 vAlpha;

	vNoise[0] = g_NoiseTexture.Sample(g_DefaultSampler, In.vTexCoord1);
	vNoise[1] = g_NoiseTexture.Sample(g_DefaultSampler, In.vTexCoord2);
	vNoise[2] = g_NoiseTexture.Sample(g_DefaultSampler, In.vTexCoord3);

	vNoise[0] = (vNoise[0] - 0.5f) * 2.0f;
	vNoise[1] = (vNoise[1] - 0.5f) * 2.0f;
	vNoise[2] = (vNoise[2] - 0.5f) * 2.0f;

	vNoise[0].xy = vNoise[0].xy * g_vDistortion[0].xy;
	vNoise[1].xy = vNoise[1].xy * g_vDistortion[1].xy;
	vNoise[2].xy = vNoise[2].xy * g_vDistortion[2].xy;

	vFinalNoise = vNoise[0] + vNoise[1] + vNoise[2];

	fPerturb = ((1.f - In.vTexUV.y) * g_fDistortionScale) + g_fDistortionBias;

	vNoiseCoord = (vFinalNoise.xy * fPerturb) + In.vTexUV.xy;

	vDiffuseColor = g_DiffuseTexture.Sample(g_DefaultSampler, vNoiseCoord.xy);

	vAlpha = g_MaskTexture.Sample(g_BorderSampler, vNoiseCoord.xy);

	vAlpha.a = (vAlpha.r + vAlpha.g + vAlpha.b) * 3.f;

	if (vAlpha.a <= 0.1f)
		discard;

	vDiffuseColor.r = 0.5f;
	vDiffuseColor.g = 0.4f;
	vDiffuseColor.b = vAlpha.r * 0.5f;
	vDiffuseColor.rgb += g_vOffsetColor.rgb;
	vDiffuseColor.a = (vAlpha.a + g_vOffsetColor.a);
	vDiffuseColor.a *= g_fFadeAlpha;

	if (vDiffuseColor.a <= 0.1f)
		discard;

	return vDiffuseColor;
}

vector PS_MAIN_MESHREDUPDiffuse(PS_IN_TEST In) : SV_TARGET
{
	float4 vNoise[3];
	float4 vFinalNoise;
	float fPerturb;
	float2 vNoiseCoord;
	float4 vDiffuseColor;
	float4 vAlpha;

	vNoise[0] = g_NoiseTexture.Sample(g_DefaultSampler, In.vTexCoord1);
	vNoise[1] = g_NoiseTexture.Sample(g_DefaultSampler, In.vTexCoord2);
	vNoise[2] = g_NoiseTexture.Sample(g_DefaultSampler, In.vTexCoord3);

	vNoise[0] = (vNoise[0] - 0.5f) * 2.0f;
	vNoise[1] = (vNoise[1] - 0.5f) * 2.0f;
	vNoise[2] = (vNoise[2] - 0.5f) * 2.0f;

	vNoise[0].xy = vNoise[0].xy * g_vDistortion[0].xy;
	vNoise[1].xy = vNoise[1].xy * g_vDistortion[1].xy;
	vNoise[2].xy = vNoise[2].xy * g_vDistortion[2].xy;

	vFinalNoise = vNoise[0] + vNoise[1] + vNoise[2];

	fPerturb = ((1.f - In.vTexUV.y) * g_fDistortionScale) + g_fDistortionBias;
	In.vTexUV.y += g_OneSideTime;
	vNoiseCoord = (vFinalNoise.xy * fPerturb) + In.vTexUV.xy;

	vDiffuseColor = g_DiffuseTexture.Sample(g_BorderSampler, vNoiseCoord.xy);

	vAlpha = g_MaskTexture.Sample(g_BorderSampler, vNoiseCoord.xy);

	vAlpha.a = (vAlpha.r + vAlpha.g + vAlpha.b) * 1.5f;

	if (vAlpha.a <= 0.1f)
	discard;

	vDiffuseColor.rgb += g_vOffsetColor.rgb;
	vDiffuseColor.a = (vAlpha.a + g_vOffsetColor.a) * g_fFadeAlpha;

	if (vDiffuseColor.a <= 0.1f)
	discard;

	return vDiffuseColor;
}

vector PS_MAIN_MESHRED(PS_IN_TEST In) : SV_TARGET
{
    float4 vNoise[3];
    float4 vFinalNoise;
    float fPerturb;
    float2 vNoiseCoord;
    float4 vDiffuseColor;
    float4 vAlpha;

    vNoise[0] = g_NoiseTexture.Sample(g_DefaultSampler, In.vTexCoord1);
    vNoise[1] = g_NoiseTexture.Sample(g_DefaultSampler, In.vTexCoord2);
    vNoise[2] = g_NoiseTexture.Sample(g_DefaultSampler, In.vTexCoord3);
     
    vNoise[0] = (vNoise[0] - 0.5f) * 2.0f;
    vNoise[1] = (vNoise[1] - 0.5f) * 2.0f;
    vNoise[2] = (vNoise[2] - 0.5f) * 2.0f;

    vNoise[0].xy = vNoise[0].xy * g_vDistortion[0].xy;
    vNoise[1].xy = vNoise[1].xy * g_vDistortion[1].xy;
    vNoise[2].xy = vNoise[2].xy * g_vDistortion[2].xy;

    vFinalNoise = vNoise[0] + vNoise[1] + vNoise[2];

    fPerturb = ((1.f - In.vTexUV.y) * g_fDistortionScale) + g_fDistortionBias;

    vNoiseCoord = (vFinalNoise.xy * fPerturb) + In.vTexUV.xy;

    //vDiffuseColor = g_DiffuseTexture.Sample(g_DefaultSampler, vNoiseCoord.xy);

    vAlpha = g_MaskTexture.Sample(g_BorderSampler, vNoiseCoord.xy);
    
    vAlpha.a = (vAlpha.r + vAlpha.g + vAlpha.b);

    vDiffuseColor.a = vAlpha.a * g_fFadeAlpha;
    vDiffuseColor.r = 1.f;
    vDiffuseColor.gb = vAlpha.g;
    if (vDiffuseColor.a <= 0.1f)
        discard;

    return vDiffuseColor;
}

vector PS_MAIN_MESH_FlogasWave(PS_IN_TEST In) : SV_TARGET
{
	float4 vNoise[3];
	float4 vFinalNoise;
	float fPerturb;
	float2 vNoiseCoord;
	float4 vDiffuseColor;
	float4 vAlpha;

	vNoise[0] = g_NoiseTexture.Sample(g_DefaultSampler, In.vTexCoord1);
	vNoise[1] = g_NoiseTexture.Sample(g_DefaultSampler, In.vTexCoord2);
	vNoise[2] = g_NoiseTexture.Sample(g_DefaultSampler, In.vTexCoord3);

	vNoise[0] = (vNoise[0] - 0.5f) * 2.0f;
	vNoise[1] = (vNoise[1] - 0.5f) * 2.0f;
	vNoise[2] = (vNoise[2] - 0.5f) * 2.0f;

	vNoise[0].xy = vNoise[0].xy * g_vDistortion[0].xy;
	vNoise[1].xy = vNoise[1].xy * g_vDistortion[1].xy;
	vNoise[2].xy = vNoise[2].xy * g_vDistortion[2].xy;

	//vNoise[0].xy = vNoise[0].xy * float2(0.1f, 0.2f);
	//vNoise[1].xy = vNoise[1].xy * float2(0.1f, 0.3f);
	//vNoise[2].xy = vNoise[2].xy * float2(0.1f, 0.1f);

	vFinalNoise = vNoise[0] + vNoise[1] + vNoise[2];

	fPerturb = ((1.f - In.vTexUV.y) * g_fDistortionScale) + g_fDistortionBias;

	vNoiseCoord = (vFinalNoise.xy * fPerturb) + In.vTexUV.xy;

	vDiffuseColor = g_DiffuseTexture.Sample(g_DefaultSampler, vNoiseCoord.xy);

	vAlpha = g_MaskTexture.Sample(g_DefaultSampler, vNoiseCoord.xy);
	vAlpha.a = vAlpha.g;
	vDiffuseColor.a = vAlpha.a;     
	vDiffuseColor.a = vAlpha.a * g_fFadeAlpha;
	//vDiffuseColor.b = vAlpha.r;
	if (vDiffuseColor.a <= 0.1f)
		discard;

	return vDiffuseColor;
}

vector PS_MAIN_NORMAL(PS_IN In) : SV_TARGET
{    
    float4 DiffuseColor = float4(0.f, 0.f, 0.f, 0.f);
    float4 Normal = g_NormalTexture.Sample(g_DefaultSampler, In.vTexUV);

    DiffuseColor = g_DiffuseTexture.Sample(g_DefaultSampler, In.vTexUV + (Normal.xy * 0.01));
    if (DiffuseColor.a == 0.f)
        discard;

    return DiffuseColor;
}

vector PS_MAIN_MESH_FlogasFire(PS_IN_TEST In) : SV_TARGET
{
	float4 vNoise[3];
	float4 vFinalNoise;
	float fPerturb;
	float2 vNoiseCoord;
	float4 vDiffuseColor;
	float4 vAlpha;

	vNoise[0] = g_NoiseTexture.Sample(g_DefaultSampler, In.vTexCoord1);
	vNoise[1] = g_NoiseTexture.Sample(g_DefaultSampler, In.vTexCoord2);
	vNoise[2] = g_NoiseTexture.Sample(g_DefaultSampler, In.vTexCoord3);

	vNoise[0] = (vNoise[0] - 0.5f) * 2.0f;
	vNoise[1] = (vNoise[1] - 0.5f) * 2.0f;
	vNoise[2] = (vNoise[2] - 0.5f) * 2.0f;

	vNoise[0].xy = vNoise[0].xy * g_vDistortion[0].xy;
	vNoise[1].xy = vNoise[1].xy * g_vDistortion[1].xy;
	vNoise[2].xy = vNoise[2].xy * g_vDistortion[2].xy;

	//vNoise[0].xy = vNoise[0].xy * float2(0.1f, 0.2f);
	//vNoise[1].xy = vNoise[1].xy * float2(0.1f, 0.3f);
	//vNoise[2].xy = vNoise[2].xy * float2(0.1f, 0.1f);

	vFinalNoise = vNoise[0] + vNoise[1] + vNoise[2];

	fPerturb = ((1.f - In.vTexUV.y) * g_fDistortionScale) + g_fDistortionBias;

	vNoiseCoord = (vFinalNoise.xy * fPerturb) + In.vTexUV.xy;

	vDiffuseColor = g_DiffuseTexture.Sample(g_BorderSampler, vNoiseCoord.xy);

	vAlpha = g_MaskTexture.Sample(g_BorderSampler, vNoiseCoord.xy);
	vAlpha.a = (vAlpha.r + vAlpha.g + vAlpha.b) /3.f;

	if (vAlpha.a == 0)
		discard;

	vDiffuseColor.a = vAlpha.a;
	vDiffuseColor.a = vAlpha.a * g_fFadeAlpha;
	if (vDiffuseColor.a <= 0.2f)
		discard;

	return vDiffuseColor;
}

vector PS_MAINNOAMSK(PS_IN In) : SV_TARGET
{
    float4 DiffuseColor = float4(0.f, 0.f, 0.f, 0.f);
    DiffuseColor = g_DiffuseTexture.Sample(g_DefaultSampler, In.vTexUV);
    DiffuseColor.a = (DiffuseColor.r + DiffuseColor.g + DiffuseColor.b) * g_fFadeAlpha;
    if (DiffuseColor.a <= 0.01f)
        discard;

    return DiffuseColor;
}

technique11 DefaultDevice
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
    pass FireEffectPass
    {
        SetRasterizerState(Rasterizer_NoneCull);
        SetDepthStencilState(DepthStecil_Default, 0);
        SetBlendState(Blend_Alpha, vector(0.f, 0.f, 0.f, 0.f), 0xffffffff);

        VertexShader = compile vs_5_0 VS_MAIN_TEST();
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN_FIRE();
    }
    pass SPRITE
    {
        SetRasterizerState(Rasterizer_NoneCull);
        SetDepthStencilState(DepthStecil_Default, 0);
        SetBlendState(Blend_Alpha, vector(0.f, 0.f, 0.f, 0.f), 0xffffffff);

        VertexShader = compile vs_5_0 VS_MAIN_SPRITE();
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN_SPRITE();
    }
    pass Trail
    {
        SetRasterizerState(Rasterizer_NoneCull);
        SetDepthStencilState(DepthStecil_Default, 0);
        SetBlendState(Blend_Alpha, vector(0.f, 0.f, 0.f, 0.f), 0xffffffff);

        VertexShader = compile vs_5_0 VS_MAIN_TRAIL();
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN_TRAIL();
    }
    pass MeshEffectPass
    {
        SetRasterizerState(Rasterizer_NoneCull);
        SetDepthStencilState(DepthStecil_Default, 0);
        SetBlendState(Blend_Alpha, vector(0.f, 0.f, 0.f, 0.f), 0xffffffff);

        VertexShader = compile vs_5_0 VS_MAIN_TEST();
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN_MESH();
    }

	pass BoarderEffectPass // 5
	{
		SetRasterizerState(Rasterizer_NoneCull);
		SetDepthStencilState(DepthStecil_Default, 0);
		SetBlendState(Blend_Alpha, vector(0.f, 0.f, 0.f, 0.f), 0xffffffff);

		VertexShader = compile vs_5_0 VS_MAIN_TEST();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN_MESH_BOADER();
	}

    pass MeshEffectMaskColor
    {
        SetRasterizerState(Rasterizer_NoneCull);
        SetDepthStencilState(DepthStecil_Default, 0);
        SetBlendState(Blend_Alpha, vector(0.f, 0.f, 0.f, 0.f), 0xffffffff);

        VertexShader = compile vs_5_0 VS_MAIN_TEST();
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN_MESHALPHA();
    }

    pass MeshEffectUPPass
    {
        SetRasterizerState(Rasterizer_NoneCull);
        SetDepthStencilState(DepthStecil_Default, 0);
        SetBlendState(Blend_Alpha, vector(0.f, 0.f, 0.f, 0.f), 0xffffffff);

        VertexShader = compile vs_5_0 VS_MAIN_TEST();
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN_MESHRED();
    }

    pass MeshEffectHalfUV
    {
        SetRasterizerState(Rasterizer_NoneCull);
        SetDepthStencilState(DepthStecil_Default, 0);
        SetBlendState(Blend_Alpha, vector(0.f, 0.f, 0.f, 0.f), 0xffffffff);

        VertexShader = compile vs_5_0 VS_MAIN_UVHALF();
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN_MESHALPHA();
    }

    pass MeshEffectMoveRed
    {
        SetRasterizerState(Rasterizer_NoneCull);
        SetDepthStencilState(DepthStecil_Default, 0);
        SetBlendState(Blend_Alpha, vector(0.f, 0.f, 0.f, 0.f), 0xffffffff);

        VertexShader = compile vs_5_0 VS_MAIN_TESTUVMOVE();
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN_MESHREDUP();
    }

	pass MeshEffectFlogasWave //10
	{
		SetRasterizerState(Rasterizer_NoneCull);
		SetDepthStencilState(DepthStecil_Default, 0);
		SetBlendState(Blend_Alpha, vector(0.f, 0.f, 0.f, 0.f), 0xffffffff);

		VertexShader = compile vs_5_0 VS_MAIN_TESTUVMOVE();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN_MESH_FlogasWave();
	}

	pass MeshEffectFlogasFire
	{
		SetRasterizerState(Rasterizer_NoneCull);
		SetDepthStencilState(DepthStecil_Default, 0);
		SetBlendState(Blend_Alpha, vector(0.f, 0.f, 0.f, 0.f), 0xffffffff);

		VertexShader = compile vs_5_0 VS_MAIN_FlogasUVMove();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN_MESH_FlogasFire();
	}

	pass SPriteMask
	{
		SetRasterizerState(Rasterizer_NoneCull);
		SetDepthStencilState(DepthStecil_Default, 0);
		SetBlendState(Blend_Alpha, vector(0.f, 0.f, 0.f, 0.f), 0xffffffff);

		VertexShader = compile vs_5_0 VS_MAIN_SPRITEMASK();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN_SPRITEMASK();
	}
  
  pass NORMAL_SAMPLE
  {
      SetRasterizerState(Rasterizer_Solid);
      SetDepthStencilState(DepthStecil_Default, 0);
      SetBlendState(Blend_Alpha, vector(0.f, 0.f, 0.f, 0.f), 0xffffffff);

      VertexShader = compile vs_5_0 VS_MAIN();
      GeometryShader = NULL;
      PixelShader = compile ps_5_0 PS_MAIN_NORMAL();
  }
  
  pass BothSpriteMask
  {
	  SetRasterizerState(Rasterizer_NoneCull);
	  SetDepthStencilState(DepthStecil_Default, 0);
	  SetBlendState(Blend_Alpha, vector(0.f, 0.f, 0.f, 0.f), 0xffffffff);

	  VertexShader = compile vs_5_0 VS_MAIN_SPRITEMASK();
	  GeometryShader = NULL;
	  PixelShader = compile ps_5_0 PS_MAIN_MaskAlsoSPRITE();
  }

    pass NoMaskEffect//15
    {
        SetRasterizerState(Rasterizer_NoneCull);
        SetDepthStencilState(DepthStecil_Default, 0);
        SetBlendState(Blend_One, vector(0.f, 0.f, 0.f, 0.f), 0xffffffff);

        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_MAINNOAMSK();
    }

    pass MeshEffectMoveRedX //16
    {
        SetRasterizerState(Rasterizer_NoneCull);
        SetDepthStencilState(DepthStecil_Default, 0);
        SetBlendState(Blend_Alpha, vector(0.f, 0.f, 0.f, 0.f), 0xffffffff);

        VertexShader = compile vs_5_0 VS_MAIN_TESTUVMOVEX();
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN_MESHREDUP();
    }
       
    pass TestSpriteMask //17
    {
        SetRasterizerState(Rasterizer_NoneCull);
        SetDepthStencilState(DepthStecil_Default, 0);
        SetBlendState(Blend_Alpha, vector(0.f, 0.f, 0.f, 0.f), 0xffffffff);

        VertexShader = compile vs_5_0 VS_MAIN_SPRITEMASK();
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN_TESTSPRITE();
    }

    pass SPRITE_TWO //18
    {
        SetRasterizerState(Rasterizer_NoneCull);
        SetDepthStencilState(DepthStecil_Default, 0);
        SetBlendState(Blend_Alpha, vector(0.f, 0.f, 0.f, 0.f), 0xffffffff);

        VertexShader = compile vs_5_0 VS_MAIN_SPRITE();
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN_SPRITEALPHAR();
    }

	pass MeshEffectMoveoffsetcolorX //19
	{
		SetRasterizerState(Rasterizer_NoneCull);
		SetDepthStencilState(DepthStecil_Default, 0);
		SetBlendState(Blend_Alpha, vector(0.f, 0.f, 0.f, 0.f), 0xffffffff);

		VertexShader = compile vs_5_0 VS_MAIN_TESTUVMOVEYSpeed();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN_MESHREDUPOffset();
	}

	pass MeshEffectMoveXDiffuse //20
	{
		SetRasterizerState(Rasterizer_NoneCull);
		SetDepthStencilState(DepthStecil_Default, 0);
		SetBlendState(Blend_Alpha, vector(0.f, 0.f, 0.f, 0.f), 0xffffffff);

		VertexShader = compile vs_5_0 VS_MAIN_TEST();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN_MESHREDUPDiffuse();
	}
}