#include "EnginePCH.h"
#include "..\Public\Scene.h"
#include "Engine.h"

USING(Engine)

_bool CScene::m_bIsClearEnd = false;

CScene::CScene(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext, _uint iSceneIndex)
	: m_pDevice(pDevice)
	, m_pDeviceContext(pDeviceContext)
	, m_iSceneIndex(iSceneIndex)
	, m_pEngine(CEngine::GetInstance())
{
}

HRESULT CScene::Initialize()
{
	m_bIsClearEnd = false;
	return S_OK;
}

_uint CScene::Update(_double TimeDelta)
{
	return _uint();
}

HRESULT CScene::Render()
{
	return S_OK;
}

void CScene::StartLoader()
{
}

void CScene::Clear()
{
	m_pEngine->ClearGameObjectManager(m_iSceneIndex);
	m_pEngine->ClearComponentManager(m_iSceneIndex);
	m_pEngine->ClearScriptObject(m_iSceneIndex);

	m_bIsClearEnd = true;
}

void CScene::Free()
{

}
