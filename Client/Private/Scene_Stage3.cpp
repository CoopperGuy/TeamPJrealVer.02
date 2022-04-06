#include "stdafx.h"
#include "..\Public\Scene_Stage3.h"
#include "Engine.h"
#include "Ursa.h"
#include "DropRock.h"
#include "UrsaDunDoor.h"
#include "EventCheck.h"
#include "Scene_Loading.h"
USING(Client)

CScene_Stage3::CScene_Stage3(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext, _uint iLevelIndex)
	: CScene(pDevice, pDeviceContext, iLevelIndex)
{
}

HRESULT CScene_Stage3::Initialize()
{
	__super::Initialize();

	CEngine::GetInstance()->SetCurSceneNumber(SCENE_STAGE3);

	ReadyScript();

	CEngine::GetInstance()->StopSound(CHANNELID::BGM);
	CEngine::GetInstance()->PlayBGM("DragonBallZThema.mp3");

	CEmptyGameObject* pPlayer = static_cast<CEmptyGameObject*>(CEngine::GetInstance()->FindGameObjectWithName(SCENE_STATIC, "Player"));
	static_cast<CCollider*>(pPlayer->GetComponent("Com_Collider"))->SetPosition(_float3(0.f, 0.5f, 5.f));

	return S_OK;
}

_uint CScene_Stage3::Update(_double TimeDelta)
{
	__super::Update(TimeDelta);

	if (CEventCheck::GetInstance()->GetChangeScene()) {
		CEventCheck::GetInstance()->SetChangeScene(false);
		CEventCheck::GetInstance()->SetSceneNumber(SCENE_END);
		if (FAILED(CEngine::GetInstance()->SetUpCurrentScene(CScene_Loading::Create(m_pDevice, m_pDeviceContext, SCENE::SCENE_STAGE1, (_uint)SCENE_LOADING), CEngine::GetInstance()->GetCurSceneNumber())))
			return E_FAIL;
	}

	return _uint();
}

HRESULT CScene_Stage3::Render()
{
	__super::Render();

	return S_OK;
}

HRESULT CScene_Stage3::ReadyScript()
{
	m_pEngine->AddScriptObject(CUrsa::Create(nullptr), CEngine::GetInstance()->GetCurSceneNumber());
	m_pEngine->AddScriptObject(CUrsaDunDoor::Create(nullptr), CEngine::GetInstance()->GetCurSceneNumber());

	return S_OK;
}

CScene_Stage3 * CScene_Stage3::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext, _uint iLevelIndex)
{
	CScene_Stage3*		pInstance = new CScene_Stage3(pDevice, pDeviceContext, iLevelIndex);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed to Creating CScene_Stage3");
		SafeRelease(pInstance);
	}

	return pInstance;
}

void CScene_Stage3::Free()
{
	__super::Free();
}
