
#include "Shader_Defines.hpp"

cbuffer DirectionLight {
	
};

cbuffer Matrices
{
    matrix g_WorldMatrix;
    matrix g_ViewMatrix;
    matrix g_ProjMatrix;
    
    matrix g_LightViewMatrix;
    matrix g_LightProjMatrix;
}

cbuffer LightBuffer
{
    float3 lightPosition;
    float3 lightDir;
};

cbuffer RimLightBuffer
{
    bool   g_RimLightEnable;
    float3 g_CamPosition;
    float4 g_RimLitghColor;
};

cbuffer Dissolve
{
    float g_fDissolve;
};
struct MeshBoneMatrices
{   
    matrix BoneMatrices[512];
};

MeshBoneMatrices g_BoneMatrices;

Texture2D g_DiffuseTexture;
Texture2D g_NormalTexture;
Texture2D g_EmissiveTexture;
Texture2D g_SpecularTexture;
Texture2D g_DissolveTexture;
Texture2D g_DepthTexture;
Texture2D ShadowDepthTexture;

SamplerState g_DiffuseSampler
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
    float3 vPosition : POSITION; /* 로컬스페이스 */
    float2 vTexUV : TEXCOORD0;
    float3 vNormal : NORMAL;
    float3 vTangent : TANGENT;
    float3 vBiNormal : BINORMAL;
    uint4 vBlendIndex : BLENDINDEX;
    float4 vBlendWeight : BLENDWEIGHT;
};

struct VS_OUT
{
    float4 vPosition : SV_POSITION;
    float2 vTexUV : TEXCOORD0;
    float3 vNormal : NORMAL;
    float3 vTangent : TANGENT;
    float3 vBiNormal : BINORMAL;
    float4 lightViewPosition : TEXCOORD1;
    float3 lightPos : TEXCOORD2;
    float4 vProjPos : TEXCOORD3;
    float3 vWPos : TEXCOORD4;
};

struct VS_OUT_LIGHT_DEPTH
{
    float4 vPosition : SV_POSITION;
    float4 vLightDepthPosition : TEXCOORD0;
    float2 vTexUV : TEXCOORD1;
};

/* 정점의 스페이스 변환. (월드, 뷰, 투영행렬의 곱.)*/
VS_OUT VS_MAIN(VS_IN In)
{
    VS_OUT Out = (VS_OUT) 0;
    
    matrix matWV, matWVP;
    matWV = mul(g_WorldMatrix, g_ViewMatrix);
    matWVP = mul(matWV, g_ProjMatrix);
    Out.vPosition = mul(vector(In.vPosition, 1.f), matWVP);
    Out.vProjPos = Out.vPosition;
    Out.vTexUV = In.vTexUV;
    Out.vNormal = normalize(mul(vector(In.vNormal, 0.f), g_WorldMatrix)).xyz;
    Out.vTangent = normalize(mul(vector(In.vTangent, 0.f), g_WorldMatrix)).xyz;
    Out.vBiNormal = normalize(mul(vector(In.vBiNormal, 0.f), g_WorldMatrix)).xyz;
    
    Out.vWPos = mul(vector(In.vPosition, 1.f), g_WorldMatrix).xyz;
    Out.lightViewPosition = mul(vector(In.vPosition, 1.f), g_WorldMatrix);
    Out.lightViewPosition = mul(Out.lightViewPosition, g_LightViewMatrix);
    Out.lightViewPosition = mul(Out.lightViewPosition, g_LightProjMatrix);

    float4 worldPosition = mul(vector(In.vPosition, 1.f), g_WorldMatrix);
    Out.lightPos = lightPosition.xyz - worldPosition.xyz;
    return Out;
}

VS_OUT_LIGHT_DEPTH VS_MAIN_LIGHT_DEPTH(VS_IN In)
{
    VS_OUT_LIGHT_DEPTH Out = (VS_OUT_LIGHT_DEPTH) 0;

    matrix matLightWV, matLightWVP;

    matLightWV = mul(g_WorldMatrix, g_LightViewMatrix);
    matLightWVP = mul(matLightWV, g_LightProjMatrix);
    Out.vPosition = mul(vector(In.vPosition, 1.f), matLightWVP);

    Out.vLightDepthPosition = Out.vPosition;
    Out.vTexUV = In.vTexUV;
    return Out;
}

