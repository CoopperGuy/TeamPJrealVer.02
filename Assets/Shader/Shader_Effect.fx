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
    float g_fFadeAlpha;
    float g_fAlpha;
    uint g_iSpriteNum;
};

Texture2D g_DiffuseTexture;
Texture2D g_MaskTexture;
Texture2D g_NoiseTexture;

SamplerState g_DefaultSampler
{
    AddressU = wrap;
    AddressV = wrap;
};

SamplerState g_BorderSampler
{
	AddressU = border;
	AddressV = border;
};

SamplerState g_ClampSampler
{
    AddressU = clamp;
    AddressV = clamp;
};

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


VS_OUT_TEST VS_MAIN_TRAIL(VS_IN In)
{
    VS_OUT_TEST Out = (VS_OUT_TEST) 0;

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
    
    uint UVx = 0;
    uint UVy = 0;

    UVx = g_iSpriteNum % 4;
    UVy = g_iSpriteNum / 4;

    Out.vTexUV.x = ((In.vTexUV.x + UVx) / 4.f);
    Out.vTexUV.y = ((In.vTexUV.y + UVy) / 8.f);
   
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
    //if (vAlpha.a <= 0.1f)
       //discard;
   vAlpha.a = (vAlpha.r + vAlpha.g + vAlpha.b) / 3;
   // vDiffuseColor.a = vAlpha.a;     
   vDiffuseColor.a = vAlpha.a * g_fFadeAlpha * g_fAlpha;
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
    vDiffuseColor.a = vAlpha.a * g_fFadeAlpha * g_fAlpha;
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

   // vMask = g_MaskTexture.Sample(g_DefaultSampler, In.vMaskUV);
    vMask = g_DiffuseTexture.Sample(g_DefaultSampler, In.vTexUV);
    vMask.a = ((vMask.r + vMask.g + vMask.b));
    
    vDiffuseColor = g_DiffuseTexture.Sample(g_DefaultSampler, In.vTexUV);


    if (vDiffuseColor.r == 1 || vDiffuseColor.g == 1 || vDiffuseColor.b == 1)
        discard;
   
    vDiffuseColor *= vMask;
       
    if (vDiffuseColor.a <= 0.1f)
        discard;

    return vDiffuseColor;
}


vector PS_MAIN_TRAIL(PS_IN_TEST In) : SV_TARGET
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
   
    vAlpha.a = (vAlpha.r + vAlpha.g + vAlpha.b) / 3;

    vDiffuseColor.a = vAlpha.a * g_fFadeAlpha + g_AlphaSet;
    if (vDiffuseColor.a <= 0.1f)
        discard;

    return vDiffuseColor;
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
    vDiffuseColor.a = vAlpha.a * g_fFadeAlpha * g_fAlpha;
    vDiffuseColor.rgb = vDiffuseColor.rgb * vAlpha.a;
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

    vDiffuseColor.a = vAlpha.a * g_fFadeAlpha * g_fAlpha;
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

    vDiffuseColor.a = vAlpha.a * g_fFadeAlpha * g_fAlpha;
    vDiffuseColor.r = 1.f;
    vDiffuseColor.gb = vAlpha.g;
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

    vDiffuseColor = g_DiffuseTexture.Sample(g_DefaultSampler, vNoiseCoord.xy);

    vAlpha = g_MaskTexture.Sample(g_BorderSampler, vNoiseCoord.xy);
    
    vAlpha.a = (vAlpha.r + vAlpha.g + vAlpha.b);

    vDiffuseColor.a = vAlpha.a * g_fFadeAlpha * g_fAlpha;
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
	vDiffuseColor.a = vAlpha.a * g_fFadeAlpha * g_fAlpha;
	//vDiffuseColor.b = vAlpha.r;
	if (vDiffuseColor.a <= 0.1f)
		discard;

	return vDiffuseColor;
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

vDiffuseColor = g_DiffuseTexture.Sample(g_DefaultSampler, vNoiseCoord.xy);

vAlpha = g_MaskTexture.Sample(g_BorderSampler, vNoiseCoord.xy);
vAlpha.a = vAlpha.g;
vDiffuseColor.a = vAlpha.a;
vDiffuseColor.a = vAlpha.a * g_fFadeAlpha * g_fAlpha;
//vDiffuseColor.b = vAlpha.r;
if (vDiffuseColor.a <= 0.1f)
discard;

return vDiffuseColor;
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
        SetRasterizerState(Rasterizer_Solid);
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

	pass BoarderEffectPass
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

	pass MeshEffectFlogasWave
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

		VertexShader = compile vs_5_0 VS_MAIN_TEST();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN_MESH_FlogasFire();
	}
}