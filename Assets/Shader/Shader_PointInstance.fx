
#include "Shader_Defines.hpp"

cbuffer Matrices
{
	matrix		g_WorldMatrix;
	matrix		g_ViewMatrix;
	matrix		g_ProjMatrix;	
}

cbuffer CameraDesc
{
	vector		g_vCamPosition;
}

cbuffer NoiseBuffer
{
    float g_fFrameTime;
    float3 g_vScrollSpeedX;
    float3 g_vScrollSpeedY;
    float3 g_vScale;
    float g_fPadding;
}

cbuffer DistortionBuffer
{
    float2 g_vDistortion[3];
    float g_fDistortionScale;
    float g_fDistortionBias;
}

cbuffer EffectBuffer
{
    float g_fFadeAlpha;
    float g_fAlpha;
    uint g_iSpriteNum;
}

Texture2D g_DiffuseTexture;
Texture2D g_MaskTexture;
Texture2D g_NoiseTexture;

struct VS_IN
{
	float3	vPosition : POSITION; /* 로컬스페이스 */
	float2	vSize : PSIZE;

    float4 vRight : INSTANCE0;
    float4 vUp : INSTANCE1;
    float4 vLook : INSTANCE2;
    float4 vTranslation : INSTANCE3;
};

struct VS_OUT
{
	float4	vPosition : POSITION;
	float2	vSize : PSIZE;
};

/* 정점의 스페이스 변환. (월드, 뷰, 투영행렬의 곱.)*/
VS_OUT VS_MAIN(VS_IN In)
{
	VS_OUT		Out = (VS_OUT)0;

	matrix		matWV, matWVP;

	matrix		WorldMatrix = float4x4(In.vRight, In.vUp, In.vLook, In.vTranslation);

	Out.vPosition = mul(vector(In.vPosition, 1.f), WorldMatrix);
	Out.vSize = In.vSize;

	return Out;
}

struct GS_OUT
{
	float4	vPosition : SV_POSITION;
	float2	vTexUV : TEXCOORD0;
};

struct GS_OUT_FIRE
{
    float4 vPosition : SV_POSITION;
    float2 vTexUV : TEXCOORD0;
    float2 vTexCoord1 : TEXCOORD1;
    float2 vTexCoord2 : TEXCOORD2;
    float2 vTexCoord3 : TEXCOORD3;
};

[maxvertexcount(6)]
void GS_MAIN(/*입력*/ point VS_OUT In[1], /*출력*/ inout TriangleStream<GS_OUT> Out)
{
	GS_OUT		Vertices[6];

	float3		vLook = normalize(g_vCamPosition - In[0].vPosition).xyz;
	float3		vAxisY = float3(0.f, 1.f, 0.f);

	float3		vRight = normalize(cross(vAxisY, vLook));
	float3		vUp = normalize(cross(vLook, vRight));

	matrix		matVP = mul(g_ViewMatrix, g_ProjMatrix);

	Vertices[0].vPosition = vector(In[0].vPosition.xyz + In[0].vSize.x * 0.5f * vRight + In[0].vSize.y * 0.5f * vUp, 1.f);
	Vertices[0].vPosition = mul(Vertices[0].vPosition, matVP);
	Vertices[0].vTexUV = float2(0.0f, 0.0f);
   
	Vertices[1].vPosition = vector(In[0].vPosition.xyz + In[0].vSize.x * -0.5f * vRight + In[0].vSize.y * 0.5f * vUp, 1.f);
	Vertices[1].vPosition = mul(Vertices[1].vPosition, matVP);
	Vertices[1].vTexUV = float2(1.0f, 0.0f);

	Vertices[2].vPosition = vector(In[0].vPosition.xyz + In[0].vSize.x * -0.5f * vRight + In[0].vSize.y * -0.5f * vUp, 1.f);
	Vertices[2].vPosition = mul(Vertices[2].vPosition, matVP);
	Vertices[2].vTexUV = float2(1.0f, 1.0f);
	Out.Append(Vertices[0]);
	Out.Append(Vertices[1]);
	Out.Append(Vertices[2]);

	Out.RestartStrip();

	Vertices[3] = Vertices[0];
	Vertices[4] = Vertices[2];
	Vertices[5].vPosition = vector(In[0].vPosition.xyz + In[0].vSize.x * 0.5f * vRight + In[0].vSize.y * -0.5f * vUp, 1.f);
	Vertices[5].vPosition = mul(Vertices[5].vPosition, matVP);
	Vertices[5].vTexUV = float2(0.0f, 1.0f);
	Out.Append(Vertices[3]);
	Out.Append(Vertices[4]);
	Out.Append(Vertices[5]);
}