VS_OUT_LIGHT_DEPTH VS_MAIN_LIGHT_DEPTH_ANIM(VS_IN In)
{
    VS_OUT_LIGHT_DEPTH Out = (VS_OUT_LIGHT_DEPTH) 0;

    matrix matLightWV, matLightWVP;

    matLightWV = mul(g_WorldMatrix, g_LightViewMatrix);
    matLightWVP = mul(matLightWV, g_LightProjMatrix);

    matrix BoneMatrix = g_BoneMatrices.BoneMatrices[In.vBlendIndex.x] * In.vBlendWeight.x +
		g_BoneMatrices.BoneMatrices[In.vBlendIndex.y] * In.vBlendWeight.y +
		g_BoneMatrices.BoneMatrices[In.vBlendIndex.z] * In.vBlendWeight.z +
		g_BoneMatrices.BoneMatrices[In.vBlendIndex.w] * In.vBlendWeight.w;

    vector vPosition = mul(vector(In.vPosition, 1.f), BoneMatrix);
    Out.vPosition = mul(vPosition, matLightWVP);
    Out.vLightDepthPosition = Out.vPosition;
    Out.vTexUV = In.vTexUV;
    return Out;
}


VS_OUT_LIGHT_DEPTH VS_MAIN_LIGHT_DEPTH_ANIM_RAGDOLL(VS_IN In)
{
    VS_OUT_LIGHT_DEPTH Out = (VS_OUT_LIGHT_DEPTH) 0;

    matrix BoneMatrix = g_BoneMatrices.BoneMatrices[In.vBlendIndex.x] * In.vBlendWeight.x +
		g_BoneMatrices.BoneMatrices[In.vBlendIndex.y] * In.vBlendWeight.y +
		g_BoneMatrices.BoneMatrices[In.vBlendIndex.z] * In.vBlendWeight.z +
		g_BoneMatrices.BoneMatrices[In.vBlendIndex.w] * In.vBlendWeight.w;

    matrix matLightWV, matLightWVP;

    matLightWV = mul(BoneMatrix, g_LightViewMatrix);
    matLightWVP = mul(matLightWV, g_LightProjMatrix);

    Out.vPosition = mul(vector(In.vPosition, 1.f), matLightWVP);
    Out.vLightDepthPosition = Out.vPosition;
    Out.vTexUV = In.vTexUV;
    return Out;
}

VS_OUT VS_MAIN_ANIM(VS_IN In)
{
    VS_OUT Out = (VS_OUT) 0;

    matrix matWV, matWVP;

    matWV = mul(g_WorldMatrix, g_ViewMatrix);
    matWVP = mul(matWV, g_ProjMatrix);

    matrix BoneMatrix = g_BoneMatrices.BoneMatrices[In.vBlendIndex.x] * In.vBlendWeight.x +
		g_BoneMatrices.BoneMatrices[In.vBlendIndex.y] * In.vBlendWeight.y +
		g_BoneMatrices.BoneMatrices[In.vBlendIndex.z] * In.vBlendWeight.z +
		g_BoneMatrices.BoneMatrices[In.vBlendIndex.w] * In.vBlendWeight.w;

    vector vPosition = mul(vector(In.vPosition, 1.f), BoneMatrix);
    vector vNormal = mul(vector(In.vNormal, 0.f), BoneMatrix);
    vector vTangent = mul(vector(In.vTangent, 0.f), BoneMatrix);
    vector vBiNormal = mul(vector(In.vBiNormal, 0.f), BoneMatrix);
   
    Out.vPosition = mul(vPosition, matWVP);
    Out.vTexUV = In.vTexUV;
    Out.vProjPos = Out.vPosition;  
   
    Out.vNormal = normalize(mul(vNormal, g_WorldMatrix)).xyz;
    Out.vTangent = normalize(mul(vTangent, g_WorldMatrix)).xyz;
    Out.vBiNormal = normalize(mul(vBiNormal, g_WorldMatrix)).xyz;

    Out.vWPos = mul(vPosition, g_WorldMatrix).xyz;
    Out.lightViewPosition = mul(vPosition, g_WorldMatrix);
    Out.lightViewPosition = mul(Out.lightViewPosition, g_LightViewMatrix);
    Out.lightViewPosition = mul(Out.lightViewPosition, g_LightProjMatrix);
    Out.vTexUV = In.vTexUV;
    
    float4 worldPosition = mul(vector(In.vPosition, 1.f), g_WorldMatrix);
    Out.lightPos = lightPosition.xyz - worldPosition.xyz;

    return Out;
}

