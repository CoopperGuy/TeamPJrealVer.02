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
#include "FlogasDunDoor.h"


CLoader::CLoader()
{
}

void ThreadTest(CLoader*	loader, string path, SCENE sceneNum, _int threadIdx) {
	CEngine::GetInstance()->DeserializeScene(path, sceneNum);
	loader->SetFinish(threadIdx);
}

_int ThreadPrefab(CLoader*	loader, string _prototypeTag, string _name, _int bit) {
	if (FAILED(CEngine::GetInstance()->CreatePrefab(_prototypeTag, _name))) {
		wstring path;
		path.assign(_name.begin(), _name.end());
		wstring errMsg = L"Failed To Create Prefab" + path;
		MessageBox(NULL, errMsg.c_str(), L"System Message", MB_OK);
	}
	loader->SetCompleteBit(bit);

	return bit;
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
	case SCENE_STAGE3:
		hr = pLoader->GameSceneStage03();
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
		m_pLoadingGauge->AddPercentage(1.f*(_float)deltaTime);
		//	cout << percentage << "\n";
	}
	return S_OK;
}

HRESULT CLoader::GamePlayLoader()
{

	m_isFinish = true;

	return S_OK;
}

HRESULT CLoader::GameFlogasLoader()
{
	m_ThreadLoader = new CThreadLoader(23);

	std::vector<std::future<_int>> futures;
	m_ThreadLoader->EnqueueJob(ThreadPrefab, this, "Prototype_Effect_BossFly", "E_BossFly", 0);
	m_ThreadLoader->EnqueueJob(ThreadPrefab, this, "Prototype_Effect_BossFlyLaser", "E_Laser", 1);
	m_ThreadLoader->EnqueueJob(ThreadPrefab, this, "Prototype_Effect_Dust", "E_DustEffect", 2);
	m_ThreadLoader->EnqueueJob(ThreadPrefab, this, "Prototype_Effect_SwordRing", "E_SwordRing", 3);
	m_ThreadLoader->EnqueueJob(ThreadPrefab, this, "Prototype_Effect_Blackhole", "E_Blackhole", 4);
	m_ThreadLoader->EnqueueJob(ThreadPrefab, this, "Prototype_Effect_Grab", "E_Grab", 5);
	m_ThreadLoader->EnqueueJob(ThreadPrefab, this, "Prototype_Effect_Pajang", "E_Pajangs", 6);
	m_ThreadLoader->EnqueueJob(ThreadPrefab, this, "Prototype_Effect_FireSlash", "E_FireSlash", 7);
	m_ThreadLoader->EnqueueJob(ThreadPrefab, this, "Prototype_Effect_MeteoTrail", "E_MeteoTrail", 8);
	m_ThreadLoader->EnqueueJob(ThreadPrefab, this, "Prototype_Effect_MeteoFire", "E_MeteoFire", 9);
	m_ThreadLoader->EnqueueJob(ThreadPrefab, this, "Prototype_Effect_MeteoDropArea", "E_MeteoDropArea", 10);
	m_ThreadLoader->EnqueueJob(ThreadPrefab, this, "Prototype_Effect_MeteoDropAf", "E_MeteoDropAfter", 11);
	m_ThreadLoader->EnqueueJob(ThreadPrefab, this, "Prototype_Effect_Meteo", "E_Meteo", 12);
	m_ThreadLoader->EnqueueJob(ThreadPrefab, this, "Prototype_Effect_MeteoEnd", "E_MeteoEnd", 13);
	m_ThreadLoader->EnqueueJob(ThreadPrefab, this, "Prototype_GameObecjt_MeteoOBB", "O_MeteoOBB", 14);
	m_ThreadLoader->EnqueueJob(ThreadPrefab, this, "Prototype_Effect_Smoke", "E_Smoke", 15);
	m_ThreadLoader->EnqueueJob(ThreadPrefab, this, "Prototype_Effect_Fire_explosion", "E_MeteoExplosion", 16);
	m_ThreadLoader->EnqueueJob(ThreadPrefab, this, "Prototype_Effect_FireBoob", "E_FireBoob", 17);
	m_ThreadLoader->EnqueueJob(ThreadPrefab, this, "Prototype_Effect_Fire", "E_EAFire", 18);
	m_ThreadLoader->EnqueueJob(ThreadPrefab, this, "Prototype_Effect_FireBall", "E_EAFireBall", 19);
	m_ThreadLoader->EnqueueJob(ThreadPrefab, this, "Prototype_GameObecjt_WaterEA", "O_WaterEA", 20);
	m_ThreadLoader->EnqueueJob(ThreadPrefab, this, "Prototype_GameObecjt_EAFire", "O_EAFire", 21);
	m_ThreadLoader->EnqueueJob(ThreadPrefab, this, "Prototype_GameObecjt_EAFire", "O_EAFire", 22);
	m_ThreadLoader->EnqueueJob(ThreadPrefab, this, "Prototype_Effect_ElementBomb", "E_Element_Bomb", 23);

	for (auto& f : futures) {
		cout << "return result : " << f.get();
	}


	m_iCompleteBit = 0;

	SafeRelease(m_ThreadLoader);


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
	CEmptyGameObject* pPlayer = static_cast<CEmptyGameObject*>(CEngine::GetInstance()->FindGameObjectWithName(SCENE_STATIC, "Player"));
	static_cast<CCollider*>(pPlayer->GetComponent("Com_Collider"))->SetPosition(_float3(0.f, 0.5f, -4.f));

	CEngine::GetInstance()->DeserializeScene("../../Assets/Scenes/Flogas_Dungeon.yaml", SCENE_STAGE2);
	CEngine::GetInstance()->DeserializeScene("../../Assets/Scenes/Flogas.yaml", SCENE_STAGE2);

	if (FAILED(GameFlogasLoader()))
		MSG_BOX("Failed To Create Flogas Effect");

	m_isFinish = true;
	return S_OK;
}

