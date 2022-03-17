#include "stdafx.h"
#include "..\Public\Scene_Effect.h"

#include "Engine.h"
#include "Terrain.h"

#include "Camera_Fly.h"


USING(Client)

CScene_Effect::CScene_Effect(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext, _uint iSceneIndex)
	: CScene(pDevice, pDeviceContext, iSceneIndex)
{
}

CScene_Effect * CScene_Effect::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext, _uint iSceneIndex)
{
	CScene_Effect*		pInstance = new CScene_Effect(pDevice, pDeviceContext, iSceneIndex);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed to Creating CScene_Effect");
		SafeRelease(pInstance);
	}

	return pInstance;
}

void CScene_Effect::Free()
{
	__super::Free();
}

HRESULT CScene_Effect::Initialize()
{
	__super::Initialize();
	m_pEngine->DeserializeScene("../../Assets/Scenes/Effect.yaml", m_pEngine->GetCurSceneNumber());

	if (FAILED(ReadyLayerCamera("LAYER_CAMERA")))
		return E_FAIL;

	ReadyScript();

	return S_OK;
}

_uint CScene_Effect::Update(_double TimeDelta)
{
	__super::Update(TimeDelta);

	return _uint();
}

HRESULT CScene_Effect::Render()
{
	__super::Render();

	return S_OK;
}


HRESULT CScene_Effect::ReadyLayerCamera(string pLayerTag)
{

	CCamera::CAMERADESC		CameraDesc;
	ZeroMemory(&CameraDesc, sizeof(CCamera::CAMERADESC));

	CameraDesc.vEye = _float3(0.f, 3.f, -5.f);
	CameraDesc.vAt = _float3(0.f, 0.f, 0.f);
	CameraDesc.vAxisY = _float3(0.f, 1.f, 0.f);

	if (nullptr == CEngine::GetInstance()->AddGameObject(0, "GameObject_Camera_Fly", pLayerTag, &CameraDesc))
		return E_FAIL;


	return S_OK;
}

HRESULT CScene_Effect::ReadyLayerEffect(string pLayerTag)
{

	if (nullptr == CEngine::GetInstance()->AddGameObject(0, "GameObject_Effect_Fire", pLayerTag))
		return E_FAIL;


	return S_OK;
}

HRESULT CScene_Effect::ReadyScript()
{
	m_pEngine->AddScriptObject(CTerrain::Create(nullptr), CEngine::GetInstance()->GetCurSceneNumber());

	return S_OK;
}



