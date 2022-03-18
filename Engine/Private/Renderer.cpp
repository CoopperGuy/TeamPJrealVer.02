#include "EnginePCH.h"
#include "..\public\Renderer.h"
#include "GameObject.h"
#include "EmptyUI.h"
#include "TargetManager.h"
#include "LightManager.h"
#include "VIBuffer_Rect_Viewport.h"
#include "Engine.h"

CRenderer::CRenderer(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
	: CComponent(pDevice, pDeviceContext)
	, m_pTargetManager(CTargetManager::GetInstance())
{

}

CRenderer::CRenderer(const CRenderer & rhs)
	: CComponent(rhs)
{

}

HRESULT CRenderer::InitializePrototype()
{
	if (FAILED(__super::InitializePrototype()))
		return E_FAIL;

	m_iValue = 4;
	_uint	iNumViewports = 1;

	D3D11_VIEWPORT			ViewportDesc;

	m_pDeviceContext->RSGetViewports(&iNumViewports, &ViewportDesc);
	
	/* MainBuffer */
	if (FAILED(m_pTargetManager->Add_RenderTarget(m_pDevice, m_pDeviceContext, "Target_Main", (_uint)ViewportDesc.Width, (_uint)ViewportDesc.Height, DXGI_FORMAT_R16G16B16A16_FLOAT, _float4(0.f, 0.f, 0.f, 0.f))))
		return E_FAIL;

	/* Target_Diffuse */
	if (FAILED(m_pTargetManager->Add_RenderTarget(m_pDevice, m_pDeviceContext, "Target_Diffuse", (_uint)ViewportDesc.Width, (_uint)ViewportDesc.Height, DXGI_FORMAT_R16G16B16A16_FLOAT, _float4(0.f, 0.f, 0.f, 0.f))))
		return E_FAIL;

	/* Target_Normal*/
	if (FAILED(m_pTargetManager->Add_RenderTarget(m_pDevice, m_pDeviceContext, "Target_Normal", (_uint)ViewportDesc.Width, (_uint)ViewportDesc.Height, DXGI_FORMAT_R16G16B16A16_FLOAT, _float4(0.f, 0.f, 0.f, 1.f))))
		return E_FAIL;

	/* Target_Depth*/
	if (FAILED(m_pTargetManager->Add_RenderTarget(m_pDevice, m_pDeviceContext, "Target_Depth", (_uint)ViewportDesc.Width, (_uint)ViewportDesc.Height, DXGI_FORMAT_R32G32B32A32_FLOAT, _float4(0.f, 0.f, 0.f, 1.f))))
		return E_FAIL;

	/* Target_Shadow_Depth*/
	if (FAILED(m_pTargetManager->Add_RenderTarget(m_pDevice, m_pDeviceContext, "Target_ShadowDepth", (_uint)ViewportDesc.Width * 10, (_uint)ViewportDesc.Height * 10, DXGI_FORMAT_R32G32B32A32_FLOAT, _float4(0.f, 0.f, 1.f, 1.f))))
		return E_FAIL;

	/* Target_Reflect*/
	if (FAILED(m_pTargetManager->Add_RenderTarget(m_pDevice, m_pDeviceContext, "Target_Specular2", (_uint)ViewportDesc.Width, (_uint)ViewportDesc.Height, DXGI_FORMAT_R32G32B32A32_FLOAT, _float4(0.f, 0.f, 0.f, 1.f))))
		return E_FAIL;

	/* Target_Shade */
	if (FAILED(m_pTargetManager->Add_RenderTarget(m_pDevice, m_pDeviceContext, "Target_Shade", (_uint)ViewportDesc.Width, (_uint)ViewportDesc.Height, DXGI_FORMAT_R16G16B16A16_FLOAT, _float4(0.f, 0.f, 0.f, 1.f))))
		return E_FAIL;

	/* Target_Specular */
	if (FAILED(m_pTargetManager->Add_RenderTarget(m_pDevice, m_pDeviceContext, "Target_Specular", (_uint)ViewportDesc.Width, (_uint)ViewportDesc.Height, DXGI_FORMAT_R16G16B16A16_FLOAT, _float4(0.f, 0.f, 0.f, 0.f))))
		return E_FAIL;

	/* Target_HDR */
	if (FAILED(m_pTargetManager->Add_RenderTarget(m_pDevice, m_pDeviceContext, "Target_HDR_Diffuse", (_uint)ViewportDesc.Width, (_uint)ViewportDesc.Height, DXGI_FORMAT_R16G16B16A16_FLOAT, _float4(0.f, 0.f, 0.f, 0.f))))
		return E_FAIL;

	if (FAILED(m_pTargetManager->Add_RenderTarget(m_pDevice, m_pDeviceContext, "Target_HDR", (_uint)ViewportDesc.Width, (_uint)ViewportDesc.Height, DXGI_FORMAT_R16G16B16A16_FLOAT, _float4(0.f, 0.f, 0.f, 0.f))))
		return E_FAIL;

	if (FAILED(m_pTargetManager->Add_RenderTarget(m_pDevice, m_pDeviceContext, "Target_Bright", (_uint)ViewportDesc.Width, (_uint)ViewportDesc.Height, DXGI_FORMAT_R16G16B16A16_FLOAT, _float4(0.f, 0.f, 0.f, 0.f))))
		return E_FAIL;

	/* Target_Bloom */
	if (FAILED(m_pTargetManager->Add_RenderTarget(m_pDevice, m_pDeviceContext, "Target_DownTexture", (_uint)ViewportDesc.Width / m_iValue, (_uint)ViewportDesc.Height / m_iValue, DXGI_FORMAT_R16G16B16A16_FLOAT, _float4(0.f, 0.f, 0.f, 0.f))))
		return E_FAIL;

	if (FAILED(m_pTargetManager->Add_RenderTarget(m_pDevice, m_pDeviceContext, "Target_BlurX", (_uint)ViewportDesc.Width / m_iValue, (_uint)ViewportDesc.Height / m_iValue, DXGI_FORMAT_R16G16B16A16_FLOAT, _float4(0.f, 0.f, 0.f, 0.f))))
		return E_FAIL;

	if (FAILED(m_pTargetManager->Add_RenderTarget(m_pDevice, m_pDeviceContext, "Target_BlurY", (_uint)ViewportDesc.Width / m_iValue, (_uint)ViewportDesc.Height / m_iValue, DXGI_FORMAT_R16G16B16A16_FLOAT, _float4(0.f, 0.f, 0.f, 0.f))))
		return E_FAIL;
	
	/* Target_SSAO */
	if (FAILED(m_pTargetManager->Add_RenderTarget(m_pDevice, m_pDeviceContext, "Target_SSAO", (_uint)ViewportDesc.Width, (_uint)ViewportDesc.Height, DXGI_FORMAT_R16G16B16A16_FLOAT, _float4(0.f, 0.f, 0.f, 0.f))))
		return E_FAIL;

	if (FAILED(m_pTargetManager->Add_RenderTarget(m_pDevice, m_pDeviceContext, "Target_SSAO_Blur", (_uint)ViewportDesc.Width, (_uint)ViewportDesc.Height, DXGI_FORMAT_R16G16B16A16_FLOAT, _float4(0.f, 0.f, 0.f, 0.f))))
		return E_FAIL;
	
	/* MRT_Deferred */
	if (FAILED(m_pTargetManager->Add_MRT("MRT_Deferred", "Target_Diffuse")))
		return E_FAIL;
	if (FAILED(m_pTargetManager->Add_MRT("MRT_Deferred", "Target_Normal")))
		return E_FAIL;
	if (FAILED(m_pTargetManager->Add_MRT("MRT_Deferred", "Target_Depth")))
		return E_FAIL;
	if (FAILED(m_pTargetManager->Add_MRT("MRT_Deferred", "Target_Specular2")))
		return E_FAIL;
	/* MRT_LightAcc */
	if (FAILED(m_pTargetManager->Add_MRT("MRT_LightAcc", "Target_Shade")))
		return E_FAIL;
	if (FAILED(m_pTargetManager->Add_MRT("MRT_LightAcc", "Target_Specular")))
		return E_FAIL;

	/* MRT_Shader */
	if (FAILED(m_pTargetManager->Add_MRT("MRT_HDR", "Target_HDR")))
		return E_FAIL;	
	if (FAILED(m_pTargetManager->Add_MRT("MRT_HDR", "Target_Bright")))
		return E_FAIL;
	/*if (FAILED(m_pTargetManager->Add_MRT("MRT_Bloom", "Target_DownTexture")))
		return E_FAIL;*/
	if (FAILED(m_pTargetManager->Add_MRT("MRT_Bloom", "Target_BlurX")))
		return E_FAIL;
	if (FAILED(m_pTargetManager->Add_MRT("MRT_Bloom", "Target_BlurY")))
		return E_FAIL;
	if (FAILED(m_pTargetManager->Add_MRT("MRT_SSAO", "Target_SSAO")))
		return E_FAIL; 
	if (FAILED(m_pTargetManager->Add_MRT("MRT_SSAO", "Target_SSAO_Blur")))
		return E_FAIL;

	if (FAILED(m_pTargetManager->Add_MRT("MRT_ShadowDepth", "Target_ShadowDepth")))
		return E_FAIL;
	if (FAILED(m_pTargetManager->Add_MRT("MRT_Main", "Target_Main")))
		return E_FAIL;
	

	if (CEngine::GetInstance()->GetCurrentUsage() == CEngine::USAGE::USAGE_TOOL)
	{
		if (FAILED(m_pTargetManager->Add_RenderTarget(m_pDevice, m_pDeviceContext, "Target_EditorWindow", (_uint)ViewportDesc.Width, (_uint)ViewportDesc.Height, DXGI_FORMAT_R32G32B32A32_FLOAT, _float4(0.f, 0.f, 0.f, 1.f))))
			return E_FAIL;
		if (FAILED(m_pTargetManager->Add_MRT("MRT_EditorWindow", "Target_EditorWindow")))
			return E_FAIL;
	}
	
	m_pVIBuffer = CVIBuffer_Rect_Viewport::Create(m_pDevice, m_pDeviceContext, 0.f, 0.f, ViewportDesc.Width, ViewportDesc.Height, "../../Assets/Shader/Shader_Rect_Viewport.fx");
	if (nullptr == m_pVIBuffer)
		return E_FAIL;
	
	m_pVIBuffer_HDR = CVIBuffer_Rect_Viewport::Create(m_pDevice, m_pDeviceContext, 0.f, 0.f, ViewportDesc.Width, ViewportDesc.Height, "../../Assets/Shader/Shader_HDR.fx");
	if (nullptr == m_pVIBuffer_HDR)
		return E_FAIL;

	m_pVIBuffer_Bloom = CVIBuffer_Rect_Viewport::Create(m_pDevice, m_pDeviceContext, 0.f, 0.f, ViewportDesc.Width, ViewportDesc.Height, "../../Assets/Shader/Shader_Bloom.fx");
	if (nullptr == m_pVIBuffer_Bloom)
		return E_FAIL;

	m_pVIBuffer_SSAO = CVIBuffer_Rect_Viewport::Create(m_pDevice, m_pDeviceContext, 0.f, 0.f, ViewportDesc.Width, ViewportDesc.Height, "../../Assets/Shader/Shader_SSAO.fx");
	if (nullptr == m_pVIBuffer_SSAO)
		return E_FAIL;

	if (FAILED(m_pTargetManager->Ready_DebugBuffer("Target_Main", 0.f, 0.f, 200.f, 200.f)))
		return E_FAIL;
	if (FAILED(m_pTargetManager->Ready_DebugBuffer("Target_Normal", 0.f, 200.f, 200.f, 200.f)))
		return E_FAIL;	
	if (FAILED(m_pTargetManager->Ready_DebugBuffer("Target_Depth", 0.f, 400.f, 200.f, 200.f)))
		return E_FAIL; 
	//if (FAILED(m_pTargetManager->Ready_DebugBuffer("Target_Depth", 200.f, 0.f, 200.f, 200.f)))
		//return E_FAIL;
	/*if (FAILED(m_pTargetManager->Ready_DebugBuffer("Target_BlurY", 0.f, 200.f, 200.f, 200.f)))
		return E_FAIL;*/
	
	/*if (FAILED(m_pTargetManager->Ready_DebugBuffer("Target_Specular", 200.f, 200.f, 200.f, 200.f)))
		return E_FAIL;*/

	//if (FAILED(m_pTargetManager->Ready_DebugBuffer("Target_Shadow0", 0.f, 600.f, 200.f, 200.f)))
	//	return E_FAIL;
	//if (FAILED(m_pTargetManager->Ready_DebugBuffer("Target_Shadow1", 0.f, 800.f, 200.f, 200.f)))
	//	return E_FAIL;

	m_pTargetManager->Initialize(m_pDevice, m_pDeviceContext);

	return S_OK;
}

HRESULT CRenderer::Initialize(void * pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;


	return S_OK;
}

HRESULT CRenderer::AddRenderGroup(RENDER eRenderID, CGameObject * pRenderObject)
{
	if (eRenderID >= RENDER_END ||
		nullptr == pRenderObject)
		return E_FAIL;

	m_RenderGroups[eRenderID].push_back(pRenderObject);

	SafeAddRef(pRenderObject);

	return S_OK;
}

HRESULT CRenderer::DrawRenderGroup()
{
	//m_pTargetManager->Initialize(m_pDeviceContext);

	if (CEngine::GetInstance()->GetCurrentUsage() == CEngine::USAGE::USAGE_TOOL)
	{
		m_pTargetManager->Clear_MRT(m_pDeviceContext, "MRT_EditorWindow");
		m_pTargetManager->Set_MRT(m_pDeviceContext, "MRT_EditorWindow");
	}
	else 
	{
		m_pTargetManager->Clear_MainRT(m_pDeviceContext);
	}
	if (FAILED(RenderPriority()))
		return E_FAIL;

#ifndef _DEBUG
	//if (FAILED(RenderLightDepth()))
	//	return E_FAIL;
#endif // !_DEBUG	

	if (FAILED(RenderNonAlpha()))
		return E_FAIL;

	if (FAILED(Render_LightAcc()))
		return E_FAIL;

	if (FAILED(Render_Blend()))
		return E_FAIL;

	//if (FAILED(Render_Extra()))
		//return E_FAIL;

	if (FAILED(RenderAlpha()))
		return E_FAIL;

	if (FAILED(RenderTrail()))
		return E_FAIL;

	if (FAILED(RenderUI()))
		return E_FAIL;

	if (FAILED(RenderText()))
		return E_FAIL;

	if (CEngine::GetInstance()->GetCurrentUsage() == CEngine::USAGE::USAGE_CLIENT)
	{
		//if (FAILED(m_pTargetManager->Render_DebugBuffers("MRT_Deferred")))
			//return E_FAIL;
		//if (FAILED(m_pTargetManager->Render_DebugBuffers("MRT_Bloom")))
			//return E_FAIL;
	}
	if (FAILED(m_pTargetManager->Render_DebugBuffers("MRT_Main")))
		return E_FAIL;

	//CLightManager::GetInstance()->Render_DebugBuffer();

	if (CEngine::GetInstance()->GetCurrentUsage() == CEngine::USAGE::USAGE_TOOL)
	{
		if (FAILED(m_pTargetManager->End_MRT(m_pDeviceContext)))
			return E_FAIL;
	}
	return S_OK;
}

ID3D11ShaderResourceView * CRenderer::GetShaderResourceView(string pTargetTag)
{
	return m_pTargetManager->GetShaderResourceView(pTargetTag);
}

HRESULT CRenderer::RenderPriority()
{
	if (CEngine::GetInstance()->GetCurrentUsage() == CEngine::USAGE::USAGE_CLIENT)
		m_pTargetManager->Begin_MainRT(m_pDeviceContext);

	for (auto& pGameObject : m_RenderGroups[RENDER_PRIORITY])
	{
		if (nullptr != pGameObject)
		{
			if (FAILED(pGameObject->Render()))
				return E_FAIL;

			SafeRelease(pGameObject);
		}
	}
	m_RenderGroups[RENDER_PRIORITY].clear();

	if (CEngine::GetInstance()->GetCurrentUsage() == CEngine::USAGE::USAGE_CLIENT)
	{
		if (FAILED(m_pTargetManager->End_MRT(m_pDeviceContext)))
			return E_FAIL;
	}

	return S_OK;
}

HRESULT CRenderer::RenderLightDepth()
{
	CLightManager::GetInstance()->SortLight();

	/*_uint numLights = CLightManager::GetInstance()->GetNumRenderLights();
	for (int i = 0; i < (_int)numLights; ++i)
	{
		CLightManager::GetInstance()->SetCurrentIndex(i);

		string targetName = CLightManager::GetInstance()->GetTargetName(i);*/
		if (FAILED(m_pTargetManager->Begin_ShadowRT(m_pDeviceContext, "Target_ShadowDepth")))
			return E_FAIL;

		for (auto& pGameObject : m_RenderGroups[RENDER_NONALPHA])
		{
			if (nullptr != pGameObject)
			{
				if (FAILED(pGameObject->Render(4)))
					return E_FAIL;
			}
		}
		
		if (CEngine::GetInstance()->GetCurrentUsage() == CEngine::USAGE::USAGE_TOOL)
		{
			m_pTargetManager->End_MRT(m_pDeviceContext);

			if (FAILED(m_pTargetManager->Set_MRT(m_pDeviceContext, "MRT_EditorWindow")))
				return E_FAIL;
		}
		else
		{
			if (FAILED(m_pTargetManager->End_MRT(m_pDeviceContext)))
				return E_FAIL;
		}
	//}
	if (CEngine::GetInstance()->GetCurrentUsage() == CEngine::USAGE::USAGE_TOOL)
	{
		if (FAILED(m_pTargetManager->Set_MRT(m_pDeviceContext, "MRT_EditorWindow")))
			return E_FAIL;
	}
	else
	{
		if (FAILED(m_pTargetManager->End_MRT(m_pDeviceContext)))
			return E_FAIL;
	}


	return S_OK;
}

HRESULT CRenderer::RenderNonAlpha()
{
	if (nullptr == m_pTargetManager)
		return E_FAIL;

	// Light depth�� ���Ѵ�

	//if (CEngine::GetInstance()->GetCurrentUsage() == CEngine::USAGE::USAGE_CLIENT)
	//{
	//	if (FAILED(m_pTargetManager->Begin_MRT(m_pDeviceContext, "MRT_Shadow")))
	//		return E_FAIL;

	//	for (auto& pGameObject : m_RenderGroups[RENDER_NONALPHA])
	//	{
	//		if (nullptr != pGameObject)
	//		{
	//			if (FAILED(pGameObject->Render(4)))
	//				return E_FAIL;

	//			// SafeRelease(pGameObject);
	//		}
	//	}
	//	if (FAILED(m_pTargetManager->End_MRT(m_pDeviceContext)))
	//		return E_FAIL;
	//}


	if (FAILED(m_pTargetManager->Begin_MRT(m_pDeviceContext, "MRT_Deferred")))
		return E_FAIL;


	for (auto& pGameObject : m_RenderGroups[RENDER_NONALPHA])
	{
		if (nullptr != pGameObject)
		{
			if (FAILED(pGameObject->Render()))
				return E_FAIL;

			SafeRelease(pGameObject);
		}
	}
	m_RenderGroups[RENDER_NONALPHA].clear();

	if (CEngine::GetInstance()->GetCurrentUsage() == CEngine::USAGE::USAGE_TOOL)
	{
		if (FAILED(m_pTargetManager->Set_MRT(m_pDeviceContext, "MRT_EditorWindow")))
			return E_FAIL;
	}
	else
	{
		if (FAILED(m_pTargetManager->End_MRT(m_pDeviceContext)))
			return E_FAIL;
	}


	return S_OK;
}

HRESULT CRenderer::Render_LightAcc()
{
	if (nullptr == m_pTargetManager)
		return E_FAIL;

	CLightManager*		pLights = GET_INSTANCE(CLightManager);

	m_pTargetManager->Begin_MRT(m_pDeviceContext, "MRT_LightAcc");

	pLights->Render_Lights();

	if (CEngine::GetInstance()->GetCurrentUsage() == CEngine::USAGE::USAGE_TOOL)
	{
		if (FAILED(m_pTargetManager->Set_MRT(m_pDeviceContext, "MRT_EditorWindow")))
			return E_FAIL;
	}
	else
	{
		if (FAILED(m_pTargetManager->End_MRT(m_pDeviceContext)))
			return E_FAIL;
	}
	//m_pTargetManager->End_MRT(m_pDeviceContext);

	RELEASE_INSTANCE(CLightManager);

	return S_OK;
}

HRESULT CRenderer::Render_Blend()
{
	if (nullptr == m_pTargetManager)
		return E_FAIL;
	
	if (CEngine::GetInstance()->GetCurrentUsage() == CEngine::USAGE::USAGE_TOOL)
	{
		ID3D11ShaderResourceView*	pDiffuseSRV = m_pTargetManager->GetShaderResourceView("Target_Diffuse");
		if (nullptr == pDiffuseSRV)
			return E_FAIL;

		ID3D11ShaderResourceView*	pShadeSRV = m_pTargetManager->GetShaderResourceView("Target_Shade");
		if (nullptr == pShadeSRV)
			return E_FAIL;

		ID3D11ShaderResourceView*	pSpecularSRV = m_pTargetManager->GetShaderResourceView("Target_Specular");
		if (nullptr == pSpecularSRV)
			return E_FAIL;

		m_pVIBuffer->GetShader()->SetUp_TextureOnShader("g_DiffuseTexture", pDiffuseSRV);
		m_pVIBuffer->GetShader()->SetUp_TextureOnShader("g_ShadeTexture", pShadeSRV);
		m_pVIBuffer->GetShader()->SetUp_TextureOnShader("g_SpecularTexture", pSpecularSRV);

		m_pVIBuffer->Render(4);
	}
	else
	{
		Render_SSAO();

		// Draw Texture
		//m_pTargetManager->Begin_RT(m_pDeviceContext, "Target_HDR_Diffuse");
		m_pTargetManager->Begin_MainRT(m_pDeviceContext);

		ID3D11ShaderResourceView*	pDiffuseSRV = m_pTargetManager->GetShaderResourceView("Target_Diffuse");
		if (nullptr == pDiffuseSRV)
			return E_FAIL;

		ID3D11ShaderResourceView*	pShadeSRV = m_pTargetManager->GetShaderResourceView("Target_Shade");
		if (nullptr == pShadeSRV)
			return E_FAIL;

		ID3D11ShaderResourceView*	pSpecularSRV = m_pTargetManager->GetShaderResourceView("Target_Specular");
		if (nullptr == pSpecularSRV)
			return E_FAIL;

		ID3D11ShaderResourceView*	pSSAOSRV = m_pTargetManager->GetShaderResourceView("Target_SSAO_Blur");
		if (nullptr == pSSAOSRV)
			return E_FAIL;

		m_pVIBuffer->GetShader()->SetUp_TextureOnShader("g_DiffuseTexture", pDiffuseSRV);
		m_pVIBuffer->GetShader()->SetUp_TextureOnShader("g_ShadeTexture", pShadeSRV);
		m_pVIBuffer->GetShader()->SetUp_TextureOnShader("g_SpecularTexture", pSpecularSRV);
		m_pVIBuffer->GetShader()->SetUp_TextureOnShader("g_SSAOTexture", pSSAOSRV);

		m_pVIBuffer->Render(6);

		if (FAILED(m_pTargetManager->End_MRT(m_pDeviceContext)))
			return E_FAIL;
		
		//Render_Shader();
	}

	return S_OK;
}

HRESULT CRenderer::Render_Shader()
{		
	
	// LDR(Tone Mapping)
	m_pTargetManager->Begin_SingleRT(m_pDeviceContext, "Target_HDR");

	//ID3D11ShaderResourceView*	pDiffuseSRV = m_pTargetManager->GetShaderResourceView("Target_HDR_Diffuse");
	ID3D11ShaderResourceView*	pDiffuseSRV = m_pTargetManager->GetShaderResourceView("Target_Main");
	if (nullptr == pDiffuseSRV)
		return E_FAIL;

	m_pVIBuffer_HDR->GetShader()->SetUp_TextureOnShader("g_DiffuseTexture", pDiffuseSRV);

	m_pVIBuffer_HDR->Render(0);
	
	if (FAILED(m_pTargetManager->End_MRT(m_pDeviceContext)))
		return E_FAIL;

	// BirghtFilter
	m_pTargetManager->Begin_RT(m_pDeviceContext, "Target_Bright");

	//pDiffuseSRV = m_pTargetManager->GetShaderResourceView("Target_HDR_Diffuse");
	pDiffuseSRV = m_pTargetManager->GetShaderResourceView("Target_Main");
	if (nullptr == pDiffuseSRV)
		return E_FAIL;

	m_pVIBuffer_HDR->GetShader()->SetUp_TextureOnShader("g_DiffuseTexture", pDiffuseSRV);

	m_pVIBuffer_HDR->Render(1);

	if (FAILED(m_pTargetManager->End_MRT(m_pDeviceContext)))
		return E_FAIL;

	Render_Bloom();
	
	
	// Draw BackBuffer
	 pDiffuseSRV = m_pTargetManager->GetShaderResourceView("Target_HDR");
	if (nullptr == pDiffuseSRV)
		return E_FAIL;

	ID3D11ShaderResourceView*	pBloomOriSRV = m_pTargetManager->GetShaderResourceView("Target_Bright");
	if (nullptr == pDiffuseSRV)
		return E_FAIL;

	ID3D11ShaderResourceView*	pBloomSRV = m_pTargetManager->GetShaderResourceView("Target_BlurY");
	if (nullptr == pDiffuseSRV)
		return E_FAIL;

	m_pVIBuffer->GetShader()->SetUp_TextureOnShader("g_DiffuseTexture", pDiffuseSRV);
	m_pVIBuffer->GetShader()->SetUp_TextureOnShader("g_BloomOriTexture", pBloomOriSRV);
	m_pVIBuffer->GetShader()->SetUp_TextureOnShader("g_BloomTexture", pBloomSRV);

	m_pVIBuffer->Render(5);

	return S_OK;
}

HRESULT CRenderer::Render_Bloom()
{	
	_uint iNum = 1;

	D3D11_VIEWPORT BackViewPort;
	D3D11_VIEWPORT DownViewPort;
	m_pDeviceContext->RSGetViewports(&iNum, &BackViewPort);

	DownViewPort = BackViewPort;
	DownViewPort.Width = BackViewPort.Width / (_float)m_iValue;
	DownViewPort.Height = BackViewPort.Height / (_float)m_iValue;

	m_pDeviceContext->RSSetViewports(iNum, &DownViewPort);
	
	// DownSample
	m_pTargetManager->Begin_RT(m_pDeviceContext, "Target_DownTexture");

	ID3D11ShaderResourceView* pDiffuseSRV = m_pTargetManager->GetShaderResourceView("Target_Bright");
	if (nullptr == pDiffuseSRV)
		return E_FAIL;

	m_pVIBuffer_Bloom->GetShader()->SetUp_TextureOnShader("g_DiffuseTexture", pDiffuseSRV);

	m_pVIBuffer_Bloom->Render(0);

	// BlurX
	m_pTargetManager->Begin_RT(m_pDeviceContext, "Target_BlurX");

	pDiffuseSRV = m_pTargetManager->GetShaderResourceView("Target_DownTexture");
	if (nullptr == pDiffuseSRV)
		return E_FAIL;

	m_pVIBuffer_Bloom->GetShader()->SetUp_TextureOnShader("g_DiffuseTexture", pDiffuseSRV);

	m_pVIBuffer_Bloom->Render(1);

	// BlurY
	m_pTargetManager->Begin_RT(m_pDeviceContext, "Target_BlurY");

	pDiffuseSRV = m_pTargetManager->GetShaderResourceView("Target_BlurX");
	if (nullptr == pDiffuseSRV)
		return E_FAIL;

	m_pVIBuffer_Bloom->GetShader()->SetUp_TextureOnShader("g_DiffuseTexture", pDiffuseSRV);

	m_pVIBuffer_Bloom->Render(2);

	if (FAILED(m_pTargetManager->End_MRT(m_pDeviceContext)))
		return E_FAIL;

	m_pDeviceContext->RSSetViewports(iNum, &BackViewPort);
	

	return S_OK;
}

HRESULT CRenderer::Render_SSAO()
{
	m_pTargetManager->Begin_SingleRT(m_pDeviceContext, "Target_SSAO");

	ID3D11ShaderResourceView*	pDepthSRV = m_pTargetManager->GetShaderResourceView("Target_Depth");
	if (nullptr == pDepthSRV)
		return E_FAIL;

	ID3D11ShaderResourceView*	pNormalSRV = m_pTargetManager->GetShaderResourceView("Target_Normal");
	if (nullptr == pNormalSRV)
		return E_FAIL;
	
	m_pVIBuffer_SSAO->GetShader()->SetUp_TextureOnShader("g_DepthTexture", pDepthSRV);
	m_pVIBuffer_SSAO->GetShader()->SetUp_TextureOnShader("g_NormalTexture", pNormalSRV);

	m_pVIBuffer_SSAO->Render(0);

	if (FAILED(m_pTargetManager->End_MRT(m_pDeviceContext)))
		return E_FAIL;

	_uint iNum = 1;

	D3D11_VIEWPORT BackViewPort;
	D3D11_VIEWPORT DownViewPort;
	m_pDeviceContext->RSGetViewports(&iNum, &BackViewPort);

	DownViewPort = BackViewPort;
	DownViewPort.Width = BackViewPort.Width / (_float)m_iValue;
	DownViewPort.Height = BackViewPort.Height / (_float)m_iValue;

	m_pDeviceContext->RSSetViewports(iNum, &DownViewPort);

	// DownSample
	m_pTargetManager->Begin_RT(m_pDeviceContext, "Target_DownTexture");

	ID3D11ShaderResourceView* pDiffuseSRV = m_pTargetManager->GetShaderResourceView("Target_SSAO");
	if (nullptr == pDiffuseSRV)
		return E_FAIL;

	m_pVIBuffer_Bloom->GetShader()->SetUp_TextureOnShader("g_DiffuseTexture", pDiffuseSRV);

	m_pVIBuffer_Bloom->Render(0);

	// BlurX
	m_pTargetManager->Begin_RT(m_pDeviceContext, "Target_BlurX");

	pDiffuseSRV = m_pTargetManager->GetShaderResourceView("Target_DownTexture");
	if (nullptr == pDiffuseSRV)
		return E_FAIL;

	m_pVIBuffer_Bloom->GetShader()->SetUp_TextureOnShader("g_DiffuseTexture", pDiffuseSRV);

	m_pVIBuffer_Bloom->Render(1);

	// BlurY
	m_pTargetManager->Begin_RT(m_pDeviceContext, "Target_BlurY");

	pDiffuseSRV = m_pTargetManager->GetShaderResourceView("Target_BlurX");
	if (nullptr == pDiffuseSRV)
		return E_FAIL;

	m_pVIBuffer_Bloom->GetShader()->SetUp_TextureOnShader("g_DiffuseTexture", pDiffuseSRV);

	m_pVIBuffer_Bloom->Render(2);

	if (FAILED(m_pTargetManager->End_MRT(m_pDeviceContext)))
		return E_FAIL;
	m_pDeviceContext->RSSetViewports(iNum, &BackViewPort);

	m_pTargetManager->Begin_RT(m_pDeviceContext, "Target_SSAO_Blur");

	pDiffuseSRV = m_pTargetManager->GetShaderResourceView("Target_BlurY");
	if (nullptr == pDiffuseSRV)
		return E_FAIL;

	m_pVIBuffer->GetShader()->SetUp_TextureOnShader("g_DiffuseTexture", pDiffuseSRV);

	m_pVIBuffer->Render();

	if (FAILED(m_pTargetManager->End_MRT(m_pDeviceContext)))
		return E_FAIL;
	

	return S_OK;
}

HRESULT CRenderer::Render_Main()
{
	ID3D11ShaderResourceView* pDiffuseSRV = m_pTargetManager->GetShaderResourceView("Target_Main");
	if (nullptr == pDiffuseSRV)
		return E_FAIL;

	m_pVIBuffer->GetShader()->SetUp_TextureOnShader("g_DiffuseTexture", pDiffuseSRV);

	m_pVIBuffer->Render(0);

	return S_OK;
}

HRESULT CRenderer::Render_Extra()
{
	for (auto& pGameObject : m_RenderGroups[RENDER_EXTRA])
	{
		if (nullptr != pGameObject)
		{
			if (FAILED(pGameObject->Render(3)))
				return E_FAIL;

			SafeRelease(pGameObject);
		}
	}
	m_RenderGroups[RENDER_EXTRA].clear();

	return S_OK;
}

HRESULT CRenderer::RenderAlpha()
{

	if (CEngine::GetInstance()->GetCurrentUsage() == CEngine::USAGE::USAGE_CLIENT)
		m_pTargetManager->Begin_MainRT(m_pDeviceContext);

	m_RenderGroups[RENDER_ALPHA].sort([&](CGameObject* A, CGameObject* B) {
		return A->GetViewZ() > B->GetViewZ();
	});
	for (auto& pGameObject : m_RenderGroups[RENDER_ALPHA])
	{
		if (nullptr != pGameObject)
		{
			if (FAILED(pGameObject->Render()))
				return E_FAIL;

			SafeRelease(pGameObject);
		}
	}
	m_RenderGroups[RENDER_ALPHA].clear();

	if (CEngine::GetInstance()->GetCurrentUsage() == CEngine::USAGE::USAGE_CLIENT)
	{
		if (FAILED(m_pTargetManager->End_MRT(m_pDeviceContext)))
			return E_FAIL;
		
		Render_Shader();
		//Render_Main();
	}

	return S_OK;
}

HRESULT CRenderer::RenderTrail()
{
	for (auto& pGameObject : m_RenderGroups[RENDER_TRAIL])
	{
		if (nullptr != pGameObject)
		{
			if (FAILED(pGameObject->Render()))
				return E_FAIL;

			SafeRelease(pGameObject);
		}
	}
	m_RenderGroups[RENDER_TRAIL].clear();

	return S_OK;
}



HRESULT CRenderer::RenderUI()
{
	//ComRef<ID3D11BlendState> d3dBlendState;
	//D3D11_BLEND_DESC omDesc;
	//ZeroMemory(&omDesc,

	//	sizeof(D3D11_BLEND_DESC));
	//omDesc.RenderTarget[0].BlendEnable = true;
	//omDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
	//omDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
	//omDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	//omDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	//omDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
	//omDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	//omDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;


	//if (FAILED(m_pDevice->CreateBlendState(&omDesc, &d3dBlendState)))
	//	return E_FAIL;

	//m_pDeviceContext->OMSetBlendState(d3dBlendState.Get(), 0, 0xffffffff);

	m_RenderGroups[RENDER_UI].sort([](CGameObject* pFirst, CGameObject* pSecond) {
		return dynamic_cast<CEmptyUI*>(pFirst)->GetSortingOrder() < dynamic_cast<CEmptyUI*>(pSecond)->GetSortingOrder();
	});

	for (auto& pGameObject : m_RenderGroups[RENDER_UI])
	{
		if (nullptr != pGameObject)
		{
			if (FAILED(pGameObject->Render()))
				return E_FAIL;

			SafeRelease(pGameObject);
		}
	}
	m_RenderGroups[RENDER_UI].clear();

	return S_OK;
}

HRESULT CRenderer::RenderText()
{
	for (auto& pGameObject : m_RenderGroups[RENDER_TEXT])
	{
		if (nullptr != pGameObject)
		{
			if (FAILED(pGameObject->Render()))
				return E_FAIL;

			SafeRelease(pGameObject);
		}
	}
	m_RenderGroups[RENDER_TEXT].clear();

	return S_OK;
}

CRenderer * CRenderer::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
{
	CRenderer*	pInstance = new CRenderer(pDevice, pDeviceContext);

	if (FAILED(pInstance->InitializePrototype()))
	{
		MSG_BOX("Failed To Creating CRenderer");
		SafeRelease(pInstance);
	}
	return pInstance;
}

CComponent * CRenderer::Clone(void * pArg)
{
	AddRef();

	return this;
}

void CRenderer::Free()
{
	SafeRelease(m_pVIBuffer);
	SafeRelease(m_pVIBuffer_HDR);
	SafeRelease(m_pVIBuffer_Bloom);
	SafeRelease(m_pVIBuffer_SSAO);
	__super::Free();
}