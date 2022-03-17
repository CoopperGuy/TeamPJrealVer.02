
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

Texture2D		g_DiffuseTexture;

SamplerState	g_DiffuseSampler
{
	AddressU = mirror;
	AddressV = mirror;
};

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
};

/* 정점의 스페이스 변환. (월드, 뷰, 투영행렬의 곱.)*/
VS_OUT VS_MAIN(VS_IN In)
{
	//VS_OUT		Out = (VS_OUT)0;


 //   matrix WorldMatrix = float4x4(In.vRight, In.vUp, In.vLook, In.vTranslation);

 //   matrix matWV, matWVP;
 //   matWV = mul(WorldMatrix, g_ViewMatrix);
 //   matWVP = mul(matWV, g_ProjMatrix);



 //   Out.vPosition = mul(vector(In.vPosition, 1.f), matWVP);
	//Out.vSize = In.vSize;


    VS_OUT Out = (VS_OUT) 0;

    matrix matWV, matWVP;

    matrix WorldMatrix = float4x4(In.vRight, In.vUp, In.vLook, In.vTranslation);

    Out.vPosition = mul(vector(In.vPosition, 1.f), WorldMatrix);

	return Out;
}

struct GS_OUT
{
	float4	vPosition : SV_POSITION;
};

[maxvertexcount(6)]
void GS_MAIN( /*입력*/line VS_OUT points[2], /*출력*/inout TriangleStream<GS_OUT> Out)
{
    float3 inVertex1Pos = points[0].vPosition.xyz;
    float3 inVertex2Pos = points[1].vPosition.xyz;


    float3 lineVec = normalize(inVertex1Pos - inVertex2Pos);

    float3 lineCenter = (inVertex1Pos + inVertex2Pos) / 2.f;
    float3 towardsCamVec = normalize(g_vCamPosition.xyz - lineCenter);

    float3 localUpVec = normalize(cross(lineVec, towardsCamVec));

    GS_OUT v[6];
    float LineThinkness = 0.01f;
    v[0].vPosition.xyz = inVertex1Pos + LineThinkness * localUpVec;
    v[1].vPosition.xyz = inVertex2Pos + LineThinkness * localUpVec;
    v[2].vPosition.xyz = inVertex1Pos - LineThinkness * localUpVec;
    v[3].vPosition.xyz = inVertex1Pos - LineThinkness * localUpVec;
    v[4].vPosition.xyz = inVertex2Pos + LineThinkness * localUpVec;
    v[5].vPosition.xyz = inVertex2Pos - LineThinkness * localUpVec;
    
    matrix matVP = mul(g_ViewMatrix, g_ProjMatrix);

    for (int i = 0; i < 6; i++)
    {
        v[i].vPosition.w = 1.f;
        v[i].vPosition = mul(v[i].vPosition, matVP);
    }

    Out.Append(v[0]);
    Out.Append(v[1]);
    Out.Append(v[2]);
    Out.RestartStrip();
    Out.Append(v[3]);
    Out.Append(v[4]);
    Out.Append(v[5]);
    Out.RestartStrip();

    //float4 p0 = points[0].vPosition;
    //float4 p1 = points[1].vPosition;

    //float w0 = p0.w;
    //float w1 = p1.w;

    //p0.xyz /= p0.w;
    //p1.xyz /= p1.w;

    //float3 line01 = p1 - p0;
    //float3 dir = normalize(line01);

    //float2 RenderTargetSize = { 1920, 1280 };
    //float3 ratio = float3(RenderTargetSize.y, RenderTargetSize.x, 0);
    //ratio = normalize(ratio);

    //float3 unit_z = normalize(float3(0, 0, -1));

    //float3 normal = normalize(cross(unit_z, dir) * ratio);

    //float width = 0.01;

    //GS_OUT v[4];

    //float3 dir_offset = dir * ratio * width;
    //float3 normal_scaled = normal * ratio * width;

    //float3 p0_ex = p0.xyz - dir_offset;
    //float3 p1_ex = p1.xyz + dir_offset;

    //v[0].vPosition = float4(p0_ex - normal_scaled, 1) * w0;
    //v[0].vTexUV = float2(0, 0);

    //v[1].vPosition = float4(p0_ex + normal_scaled, 1) * w0;
    //v[1].vTexUV = float2(0, 0);

    //v[2].vPosition = float4(p1_ex + normal_scaled, 1) * w1;
    //v[2].vTexUV = float2(0, 0);

    //v[3].vPosition = float4(p1_ex - normal_scaled, 1) * w1;
    //v[3].vTexUV = float2(0, 0);

    //Out.Append(v[2]);
    //Out.Append(v[1]);
    //Out.Append(v[0]);

    //Out.RestartStrip();

    //Out.Append(v[3]);
    //Out.Append(v[2]);
    //Out.Append(v[0]);

    //Out.RestartStrip();
}

/* VS_OUT 구조체의 SV_POSITION이라는 시멘틱을 가진 데이터에 한해서만  */
/* W값으로 XYZW모두를 나눈다. */
/* 뷰포트로 정점의 위치를 변환한다. */
/* 래스터라이즈 : 정점으로 둘러쌓여진 픽셀의 정보를 생성하낟.(둘러쌓여진 정점의 정보를 기반으로하여)  */

struct PS_IN
{
    float4 vPosition : SV_POSITION;
};

vector	PS_MAIN(PS_IN In) : SV_TARGET
{
    vector vColor = (vector) 0;
    
    vColor.r = 1;
    vColor.a = 1;

    return vColor;
}



technique11		DefaultDevice
{
	pass DefaultPass
	{
        SetRasterizerState(Rasterizer_Sky);
		SetDepthStencilState(DepthStecil_Default, 0);
		SetBlendState(Blend_Alpha, vector(0.f, 0.f, 0.f, 0.f), 0xffffffff);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = compile gs_5_0 GS_MAIN();
		PixelShader = compile ps_5_0 PS_MAIN();
	}	
}