[maxvertexcount(6)]
void GS_MAIN_FIRE( /*입력*/point VS_OUT In[1], /*출력*/inout TriangleStream<GS_OUT_FIRE> Out)
{
    GS_OUT_FIRE Vertices[6];

    float3 vLook = normalize(g_vCamPosition - In[0].vPosition).xyz;
    float3 vAxisY = float3(0.f, 1.f, 0.f);

    float3 vRight = normalize(cross(vAxisY, vLook));
    float3 vUp = normalize(cross(vLook, vRight));

    matrix matVP = mul(g_ViewMatrix, g_ProjMatrix);

    Vertices[0].vPosition = vector(In[0].vPosition.xyz + In[0].vSize.x * 0.5f * vRight + In[0].vSize.y * 0.5f * vUp, 1.f);
    Vertices[0].vPosition = mul(Vertices[0].vPosition, matVP);
    Vertices[0].vTexUV = float2(0.0f, 0.0f);
    
    Vertices[1].vPosition = vector(In[0].vPosition.xyz + In[0].vSize.x * -0.5f * vRight + In[0].vSize.y * 0.5f * vUp, 1.f);
    Vertices[1].vPosition = mul(Vertices[1].vPosition, matVP);
    Vertices[1].vTexUV = float2(1.0f, 0.0f);
   
    Vertices[2].vPosition = vector(In[0].vPosition.xyz + In[0].vSize.x * -0.5f * vRight + In[0].vSize.y * -0.5f * vUp, 1.f);
    Vertices[2].vPosition = mul(Vertices[2].vPosition, matVP);
    Vertices[2].vTexUV = float2(1.0f, 1.0f);    

    for (uint i = 0; i < 3; ++i)
    {
        Vertices[i].vTexCoord1 = Vertices[i].vTexUV * g_vScale.x;
        Vertices[i].vTexCoord1.x = Vertices[i].vTexCoord1.x + (g_fFrameTime * g_vScrollSpeedX.x);
        Vertices[i].vTexCoord1.y = Vertices[i].vTexCoord1.y + (g_fFrameTime * g_vScrollSpeedY.x);
        Vertices[i].vTexCoord2 = Vertices[i].vTexUV * g_vScale.y;
        Vertices[i].vTexCoord2.x = Vertices[i].vTexCoord2.x + (g_fFrameTime * g_vScrollSpeedX.y);
        Vertices[i].vTexCoord2.y = Vertices[i].vTexCoord2.y + (g_fFrameTime * g_vScrollSpeedY.y);
        Vertices[i].vTexCoord3 = Vertices[i].vTexUV * g_vScale.z;
        Vertices[i].vTexCoord3.x = Vertices[i].vTexCoord3.x + (g_fFrameTime * g_vScrollSpeedX.z);
        Vertices[i].vTexCoord3.y = Vertices[i].vTexCoord3.y + (g_fFrameTime * g_vScrollSpeedY.z);
    }

    Out.Append(Vertices[0]);
    Out.Append(Vertices[1]);
    Out.Append(Vertices[2]);

    Out.RestartStrip();

    Vertices[3] = Vertices[0];
    Vertices[4] = Vertices[2];
    Vertices[5].vPosition = vector(In[0].vPosition.xyz + In[0].vSize.x * 0.5f * vRight + In[0].vSize.y * -0.5f * vUp, 1.f);
    Vertices[5].vPosition = mul(Vertices[5].vPosition, matVP);
    Vertices[5].vTexUV = float2(0.0f, 1.0f);
    Vertices[5].vTexCoord1 = Vertices[5].vTexUV * g_vScale.x;
    Vertices[5].vTexCoord1.x = Vertices[5].vTexCoord1.x + (g_fFrameTime * g_vScrollSpeedX.x);
    Vertices[5].vTexCoord1.y = Vertices[5].vTexCoord1.y + (g_fFrameTime * g_vScrollSpeedY.x);
    Vertices[5].vTexCoord2 = Vertices[5].vTexUV * g_vScale.y;
    Vertices[5].vTexCoord2.x = Vertices[5].vTexCoord2.x + (g_fFrameTime * g_vScrollSpeedX.y);
    Vertices[5].vTexCoord2.y = Vertices[5].vTexCoord2.y + (g_fFrameTime * g_vScrollSpeedY.y);
    Vertices[5].vTexCoord3 = Vertices[5].vTexUV * g_vScale.z;
    Vertices[5].vTexCoord3.x = Vertices[5].vTexCoord3.x + (g_fFrameTime * g_vScrollSpeedX.z);
    Vertices[5].vTexCoord3.y = Vertices[5].vTexCoord3.y + (g_fFrameTime * g_vScrollSpeedY.z);
    Out.Append(Vertices[3]);
    Out.Append(Vertices[4]);
    Out.Append(Vertices[5]);
}

