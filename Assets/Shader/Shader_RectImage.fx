#include "Shader_Defines.hpp"

cbuffer UiInformation
{
    float g_Percentage = 1.f;
    float g_Back = 1.f;
    float g_Time = 0.f;
    float g_SHTime = 0.f;
    float g_degree = 0.f;
    float g_USpeed = 0.f;
    float g_VSpeed = 0.f;
    bool  g_isHover = false;
    bool g_isSelected = false;
    
};

cbuffer Matrices
{
	matrix		g_WorldMatrix;
	matrix		g_ViewMatrix;
	matrix		g_ProjMatrix;	
}

cbuffer Color
{
    float4 vColor;
};

struct VS_IN
{
	float3 vPosition : POSITION; /* 로컬스페이스 */
	float2 vTexUV : TEXCOORD0;
};

struct VS_OUT
{
	float4	vPosition : SV_POSITION;
    float2 vTexUV : TEXCOORD0;
};

struct VS_OUTMASK
{
    float4 vPosition : SV_POSITION;
    float2 vTexUV : TEXCOORD0;
    float2 vMaskUV : TEXCOORD1;
};

struct VS_OUTITEM
{
    float4 vPosition : SV_POSITION;
    float2 vTexUV : TEXCOORD0;
    float2 vPos : TEXCOORD1;
};

/* 정점의 스페이스 변환. (월드, 뷰, 투영행렬의 곱.)*/
VS_OUT VS_MAIN(VS_IN In)
{
	VS_OUT		Out = (VS_OUT)0;

	matrix		matWV, matWVP;

	matWV = mul(g_WorldMatrix, g_ViewMatrix);
	matWVP = mul(matWV, g_ProjMatrix);

	Out.vPosition = mul(vector(In.vPosition, 1.f), matWVP);
	Out.vTexUV = In.vTexUV;

	return Out;
}

VS_OUTMASK VS_MAIN_SELECTHUD(VS_IN In)
{
    VS_OUTMASK Out = (VS_OUTMASK) 0;
    
    matrix matWV, matWVP;

    matWV = mul(g_WorldMatrix, g_ViewMatrix);
    matWVP = mul(matWV, g_ProjMatrix);

    Out.vPosition = mul(vector(In.vPosition, 1.f), matWVP);
    Out.vTexUV = In.vTexUV;
    if(g_Time<1.f)
        Out.vMaskUV.x = (In.vTexUV.x - 1.f);
    else
        Out.vMaskUV.x = (In.vTexUV.x - 1.f) / g_Time;
    Out.vMaskUV.y = In.vTexUV.y;
    //if (g_Time > 3.f)
    //{
    //    if (Out.vMaskUV.x < 0.1f)
    //        Out.vMaskUV.x =0.1f + g_Time * 0.5f;
    //    else if (Out.vMaskUV.x > 0.9f)
    //        Out.vMaskUV.x = 0.9f + g_Time * 0.5f;
    //    Out.vMaskUV.y = In.vTexUV.y *0.01f;
    //}



    return Out;
}

VS_OUTITEM VS_MAIN_ITEMLIST(VS_IN In){

    VS_OUTITEM Out = (VS_OUTITEM) 0;

    matrix matWV, matWVP;

    matWV = mul(g_WorldMatrix, g_ViewMatrix);
    matWVP = mul(matWV, g_ProjMatrix);

    Out.vPosition = mul(vector(In.vPosition, 1.f), matWVP);
    Out.vTexUV = In.vTexUV;
    Out.vPos = mul(vector(In.vPosition, 1.f), g_WorldMatrix);

    return Out;
}

VS_OUT VS_MAIN_TEXT(VS_IN In)
{
    VS_OUT Out = (VS_OUT) 0;

    matrix matWV, matWVP;

    matWV = mul(g_WorldMatrix, g_ViewMatrix);
    matWVP = mul(matWV, g_ProjMatrix);

    Out.vPosition = mul(vector(In.vPosition, 1.f), matWVP);
    if (g_SHTime > 2.f)
        Out.vTexUV = In.vTexUV.x / (3.f);
    else
        Out.vTexUV = In.vTexUV.x / (g_SHTime + 1.f);
    In.vTexUV.x += g_SHTime * 2.f;
    return Out;
}

VS_OUT VS_MAIN_TEXT1(VS_IN In)
{
    VS_OUT Out = (VS_OUT) 0;

    matrix matWV, matWVP;

    matWV = mul(g_WorldMatrix, g_ViewMatrix);
    matWVP = mul(matWV, g_ProjMatrix);

    Out.vPosition = mul(vector(In.vPosition, 1.f), matWVP);
    Out.vTexUV = In.vTexUV;

    return Out;
}
/* VS_OUT 구조체의 SV_POSITION이라는 시멘틱을 가진 데이터에 한해서만  */
/* W값으로 XYZW모두를 나눈다. */
/* 뷰포트로 정점의 위치를 변환한다. */
/* 래스터라이즈 : 정점으로 둘러쌓여진 픽셀의 정보를 생성하낟.(둘러쌓여진 정점의 정보를 기반으로하여)  */

