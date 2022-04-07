#include "stdafx.h"
#include "..\Public\Scene_Logo.h"

#include "Engine.h"
#include "Background.h"
#include "Scene_Test.h"
#include "Scene_Loading.h"
#include "Player.h"
#include "Axe.h"
#include "Scene_Kim.h"
#include "Scene_LEE.h"
#include "Scene_JUN.h"
#include "Scene_SEO.h"
#include "MouseUI.h"
CScene_Logo::CScene_Logo(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext, _uint iSceneIndex)
	: CScene(pDevice, pDeviceContext, iSceneIndex)
{

}

HRESULT CScene_Logo::Initialize()
{
	__super::Initialize();
	CEngine::GetInstance()->SetCurSceneNumber(SCENE_LOGO);

	if (FAILED(ReadyLayerCamera("LAYER_CAMERA")))
		return E_FAIL;

	if (FAILED(ReadyLayerSky("LAYER_SKYBOX")))
		return E_FAIL;

	ReadyScript();

	//CEngine::GetInstance()->StopSound(CHANNELID::BGM);
	//CEngine::GetInstance()->PlayBGM("Baram.mp3");
	CEngine::GetInstance()->StopSound(CHANNELID::BGM);
	CEngine::GetInstance()->PlayBGM("Logo.mp3");
	return S_OK;
}

_uint CScene_Logo::Update(_double TimeDelta)
{
	__super::Update(TimeDelta);


	if (m_pEngine->Get_DIKDown(DIK_1))
	{
		if (FAILED(CEngine::GetInstance()->SetUpCurrentScene(CScene_Loading::Create(m_pDevice, m_pDeviceContext, SCENE_STAGE1, (_uint)SCENE_LOADING), (_uint)SCENE_LOGO)))
			return E_FAIL;
	}
	else if (m_pEngine->Get_DIKDown(DIK_2))
	{
		if (FAILED(CEngine::GetInstance()->SetUpCurrentScene(CScene_Loading::Create(m_pDevice, m_pDeviceContext, SCENE_TEST, (_uint)SCENE_LOADING), (_uint)SCENE_LOGO)))
			return E_FAIL;
	}
	else if (m_pEngine->Get_DIKDown(DIK_3))
	{
		if (FAILED(CEngine::GetInstance()->SetUpCurrentScene(CScene_Loading::Create(m_pDevice, m_pDeviceContext, SCENE_KIM, (_uint)SCENE_LOADING), (_uint)SCENE_LOGO)))
			return E_FAIL;
	}
	else if (m_pEngine->Get_DIKDown(DIK_4))
	{
		if (FAILED(CEngine::GetInstance()->SetUpCurrentScene(CScene_Loading::Create(m_pDevice, m_pDeviceContext, SCENE_LEE, (_uint)SCENE_LOADING), (_uint)SCENE_LOGO)))
			return E_FAIL;
	}
	else if (m_pEngine->Get_DIKDown(DIK_5))
	{
		if (FAILED(CEngine::GetInstance()->SetUpCurrentScene(CScene_Loading::Create(m_pDevice, m_pDeviceContext, SCENE_JUNG, (_uint)SCENE_LOADING), (_uint)SCENE_LOGO)))
			return E_FAIL;
	}
	else if (m_pEngine->Get_DIKDown(DIK_6))
	{
		if (FAILED(CEngine::GetInstance()->SetUpCurrentScene(CScene_Loading::Create(m_pDevice, m_pDeviceContext, SCENE_SEO, (_uint)SCENE_LOADING), (_uint)SCENE_LOGO)))
			return E_FAIL;
	}


	return _uint();
}

HRESULT CScene_Logo::Render()
{
	__super::Render();

	return S_OK;
}

HRESULT CScene_Logo::ReadyScript()
{
	m_pEngine->AddScriptObject(CPlayer::Create(nullptr), SCENE_STATIC);
	//m_pEngine->AddScriptObject(CAxe::Create(nullptr), SCENE_STATIC);
	CMouseUI::Create();
	return S_OK;
}

HRESULT CScene_Logo::ReadyLayerCamera(string pLayerTag)
{

	CCamera::CAMERADESC		CameraDesc;
	ZeroMemory(&CameraDesc, sizeof(CCamera::CAMERADESC));

	CameraDesc.vEye = _float3(0.f, 10.f, -20.f);
	CameraDesc.vAt = _float3(0.f, 0.f, 0.f);
	CameraDesc.vAxisY = _float3(0.f, 1.f, 0.f);

	if (nullptr == CEngine::GetInstance()->AddGameObject(0, "GameObject_Camera_Fly", pLayerTag, &CameraDesc))
		return E_FAIL;


	return S_OK;
}

HRESULT CScene_Logo::ReadyLayerSky(string pLayerTag)
{

	if (nullptr == CEngine::GetInstance()->AddGameObject(0, "Prototype_Sky", pLayerTag))
		return E_FAIL;


	return S_OK;
}

CScene_Logo * CScene_Logo::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext, _uint iLevelIndex)
{
	CScene_Logo*		pInstance = new CScene_Logo(pDevice, pDeviceContext, iLevelIndex);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed to Creating CScene_Logo");
		SafeRelease(pInstance);
	}

	return pInstance;
}

void CScene_Logo::Free()
{
	__super::Free();

}