VS_OUT VS_MAIN_ANIM_RAGDOLL(VS_IN In)
{
    VS_OUT Out = (VS_OUT) 0;

    matrix matWV, matWVP;
    
    matrix BoneMatrix = g_BoneMatrices.BoneMatrices[In.vBlendIndex.x] * In.vBlendWeight.x +
		g_BoneMatrices.BoneMatrices[In.vBlendIndex.y] * In.vBlendWeight.y +
		g_BoneMatrices.BoneMatrices[In.vBlendIndex.z] * In.vBlendWeight.z +
		g_BoneMatrices.BoneMatrices[In.vBlendIndex.w] * In.vBlendWeight.w;
    vector vNormal = mul(vector(In.vNormal, 0.f), BoneMatrix);

    matWV = mul(BoneMatrix, g_ViewMatrix);
    matWVP = mul(matWV, g_ProjMatrix);

    Out.vPosition = mul(vector(In.vPosition, 1.f), matWVP);
    Out.vNormal = mul(vNormal, g_WorldMatrix);
    Out.vTexUV = In.vTexUV;
    Out.vProjPos = Out.vPosition;


    
    Out.lightViewPosition = mul(vector(In.vPosition, 1.f), g_WorldMatrix);
    Out.lightViewPosition = mul(Out.lightViewPosition, g_LightViewMatrix);
    Out.lightViewPosition = mul(Out.lightViewPosition, g_LightProjMatrix);

    Out.vTexUV = In.vTexUV;



    float4 worldPosition = mul(vector(In.vPosition, 1.f), g_WorldMatrix);
    Out.lightPos = lightPosition.xyz - worldPosition.xyz;

    return Out;
}

struct PS_IN
{
    float4 vPosition : SV_POSITION;
    float2 vTexUV : TEXCOORD0;
    float3 vNormal : NORMAL;
    float3 vTangent : TANGENT;
    float3 vBiNormal : BINORMAL;
    float4 lightViewPosition : TEXCOORD1;
    float3 lightPos : TEXCOORD2;
    float4 vProjPos : TEXCOORD3;
    float3 vWPos : TEXCOORD4;
};

struct PS_IN_LIGHT_DEPTH
{
    float4 vPosition : SV_POSITION;
    float4 vLightDepthPosition : TEXCOORD0;
    float2 vTexUV : TEXCOORD1;
};
struct PS_OUT
{
    vector vDiffuse : SV_TARGET0;
    vector vNormal : SV_TARGET1;
    vector vDepth : SV_TARGET2;
    vector vSpecular : SV_TARGET3;
};

struct PS_OUT_LIGHT_DEPTH
{
    vector vLightDepth : SV_TARGET0;
};

struct PS_OUT_BEHIND_WALL
{
    vector vDiffuse : SV_TARGET0;

};


PS_OUT PS_MAIN(PS_IN In)
{
    PS_OUT Out = (PS_OUT) 0;

    float bias;
    float4 color;
    float4 diffuseColor;
    float4 dissolveColor;
    float2 projectTexCoord;
    float depthValue;
    float lightDepthValue;
    float lightIntensity;
    float4 textureColor;
    float3 lightDir;

    bias = 0.0001f;
 
    color = vector(0.1f, 0.1f, 0.1f, 1.f);
    diffuseColor = vector(0.4f, 0.4f, 0.4f, 1.f);
    lightDir = -(-1.f, -1.f, -1.f);
    
    projectTexCoord.x = In.lightViewPosition.x / In.lightViewPosition.w / 2.f + 0.5f;
    projectTexCoord.y = -In.lightViewPosition.y / In.lightViewPosition.w / 2.f + 0.5f;

    if ((saturate(projectTexCoord.x) == projectTexCoord.x) && (saturate(projectTexCoord.y) == projectTexCoord.y))
    {
        depthValue = ShadowDepthTexture.Sample(g_SamplerClamp, projectTexCoord).x;

        lightDepthValue = In.lightViewPosition.z / In.lightViewPosition.w;
        lightDepthValue = lightDepthValue - bias;

        color = vector(0.8, 0.8f, 0.8f, 1.f);

        if (lightDepthValue < depthValue)
            color = vector(1.f, 1.f, 1.f, 1.f);
    }
    else
        color = vector(1.f, 1.f, 1.f, 1.f);

    color = saturate(color);
    // 이 텍스처 좌표 위치에서 샘플러를 사용하여 텍스처에서 픽셀 색상을 샘플링합니다.
    textureColor = g_DiffuseTexture.Sample(g_DiffuseSampler, In.vTexUV);
    float3 Emission = g_EmissiveTexture.Sample(g_DiffuseSampler, In.vTexUV);
    dissolveColor = g_DissolveTexture.Sample(g_DiffuseSampler, In.vTexUV);
        
    // 빛과 텍스처 색상을 결합합니다.
    color = color * textureColor + (float4(Emission, 0.f) * float4(1.0f, 1.0f, 1.0f, 0.f));

    if (dissolveColor.r < g_fDissolve)
    {
        //if (dissolveColor.r > g_fDissolve - 0.1f && g_fDissolve != 1.f)
        //    color = float4(1.f, 0.2f, 0.2f, 0.8f);
        if (dissolveColor.r > g_fDissolve - 0.1f && g_fDissolve != 1.f)    // Edge부분 체크
        {
            color = float4(1.f, 0.2f, 0.2f, 0.8f);
        }
        else
            color.a = 0.0f; // 확실히 없어질 곳
    }

    if(color.a <= 0.f)
        discard;
    
    vector vNormalDesc = g_NormalTexture.Sample(g_DiffuseSampler, In.vTexUV);
    float3 vNormal = vNormalDesc.xyz * 2.f - 1.f;
    float3x3 TBN = float3x3(In.vTangent, In.vBiNormal, In.vNormal);
    vNormal = mul(vNormal, TBN);

    if (g_RimLightEnable)
    {
        float3 vCamPos = normalize(g_CamPosition - In.vWPos);
        float RimLightIntensity = smoothstep(0.f, 0.5f, 1 - max(0, dot(vNormal, vCamPos)));
        color.xyz += g_RimLitghColor * RimLightIntensity;
    }

    Out.vDiffuse = color;
    Out.vNormal = vector(vNormal.xyz * 0.5f + 0.5f, 1.0f);
    Out.vDepth = vector(In.vProjPos.w / 300.f, In.vProjPos.z / In.vProjPos.w, 0.f, 0.f);    
    Out.vSpecular = g_SpecularTexture.Sample(g_DiffuseSampler, In.vTexUV);

    return Out;
}

