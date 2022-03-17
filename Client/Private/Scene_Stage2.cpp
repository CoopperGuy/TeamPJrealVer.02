#include "stdafx.h"
#include "..\Public\Scene_Stage2.h"
#include "Flogas.h"

USING(Client)

CScene_Stage02::CScene_Stage02(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext, _uint iLevelIndex)
	: CScene(pDevice, pDeviceContext, iLevelIndex)
{
}

HRESULT CScene_Stage02::Initialize()
{
	__super::Initialize();

	CEngine::GetInstance()->SetCurSceneNumber(SCENE_STAGE2);

	ReadyScript();

	return S_OK;
}

_uint CScene_Stage02::Update(_double TimeDelta)
{
	__super::Update(TimeDelta);

	return _uint();
}

HRESULT CScene_Stage02::Render()
{
	__super::Render();

	return S_OK;
}

HRESULT CScene_Stage02::ReadyLayerCamera(string pLayerTag)
{
	return S_OK;
}

HRESULT CScene_Stage02::ReadyScript()
{
	m_pEngine->AddScriptObject(CFlogas::Create(nullptr), CEngine::GetInstance()->GetCurSceneNumber());

	return S_OK;
}

HRESULT CScene_Stage02::ReadyLayerSky(string pLayerTag)
{
	return S_OK;
}

CScene_Stage02 * CScene_Stage02::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext, _uint iLevelIndex)
{
	CScene_Stage02*		pInstance = new CScene_Stage02(pDevice, pDeviceContext, iLevelIndex);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed to Creating CScene_Stage02");
		SafeRelease(pInstance);
	}

	return pInstance;
}

void CScene_Stage02::Free()
{
	__super::Free();
}
