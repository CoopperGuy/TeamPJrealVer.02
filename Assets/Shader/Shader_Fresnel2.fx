
#include "Shader_Defines.hpp"

cbuffer Matrices
{
    matrix g_WorldMatrix;
    matrix g_ViewMatrix;
    matrix g_ProjMatrix;
}

Texture2D g_DiffuseTexture;
SamplerState g_DiffuseSampler
{
    AddressU = mirror;
    AddressV = mirror;
};

struct VS_IN
{
    float4 vPosition : POSITION; /* ���ý����̽� */
};

struct VS_OUT
{
    float4 vPosition : SV_POSITION;
    float4 vDepthPosition : TEXCOORD0;
};

/* ������ �����̽� ��ȯ. (����, ��, ��������� ��.)*/
VS_OUT VS_MAIN(VS_IN In)
{
    VS_OUT Out = (VS_OUT) 0;

    In.vPosition.w = 1.f;

    matrix matWV, matWVP;

    matWV = mul(g_WorldMatrix, g_ViewMatrix);
    matWVP = mul(matWV, g_ProjMatrix);

    Out.vPosition = mul(In.vPosition, matWVP);
    Out.vDepthPosition = Out.vPosition;
    return Out;
}

struct PS_IN
{
    float4 vPosition : SV_POSITION;
    float4 vDepthPosition : TEXCOORD0;
};

struct PS_OUT
{
    vector vDepth : SV_TARGET0;
};


PS_OUT PS_MAIN(PS_IN In)
{
    PS_OUT Out = (PS_OUT) 0;
    // In.vProjPos.w = view space���� z��
    // 0- 1���� ���� ���� �����ϱ⿡ far�� ������ ������ �� �ְ� �����
    // In.vProjPos.z / In.vProjPos.w �� �ᱹ In.vPosition.z�� �ƴѰ�?
    // Out.vDepth = vector(In.vProjPos.w / 300.f, In.vProjPos.z / In.vProjPos.w, 0.f, 0.f);
    Out.vDepth = vector(In.vDepthPosition.w / 300.f, In.vDepthPosition.z / In.vDepthPosition.w, 0.f, 0.f);


    return Out;
}

technique11 DefaultDevice
{
    pass Default
    {
        SetRasterizerState(Rasterizer_Solid);
        SetDepthStencilState(DepthStecil_Default, 0);
        SetBlendState(Blend_Alpha, vector(0.f, 0.f, 0.f, 0.f), 0xffffffff);

        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN();
    }
}