HRESULT CLoader::GameSceneStage03()
{
	CEmptyGameObject* pPlayer = static_cast<CEmptyGameObject*>(CEngine::GetInstance()->FindGameObjectWithName(SCENE_STATIC, "Player"));
	static_cast<CCollider*>(pPlayer->GetComponent("Com_Collider"))->SetPosition(_float3(0.f, 0.5f, 0.f));

	CEngine::GetInstance()->DeserializeScene("../../Assets/Scenes/UrsaDungeon.yaml", SCENE_STAGE3);
	CEngine::GetInstance()->DeserializeScene("../../Assets/Scenes/Boss_Ursa.yaml", SCENE_STAGE3);

	m_isFinish = true;
	return S_OK;
}

HRESULT CLoader::GameSceneLogo()
{
	m_ThreadLoader = new CThreadLoader(16);
	std::vector<std::future<_int>> futures;

	(m_ThreadLoader->EnqueueJob(ThreadTest, this, "../../Assets/Scenes/LogoTest.yaml", SCENE_LOGO, 0));
	(m_ThreadLoader->EnqueueJob(ThreadTest, this, "../../Assets/Scenes/MainUI.yaml", SCENE_STATIC, 1));
	(m_ThreadLoader->EnqueueJob(ThreadTest, this, "../../Assets/Scenes/InventoryUI.yaml", SCENE_STATIC, 2));
	(m_ThreadLoader->EnqueueJob(ThreadTest, this, "../../Assets/Scenes/QuestUI.yaml", SCENE_STATIC, 3));
	(m_ThreadLoader->EnqueueJob(ThreadTest, this, "../../Assets/Scenes/PortalUI.yaml", SCENE_STATIC, 4));
	(m_ThreadLoader->EnqueueJob(ThreadTest, this, "../../Assets/Scenes/PortalUI.yaml", SCENE_STATIC, 5));
	(m_ThreadLoader->EnqueueJob(ThreadTest, this, "../../Assets/Scenes/Player.yaml", SCENE_STATIC, 6));
	(m_ThreadLoader->EnqueueJob(ThreadTest, this, "../../Assets/Scenes/Equip.yaml", SCENE_STATIC, 7));

	CEngine::GetInstance()->DeserializePrefab();

	m_ThreadLoader->EnqueueJob(ThreadPrefab, this, "Prototype_GameObject_OBBs", "O_OBBs", 0);
	m_ThreadLoader->EnqueueJob(ThreadPrefab, this, "Prototype_GameObject_DMGFont", "U_DamageVIBuffer", 1);
	m_ThreadLoader->EnqueueJob(ThreadPrefab, this, "Prototype_Effect_shoulderEffect", "E_shoulderEffect", 2);
	m_ThreadLoader->EnqueueJob(ThreadPrefab, this, "Prototype_Effect_ImpactGround", "E_ImpactGround2", 3);
	m_ThreadLoader->EnqueueJob(ThreadPrefab, this, "Prototype_Effect_ImpactBeam", "E_ImpactBeam00", 4);
	m_ThreadLoader->EnqueueJob(ThreadPrefab, this, "Prototype_Effect_ImpactShort", "E_ImpactShort", 5);
	m_ThreadLoader->EnqueueJob(ThreadPrefab, this, "Prototype_Effect_Flare", "E_Flare", 6);
	m_ThreadLoader->EnqueueJob(ThreadPrefab, this, "Prototype_Effect_Wind", "E_Winds", 7);
	m_ThreadLoader->EnqueueJob(ThreadPrefab, this, "Prototype_Effect_ChargeEffect", "E_ChargeEffect", 8);
	m_ThreadLoader->EnqueueJob(ThreadPrefab, this, "Prototype_Effect_AuraEffect", "E_AuraEffect", 9);

	for (auto& f : futures) {
		cout << "return result : " << f.get();
	}
	
	m_iCompleteBit = 0;


	SafeRelease(m_ThreadLoader);
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
	CEngine::GetInstance()->DeserializeScene("../../Assets/Scenes/TestRoomLee.yaml", SCENE_LEE);
	//CEngine::GetInstance()->DeserializeScene("../../Assets/Scenes/Flogas.yaml", SCENE_LEE);

	/*if (FAILED(CEngine::GetInstance()->CreatePrefab("Prototype_Instance_Fire", "E_InstanceFire")))
	{
		MSG_BOX("Failed To Create INSTANPrefab");
	}*/

	m_isFinish = true;

	return S_OK;
}

