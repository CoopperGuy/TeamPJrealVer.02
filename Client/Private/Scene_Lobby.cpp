#include "stdafx.h"
#include "..\Public\Scene_Lobby.h"

#include "Engine.h"
#include "Camera_Fly.h"

USING(Client)
static _bool isTesting = false;

CScene_Lobby::CScene_Lobby(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext, _uint iSceneIndex)
	: CScene(pDevice, pDeviceContext, iSceneIndex)
{
}

CScene_Lobby * CScene_Lobby::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext, _uint iSceneIndex)
{
	CScene_Lobby*		pInstance = new CScene_Lobby(pDevice, pDeviceContext, iSceneIndex);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed to Creating CScene_Lobby");
		SafeRelease(pInstance);
	}

	return pInstance;
}

void CScene_Lobby::Free()
{
	__super::Free();
}

HRESULT CScene_Lobby::Initialize()
{
	__super::Initialize();

	if (isTesting)
		m_pEngine->DeserializeScene("../../Assets/Scenes/test.yaml",m_pEngine->GetCurSceneNumber());
	else
		m_pEngine->DeserializeScene("../../Assets/Scenes/Scene_Lobby.yaml", m_pEngine->GetCurSceneNumber());

	if (FAILED(ReadyLayerCamera("LAYER_CAMERA")))
		return E_FAIL;

	if (!isTesting)
	{
		if (FAILED(ReadyScript()))
			return E_FAIL;
	}


	return S_OK;
}

_uint CScene_Lobby::Update(_double TimeDelta)
{
	__super::Update(TimeDelta);

	return _uint();
}

HRESULT CScene_Lobby::Render()
{
	__super::Render();

	return S_OK;
}



HRESULT CScene_Lobby::ReadyLayerCamera(string pLayerTag)
{
	CCamera::CAMERADESC		CameraDesc;
	ZeroMemory(&CameraDesc, sizeof(CCamera::CAMERADESC));

	CameraDesc.vEye = _float3(13.f, 6.f, 8.f);
	CameraDesc.vAt = _float3(13.f, 6.f, 5.f);
	CameraDesc.vAxisY = _float3(0.f, 1.f, 0.f);

	if (nullptr == m_pEngine->AddGameObject(0, "GameObject_Camera_Lobby", pLayerTag, &CameraDesc))
		return E_FAIL;

	if (nullptr == m_pEngine->AddGameObject(0, "GameObject_Sky", "Sky"))
		return E_FAIL;

	return S_OK;
}

HRESULT CScene_Lobby::ReadyScript()
{
	return S_OK;
}



