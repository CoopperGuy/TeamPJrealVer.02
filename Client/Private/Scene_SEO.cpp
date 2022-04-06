#include "stdafx.h"
#include "..\Public\Scene_SEO.h"
#include "Flogas.h"
#include "WaterEA.h"
#include "FlogasDunDoor.h"
#include "Wolf.h"
#include "Ursa.h"
#include "DropRock.h"
#include "UrsaDunDoor.h"
#include "ItemBox.h"
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
	if (CEngine::GetInstance()->Get_DIKDown(DIK_8)) {
		CGameObject* ItemBox = CEngine::GetInstance()->AddGameObjectToPrefab(CEngine::GetInstance()->GetCurSceneNumber(), "Prototype_GameObject_BossItemBox", "O_BossItemBox");
		CEngine::GetInstance()->AddScriptObject(CItemBox::Create(ItemBox), CEngine::GetInstance()->GetCurSceneNumber());
	}
	if (CEngine::GetInstance()->Get_DIKDown(DIK_9)) {
		CGameObject* ItemBox = CEngine::GetInstance()->AddGameObjectToPrefab(CEngine::GetInstance()->GetCurSceneNumber(), "Prototype_GameObject_BasicItemBox", "O_BasicItemBox");
		CEngine::GetInstance()->AddScriptObject(CItemBox::Create(ItemBox), CEngine::GetInstance()->GetCurSceneNumber());
	}



	return _uint();
}

HRESULT CScene_SEO::Render()
{
	return S_OK;
}

HRESULT CScene_SEO::ReadyScript()
{
	//CWolf::Create(nullptr);

	//_float3 one = { 3.f,2.f,3.f };
	//_float3 two = { 5.f,2.f,5.f };
	//_float3 three = { 5.f,2.f,5.f };
	//m_pEngine->AddScriptObject(CWolf::Create(nullptr), SCENE_SEO);
	//m_pEngine->AddScriptObject(CWolf::Create(nullptr, one), SCENE_SEO);
	//m_pEngine->AddScriptObject(CWolf::Create(nullptr, two), SCENE_SEO);
	//m_pEngine->AddScriptObject(CWolf::Create(nullptr, three), SCENE_SEO);


	//m_pEngine->AddScriptObject(CFlogas::Create(nullptr), SCENE_SEO);
	//m_pEngine->AddScriptObject(CUrsa::Create(nullptr), SCENE_SEO);

	//m_pEngine->AddScriptObject(CUrsaDunDoor::Create(nullptr), CEngine::GetInstance()->GetCurSceneNumber());
	//m_pEngine->AddScriptObject(CFlogasDunDoor::Create(nullptr), CEngine::GetInstance()->GetCurSceneNumber());

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
