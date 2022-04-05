#include "stdafx.h"
#include "..\Public\Scene_Stage4.h"
#include "Engine.h"
#include "Ursa.h"
#include "DropRock.h"
#include "UrsaDunDoor.h"
USING(Client)

CScene_Stage4::CScene_Stage4(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext, _uint iLevelIndex)
	: CScene(pDevice, pDeviceContext, iLevelIndex)
{
}

HRESULT CScene_Stage4::Initialize()
{
	__super::Initialize();

	CEngine::GetInstance()->SetCurSceneNumber(SCENE_STAGE4);

	ReadyScript();

	CEngine::GetInstance()->StopSound(CHANNELID::BGM);
	CEngine::GetInstance()->PlayBGM("DragonBallZThema.mp3");

	return S_OK;
}

_uint CScene_Stage4::Update(_double TimeDelta)
{
	__super::Update(TimeDelta);
	return _uint();
}

HRESULT CScene_Stage4::Render()
{
	__super::Render();

	return S_OK;
}

HRESULT CScene_Stage4::ReadyScript()
{
	//m_pEngine->AddScriptObject(CUrsa::Create(nullptr), CEngine::GetInstance()->GetCurSceneNumber());
	//m_pEngine->AddScriptObject(CUrsaDunDoor::Create(nullptr), CEngine::GetInstance()->GetCurSceneNumber());

	return S_OK;
}

CScene_Stage4 * CScene_Stage4::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext, _uint iLevelIndex)
{
	CScene_Stage4*		pInstance = new CScene_Stage4(pDevice, pDeviceContext, iLevelIndex);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed to Creating CScene_Stage4");
		SafeRelease(pInstance);
	}

	return pInstance;
}

void CScene_Stage4::Free()
{
	__super::Free();
}
