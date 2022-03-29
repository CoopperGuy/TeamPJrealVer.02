#include "stdafx.h"
#include "..\public\Scene_Loading.h"

#include "Engine.h"
#include "Loader.h"
#include "Scene_GamePlay.h"
#include "Scene_Test.h"
#include "LoadingGauge.h"
#include "Scene_Stage01.h"
#include "Scene_Stage2.h"
#include "Scene_Stage3.h"

#include "Scene_Logo.h"
#include "Scene_Kim.h"
#include "Scene_LEE.h"
#include "Scene_JUN.h"
#include "Scene_SEO.h"

CScene_Loading::CScene_Loading(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext, _uint iLevelIndex)
	: CScene(pDevice, pDeviceContext, iLevelIndex)
{

}

HRESULT CScene_Loading::Initialize(SCENE eScene)
{
	__super::Initialize();	
	Sleep(500);
	CEngine::GetInstance()->SetCurSceneNumber(SCENE_LOADING);
	m_pEngine->DeserializeScene("../../Assets/Scenes/LoadingTest.yaml", m_pEngine->GetCurSceneNumber());

	m_eNextSCENE = eScene;

	return S_OK;
}

_uint CScene_Loading::Update(_double TimeDelta)
{
	__super::Update(TimeDelta);

	if (m_pLoader != nullptr) {

		_bool isFinish = m_pLoader->Get_Finish();
		if (isFinish == true)
		{
			CEngine*		pEngine = GET_INSTANCE(CEngine);

			SetWindowText(g_hWnd, TEXT("·Îµù ³¡!!!!!!!!!!!"));

			CScene*		pScene = nullptr;
			Client::SCENE	sceneTag;
			switch (m_eNextSCENE)
			{
			case SCENE_LOGO:
				CEngine::GetInstance()->SetCurSceneNumber(SCENE_LOGO);
				pScene = CScene_Logo::Create(m_pDevice, m_pDeviceContext, SCENE_LOGO);
				sceneTag = SCENE_LOGO;
				break;
			case SCENE_GAMEPLAY:
				CEngine::GetInstance()->SetCurSceneNumber(SCENE_GAMEPLAY);
				pScene = CScene_GamePlay::Create(m_pDevice, m_pDeviceContext, SCENE_GAMEPLAY);
				sceneTag = SCENE_GAMEPLAY;
				break;
			case SCENE_TEST:
				CEngine::GetInstance()->SetCurSceneNumber(SCENE_TEST);
				pScene = CScene_Test::Create(m_pDevice, m_pDeviceContext, SCENE_TEST);
				sceneTag = SCENE_TEST;
				break;
			case SCENE_STAGE1:
				CEngine::GetInstance()->SetCurSceneNumber(SCENE_STAGE1);
				pScene = CScene_Stage01::Create(m_pDevice, m_pDeviceContext, SCENE_STAGE1);
				sceneTag = SCENE_STAGE1;
				break;
			case SCENE_KIM:
				CEngine::GetInstance()->SetCurSceneNumber(SCENE_KIM);
				pScene = CScene_Kim::Create(m_pDevice, m_pDeviceContext, SCENE_KIM);
				sceneTag = SCENE_KIM;
				break;
			case SCENE_LEE:
				CEngine::GetInstance()->SetCurSceneNumber(SCENE_LEE);
				pScene = CScene_LEE::Create(m_pDevice, m_pDeviceContext, SCENE_LEE);
				sceneTag = SCENE_LEE;
				break;
			case SCENE_JUNG:
				CEngine::GetInstance()->SetCurSceneNumber(SCENE_JUNG);
				pScene = CScene_JUN::Create(m_pDevice, m_pDeviceContext, SCENE_JUNG);
				sceneTag = SCENE_JUNG;
				break;
			case SCENE_SEO:
				CEngine::GetInstance()->SetCurSceneNumber(SCENE_SEO);
				pScene = CScene_SEO::Create(m_pDevice, m_pDeviceContext, SCENE_SEO);
				sceneTag = SCENE_SEO;
				break;
			case SCENE_STAGE2:
				CEngine::GetInstance()->SetCurSceneNumber(SCENE_STAGE2);
				pScene = CScene_Stage02::Create(m_pDevice, m_pDeviceContext, SCENE_STAGE2);
				sceneTag = SCENE_STAGE2;
				break;
			case SCENE_STAGE3:
				CEngine::GetInstance()->SetCurSceneNumber(SCENE_STAGE3);
				pScene = CScene_Stage3::Create(m_pDevice, m_pDeviceContext, SCENE_STAGE3);
				sceneTag = SCENE_STAGE3;
				break;
				/*case SCENE_GAMEPLAY1:
				break;*/
			}
			if (FAILED(pEngine->SetUpCurrentScene(pScene, (_uint)SCENE_LOADING)))
				return E_FAIL;


			RELEASE_INSTANCE(CEngine);
		}
		else {
			m_pLoader->UpdateGauge(TimeDelta);
		}
	}

	return _uint();
}

HRESULT CScene_Loading::Render()
{
	__super::Render();

	return S_OK;
}

void CScene_Loading::StartLoader()
{
	m_pLoader = CLoader::Create(m_eNextSCENE);
}

CScene_Loading * CScene_Loading::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext, SCENE eNextScene, _uint iSceneIndex)
{
	CScene_Loading*		pInstance = new CScene_Loading(pDevice, pDeviceContext, iSceneIndex);

	if (FAILED(pInstance->Initialize(eNextScene)))
	{
		MSG_BOX("Failed to Creating CScene_Loading");
		SafeRelease(pInstance);
	}

	return pInstance;
}

void CScene_Loading::Free()
{
	__super::Free();

	SafeRelease(m_pLoader);
}
