#include "EnginePCH.h"
#include "../Public/ScriptObjectManager.h"


IMPLEMENT_SINGLETON(CScriptObjectManager)

CScriptObjectManager::CScriptObjectManager()
{
}

HRESULT CScriptObjectManager::ReserveManager(_uint iNumScenes)
{
	m_ScriptObjects = new list<IScriptObject*>[iNumScenes];
	m_iNumScenes = iNumScenes;

	return S_OK;
}

void CScriptObjectManager::Update(_double deltaTime)
{
	for (_uint i = 0; i < m_iNumScenes; i++) {
		auto iter = m_ScriptObjects[i].begin();
		for (; iter != m_ScriptObjects[i].end();) {
			if ((*iter)->IsDead())
			{
				SafeRelease(*iter);
				iter = m_ScriptObjects[i].erase(iter);
			}
			else {
				iter++;
			}
		}
	}

	for (_uint i = 0; i < m_iNumScenes; i++) {
		for (auto& obj : m_ScriptObjects[i])
			obj->Update(deltaTime);
	}
}

void CScriptObjectManager::LateUpdate(_double deltaTime)
{
	for (_uint i = 0; i < m_iNumScenes; i++) {
		for (auto& obj : m_ScriptObjects[i])
			obj->LateUpdate(deltaTime);
	}

}


void CScriptObjectManager::ClearScriptObject(_uint iSceneNum)
{
	for (auto& obj : m_ScriptObjects[iSceneNum])
		SafeRelease(obj);
	m_ScriptObjects[iSceneNum].clear();
}


IScriptObject * CScriptObjectManager::AddObject(IScriptObject * pObj, _uint iNumScenes)
{
	if (pObj)
	{
		m_ScriptObjects[iNumScenes].emplace_back(pObj);
		return pObj;
	}
	return nullptr;
}

void CScriptObjectManager::Free()
{
	for (_uint i = 0; i < m_iNumScenes; i++) {
		for (auto& obj : m_ScriptObjects[i]) {
			SafeRelease(obj);
			//cout << "script clear " << obj << "\n";
		}
		m_ScriptObjects[i].clear();
	}
	SafeDeleteArray(m_ScriptObjects);
}
