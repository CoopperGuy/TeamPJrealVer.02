#include "stdafx.h"
#include "..\Public\Scene_JUN.h"
#include "Flogas.h"
#include "Ursa.h"
#include "TrapSpear.h"

USING(Client)

CScene_JUN::CScene_JUN(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext, _uint iLevelIndex)
	:CScene(pDevice,pDeviceContext,iLevelIndex)
{
}

HRESULT CScene_JUN::Initialize()
{
	__super::Initialize();
	CEngine::GetInstance()->StopAll();
	CEngine::GetInstance()->SetCurSceneNumber(SCENE_JUNG);

	ReadyScript();

	return S_OK;
}

_uint CScene_JUN::Update(_double TimeDelta)
{
	return _uint();
}

HRESULT CScene_JUN::Render()
{
	return S_OK;
}

HRESULT CScene_JUN::ReadyScript()
{
	//m_pEngine->AddScriptObject(CFlogas::Create(nullptr), SCENE_JUNG);
	//m_pEngine->AddScriptObject(CUrsa::Create(nullptr), SCENE_JUNG);


	list<CGameObject*> listSpear = m_pEngine->GetGameObjectInLayer(SCENE_JUNG, "Layer_Spear");
	for (auto pSpear : listSpear)
	{
		_double start = (_double)(rand() % 50) / 10.0;
		if (start > 3.0)
			start += 2.0;
		m_pEngine->AddScriptObject(CTrapSpear::Create(pSpear, start), SCENE_JUNG);
	}

	return S_OK;
}

CScene_JUN * CScene_JUN::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext, _uint iLevelIndex)
{
	CScene_JUN*		pInstance = new CScene_JUN(pDevice, pDeviceContext, iLevelIndex);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed to Creating CScene_JUN");
		SafeRelease(pInstance);
	}

	return pInstance;
}

void CScene_JUN::Free()
{
}
