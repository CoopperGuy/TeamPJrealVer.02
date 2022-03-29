#include "stdafx.h"
#include "..\Public\Scene_SEO.h"
#include "Flogas.h"
#include "WaterEA.h"
#include "FlogasDunDoor.h"
#include "Wolf.h"
#include "Ursa.h"
USING(Client)

CScene_SEO::CScene_SEO(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext, _uint iLevelIndex)
	:CScene(pDevice,pDeviceContext,iLevelIndex)
{
}

HRESULT CScene_SEO::Initialize()
{
	__super::Initialize();

	CEngine::GetInstance()->SetCurSceneNumber(SCENE_SEO);
	ReadyScript();

	return S_OK;
}

_uint CScene_SEO::Update(_double TimeDelta)
{
	return _uint();
}

HRESULT CScene_SEO::Render()
{
	return S_OK;
}

HRESULT CScene_SEO::ReadyScript()
{
	//CWolf::Create(nullptr);
	//CEngine::GetInstance()->AddScriptObject(this, CEngine::GetInstance()->GetCurSceneNumber());

	//_float3 one = { 4.f,2.f,4.f };
	//_float3 two = { 16.f,2.f,16.f };
	//_float3 three = { 28.f,2.f,28.f };
	//m_pEngine->AddScriptObject(CWolf::Create(nullptr), SCENE_SEO);
	//m_pEngine->AddScriptObject(CWolf::Create(nullptr, one), SCENE_SEO);
	//m_pEngine->AddScriptObject(CWolf::Create(nullptr, two), SCENE_SEO);
	//m_pEngine->AddScriptObject(CWolf::Create(nullptr, three), SCENE_SEO);

	//m_pEngine->AddScriptObject(CFlogas::Create(nullptr), SCENE_SEO);
	m_pEngine->AddScriptObject(CUrsa::Create(nullptr), SCENE_SEO);


	//m_pEngine->AddScriptObject(CFlogasDunDoor::Create(nullptr), SCENE_SEO);

	return S_OK;
}

CScene_SEO * CScene_SEO::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext, _uint iLevelIndex)
{
	CScene_SEO*		pInstance = new CScene_SEO(pDevice, pDeviceContext, iLevelIndex);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed to Creating CScene_SEO");
		SafeRelease(pInstance);
	}

	return pInstance;
}

void CScene_SEO::Free()
{
}
