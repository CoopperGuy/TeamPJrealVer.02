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

void ThreadPrefab(CLoader*	loader, string _prototypeTag, string _name, _int bit) {
	if (FAILED(CEngine::GetInstance()->CreatePrefab(_prototypeTag, _name))) {
		wstring path;
		path.assign(_name.begin(), _name.end());
		wstring errMsg = L"Failed To Create Prefab" + path;
		MessageBox(NULL, errMsg.c_str(), L"System Message", MB_OK);
	}
	loader->SetCompleteBit(bit);
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
	std::thread t0(ThreadPrefab, this, "Prototype_Effect_BossFly", "E_BossFly" , 0);
	t0.detach();

	std::thread t1(ThreadPrefab, this, "Prototype_Effect_BossFlyLaser", "E_Laser", 1);
	t1.detach();

	std::thread t2(ThreadPrefab, this, "Prototype_Effect_Dust", "E_DustEffect", 2);
	t2.detach();

	std::thread t3(ThreadPrefab, this, "Prototype_Effect_SwordRing", "E_SwordRing", 3);
	t3.detach();

	std::thread t4(ThreadPrefab, this, "Prototype_Effect_Blackhole", "E_Blackhole", 4);
	t4.detach();

	std::thread t5(ThreadPrefab, this, "Prototype_Effect_Grab", "E_Grab", 5);
	t5.detach();

	std::thread t6(ThreadPrefab, this, "Prototype_Effect_Pajang", "E_Pajangs", 6);
	t6.detach();

	std::thread t7(ThreadPrefab, this, "Prototype_Effect_FireSlash", "E_FireSlash", 7);
	t7.detach();

	std::thread t8(ThreadPrefab, this, "Prototype_Effect_MeteoTrail", "E_MeteoTrail", 8);
	t8.detach();

	std::thread t9(ThreadPrefab, this, "Prototype_Effect_MeteoFire", "E_MeteoFire", 9);
	t9.detach();

	std::thread t10(ThreadPrefab, this, "Prototype_Effect_MeteoDropArea", "E_MeteoDropArea", 10);
	t10.join();

	std::thread t11(ThreadPrefab, this, "Prototype_Effect_MeteoDropAf", "E_MeteoDropAfter",11);
	t11.detach();

	std::thread t12(ThreadPrefab, this, "Prototype_Effect_Meteo", "E_Meteo", 12);
	t12.detach();

	std::thread t13(ThreadPrefab, this, "Prototype_Effect_MeteoEnd", "E_MeteoEnd", 13);
	t13.detach();

	std::thread t14(ThreadPrefab, this, "Prototype_GameObecjt_MeteoOBB", "O_MeteoOBB", 14);
	t14.detach();

	std::thread t15(ThreadPrefab, this, "Prototype_Effect_Smoke", "E_Smoke", 15);
	t15.detach();

	std::thread t16(ThreadPrefab, this, "Prototype_Effect_Fire_explosion", "E_MeteoExplosion", 16);
	t16.detach();

	std::thread t17(ThreadPrefab, this, "Prototype_Effect_FireBoob", "E_FireBoob", 17);
	t17.detach();

	std::thread t18(ThreadPrefab, this, "Prototype_Effect_Fire", "E_EAFire", 18);
	t18.detach();

	std::thread t19(ThreadPrefab, this, "Prototype_Effect_FireBall", "E_EAFireBall", 19);
	t19.detach();

	std::thread t20(ThreadPrefab, this, "Prototype_GameObecjt_WaterEA", "O_WaterEA", 20);
	t20.join();

	std::thread t21(ThreadPrefab, this, "Prototype_GameObecjt_EAFire", "O_EAFire", 21);
	t21.join();

	_bool isNotFinish = true;
	while (isNotFinish) {
		for (int i = 0; i < 22; i++) {
			if ((m_iCompleteBit & (1 << i))) {
				isNotFinish = false;
			}
			else {
				isNotFinish = true;
				break;
			}
		}
	}
	
	m_iCompleteBit = 0;

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

	//if (FAILED(CEngine::GetInstance()->CreatePrefab("Prototype_Effect_FireSlash", "E_FireSlash")))
	//	MSG_BOX("Failed To Create FireSlash Prefab");

	//if (FAILED(CEngine::GetInstance()->CreatePrefab("Prototype_Effect_MeteoTrail", "E_MeteoTrail")))
	//	MSG_BOX("Failed To Create E_MeteoTrail Prefab");

	//if (FAILED(CEngine::GetInstance()->CreatePrefab("Prototype_Effect_MeteoFire", "E_MeteoFire")))
	//	MSG_BOX("Failed To Create E_MeteoFire Prefab");

	//if (FAILED(CEngine::GetInstance()->CreatePrefab("Prototype_Effect_MeteoDropArea", "E_MeteoDropArea")))
	//	MSG_BOX("Failed To Create E_MeteoDropArea Prefab");

	//if (FAILED(CEngine::GetInstance()->CreatePrefab("Prototype_Effect_MeteoDropAf", "E_MeteoDropAfter")))
	//	MSG_BOX("Failed To Create E_MeteoDropAfter Prefab");

	//if (FAILED(CEngine::GetInstance()->CreatePrefab("Prototype_Effect_Meteo", "E_Meteo")))
	//	MSG_BOX("Failed To Create E_Meteo Prefab");

	//if (FAILED(CEngine::GetInstance()->CreatePrefab("Prototype_Effect_MeteoEnd", "E_MeteoEnd")))
	//	MSG_BOX("Failed To Create E_MeteoEnd Prefab");

	//if (FAILED(CEngine::GetInstance()->CreatePrefab("Prototype_GameObecjt_MeteoOBB", "O_MeteoOBB")))
	//	MSG_BOX("Failed To Create E_MeteoEnd Prefab");

	//if (FAILED(CEngine::GetInstance()->CreatePrefab("Prototype_Effect_Smoke", "E_Smoke")))
	//	MSG_BOX("Failed To Create Smoke Prefab");

	//if (FAILED(CEngine::GetInstance()->CreatePrefab("Prototype_Effect_Fire_explosion", "E_MeteoExplosion")))
	//	MSG_BOX("Failed To Create E_Fire_explosion Prefab");


	//if (FAILED(CEngine::GetInstance()->CreatePrefab("Prototype_Effect_FireBoob", "E_FireBoob")))
	//	MSG_BOX("Failed To Create E_FireBoob Prefab");

	//if (FAILED(CEngine::GetInstance()->CreatePrefab("Prototype_Effect_Fire", "E_EAFire")))
	//	MSG_BOX("Failed To Create E_FireBoob Prefab");

	//if (FAILED(CEngine::GetInstance()->CreatePrefab("Prototype_Effect_FireBall", "E_EAFireBall")))
	//	MSG_BOX("Failed To Create E_FireBoob Prefab");

	//if (FAILED(CEngine::GetInstance()->CreatePrefab("Prototype_GameObecjt_WaterEA", "O_WaterEA")))
	//	MSG_BOX("Failed To Create E_FireBoob Prefab");

	//if (FAILED(CEngine::GetInstance()->CreatePrefab("Prototype_GameObecjt_EAFire", "O_EAFire")))
	//	MSG_BOX("Failed To Create E_FireBoob Prefab");



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
		m_pLoadingGauge->AddPercentage(0.1f);
	CEngine::GetInstance()->DeserializeScene("../../Assets/Scenes/Equip.yaml", SCENE_STATIC);
	while (m_pLoadingGauge->GetPercentage() < 80.f)
		m_pLoadingGauge->AddPercentage(0.1f);

	CEngine::GetInstance()->DeserializePrefab();

	while (m_pLoadingGauge->GetPercentage() < 90.f)
		m_pLoadingGauge->AddPercentage(0.1f);
	
	/*std::thread tP0(ThreadPrefab, this, "Prototype_GameObject_OBBs", "O_OBBs", 0);
	tP0.detach();
	std::thread tP1(ThreadPrefab, this, "Prototype_GameObject_DMGFont", "U_DamageVIBuffer", 1);
	tP1.detach();
	std::thread tP2(ThreadPrefab, this, "Prototype_Effect_shoulderEffect", "E_shoulderEffect", 2);
	tP2.detach();
	std::thread tP3(ThreadPrefab, this, "Prototype_Effect_ImpactGround", "E_ImpactGround2", 3);
	tP3.detach();
	std::thread tP4(ThreadPrefab, this, "Prototype_Effect_ImpactBeam", "E_ImpactBeam00", 4);
	tP4.detach();
	std::thread tP5(ThreadPrefab, this, "Prototype_Effect_ImpactShort", "E_ImpactShort", 5);
	tP5.detach();
*/



	if (FAILED(CEngine::GetInstance()->CreatePrefab("Prototype_GameObject_OBBs", "O_OBBs")))
		MSG_BOX("Failed To Create Pajang Prefab");

	if (FAILED(CEngine::GetInstance()->CreatePrefab("Prototype_GameObject_DMGFont", "U_DamageVIBuffer")))
		MSG_BOX("Failed To Create U_DamageVIBuffer Prefab");

	if (FAILED(CEngine::GetInstance()->CreatePrefab("Prototype_Effect_shoulderEffect", "E_shoulderEffect")))
		MSG_BOX("Failed To Create E_shoulderEffect Prefab");

	if (FAILED(CEngine::GetInstance()->CreatePrefab("Prototype_Effect_ImpactGround", "E_ImpactGround2")))
		MSG_BOX("Failed To Create E_ImpactGround2 Prefab");

	if (FAILED(CEngine::GetInstance()->CreatePrefab("Prototype_Effect_ImpactBeam", "E_ImpactBeam00")))
		MSG_BOX("Failed To Create E_ImpactBeam00 Prefab");

	if (FAILED(CEngine::GetInstance()->CreatePrefab("Prototype_Effect_ImpactShort", "E_ImpactShort")))
		MSG_BOX("Failed To Create E_ImpactShort Prefab");



	_bool threadFinish = false;
	while (!threadFinish) {
		for (_int i = 0; i < ThreadIndex; i++) {
			if (m_isThreadFinish[i] == false) {
				threadFinish = false;
				break;
			}
			else {
				threadFinish = true;
			}
		}
	}
	while (m_pLoadingGauge->GetPercentage() < 95.f)
		m_pLoadingGauge->AddPercentage(0.1f);
	/*_bool isNotFinish = true;
	while (isNotFinish) {
		for (int i = 0; i < 6; i++) {
			if ((m_iCompleteBit & (1 << i))) {
				isNotFinish = false;
			}
			else {
				isNotFinish = true;
				break;
			}
		}
	}*/

	m_iCompleteBit = 0;


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
	//CEngine::GetInstance()->DeserializeScene("../../Assets/Scenes/Flogas.yaml", SCENE_JUNG);
	//CEngine::GetInstance()->DeserializeScene("../../Assets/Scenes/TestRoom_Jun.yaml", SCENE_JUNG);
	//CEngine::GetInstance()->DeserializeScene("../../Assets/Scenes/Boss_Ursa.yaml", SCENE_JUNG);
	CEngine::GetInstance()->DeserializeScene("../../Assets/Scenes/Dungeon1_JunT.yaml", SCENE_JUNG);
	CEngine::GetInstance()->DeserializeScene("../../Assets/Scenes/TestRoom_Effect_Jun2.yaml", SCENE_JUNG);

	//if (FAILED(GameFlogasLoader()))
	//	MSG_BOX("Failed To Create Flogas Effect");

	
	m_isFinish = true;

	return S_OK;
}

HRESULT CLoader::GameSceneSEO()
{
	CEmptyGameObject* pPlayer = static_cast<CEmptyGameObject*>(CEngine::GetInstance()->FindGameObjectWithName(SCENE_STATIC, "Player"));
	static_cast<CCollider*>(pPlayer->GetComponent("Com_Collider"))->SetPosition(_float3(0.f, 0.f, 5.f));

	if (FAILED(CEngine::GetInstance()->CreatePrefab("Prototype_Effect_Blood", "E_Blood")))
		MSG_BOX("Failed To Create E_Blood Prefab");

	if (FAILED(CEngine::GetInstance()->CreatePrefab("Prototype_GameObecjt_Wolf", "O_Wolf")))
		MSG_BOX("Failed To Create O_Wolf Prefab");

	//if (FAILED(GameFlogasLoader()))
	//	MSG_BOX("Failed To Create Flogas Effect");

	//CEngine::GetInstance()->DeserializeScene("../../Assets/Scenes/UrsaDungeon_ex.yaml", SCENE_SEO);
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