PS_OUT_LIGHT_DEPTH PS_MAIN_LIGHT_DEPTH(PS_IN_LIGHT_DEPTH In)
{
    PS_OUT_LIGHT_DEPTH Out = (PS_OUT_LIGHT_DEPTH) 0;
      
    Out.vLightDepth = vector(In.vLightDepthPosition.z / In.vLightDepthPosition.w, 0.f, 0.f, 1.f);

    return Out;
}

PS_OUT_BEHIND_WALL PS_MAIN_WALL(PS_IN In)
{
    PS_OUT_BEHIND_WALL Out = (PS_OUT_BEHIND_WALL) 0;

    float2 projectTexCoord;
    projectTexCoord.x = In.vProjPos.x / In.vProjPos.w / 2.f + 0.5f;
    projectTexCoord.y = -In.vProjPos.y / In.vProjPos.w / 2.f + 0.5f;

    float depthValue = g_DepthTexture.Sample(g_SamplerClamp, projectTexCoord).y;
    float camDapthValue = In.vProjPos.z / In.vProjPos.w;

    if (camDapthValue > depthValue + 0.0012f)
        Out.vDiffuse = vector(1.f, 0.5f, 0.f, 1.f);
    else
        discard;

    return Out;
}


technique11 DefaultDevice
{
    pass NonAnimation
    {
        SetRasterizerState(Rasterizer_None);
        SetDepthStencilState(DepthStecil_Default, 0);
        SetBlendState(Blend_Alpha, vector(0.f, 0.f, 0.f, 0.f), 0xffffffff);

        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN();
    }
    pass Animation
    {
        SetRasterizerState(Rasterizer_Solid);
        SetDepthStencilState(DepthStecil_Default, 0);
        SetBlendState(Blend_Alpha, vector(0.f, 0.f, 0.f, 0.f), 0xffffffff);

        VertexShader = compile vs_5_0 VS_MAIN_ANIM();
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN();
    }
    pass RagdollAnimation
    {
        SetRasterizerState(Rasterizer_Solid);
        SetDepthStencilState(DepthStecil_Default, 0);
        SetBlendState(Blend_Alpha, vector(0.f, 0.f, 0.f, 0.f), 0xffffffff);

        VertexShader = compile vs_5_0 VS_MAIN_ANIM_RAGDOLL();
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN();
    }
    pass BehindWall
    {
        SetRasterizerState(Rasterizer_Solid);
        SetDepthStencilState(DepthStecil_NotZTest, 0);
        SetBlendState(Blend_Alpha, vector(0.f, 0.f, 0.f, 0.f), 0xffffffff);

        VertexShader = compile vs_5_0 VS_MAIN_ANIM();
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN_WALL();
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

    pass LightDepthAnim
    {
        SetRasterizerState(Rasterizer_Solid);
        SetDepthStencilState(DepthStecil_Default, 0);
        SetBlendState(Blend_None, vector(0.f, 0.f, 0.f, 0.f), 0xffffffff);

        VertexShader = compile vs_5_0 VS_MAIN_LIGHT_DEPTH_ANIM();
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN_LIGHT_DEPTH();
    }

    pass LightDepthRagdoll
    {
        SetRasterizerState(Rasterizer_Solid);
        SetDepthStencilState(DepthStecil_Default, 0);
        SetBlendState(Blend_None, vector(0.f, 0.f, 0.f, 0.f), 0xffffffff);

        VertexShader = compile vs_5_0 VS_MAIN_LIGHT_DEPTH_ANIM_RAGDOLL();
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN_LIGHT_DEPTH();
    }

}