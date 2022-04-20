#include "stdafx.h"
#include "..\Public\Scene_Stage01.h"

#pragma region SCRIPTS
#include "HpBar.h"
#include "LevelFlag.h"
#include "DmgFont.h"
#include "SkillIcon.h"
#include "MonHp.h"
#include "Player.h"
#include "TestMap.h"
#include "ComboHud.h"
#include "Wolf.h"
#include "MenuHud.h"
#include "BackPackHud.h"
#include "ConsumItemHud.h"
#include "Axe.h"
#include "MaterialHud.h"
#include "TalkHud.h"
#include "DmgVIBuffer.h"
#include "Wolf.h"
#include "WolfBoss.h"
#include "MonHpVIBuffer.h"
#include "ElfNPC.h"
#include "Npc_Shop.h"
#include "Spawn.h"
#include "Glove.h"
#include "Armor.h"
#include "Boots.h"
#include "Portal.h"
#include "Scene_Loading.h"
#include "EventCheck.h"
#pragma endregion

USING(Client)

CScene_Stage01::CScene_Stage01(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext, _uint iLevelIndex)
	: CScene(pDevice, pDeviceContext, iLevelIndex)
{
}

HRESULT CScene_Stage01::Initialize()
{
	__super::Initialize();

	CEngine::GetInstance()->SetCurSceneNumber(SCENE_STAGE1);

	ReadyScript();

	/*CEngine::GetInstance()->StopSound(CHANNELID::BGM);
	CEngine::GetInstance()->PlayBGM("BGM_CityMap.mp3");
*/

	return S_OK;
}

_uint CScene_Stage01::Update(_double TimeDelta)
{
	__super::Update(TimeDelta);

	if (m_pPortal->GetChangeScene()) {
		SCENE scene = m_pPortal->GetCurScene();
		switch (scene)
		{
		case Client::SCENE_STAGE1:
			break;
		case Client::SCENE_STAGE2:
			if (FAILED(CEngine::GetInstance()->SetUpCurrentScene(CScene_Loading::Create(m_pDevice, m_pDeviceContext, SCENE_STAGE2, (_uint)SCENE_LOADING), CEngine::GetInstance()->GetCurSceneNumber())))
				return E_FAIL;
			break;
		case Client::SCENE_STAGE3:
			if (FAILED(CEngine::GetInstance()->SetUpCurrentScene(CScene_Loading::Create(m_pDevice, m_pDeviceContext, SCENE_STAGE3, (_uint)SCENE_LOADING), CEngine::GetInstance()->GetCurSceneNumber())))
				return E_FAIL;
			break;
		case Client::SCENE_STAGE4:
			if (FAILED(CEngine::GetInstance()->SetUpCurrentScene(CScene_Loading::Create(m_pDevice, m_pDeviceContext, SCENE_STAGE4, (_uint)SCENE_LOADING), CEngine::GetInstance()->GetCurSceneNumber())))
				return E_FAIL;
			break;
		}

	}

	/*if (m_pEngine->IsKeyDown('M')) {
		m_pEngine->AddDeadObject("Wolf");
	}
*/

	return _uint();
}

HRESULT CScene_Stage01::Render()
{
	__super::Render();

	return S_OK;
}

HRESULT CScene_Stage01::ReadyScript()
{
	CElfNPC::Create(m_pEngine->FindGameObjectWithName(m_pEngine->GetCurSceneNumber(), "Npc_Elf")), m_pEngine->GetCurSceneNumber();
	CNpc_Shop::Create(m_pEngine->FindGameObjectWithName(m_pEngine->GetCurSceneNumber(), "Npc_Human_Shop"));
	CNpc_Shop::Create(m_pEngine->FindGameObjectWithName(m_pEngine->GetCurSceneNumber(), "EquipNPC"));

	m_pPortal = CPortal::Create();
	CEngine::GetInstance()->AddScriptObject(m_pPortal, CEngine::GetInstance()->GetCurSceneNumber());
	return S_OK;
}

HRESULT CScene_Stage01::ReadyLayerSky(string pLayerTag)
{

	if (nullptr == CEngine::GetInstance()->AddGameObject(CEngine::GetInstance()->GetCurSceneNumber(), "Prototype_Sky", pLayerTag))
		return E_FAIL;


	return S_OK;
}

CScene_Stage01 * CScene_Stage01::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext, _uint iLevelIndex)
{
	CScene_Stage01*		pInstance = new CScene_Stage01(pDevice, pDeviceContext, iLevelIndex);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed to Creating CScene_Stage01");
		SafeRelease(pInstance);
	}

	return pInstance;
}

void CScene_Stage01::Free()
{
	__super::Free();
}
