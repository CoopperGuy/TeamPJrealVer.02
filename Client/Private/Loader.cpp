#include "stdafx.h"
#include "..\public\Loader.h"
#include "Engine.h"
#include "HpBar.h"
#include "LevelFlag.h"
#include "DmgFont.h"
#include "SkillIcon.h"
#include "MonHp.h"
#include "LoadingGauge.h"
#include "Player.h"
CLoader::CLoader()
{
}

void ThreadTest(CLoader*	loader, string path, SCENE sceneNum,_int threadIdx) {
	CEngine::GetInstance()->DeserializeScene(path, sceneNum);
	loader->SetFinish(threadIdx);
}

unsigned int APIENTRY ThreadMain(void* pArg)
{
	CLoader*		pLoader = (CLoader*)pArg;

	EnterCriticalSection(&pLoader->Get_CS());

	HRESULT			hr = 0;

	switch (pLoader->Get_NextScene())
	{
	case SCENE_LOGO:
		hr = pLoader->GameSceneLogo();
		break;
	case SCENE_GAMEPLAY:
		hr = pLoader->GamePlayLoader();
		break;
	case SCENE_TEST:
		hr = pLoader->GameTestLoader();
		break;
	case SCENE_STAGE1:
		hr = pLoader->GameSceneStage01();
		break;
	case SCENE_STAGE2:
		hr = pLoader->GameSceneStage02();
		break;
	case Client::SCENE_KIM:
		hr = pLoader->GameSceneKIM();
		break;
	case Client::SCENE_LEE:
		hr = pLoader->GameSceneLEE();
		break;
	case Client::SCENE_JUNG:
		hr = pLoader->GameSceneJUN();
		break;
	case Client::SCENE_SEO:
		hr = pLoader->GameSceneSEO();
		break;
		/*case SCENE_GAMEPLAY1:
		break;*/
	}

	if (FAILED(hr))
	{
		return 0;
	}

	LeaveCriticalSection(&pLoader->Get_CS());

	return 0;
}


HRESULT CLoader::Initialize(SCENE eScene)
{
	InitializeCriticalSection(&m_CS);

	m_eScene = eScene;
	m_pLoadingGauge = CLoadingGauge::Create(nullptr);
	CEngine::GetInstance()->AddScriptObject(m_pLoadingGauge, CEngine::GetInstance()->GetCurSceneNumber());

	m_hThread = (HANDLE)_beginthreadex(nullptr, 0, ThreadMain, this, 0, nullptr);

	switch (m_eScene)
	{
	case Client::SCENE_STATIC:
		break;
	case Client::SCENE_LOGO:
		break;
	case Client::SCENE_LOBBY:
		break;
	case Client::SCENE_LOADING:
		break;
	case Client::SCENE_GAMEPLAY:
		break;
	case Client::SCENE_TEST: {
		/*	std::thread t1(ThreadTest, this, "../../Assets/Scenes/InventoryUI.yaml" , SCENE_TEST);
		t1.detach();*/
	}
							 break;
	case Client::SCENE_EFFECT:
		break;
	case Client::SCENE_KIM:
		break;
	case Client::SCENE_LEE:
		break;
	case Client::SCENE_JUNG:
		break;
	case Client::SCENE_SEO:
		break;
	}
	// call TerminateThread to stop thread
	//TerminateThread(m_hThread, 0);
	//m_hThread = (HANDLE)_beginthreadex(nullptr, 0, ThreadMain, this, 0, nullptr);

	if (0 == m_hThread)
		return E_FAIL;



	return S_OK;
}

HRESULT CLoader::UpdateGauge(_double deltaTime)
{
	_float	percentage = m_pLoadingGauge->GetPercentage();
	if (percentage < 90.f) {
		m_pLoadingGauge->AddPercentage(1.f*deltaTime);
	//	cout << percentage << "\n";
	}
	return S_OK;
}

HRESULT CLoader::GamePlayLoader()
{

	m_isFinish = true;

	return S_OK;
}

HRESULT CLoader::GameTestLoader()
{
	CEngine::GetInstance()->DeserializeScene("../../Assets/Scenes/FieldMapGround.yaml", SCENE_TEST);

	m_isFinish = true;

	return S_OK;
}

HRESULT CLoader::GameSceneStage01()
{
	CEngine::GetInstance()->DeserializeScene("../../Assets/Scenes/CityMap.yaml", SCENE_STAGE1);
	m_isFinish = true;
	return S_OK;
}

HRESULT CLoader::GameSceneStage02()
{
	CEngine::GetInstance()->DeserializeScene("../../Assets/Scenes/Flogas_Dungeon.yaml", SCENE_STAGE2);
	CEngine::GetInstance()->DeserializeScene("../../Assets/Scenes/Flogas.yaml", SCENE_STAGE2);

	m_isFinish = true;
	return S_OK;
}

