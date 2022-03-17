#include "stdafx.h"
#include "..\Public\Scene_Test.h"
#include "Engine.h"


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

USING(Client)
static _bool pathFinding = false;

CScene_Test::CScene_Test(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext, _uint iSceneIndex)
	: CScene(pDevice, pDeviceContext, iSceneIndex)
{
}

CScene_Test * CScene_Test::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext, _uint iSceneIndex)
{
	CScene_Test*		pInstance = new CScene_Test(pDevice, pDeviceContext, iSceneIndex);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed to Creating CScene_Test");
		SafeRelease(pInstance);
	}

	return pInstance;
}

void CScene_Test::Free()
{
	__super::Free();
}

HRESULT CScene_Test::Initialize()
{
	__super::Initialize();

	ReadyScript();

	return S_OK;
}

_uint CScene_Test::Update(_double TimeDelta)
{
	__super::Update(TimeDelta);

	if (pathFinding)
	{
		if (m_pPathFinding)
			m_pPathFinding->Update();
	}



#ifdef _DEBUG
	//testDelta += TimeDelta;
	//if (testDelta > testTime) {
	//	testDelta = 0;
	//	_int dmg = rand() % 9999;
	//	_float3 pos = { 0.f,0.f,0.f };
	//	CDmgVIBuffer::Create(nullptr, pos,(_float)dmg);
	//}

	//if (m_pEngine->IsKeyDown(VK_LBUTTON)) {
	//	testHud->CreateComboIcon(CComboHud::COMBOPOS_TOP, false);
	//}
	//if (m_pEngine->IsKeyDown(VK_RBUTTON)) {
	//	testHud->CreateComboIcon(CComboHud::COMBOPOS_BOT, true, true);
	//}

	if (m_pEngine->IsKeyDown('M')) {
		m_pEngine->AddDeadObject("Wolf");
	}
#endif
	return _uint();
}

HRESULT CScene_Test::Render()
{
	__super::Render();

	return S_OK;
}


HRESULT CScene_Test::ReadyLayerCamera(string pLayerTag)
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


HRESULT CScene_Test::ReadyLayerGrid(string pLayerTag)
{

	CGrid::GRIDDESC desc = { 120, 120, 1.2f };


	if (nullptr == CEngine::GetInstance()->AddGameObject(0, "GameObject_Grid", pLayerTag, &desc))
		return E_FAIL;

	
	return S_OK;
}

HRESULT CScene_Test::ReadyScript()
{

	//list<class CGameObject*> listEnemy = CEngine::GetInstance()->GetGameObjectInLayer(0, "Enemy");
	//for(auto& iter : listEnemy)
	//	m_pEngine->AddScriptObject(CEnemy::Create(iter));

	TestScriptOhter();
	return S_OK;
}

HRESULT CScene_Test::ReadyLayerSky(string pLayerTag)
{

	if (nullptr == CEngine::GetInstance()->AddGameObject(0, "Prototype_Sky", pLayerTag))
		return E_FAIL;


	return S_OK;
}

void CScene_Test::TestScriptHud()
{

}


void CScene_Test::TestScriptOhter()
{

	list<CGameObject*> spawnList = CEngine::GetInstance()->GetGameObjectInLayer(m_pEngine->GetCurSceneNumber(), "WolfSpawner");
	for (auto& iter : spawnList) {
		CSpawn::Create(iter);
	}
	m_pEngine->AddScriptObject(CWolfBoss::Create(nullptr), SCENE_TEST);

	//CElfNPC::Create(m_pEngine->FindGameObjectWithName(m_pEngine->GetCurSceneNumber(), "Npc_Elf")), m_pEngine->GetCurSceneNumber();

	//CElfNPC::Create(m_pEngine->FindGameObjectWithName(m_pEngine->GetCurSceneNumber(),"Npc_Elf")), m_pEngine->GetCurSceneNumber();
	//CNpc_Shop::Create(m_pEngine->FindGameObjectWithName(m_pEngine->GetCurSceneNumber(),"Npc_Human_Shop"));

}
