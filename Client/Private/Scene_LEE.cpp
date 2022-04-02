#include "stdafx.h"
#include "..\Public\Scene_LEE.h"

#include "DarkKnight.h"

USING(Client)

CScene_LEE::CScene_LEE(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext, _uint iLevelIndex)
	:CScene(pDevice,pDeviceContext,iLevelIndex)
{
}

HRESULT CScene_LEE::Initialize()
{
	__super::Initialize();

	CEngine::GetInstance()->SetCurSceneNumber(SCENE_LEE);

	ReadyScript();

	return S_OK;
}

_uint CScene_LEE::Update(_double TimeDelta)
{
	return _uint();
}

HRESULT CScene_LEE::Render()
{
	return S_OK;
}

HRESULT CScene_LEE::ReadyScript()
{
	m_pEngine->AddScriptObject(CDarkKnight::Create(nullptr), SCENE_LEE);

	return S_OK;
}

CScene_LEE * CScene_LEE::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext, _uint iLevelIndex)
{
	CScene_LEE*		pInstance = new CScene_LEE(pDevice, pDeviceContext, iLevelIndex);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed to Creating CScene_LEE");
		SafeRelease(pInstance);
	}

	return pInstance;
}

void CScene_LEE::Free()
{
}
