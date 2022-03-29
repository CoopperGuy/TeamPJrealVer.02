#include "stdafx.h"
#include "..\Public\Scene_Stage3.h"
#include "Engine.h"
#include "Ursa.h"
#include "DropRock.h"
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

	return S_OK;
}

_uint CScene_Stage3::Update(_double TimeDelta)
{
	__super::Update(TimeDelta);

	m_dMakeDropRock += TimeDelta;

	if (m_dMakeDropRock >= 10.f)
	{
		if (makeRockNum <= 2)
		{
			++makeRockNum;
			CGameObject* Rock = CEngine::GetInstance()->AddGameObjectToPrefab(CEngine::GetInstance()->GetCurSceneNumber(), "Prototype_GameObecjt_Rock", "O_Rock");
			CEngine::GetInstance()->AddScriptObject(CDropRock::Create(Rock), CEngine::GetInstance()->GetCurSceneNumber());
		}

		else {
			m_dMakeDropRock = 0;
			makeRockNum = 0;
		}
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
