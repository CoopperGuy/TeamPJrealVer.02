#include "EnginePCH.h"
#include "..\Public\TargetManager.h"
#include "RenderTarget.h"
#include "Engine.h"

IMPLEMENT_SINGLETON(CTargetManager)
USING(Engine)

CTargetManager::CTargetManager()
{

}

HRESULT CTargetManager::Initialize(ID3D11Device* pDevice, ID3D11DeviceContext * pDeviceContext)
{
	pDeviceContext->OMGetRenderTargets(1, &m_pBackBufferRTV, &m_pDepthStencilView);
	
	_uint iNum = 1;

	pDeviceContext->RSGetViewports(&iNum, &m_BackBufferViewPort);
	
#pragma region create ShadowDepthStencil
	D3D11_TEXTURE2D_DESC		TextureDesc;
	ZeroMemory(&TextureDesc, sizeof(TextureDesc));

	TextureDesc.Width = WINCX * 10;
	TextureDesc.Height = WINCY * 10;
	TextureDesc.MipLevels = 1;
	TextureDesc.ArraySize = 1;

	TextureDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	TextureDesc.SampleDesc.Count = 1;
	TextureDesc.SampleDesc.Quality = 0;
	TextureDesc.Usage = D3D11_USAGE_DEFAULT;
	TextureDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	TextureDesc.CPUAccessFlags = 0;
	TextureDesc.MiscFlags = 0;

	ID3D11Texture2D*		pShadowDepthStencilTexture = nullptr;

	if (FAILED(pDevice->CreateTexture2D(&TextureDesc, nullptr, &pShadowDepthStencilTexture)))
		return E_FAIL;
	
	if (FAILED(pDevice->CreateDepthStencilView(pShadowDepthStencilTexture, nullptr, &m_pShadowDepthStencilView)))
		return E_FAIL;

	pShadowDepthStencilTexture->Release();
#pragma endregion

	return S_OK;
}

HRESULT CTargetManager::Add_RenderTarget(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext, string pRenderTargetTag, _uint iWidth, _uint iHeight, DXGI_FORMAT Format, _float4 vClearColor)
{
	if (nullptr != Find_RenderTarget(pRenderTargetTag))
		return E_FAIL;

	CRenderTarget*		pRenderTarget = CRenderTarget::Create(pDevice, pDeviceContext, iWidth, iHeight, Format, vClearColor);
	if (nullptr == pRenderTarget)
		return E_FAIL;

	m_RenderTargets.emplace(pRenderTargetTag, pRenderTarget);

	return S_OK;
}

HRESULT CTargetManager::Add_MRT(string pMRTTag, string pRenderTargetTag)
{
	CRenderTarget*		pRenderTarget = Find_RenderTarget(pRenderTargetTag);
	if (nullptr == pRenderTarget)
		return E_FAIL;

	list<CRenderTarget*>*		pMRTList = Find_MRT(pMRTTag);

	if (nullptr == pMRTList)
	{
		list<CRenderTarget*>		MRTList;
		MRTList.push_back(pRenderTarget);

		m_MRTs.emplace(pMRTTag, MRTList);
	}
	else
		pMRTList->push_back(pRenderTarget);

	SafeAddRef(pRenderTarget);

	return S_OK;
}

HRESULT CTargetManager::Begin_MRT(ID3D11DeviceContext* pDeviceContext, string pMRTTag)
{
	list<CRenderTarget*>*		pMRTList = Find_MRT(pMRTTag);
	if (nullptr == pMRTList)
		return E_FAIL;

	//pDeviceContext->OMGetRenderTargets(1, &m_pBackBufferRTV, &m_pDepthStencilView);

	ID3D11RenderTargetView*		pRenderTargets[8] = { nullptr };

	_uint		iIndex = 0;

	for (auto& pRenderTarget : *pMRTList)
	{
		pRenderTarget->Clear();

		ID3D11RenderTargetView*		pRTV = pRenderTarget->Get_RenderTargetView();
		pRenderTargets[iIndex++] = pRTV;
	}
	
	pDeviceContext->OMSetRenderTargets((_uint)pMRTList->size(), pRenderTargets, m_pDepthStencilView);

	return S_OK;
}

HRESULT CTargetManager::Set_MRT(ID3D11DeviceContext * pDeviceContext, string pMRTTag)
{
	list<CRenderTarget*>*		pMRTList = Find_MRT(pMRTTag);
	if (nullptr == pMRTList)
		return E_FAIL;

	//pDeviceContext->OMGetRenderTargets(1, &m_pBackBufferRTV, &m_pDepthStencilView);

	ID3D11RenderTargetView*		pRenderTargets[8] = { nullptr };

	_uint		iIndex = 0;

	for (auto& pRenderTarget : *pMRTList)
	{
		ID3D11RenderTargetView*		pRTV = pRenderTarget->Get_RenderTargetView();
		pRenderTargets[iIndex++] = pRTV;
	}

	//pDeviceContext->ClearDepthStencilView(m_pDepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);
	pDeviceContext->OMSetRenderTargets((_uint)pMRTList->size(), pRenderTargets, m_pDepthStencilView);

	return S_OK;
}

HRESULT CTargetManager::Clear_MRT(ID3D11DeviceContext * pDeviceContext, string pMRTTag)
{
	list<CRenderTarget*>*		pMRTList = Find_MRT(pMRTTag);
	if (nullptr == pMRTList)
		return E_FAIL;

	for (auto& pRenderTarget : *pMRTList)
		pRenderTarget->Clear();
	return S_OK;
}