HRESULT CLoader::GameSceneLogo()
{
	std::thread t0(ThreadTest, this, "../../Assets/Scenes/LogoTest.yaml", SCENE_LOGO, 0);
	t0.detach();

	std::thread t1(ThreadTest, this, "../../Assets/Scenes/MainUI.yaml", SCENE_STATIC, 1);
	t1.detach();

	std::thread t2(ThreadTest, this, "../../Assets/Scenes/InventoryUI.yaml", SCENE_STATIC, 2);
	t2.detach();

	std::thread t3(ThreadTest, this, "../../Assets/Scenes/QuestUI.yaml", SCENE_STATIC, 3);
	t3.detach();

	std::thread t4(ThreadTest, this, "../../Assets/Scenes/ShopUI.yaml", SCENE_STATIC, 4);
	t4.detach();

	std::thread t5(ThreadTest, this, "../../Assets/Scenes/PortalUI.yaml", SCENE_STATIC, 5);
	t5.detach();

	CEngine::GetInstance()->DeserializeScene("../../Assets/Scenes/Player.yaml", SCENE_STATIC);
	while (m_pLoadingGauge->GetPercentage() < 30.f)
		m_pLoadingGauge->AddPercentage(0.001f);
	CEngine::GetInstance()->DeserializeScene("../../Assets/Scenes/Equip.yaml", SCENE_STATIC);
	while (m_pLoadingGauge->GetPercentage() < 80.f)
		m_pLoadingGauge->AddPercentage(0.001f);

	CEngine::GetInstance()->DeserializePrefab();

	while (m_pLoadingGauge->GetPercentage() < 90.f)
		m_pLoadingGauge->AddPercentage(0.001f);
	////AddEffect
	//if (FAILED(CEngine::GetInstance()->CreatePrefab("Prototype_Effect_BossFly", "E_BossFly")))
	//	MSG_BOX("Failed To Create BossFly Prefab");

	//if (FAILED(CEngine::GetInstance()->CreatePrefab("Prototype_Effect_BossFlyLaser", "E_Laser")))
	//	MSG_BOX("Failed To Create BossFlyLaser Prefab");

	//if (FAILED(CEngine::GetInstance()->CreatePrefab("Prototype_Effect_Dust", "E_DustEffect")))
	//	MSG_BOX("Failed To Create DustEffect Prefab");

	//if (FAILED(CEngine::GetInstance()->CreatePrefab("Prototype_Effect_SwordRing", "E_SwordRing")))
	//	MSG_BOX("Failed To Create ring Prefab");

	//if (FAILED(CEngine::GetInstance()->CreatePrefab("Prototype_Effect_Blackhole", "E_Blackhole")))
	//	MSG_BOX("Failed To Create Blackhole Prefab");

	//if (FAILED(CEngine::GetInstance()->CreatePrefab("Prototype_Effect_Grab", "E_Grab")))
	//	MSG_BOX("Failed To Create Grab Prefab");

	//if (FAILED(CEngine::GetInstance()->CreatePrefab("Prototype_Effect_Pajang", "E_Pajangs")))
	//	MSG_BOX("Failed To Create Pajang Prefab");

	if (FAILED(CEngine::GetInstance()->CreatePrefab("Prototype_GameObject_OBBs", "O_OBBs")))
		MSG_BOX("Failed To Create Pajang Prefab");

	if (FAILED(CEngine::GetInstance()->CreatePrefab("Prototype_GameObject_DMGFont", "U_DamageVIBuffer")))
		MSG_BOX("Failed To Create shoulderEffect Prefab");

	if (FAILED(CEngine::GetInstance()->CreatePrefab("Prototype_Effect_shoulderEffect", "E_shoulderEffect")))
		MSG_BOX("Failed To Create shoulderEffect Prefab");

	if (FAILED(CEngine::GetInstance()->CreatePrefab("Prototype_Effect_ImpactGround", "E_ImpactGround")))
		MSG_BOX("Failed To Create shoulderEffect Prefab");

	_bool threadFinish = false;
	while (!threadFinish) {
		for (_int i = 0; i < ThreadIndex; i++) {
			if (m_isThreadFinish[i] == false)
				break;
		}
		threadFinish = true;
	}

	m_pLoadingGauge->SetPercentage(100.f);
	m_isFinish = true;

	return S_OK;
}