struct PS_IN
{
	float4	vPosition : SV_POSITION;
    float2 vTexUV : TEXCOORD0;
};
struct PS_INMASK
{
    float4 vPosition : SV_POSITION;
    float2 vTexUV : TEXCOORD0;
    float2 vMaskUV : TEXCOORD1;
};

struct PS_INITEM
{
    float4 vPosition : SV_POSITION;
    float2 vTexUV : TEXCOORD0;
    float2 vPos : TEXCOORD1;
};

SamplerState Sampler
{
    Filter = MIN_MAG_MIP_LINEAR;
};
SamplerState SamplerWrap
{
    AddressU = wrap;
    AddressV = wrap;
};


Texture2D Map;
Texture2D Mask;
Texture2D Noise;
Texture2D Any;


float4 PS_MAIN(PS_IN input) : SV_TARGET
{
    float4 color = Map.Sample(Sampler, input.vTexUV);
    return color;
}

float4 PS_MAIN_COLOR(PS_IN input) : SV_TARGET
{
    float4 color = Map.Sample(Sampler, input.vTexUV);
  //clip(color.a < 0.1f ? -1 : 1);

    return vColor;
}   

float4 PS_MAIN_HP(PS_IN input) : SV_TARGET
{
     float4 color = Map.Sample(Sampler, input.vTexUV);
    clip(color.a < 0.1f ? -1 : 1);
    if (input.vTexUV.x > g_Percentage && input.vTexUV.x < g_Back)
    {
        color.rgb += 0.3f;
    }else if (input.vTexUV.x > g_Percentage)
    {
        discard;
    }
    // color += vColor;
    return color;
}

float4 PS_MAIN_SKILL(PS_IN input) : SV_TARGET
{
    float4 color = Map.Sample(Sampler, input.vTexUV);

    if (g_degree > radians(180.f))
    {
        float2 upVec = float2(1.f, 0.f);
        float2 dirVec = float2(-cos(g_degree), -sin(g_degree));

        float x = input.vTexUV.x -0.5f;
        float y = input.vTexUV.y - 0.5f;
    
        float up = (x) * upVec.x + (y) * upVec.y;
        float dir = (x) * dirVec.x + (y) * dirVec.y;
        if (up < 0 && dir > 0)
        {
            return color;
        }
        else if (x > 0.f)
        {
            return color;
        }
    }
    else
    {
        float2 upVec = float2(-1.f, 0.f);
        float2 dirVec = float2(-cos(g_degree), -sin(g_degree));

        float x = input.vTexUV.x - 0.5f;
        float y = input.vTexUV.y - 0.5f;
    
        float up = (x) * upVec.x + (y) * upVec.y;
        float dir = (x) * dirVec.x + (y) * dirVec.y;
        if (up < 0 && dir > 0)
        {
            return color;
        }
    }

    
    color.a = 0.f;
    return color;
}

float4 PS_MAIN_ALRET(PS_IN input) : SV_TARGET
{
    float4 color = Map.Sample(Sampler, input.vTexUV);
    color.rgb = 0.65f;
    return color;
}

float4 PS_MAIN_HUD(PS_INMASK input) : SV_TARGET
{
    float4 diffuse = Map.Sample(Sampler, input.vTexUV);
    float4 mask = Mask.Sample(SamplerWrap, input.vMaskUV);
    
    float4 color = (float4)0;
    if (g_isHover)
    {
        color = (1.f - diffuse.r) * 0.7f + (mask * g_Time) * 0.3f;
        color.r += 0.2f;    
    }
    else
    {
        color.a = 1.f - diffuse.r;
        color.rgb = diffuse.r + 0.1f;
    }
    

    return color;
}

float4 PS_MAIN_DISALPHA(PS_IN input) : SV_TARGET
{
    float4 color = Map.Sample(Sampler, input.vTexUV);
    float alpha = (color.r + color.g + color.b) / 3.f;
    color.a = alpha;
    return color;
}

float4 PS_MAIN_ITEMLIST(PS_INITEM input) : SV_TARGET
{
    float4 color = Map.Sample(Sampler, input.vTexUV);
    if(input.vPos.y < 300.f && input.vPos.y > -300.f)
        return color;
    else 
        discard;
    return color;
}

float4 PS_MAIN_ALPHAPLUS(PS_IN input) : SV_Target
{
    float4 color = Map.Sample(Sampler, input.vTexUV);
    color += 0.5f;
    return color;
}

float4 PS_MAIN_SELECTEDTEXT(PS_IN input) : SV_Target
{
    float4 color = Map.Sample(Sampler, input.vTexUV);
    if(input.vTexUV.x > 0.5f)
        discard;
    if (g_isSelected || g_isHover)
    {
        color.a = color.r - 0.5f;
        color.r = color.r;
        color.gb = (color.r) * 0.5f;
    }
    else
    {
        return 0;
    }
    return color;
}

