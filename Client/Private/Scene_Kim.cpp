#include "stdafx.h"
#include "..\Public\Scene_Kim.h"
#include "WaterEA.h"
#include "TestMonster.h"
#include "Wolf.h"
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


	return _uint();
}

HRESULT CScene_Kim::Render()
{
	return S_OK;
}

HRESULT CScene_Kim::ReadyScript()
{
	//CEngine::GetInstance()->AddScriptObject(CTestMonster::Create(nullptr), CEngine::GetInstance()->GetCurSceneNumber());
	_float3 one = { 4.f,0.f,4.f };
	_float3 two = { 16.f,0.f,16.f };
	_float3 three = { 28.f,0.f,28.f };
	m_pEngine->AddScriptObject(CWolf::Create(nullptr), CEngine::GetInstance()->GetCurSceneNumber());
	m_pEngine->AddScriptObject(CWolf::Create(nullptr, one), CEngine::GetInstance()->GetCurSceneNumber());
	m_pEngine->AddScriptObject(CWolf::Create(nullptr, two), CEngine::GetInstance()->GetCurSceneNumber());
	m_pEngine->AddScriptObject(CWolf::Create(nullptr, three), CEngine::GetInstance()->GetCurSceneNumber());

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