HRESULT CLoader::GameSceneKIM()
{
	CEngine::GetInstance()->DeserializeScene("../../Assets/Scenes/m_pxVertices2.yaml", SCENE_KIM);
	CEmptyGameObject* pPlayer = static_cast<CEmptyGameObject*>(CEngine::GetInstance()->FindGameObjectWithName(SCENE_STATIC, "Player"));
	static_cast<CCollider*>(pPlayer->GetComponent("Com_Collider"))->SetPosition(_float3(0.f, 0.f, 0.f));

	if (FAILED(CEngine::GetInstance()->CreatePrefab("Prototype_GameObecjt_WaterEA", "WaterEA")))
	{
		MSG_BOX("Failed To Create Prefab");
	}
	m_isFinish = true;

	return S_OK;
}

HRESULT CLoader::GameSceneLEE()
{
	//CEngine::GetInstance()->DeserializeScene("../../Assets/Scenes/TestLee.yaml", SCENE_LEE);
	CEngine::GetInstance()->DeserializeScene("../../Assets/Scenes/Flogas_Dungeon.yaml", SCENE_LEE);
	CEngine::GetInstance()->DeserializeScene("../../Assets/Scenes/Flogas.yaml", SCENE_LEE);

	/*if (FAILED(CEngine::GetInstance()->CreatePrefab("Prototype_Instance_Fire", "E_InstanceFire")))
	{
		MSG_BOX("Failed To Create INSTANPrefab");
	}*/

	m_isFinish = true;

	return S_OK;
}

HRESULT CLoader::GameSceneJUN()
{
	//CEngine::GetInstance()->DeserializeScene("../../Assets/Scenes/Flogas.yaml", SCENE_JUNG);
	//CEngine::GetInstance()->DeserializeScene("../../Assets/Scenes/TestRoom_Jun.yaml", SCENE_JUNG);
	CEngine::GetInstance()->DeserializeScene("../../Assets/Scenes/Boss.yaml", SCENE_JUNG);
	CEngine::GetInstance()->DeserializeScene("../../Assets/Scenes/Dungeon1_JunT.yaml", SCENE_JUNG);
	CEngine::GetInstance()->DeserializeScene("../../Assets/Scenes/TestRoom_Effect_Jun.yaml", SCENE_JUNG);
	//if (FAILED(CEngine::GetInstance()->CreatePrefab("Prototype_Instance_Fire", "E_InstanceFire")))
	//{
	//	MSG_BOX("Failed To Create INSTANPrefab");
	//}

	m_isFinish = true;

	return S_OK;
}

HRESULT CLoader::GameSceneSEO()
{
	//CEngine::GetInstance()->DeserializeScene("../../Assets/Scenes/DungeonTest.yaml", SCENE_SEO);
	//CEngine::GetInstance()->DeserializeScene("../../Assets/Scenes/Dungeon1.yaml", SCENE_SEO);
	//CEngine::GetInstance()->DeserializeScene("../../Assets/Scenes/TestSeongyeon.yaml", SCENE_SEO);
	//CEngine::GetInstance()->DeserializeScene("../../Assets/Scenes/Boss.yaml", SCENE_SEO);
	//CEngine::GetInstance()->DeserializeScene("../../Assets/Scenes/Flogas.yaml", SCENE_SEO);
	//CEngine::GetInstance()->DeserializeScene("../../Assets/Scenes/BossSY.yaml", SCENE_SEO);

	if (FAILED(CEngine::GetInstance()->CreatePrefab("Prototype_Effect_EA_Att_Fire", "E_EA_Att_Fire")))
		MSG_BOX("Failed To Create Fire Prefab");
	if (FAILED(CEngine::GetInstance()->CreatePrefab("Prototype_Effect_FireBall", "E_FireBall")))
		MSG_BOX("Failed To Create FireBall Prefab");

	if (FAILED(CEngine::GetInstance()->CreatePrefab("Prototype_GameObecjt_WaterEA", "O_WaterEA")))
		MSG_BOX("Failed To Create Prefab");

	//CEngine::GetInstance()->DeserializeScene("../../Assets/Scenes/Boss.yaml", SCENE_SEO);
	CEngine::GetInstance()->DeserializeScene("../../Assets/Scenes/Dungeon1_Seo2.yaml", SCENE_SEO);

	m_isFinish = true;

	return S_OK;
}

CLoader * CLoader::Create(SCENE eScene)
{
	CLoader*		pInstance = new CLoader();

	if (FAILED(pInstance->Initialize(eScene)))
	{
		MSG_BOX("Failed to Creating CLoader");
		SafeRelease(pInstance);
	}

	return pInstance;
}

void CLoader::Free()
{
	WaitForSingleObject(m_hThread, INFINITE);

	DeleteCriticalSection(&m_CS);

	DeleteObject(m_hThread);

}