/* VS_OUT 구조체의 SV_POSITION이라는 시멘틱을 가진 데이터에 한해서만  */
/* W값으로 XYZW모두를 나눈다. */
/* 뷰포트로 정점의 위치를 변환한다. */
/* 래스터라이즈 : 정점으로 둘러쌓여진 픽셀의 정보를 생성하낟.(둘러쌓여진 정점의 정보를 기반으로하여)  */

struct PS_IN
{
	float4	vPosition : SV_POSITION;
	float2	vTexUV : TEXCOORD0;
};

struct PS_IN_FIRE
{
    float4 vPosition : SV_POSITION;
    float2 vTexUV : TEXCOORD0;
    float2 vTexCoord1 : TEXCOORD1;
    float2 vTexCoord2 : TEXCOORD2;
    float2 vTexCoord3 : TEXCOORD3;
};

vector	PS_MAIN(PS_IN In) : SV_TARGET
{
    vector vColor = { 1, 0, 0, 1};
    
    vColor = g_DiffuseTexture.Sample(g_DefaultSampler, In.vTexUV);

    if (vColor.a < 0.3f)
        discard;

    return vColor;
}

vector PS_MAIN_FIRE(PS_IN_FIRE In) : SV_TARGET
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
      
    vDiffuseColor.a = vAlpha.a * g_fFadeAlpha * g_fAlpha;
    if (vDiffuseColor.a <= 0.1f)
        discard;

    return vDiffuseColor;
}

technique11		DefaultDevice
{
	pass DefaultPass
	{
        SetRasterizerState(Rasterizer_NoneCull);
		SetDepthStencilState(DepthStecil_Default, 0);
		SetBlendState(Blend_Alpha, vector(0.f, 0.f, 0.f, 0.f), 0xffffffff);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = compile gs_5_0 GS_MAIN();
		PixelShader = compile ps_5_0 PS_MAIN();
	}
    pass FirePass
    {
        SetRasterizerState(Rasterizer_NoneCull);
        SetDepthStencilState(DepthStecil_Default, 0);
        SetBlendState(Blend_Alpha, vector(0.f, 0.f, 0.f, 0.f), 0xffffffff);

        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = compile gs_5_0 GS_MAIN_FIRE();
        PixelShader = compile ps_5_0 PS_MAIN_FIRE();
    }
}