float4 PS_MAIN_SELECTEDTEXT1(PS_IN input) : SV_Target
{
    float4 color = Map.Sample(Sampler, input.vTexUV);
    if (g_isSelected || g_isHover)
    {
        color.a = color.r;
        color.r = color.r - 0.3f;
        color.gb = (color.r - 0.3f) * 0.5f;
    }
    else
    {
        return 0;
    }
    return color;
}


float4 PS_DEFAULT(PS_IN input) : SV_TARGET
{
    return vColor;
}

technique11		DefaultDevice
{
	pass DefaultPass
	{
        SetRasterizerState(Rasterizer_Solid);
        SetDepthStencilState(DepthStecil_NotZTestWrite, 0);
        SetBlendState(Blend_Alpha, vector(0.f, 0.f, 0.f, 0.f), 0xffffffff);

		VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_DEFAULT();
    }

    pass WithImage
    {
        SetRasterizerState(Rasterizer_Solid);
        SetDepthStencilState(DepthStecil_NotZTestWrite, 0);
        SetBlendState(Blend_Alpha, vector(0.f, 0.f, 0.f, 0.f), 0xffffffff);

        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN();
    }

    pass ImageColor
    {
        SetRasterizerState(Rasterizer_Solid);
        SetDepthStencilState(DepthStecil_NotZTestWrite, 0);
        SetBlendState(Blend_Alpha, vector(0.f, 0.f, 0.f, 0.f), 0xffffffff);

        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN_COLOR();
    }

    pass HpHud
    {
        SetRasterizerState(Rasterizer_Solid);
        SetDepthStencilState(DepthStecil_NotZTestWrite, 0);
        SetBlendState(Blend_Alpha, vector(0.f, 0.f, 0.f, 0.f), 0xffffffff);

        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN_HP();
    }

    pass Skill
    {
        SetRasterizerState(Rasterizer_Solid);
        SetDepthStencilState(DepthStecil_NotZTestWrite, 0);
        SetBlendState(Blend_Alpha, vector(0.f, 0.f, 0.f, 0.f), 0xffffffff);

        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN_SKILL();
    }
    pass Alret
    {
        SetRasterizerState(Rasterizer_Solid);
        SetDepthStencilState(DepthStecil_NotZTestWrite, 0);
        SetBlendState(Blend_Alpha, vector(0.f, 0.f, 0.f, 0.f), 0xffffffff);

        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN_ALRET();
    }
    pass SelectHUD
    {
        SetRasterizerState(Rasterizer_Solid);
        SetDepthStencilState(DepthStecil_NotZTestWrite, 0);
        SetBlendState(Blend_Alpha, vector(0.f, 0.f, 0.f, 0.f), 0xffffffff);

        VertexShader = compile vs_5_0 VS_MAIN_SELECTHUD();
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN_HUD();
    }
    pass DisableAlpha
    {
        SetRasterizerState(Rasterizer_Solid);
        SetDepthStencilState(DepthStecil_NotZTestWrite, 0);
        SetBlendState(Blend_One, vector(0.f, 0.f, 0.f, 0.f), 0xffffffff);

        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN_DISALPHA();
    }
    pass ItemListHUD
    {
        SetRasterizerState(Rasterizer_Solid);
        SetDepthStencilState(DepthStecil_NotZTestWrite, 0);
        SetBlendState(Blend_Alpha, vector(0.f, 0.f, 0.f, 0.f), 0xffffffff);

        VertexShader = compile vs_5_0 VS_MAIN_ITEMLIST();
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN_ITEMLIST();
    }
    pass AlphaPlus
    {
        SetRasterizerState(Rasterizer_Solid);
        SetDepthStencilState(DepthStecil_NotZTestWrite, 0);
        SetBlendState(Blend_Alpha, vector(0.f, 0.f, 0.f, 0.f), 0xffffffff);

        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN_ALPHAPLUS();
    }
    pass SelectText0
    {
        SetRasterizerState(Rasterizer_Solid);
        SetDepthStencilState(DepthStecil_NotZTestWrite, 0);
        SetBlendState(Blend_Alpha, vector(0.f, 0.f, 0.f, 0.f), 0xffffffff);

        VertexShader = compile vs_5_0 VS_MAIN_TEXT();
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN_SELECTEDTEXT();
    }
    pass SelectText1
    {
        SetRasterizerState(Rasterizer_Solid);
        SetDepthStencilState(DepthStecil_NotZTestWrite, 0);
        SetBlendState(Blend_Alpha, vector(0.f, 0.f, 0.f, 0.f), 0xffffffff);

        VertexShader = compile vs_5_0 VS_MAIN_TEXT1();
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN_SELECTEDTEXT1();
    }
}