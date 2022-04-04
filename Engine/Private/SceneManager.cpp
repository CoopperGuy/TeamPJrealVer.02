#include "EnginePCH.h"
#include "..\Public\SceneManager.h"
#include	"CameraManager.h"
IMPLEMENT_SINGLETON(CSceneManager)

CSceneManager::CSceneManager()
{
}

HRESULT CSceneManager::SetUpCurrentScene(CScene * pCurrentScene, _uint sceneNum)
{

	if (nullptr == pCurrentScene)
		return E_FAIL;

	if (nullptr != m_pCurrentScene)
	{
		CameraManager::GetInstance()->SeceneChangeClear();
		m_pCurrentScene->Clear();
	}
	if (0 != SafeRelease(m_pCurrentScene))
		return E_FAIL;
	
	m_pCurrentScene = pCurrentScene;
	m_pCurrentScene->StartLoader();

	return S_OK;
}

_uint CSceneManager::UpdateScene(_double TimeDelta)
{
	if (nullptr == m_pCurrentScene)
		return -1;

	return m_pCurrentScene->Update(TimeDelta);
}

HRESULT CSceneManager::RenderScene()
{
	if (nullptr == m_pCurrentScene)
		return E_FAIL;

	return m_pCurrentScene->Render();
}

void CSceneManager::Free()
{
	SafeRelease(m_pCurrentScene);
}