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
	:m_isFinish(false)
{
}


void ThreadTest(CLoader*	loader, string path, SCENE sceneNum, _int threadIdx) {
	CEngine::GetInstance()->DeserializeScene(path, sceneNum);
}

void ThreadPrefab(CLoader*	loader, string _prototypeTag, string _name, _int bit) {
	CEngine::GetInstance()->CreatePrefab(_prototypeTag, _name);
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


_bool CLoader::Get_IsEnd()
{
	return _bool();
}

HRESULT CLoader::Initialize(SCENE eScene)
{
	InitializeCriticalSection(&m_CS);
	m_threadSize = std::thread::hardware_concurrency();
	m_ThreadLoader = new CThreadLoader(m_threadSize);

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
	if (m_ThreadLoader != nullptr) {
		_float percentage = m_ThreadLoader->loadingPercentage() * 100.f;
		m_pLoadingGauge->SetPercentage(percentage);
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
	m_ThreadLoader->EnqueueJob(ThreadPrefab, this, "Prototype_Effect_Bomb_Center", "E_Bomb_Center", 17);
	m_ThreadLoader->EnqueueJob(ThreadPrefab, this, "Prototype_Effect_Fire", "E_EAFire", 18);
	m_ThreadLoader->EnqueueJob(ThreadPrefab, this, "Prototype_Effect_FireBall", "E_EAFireBall", 19);
	m_ThreadLoader->EnqueueJob(ThreadPrefab, this, "Prototype_GameObject_WaterEA", "O_WaterEA", 20);
	m_ThreadLoader->EnqueueJob(ThreadPrefab, this, "Prototype_GameObecjt_EAFire", "O_EAFire", 21);
	m_ThreadLoader->EnqueueJob(ThreadPrefab, this, "Prototype_Effect_ElementBomb", "E_Element_Bomb", 22);
	m_ThreadLoader->EnqueueJob(ThreadPrefab, this, "Prototype_E_Bomb_Sprite", "E_Bomb_Sprite", 23);
	m_ThreadLoader->EnqueueJob(ThreadPrefab, this, "Prototype_E_Flash", "E_Flash", 24);
	m_ThreadLoader->EnqueueJob(ThreadPrefab, this, "Prototype_E_Sprite_Fire", "E_Sprite_Fire", 25);
	m_ThreadLoader->EnqueueJob(ThreadPrefab, this, "Prototype_GameObecjt_FlogasEffLight", "O_FlogasLighte", 26);
	m_ThreadLoader->EnqueueJob(ThreadPrefab, this, "Prototype_GameObecjt_FootHammer_Decal", "E_FootHammer_Decal", 27);
	m_ThreadLoader->EnqueueJob(ThreadPrefab, this, "Prototype_GameObecjt_FootHamer_Bomb", "E_FootHamer_Bomb", 28);

	m_iCompleteBit = 0;


	return S_OK;
}

HRESULT CLoader::GameUrsaLoader()
{
	m_ThreadLoader->EnqueueJob(ThreadPrefab, this, "Prototype_Effect_Ursa_SoilDust", "E_Ursa_SoilDust", 0);
	m_ThreadLoader->EnqueueJob(ThreadPrefab, this, "Prototype_Effect_UrsaeDust", "E_UDust", 1);
	m_ThreadLoader->EnqueueJob(ThreadPrefab, this, "Prototype_Effect_RockDust", "E_RockEff", 2);
	m_ThreadLoader->EnqueueJob(ThreadPrefab, this, "Prototype_GameObecjt_Rock", "O_Rock", 3);
	m_ThreadLoader->EnqueueJob(ThreadPrefab, this, "Prototype_Effect_DropRockEff", "E_RockEff2", 4);
	m_ThreadLoader->EnqueueJob(ThreadPrefab, this, "Prototype_GameObecjt_RockSmall", "O_RockSmall", 5);
	m_ThreadLoader->EnqueueJob(ThreadPrefab, this, "Prototype_Effect_RockDecal", "E_RockDecal", 6);
	m_ThreadLoader->EnqueueJob(ThreadPrefab, this, "Prototype_GameObecjt_Wolf", "O_Wolf", 7);
	m_ThreadLoader->EnqueueJob(ThreadPrefab, this, "Prototype_GameObecjt_DropDust", "E_DropDust", 8);
	m_ThreadLoader->EnqueueJob(ThreadPrefab, this, "Prototype_GameObecjt_UrsaShoulder", "E_UrsaShoulder", 9);
	m_ThreadLoader->EnqueueJob(ThreadPrefab, this, "Prototype_GameObecjt_Vine", "E_Vine", 10);
	m_ThreadLoader->EnqueueJob(ThreadPrefab, this, "Prototype_GameObecjt_SoilDecal", "E_SoilDecal", 11);
	m_ThreadLoader->EnqueueJob(ThreadPrefab, this, "Prototype_GameObecjt_UrsaWind", "E_UrsaWinds", 12);
	m_ThreadLoader->EnqueueJob(ThreadPrefab, this, "Prototype_GameObecjt_Ursa_DustWh", "E_Ursa_DustWh", 13);
	m_ThreadLoader->EnqueueJob(ThreadPrefab, this, "Prototype_GameObecjt_Ursa_PajangDecal", "E_UrsaPajang", 14);
	m_ThreadLoader->EnqueueJob(ThreadPrefab, this, "Prototype_GameObecjt_Ursa_PajangMesh", "E_UrsaPajangMesh", 15);
	m_ThreadLoader->EnqueueJob(ThreadPrefab, this, "Prototype_GameObecjt_Ursa_ROARDecal", "E_ROARDecal", 16);
	m_ThreadLoader->EnqueueJob(ThreadPrefab, this, "Prototype_GameObecjt_Ursa_Ring", "E_UrsRing", 17);
  
	m_iCompleteBit = 0;

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
	CEmptyGameObject* pPlayer = static_cast<CEmptyGameObject*>(CEngine::GetInstance()->FindGameObjectWithName(SCENE_STATIC, "Player"));
	static_cast<CCollider*>(pPlayer->GetComponent("Com_Collider"))->SetPosition(_float3(5.f, 1.f, 30.f));

	m_ThreadLoader->EnqueueJob(ThreadTest, this, "../../Assets/Scenes/CityMap.yaml", SCENE_STAGE1, 0);

	m_ThreadLoader->Start_Thread();


	while (!m_isFinish) {
		Sleep(200);
		if (m_ThreadLoader->GetIsEnd()) {
			m_isFinish = true;
		}
	}
	return S_OK;
}

HRESULT CLoader::GameSceneStage02()
{
	CEmptyGameObject* pPlayer = static_cast<CEmptyGameObject*>(CEngine::GetInstance()->FindGameObjectWithName(SCENE_STATIC, "Player"));
	static_cast<CCollider*>(pPlayer->GetComponent("Com_Collider"))->SetPosition(_float3(0.f, 0.5f, -22.f));

	m_ThreadLoader->EnqueueJob(ThreadTest, this, "../../Assets/Scenes/Flogas_Dungeon.yaml", SCENE_STAGE2, 0);
	m_ThreadLoader->EnqueueJob(ThreadTest, this, "../../Assets/Scenes/Boss_Flogas.yaml", SCENE_STAGE2, 1);


	if (FAILED(GameFlogasLoader()))
		MSG_BOX("Failed To Create Flogas Effect");


	

	m_ThreadLoader->Start_Thread();



	while (!m_isFinish) {
		Sleep(200);
		if (m_ThreadLoader->GetIsEnd()) {
			m_isFinish = true;
		}
	}
	return S_OK;
}

HRESULT CLoader::GameSceneStage03()
{
	CEmptyGameObject* pPlayer = static_cast<CEmptyGameObject*>(CEngine::GetInstance()->FindGameObjectWithName(SCENE_STATIC, "Player"));
	static_cast<CCollider*>(pPlayer->GetComponent("Com_Collider"))->SetPosition(_float3(0.f, 0.5f, 0.f));


	m_ThreadLoader->EnqueueJob(ThreadTest, this, "../../Assets/Scenes/UrsaDungeon.yaml", SCENE_STAGE3, 0);
	m_ThreadLoader->EnqueueJob(ThreadTest, this, "../../Assets/Scenes/Boss_Ursa.yaml", SCENE_STAGE3, 1);

	if (FAILED(GameUrsaLoader()))
		MSG_BOX("Failed To Create Ursa Effect");



	m_ThreadLoader->Start_Thread();



	while (!m_isFinish) {
		Sleep(200);
		if (m_ThreadLoader->GetIsEnd()) {
			m_isFinish = true;
		}
		else if (m_ThreadLoader->GetIsEnable()) {
			m_isFinish = true;
		}
	}

	return S_OK;
}

HRESULT CLoader::GameSceneLogo()
{
	CEngine::GetInstance()->DeserializePrefab();

	(m_ThreadLoader->EnqueueJob(ThreadTest, this, "../../Assets/Scenes/Player.yaml", SCENE_STATIC, 6));
	(m_ThreadLoader->EnqueueJob(ThreadTest, this, "../../Assets/Scenes/Equip.yaml", SCENE_STATIC, 7));
	(m_ThreadLoader->EnqueueJob(ThreadTest, this, "../../Assets/Scenes/LogoTest.yaml", SCENE_LOGO, 0));
	(m_ThreadLoader->EnqueueJob(ThreadTest, this, "../../Assets/Scenes/MainUI.yaml", SCENE_STATIC, 1));
	(m_ThreadLoader->EnqueueJob(ThreadTest, this, "../../Assets/Scenes/InventoryUI.yaml", SCENE_STATIC, 2));
	(m_ThreadLoader->EnqueueJob(ThreadTest, this, "../../Assets/Scenes/QuestUI.yaml", SCENE_STATIC, 3));
	(m_ThreadLoader->EnqueueJob(ThreadTest, this, "../../Assets/Scenes/ShopUI.yaml", SCENE_STATIC, 4));
	(m_ThreadLoader->EnqueueJob(ThreadTest, this, "../../Assets/Scenes/PortalUI.yaml", SCENE_STATIC, 5));
	(m_ThreadLoader->EnqueueJob(ThreadTest, this, "../../Assets/Scenes/ReinforceUI.yaml", SCENE_STATIC, 6));
	(m_ThreadLoader->EnqueueJob(ThreadTest, this, "../../Assets/Scenes/StatusUI.yaml", SCENE_STATIC, 7));
	(m_ThreadLoader->EnqueueJob(ThreadTest, this, "../../Assets/Scenes/SoundUI.yaml", SCENE_STATIC, 8));

	(m_ThreadLoader->EnqueueJob(ThreadPrefab, this, "Prototype_GameObject_OBBs", "O_OBBs", 0));
	(m_ThreadLoader->EnqueueJob(ThreadPrefab, this, "Prototype_GameObject_DMGFont", "U_DamageVIBuffer", 1));
	(m_ThreadLoader->EnqueueJob(ThreadPrefab, this, "Prototype_Effect_shoulderEffect", "E_shoulderEffect", 2));
	(m_ThreadLoader->EnqueueJob(ThreadPrefab, this, "Prototype_Effect_ImpactGround", "E_ImpactGround2", 3));
	(m_ThreadLoader->EnqueueJob(ThreadPrefab, this, "Prototype_Effect_Flare", "E_Flare", 6));
	(m_ThreadLoader->EnqueueJob(ThreadPrefab, this, "Prototype_Effect_Wind", "E_Winds", 7));
	(m_ThreadLoader->EnqueueJob(ThreadPrefab, this, "Prototype_Effect_ChargeEffect", "E_ChargeEffect", 8));
	(m_ThreadLoader->EnqueueJob(ThreadPrefab, this, "Prototype_Effect_AuraEffect", "E_AuraEffect", 9));
	(m_ThreadLoader->EnqueueJob(ThreadPrefab, this, "Prototype_Effect_BloodDecal", "E_BloodDecal", 10));
	(m_ThreadLoader->EnqueueJob(ThreadPrefab, this, "Prototype_Effect_DecalCrash", "E_DecalCrash", 11));
	(m_ThreadLoader->EnqueueJob(ThreadPrefab, this, "Prototype_Effect_IBlood", "E_IBlood", 12));
	(m_ThreadLoader->EnqueueJob(ThreadPrefab, this, "Prototype_Effect_IIBlood", "E_IIBlood", 13));
	(m_ThreadLoader->EnqueueJob(ThreadPrefab, this, "Prototype_Effect_ImpactBeam", "E_ImpactBeam00", 4));
	(m_ThreadLoader->EnqueueJob(ThreadPrefab, this, "Prototype_Effect_ImpactShort", "E_ImpactShort", 5));

	m_ThreadLoader->Start_Thread();

	while (!m_isFinish) {
		Sleep(200);
		if (m_ThreadLoader->GetIsEnd()) {
			m_isFinish = true;
		}
	}


	return S_OK;
}

HRESULT CLoader::GameSceneKIM()
{

	CEmptyGameObject* pPlayer = static_cast<CEmptyGameObject*>(CEngine::GetInstance()->FindGameObjectWithName(SCENE_STATIC, "Player"));
	static_cast<CCollider*>(pPlayer->GetComponent("Com_Collider"))->SetPosition(_float3(0.f, 2.f, 0.f));

	m_ThreadLoader->EnqueueJob(ThreadTest, this, "../../Assets/Scenes/Dungeon1_kim.yaml", SCENE_KIM, 1);
	m_ThreadLoader->EnqueueJob(ThreadPrefab, this, "Prototype_GameObecjt_Wolf", "O_Wolf", 1);

	m_ThreadLoader->Start_Thread();
	
	while (!m_isFinish) {
		if (m_ThreadLoader->GetIsEnd())
			m_isFinish = true;
	}

	return S_OK;
}

HRESULT CLoader::GameSceneLEE()
{
	CEmptyGameObject* pPlayer = static_cast<CEmptyGameObject*>(CEngine::GetInstance()->FindGameObjectWithName(SCENE_STATIC, "Player"));
	static_cast<CCollider*>(pPlayer->GetComponent("Com_Collider"))->SetPosition(_float3(0.f, 2.f, 0.f));

	(m_ThreadLoader->EnqueueJob(ThreadTest, this, "../../Assets/Scenes/TestRoomLee.yaml", SCENE_LEE, 0));

	m_ThreadLoader->Start_Thread();
	
	while (!m_isFinish) {
		if (m_ThreadLoader->GetIsEnd())
			m_isFinish = true;
	}

	return S_OK;
}

HRESULT CLoader::GameSceneJUN()
{
	CEmptyGameObject* pPlayer = static_cast<CEmptyGameObject*>(CEngine::GetInstance()->FindGameObjectWithName(SCENE_STATIC, "Player"));
	static_cast<CCollider*>(pPlayer->GetComponent("Com_Collider"))->SetPosition(_float3(1.f, 0.5f, -4.f));

	//(m_ThreadLoader->EnqueueJob(ThreadTest, this, "../../Assets/Scenes/Dungeon1_JunT.yaml", SCENE_JUNG, 1));
	//(m_ThreadLoader->EnqueueJob(ThreadTest, this, "../../Assets/Scenes/Boss_Flogas.yaml", SCENE_JUNG, 0));
	(m_ThreadLoader->EnqueueJob(ThreadTest, this, "../../Assets/Scenes/Dungeon1_seo.yaml", SCENE_JUNG, 1));
	//m_ThreadLoader->EnqueueJob(ThreadTest, this, "../../Assets/Scenes/TestRoom_Effect_Jun.yaml", SCENE_JUNG, 3);
	//m_ThreadLoader->EnqueueJob(ThreadTest, this, "../../Assets/Scenes/Boss_Ursa.yaml", SCENE_JUNG, 0);
	//m_ThreadLoader->EnqueueJob(ThreadTest, this, "../../Assets/Scenes/UrsaDungeon.yaml", SCENE_JUNG, 1);
	
	//if (FAILED(GameFlogasLoader()))
	//	MSG_BOX("Failed To Create Flogas Effect");
	
	if (FAILED(GameUrsaLoader()))
		MSG_BOX("Failed To Create Ursa Effect");
	
	m_ThreadLoader->Start_Thread();
	
	while (!m_isFinish) {
		if (m_ThreadLoader->GetIsEnd())
			m_isFinish = true;
	}

	return S_OK;
}

HRESULT CLoader::GameSceneSEO()
{
	CEmptyGameObject* pPlayer = static_cast<CEmptyGameObject*>(CEngine::GetInstance()->FindGameObjectWithName(SCENE_STATIC, "Player"));
	static_cast<CCollider*>(pPlayer->GetComponent("Com_Collider"))->SetPosition(_float3(0.f, 2.f, 0.f));

	//m_ThreadLoader->EnqueueJob(ThreadTest, this, "../../Assets/Scenes/Flogas_Dungeon.yaml", SCENE_SEO, 0);
	//m_ThreadLoader->EnqueueJob(ThreadTest, this, "../../Assets/Scenes/Boss_Flogas.yaml", SCENE_SEO, 1);

		//(m_ThreadLoader->EnqueueJob(ThreadPrefab, this, "Prototype_GameObecjt_Wolf", "O_Wolf", 1));
		(m_ThreadLoader->EnqueueJob(ThreadTest, this, "../../Assets/Scenes/UrsaDungeon.yaml", SCENE_SEO, 0));
		(m_ThreadLoader->EnqueueJob(ThreadTest, this, "../../Assets/Scenes/Boss_Ursa.yaml", SCENE_SEO, 1));

		//if (FAILED(GameFlogasLoader()))
		//	MSG_BOX("Failed To Create Flogas Effect");

	//GameSceneStage02();

	if (FAILED(GameUrsaLoader()))
		MSG_BOX("Failed To Create Ursa Effect");

	m_ThreadLoader->Start_Thread();


	while (!m_isFinish) {
		if (m_ThreadLoader->GetIsEnd())
			m_isFinish = true;
	}

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
	SafeRelease(m_ThreadLoader);

	WaitForSingleObject(m_hThread, INFINITE);

	DeleteCriticalSection(&m_CS);

	DeleteObject(m_hThread);

}