HRESULT CTargetManager::Begin_SingleRT(ID3D11DeviceContext * pDeviceContext, string pTargetTag)
{
	CRenderTarget* renderTarget = Find_RenderTarget(pTargetTag);
	
	if (nullptr == renderTarget)
		return E_FAIL;
	renderTarget->Clear();
	ID3D11RenderTargetView*		pRenderTargets[8] = { nullptr };
	pRenderTargets[0] = renderTarget->Get_RenderTargetView();

	pDeviceContext->OMSetRenderTargets(1, pRenderTargets, m_pDepthStencilView);

	return S_OK;
}

HRESULT CTargetManager::Begin_RT(ID3D11DeviceContext * pDeviceContext, string pTargetTag)
{
	CRenderTarget* renderTarget = Find_RenderTarget(pTargetTag);
	
	if (nullptr == renderTarget)
		return E_FAIL;
	renderTarget->Clear();
	ID3D11RenderTargetView*		pRenderTargets[8] = { nullptr };
	pRenderTargets[0] = renderTarget->Get_RenderTargetView();

	pDeviceContext->OMSetRenderTargets(1, pRenderTargets, nullptr);

	return S_OK;
}

HRESULT CTargetManager::Begin_ShadowRT(ID3D11DeviceContext * pDeviceContext, string pTargetTag)
{
	CRenderTarget* renderTarget = Find_RenderTarget(pTargetTag);

	if (nullptr == renderTarget)
		return E_FAIL;
	renderTarget->Clear();
	ID3D11RenderTargetView*		pRenderTargets[8] = { nullptr };
	pRenderTargets[0] = renderTarget->Get_RenderTargetView();

	_uint iNum = 1;
	D3D11_VIEWPORT ViewPort;

	ViewPort = m_BackBufferViewPort;
	ViewPort.Width = WINCX * 10;
	ViewPort.Height = WINCY * 10;

	pDeviceContext->RSSetViewports(iNum, &ViewPort);

	pDeviceContext->OMSetRenderTargets(1, pRenderTargets, m_pShadowDepthStencilView);	
	pDeviceContext->ClearDepthStencilView(m_pShadowDepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);
	
	return S_OK;
}

HRESULT CTargetManager::Begin_MainRT(ID3D11DeviceContext * pDeviceContext)
{
	CRenderTarget* renderTarget = Find_RenderTarget("Target_Main");

	if (nullptr == renderTarget)
		return E_FAIL;

	ID3D11RenderTargetView*		pRenderTargets[8] = { nullptr };
	pRenderTargets[0] = renderTarget->Get_RenderTargetView();

	pDeviceContext->OMSetRenderTargets(1, pRenderTargets, m_pDepthStencilView);

	return S_OK;
}

HRESULT CTargetManager::Clear_MainRT(ID3D11DeviceContext * pDeviceContext)
{
	CRenderTarget* renderTarget = Find_RenderTarget("Target_Main");

	if (nullptr == renderTarget)
		return E_FAIL;
	renderTarget->Clear();

	return S_OK;
}

HRESULT CTargetManager::End_MRT(ID3D11DeviceContext* pDeviceContext)
{	
	_uint iNum = 1;

	pDeviceContext->RSSetViewports(iNum, &m_BackBufferViewPort);

	pDeviceContext->OMSetRenderTargets(1, &m_pBackBufferRTV, m_pDepthStencilView);
	
	return S_OK;
}


HRESULT CTargetManager::Ready_DebugBuffer(string pTargetTag, _float fX, _float fY, _float fWidth, _float fHeight)
{
	CRenderTarget*	pRenderTarget = Find_RenderTarget(pTargetTag);
	if (nullptr == pRenderTarget)
		return E_FAIL;

	return pRenderTarget->Ready_DebugBuffer(fX, fY, fWidth, fHeight);
}
HRESULT CTargetManager::Render_DebugBuffers(string pMRTTag)
{
	list<CRenderTarget*>*		pMRTList = Find_MRT(pMRTTag);
	if (nullptr == pMRTList)
		return E_FAIL;

	for (auto& pRenderTarget : *pMRTList)
		pRenderTarget->Render_DebugBuffer();

	return S_OK;
}
ID3D11ShaderResourceView * CTargetManager::GetShaderResourceView(string pTargetTag)
{
	CRenderTarget* renderTarget = Find_RenderTarget(pTargetTag);
	if (renderTarget)
		return renderTarget->GetShaderResourceView();

	return nullptr;
}

CRenderTarget * CTargetManager::Find_RenderTarget(string pRenderTagetTag)
{
	auto	iter = find_if(m_RenderTargets.begin(), m_RenderTargets.end(), STagFinder(pRenderTagetTag));

	if (iter == m_RenderTargets.end())
		return nullptr;

	return iter->second;
}

list<class CRenderTarget*>* CTargetManager::Find_MRT(string pMRTTag)
{
	auto	iter = find_if(m_MRTs.begin(), m_MRTs.end(), STagFinder(pMRTTag));

	if (iter == m_MRTs.end())
		return nullptr;

	return &iter->second;
}

void CTargetManager::Free()
{
	SafeRelease(m_pBackBufferRTV);
	SafeRelease(m_pDepthStencilView);
	SafeRelease(m_pShadowDepthStencilView);

	for (auto& Pair : m_MRTs)
	{
		for (auto& pRenderTarget : Pair.second)
			SafeRelease(pRenderTarget);

		Pair.second.clear();
	}

	m_MRTs.clear();



	for (auto& Pair : m_RenderTargets)
		SafeRelease(Pair.second);

	m_RenderTargets.clear();
}
