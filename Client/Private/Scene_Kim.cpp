#include "stdafx.h"
#include "..\Public\Scene_Kim.h"
#include "WaterEA.h"
#include "TestMonster.h"
USING(Client)

CScene_Kim::CScene_Kim(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext, _uint iLevelIndex)
	:CScene(pDevice,pDeviceContext,iLevelIndex)
{
}

HRESULT CScene_Kim::Initialize()
{
	__super::Initialize();

	CEngine::GetInstance()->SetCurSceneNumber(SCENE_KIM);

	ReadyScript();

	return S_OK;
}

_uint CScene_Kim::Update(_double TimeDelta)
{
#ifdef _DEBUG
	if (CEngine::GetInstance()->Get_DIKDown(DIK_NUMPAD9)) {
		CEngine::GetInstance()->AddScriptObject(CWaterEA::Create(nullptr),CEngine::GetInstance()->GetCurSceneNumber()) ;
	}
#endif // _DEBUG

	return _uint();
}

HRESULT CScene_Kim::Render()
{
	return S_OK;
}

HRESULT CScene_Kim::ReadyScript()
{
	CEngine::GetInstance()->AddScriptObject(CTestMonster::Create(nullptr), CEngine::GetInstance()->GetCurSceneNumber());
	return S_OK;
}

CScene_Kim * CScene_Kim::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext, _uint iLevelIndex)
{
	CScene_Kim*		pInstance = new CScene_Kim(pDevice, pDeviceContext, iLevelIndex);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed to Creating CScene_Kim");
		SafeRelease(pInstance);
	}

	return pInstance;
}

void CScene_Kim::Free()
{
}