HRESULT CLoader::GameSceneJUN()
{
	CEmptyGameObject* pPlayer = static_cast<CEmptyGameObject*>(CEngine::GetInstance()->FindGameObjectWithName(SCENE_STATIC, "Player"));
	static_cast<CCollider*>(pPlayer->GetComponent("Com_Collider"))->SetPosition(_float3(0.f, 0.5f, -4.f));
	CEngine::GetInstance()->DeserializeScene("../../Assets/Scenes/Flogas.yaml", SCENE_JUNG);
	//CEngine::GetInstance()->DeserializeScene("../../Assets/Scenes/TestRoom_Jun.yaml", SCENE_JUNG);
	//CEngine::GetInstance()->DeserializeScene("../../Assets/Scenes/Boss_Ursa.yaml", SCENE_JUNG);
	CEngine::GetInstance()->DeserializeScene("../../Assets/Scenes/Dungeon1_JunT.yaml", SCENE_JUNG);
	//CEngine::GetInstance()->DeserializeScene("../../Assets/Scenes/TestRoom_Effect_Jun.yaml", SCENE_JUNG);

	if (FAILED(GameFlogasLoader()))
		MSG_BOX("Failed To Create Flogas Effect");
	
	
	m_isFinish = true;

	return S_OK;
}

HRESULT CLoader::GameSceneSEO()
{
	CEmptyGameObject* pPlayer = static_cast<CEmptyGameObject*>(CEngine::GetInstance()->FindGameObjectWithName(SCENE_STATIC, "Player"));
	static_cast<CCollider*>(pPlayer->GetComponent("Com_Collider"))->SetPosition(_float3(0.f, 0.f, 5.f));

	if (FAILED(CEngine::GetInstance()->CreatePrefab("Prototype_Effect_Blood", "E_IIBlood")))
		MSG_BOX("Failed To Create E_Blood Prefab");

	if (FAILED(CEngine::GetInstance()->CreatePrefab("Prototype_Effect_BloodDecal", "E_BloodDecal")))
		MSG_BOX("Failed To Create BloodDecal Prefab");

	if (FAILED(CEngine::GetInstance()->CreatePrefab("Prototype_Effect_RockDust", "E_InsDust")))
		MSG_BOX("Failed To Create InsDust Prefab");

	if (FAILED(CEngine::GetInstance()->CreatePrefab("Prototype_GameObecjt_Wolf", "O_Wolf")))
		MSG_BOX("Failed To Create O_Wolf Prefab");

	if (FAILED(CEngine::GetInstance()->CreatePrefab("Prototype_GameObecjt_Rock", "O_Rock")))
		MSG_BOX("Failed To Create Rock Prefab");

	//if (FAILED(GameFlogasLoader()))
	//	MSG_BOX("Failed To Create Flogas Effect");

	//CEngine::GetInstance()->DeserializeScene("../../Assets/Scenes/CityMap.yaml", SCENE_SEO);
	//CEngine::GetInstance()->DeserializeScene("../../Assets/Scenes/Flogas.yaml", SCENE_SEO);
	CEngine::GetInstance()->DeserializeScene("../../Assets/Scenes/Dungeon1_SY.yaml", SCENE_SEO);
	//CEngine::GetInstance()->DeserializeScene("../../Assets/Scenes/TestRoom_Effect_Jun.yaml", SCENE_SEO);

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
