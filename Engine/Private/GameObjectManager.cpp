#include "EnginePCH.h"
#include "..\public\GameObjectManager.h"
#include "Layer.h"
#include "GameObject.h"
#include "SceneSerializer.h"
#include "BasicCollider.h"

IMPLEMENT_SINGLETON(CGameObjectManager)

CGameObjectManager::CGameObjectManager()
{
}

HRESULT CGameObjectManager::ReserveManager(_uint iNumScenes)
{
	if (nullptr != m_pGameObjects)
		return E_FAIL;

	m_pGameObjects = new GAMEOBJECTS[iNumScenes];
	m_pUUIDObjects = new unordered_map<uint64_t, CGameObject*>[iNumScenes];
	m_pNameObjects = new unordered_map<string, CGameObject*>[iNumScenes];
	m_iNumScenes = iNumScenes;

	return S_OK;
}

_int CGameObjectManager::Update(_double dDeltaTime)
{
	_int		iProgress = 0;

	for (_uint i = 0; i < m_iNumScenes; ++i)
	{
		for (auto& Pair : m_pGameObjects[i])
		{
			iProgress = Pair.second->Update(dDeltaTime);
			if (0 > iProgress)
				return -1;
		}
	}

	CollsionCheck();

	return S_OK;
}

_int CGameObjectManager::LateUpdate(_double dDeltaTime)
{
	_int		iProgress = 0;

	for (_uint i = 0; i < m_iNumScenes; ++i)
	{
		for (auto& Pair : m_pGameObjects[i])
		{
			iProgress = Pair.second->LateUpdate(dDeltaTime);
			if (0 > iProgress)
				return -1;
		}
	}

	ClearList();

	return S_OK;
}

HRESULT CGameObjectManager::AddPrototype(string pPrototypeTag, CGameObject * pPrototype)
{
	if (nullptr == pPrototype ||
		nullptr != FindPrototype(pPrototypeTag))
		return E_FAIL;

	m_pPrototypes.emplace(pPrototypeTag, pPrototype);

	return S_OK;
}

CGameObject* CGameObjectManager::AddGameObjectToPrefab(_uint iSceneIndex, string pPrototypeTag, string pLayerTag, void * pArg)
{
	CGameObject*	pPrototype = FindPrototype(pPrototypeTag);
	if (nullptr == pPrototype)
		return nullptr;

	CGameObject*	pGameObject = pPrototype->PrefabClone(pArg); // PrefabClone
	if (nullptr == pGameObject)
		return nullptr;

	CLayer*		pLayer = FindLayer(iSceneIndex, pLayerTag);

	if (nullptr == pLayer)
	{
		CLayer*		pNewLayer = CLayer::Create();
		if (nullptr == pNewLayer)
			return nullptr;

		pNewLayer->AddGameObjectInLayer(pGameObject);

		m_pGameObjects[iSceneIndex].emplace(pLayerTag, pNewLayer);
	}
	else
		pLayer->AddGameObjectInLayer(pGameObject);

	// AddChildrenPrefab
	for (auto pChildPrototypeTag : pPrototype->GetChildrenPrototypeTag())
	{
		CGameObject*	pChildPrototype = FindPrototype(pChildPrototypeTag);
		if (nullptr == pChildPrototype)
			return nullptr;

		CGameObject*	pChildGameObject = pChildPrototype->ChildrenPrefabClone(pGameObject, pArg);
		if (nullptr == pChildGameObject)
			return nullptr;

		if (pLayer == nullptr)
			pLayer = FindLayer(iSceneIndex, pLayerTag);

		pLayer->AddGameObjectInLayer(pChildGameObject);
		pGameObject->AddChild2(pChildGameObject);
	}
	
	return pGameObject;
}

CGameObject * CGameObjectManager::ClonePrototype(string pFindPrototypeTag, string pPrototypeTag, void * pArg)
{
	CGameObject*	pPrototype = FindPrototype(pFindPrototypeTag);
	if (nullptr == pPrototype)
		return nullptr;

	CGameObject*	pGameObject = pPrototype->Clone(pArg);
	if (nullptr == pGameObject)
		return nullptr;

	m_pPrototypes.emplace(pPrototypeTag, pGameObject);

	return pGameObject;
}

CGameObject* CGameObjectManager::AddGameObject(_uint iSceneIndex, string pPrototypeTag, string pLayerTag, void * pArg)
{
	CGameObject*	pPrototype = FindPrototype(pPrototypeTag);
	if (nullptr == pPrototype)
		return nullptr;

	CGameObject*	pGameObject = pPrototype->Clone(pArg);
	if (nullptr == pGameObject)
		return nullptr;

	CLayer*		pLayer = FindLayer(iSceneIndex, pLayerTag);

	if (nullptr == pLayer)
	{
		CLayer*		pNewLayer = CLayer::Create();
		if (nullptr == pNewLayer)
			return nullptr;

		pNewLayer->AddGameObjectInLayer(pGameObject);

		m_pGameObjects[iSceneIndex].emplace(pLayerTag, pNewLayer);
	}
	else
		pLayer->AddGameObjectInLayer(pGameObject);

	return pGameObject;
}


list<class CGameObject*> CGameObjectManager::GetGameObjectInLayer(_uint iSceneIndex, string pLayerTag)
{
	CLayer* pLayer = FindLayer(iSceneIndex, pLayerTag);
	if (nullptr == pLayer)
	{
		//MSG_BOX("Failed to GetGameObjectInLayer");
		return list<class CGameObject*>();
	}

	return pLayer->GetGameObjectList();
}

CGameObject * CGameObjectManager::FindPrototype(string pPrototypeTag)
{
	auto iter = find_if(m_pPrototypes.begin(), m_pPrototypes.end(), STagFinder(pPrototypeTag));

	if (iter == m_pPrototypes.end())
		return nullptr;

	return iter->second;
}

CLayer * CGameObjectManager::FindLayer(_uint iSceneIndex, string pLayerTag)
{
	auto iter = find_if(m_pGameObjects[iSceneIndex].begin(), m_pGameObjects[iSceneIndex].end(), STagFinder(pLayerTag));

	if (iter == m_pGameObjects[iSceneIndex].end())
		return nullptr;

	return iter->second;
}

CGameObject * CGameObjectManager::FindGameObjectWithUUID(_uint iSceneIndex, uint64_t uuid)
{
	//auto find = m_pUUIDObjects[iSceneIndex].find(uuid);

	auto find = find_if(m_pUUIDObjects[iSceneIndex].begin(), m_pUUIDObjects[iSceneIndex].end(), [&](const pair<uint64_t, CGameObject*>& pair) {
		return(uuid == pair.first);
	}
	);


	if (find != m_pUUIDObjects[iSceneIndex].end())
		return find->second;

	return nullptr;
}

CGameObject * CGameObjectManager::FindGameObjectWithName(_uint iSceneIndex, string name)
{

	auto find = find_if(m_pNameObjects[iSceneIndex].begin(), m_pNameObjects[iSceneIndex].end(), STagFinder(name));

	if (find != m_pNameObjects[iSceneIndex].end())
		return find->second;

	return nullptr;
}

void CGameObjectManager::AddOBBCollsionList(ID Type, CGameObject * _pGameObject, CBasicCollider* _obb)
{
	if (nullptr == _pGameObject)
	{
		MSG_BOX("_pGameObject nullptr");
		return ;
	}

	m_pColliderList[(_int)Type].emplace_back(make_pair(_pGameObject, _obb));
	SafeAddRef(_pGameObject);
	SafeAddRef(_obb);
}


void CGameObjectManager::CollsionCheck()
{
	CGameObject * m_pGameObject = CEngine::GetInstance()->FindGameObjectWithName(0, "Player");

	//일단 여기는 플레이어 ATT 상태에서만 들어옴 
	if (m_pColliderList[(_int)ID::PLAYER].empty())
		return;

	CBasicCollider* m_pOBB = static_cast<CBasicCollider*>(m_pGameObject->GetComponent("Com_OBB"));

	m_pOBB->CollisionWeaponeToTarget(m_pColliderList[(_int)ID::WEAPONE], m_pColliderList[(_int)ID::MONSTER]);
	m_pOBB->Collision_MonsterWeaponToPlayer(m_pColliderList[(_int)ID::MONSTER_ATK], m_pColliderList[(_int)ID::PLAYER]);
	m_pOBB->Collision_MonsterWeaponToPlayer(m_pColliderList[(_int)ID::MONSTER_EFFECT], m_pColliderList[(_int)ID::PLAYER]);
}

void CGameObjectManager::ClearList()
{
	for (int i = 0; i < (_int)ID::IDEND; ++i)
	{
		for (auto& pair : m_pColliderList[i])
		{
			SafeRelease(pair.first);
			SafeRelease(pair.second);
		}
		m_pColliderList[i].clear();
	}
}


void CGameObjectManager::AddPrefab(string name, YAML::Node node)
{
	m_pPrefabs.emplace(name, node);
}

CGameObject * CGameObjectManager::SpawnPrefab(string name)
{
	CSceneSerializer serializer;
	if (m_pPrefabs[name]) {
		/*auto &iter = m_pPrefabs.find(name);
		if (m_pPrefabs.end() == iter) {

		}
		else {

		}*/

		return serializer.SpawnPrefab(m_pPrefabs[name]);
	}
	return nullptr;
}

HRESULT CGameObjectManager::CreatePrefab(string pPrototypeTag, string name)
{
	CSceneSerializer serializer;
	if (m_pPrefabs[name])
	{
		serializer.CreatePrefab(pPrototypeTag, m_pPrefabs[name]);
		return S_OK;
	}

	return E_FAIL;
}

void CGameObjectManager::Free()
{
	for (_uint i = 0; i < m_iNumScenes; ++i)
	{
		for (auto& Pair : m_pGameObjects[i]) {
			SafeRelease(Pair.second);
		}

		m_pGameObjects[i].clear();
	}

	SafeDeleteArray(m_pGameObjects);
	for (_uint i = 0; i < m_iNumScenes; ++i)
	{

		m_pUUIDObjects[i].clear();
	}

	SafeDeleteArray(m_pUUIDObjects);
	for (_uint i = 0; i < m_iNumScenes; ++i)
	{

		m_pNameObjects[i].clear();
	}

	SafeDeleteArray(m_pNameObjects);

	for (auto& Pair : m_pPrototypes) {
		SafeRelease(Pair.second);
		cout << "m_pPrototypes" << Pair.second << "\n";
	}
	for (int i = 0; i < (_int)ID::IDEND; ++i)
	{
		for (auto& pair : m_pColliderList[i]) {
			SafeRelease(pair.first);
			SafeRelease(pair.second);
		}
		m_pColliderList[i].clear();
	}

	m_pPrototypes.clear();
}



void CGameObjectManager::Clear(_uint iSceneIndex)
{
	if (iSceneIndex >= m_iNumScenes)
		return;

	for (auto& iter : m_pGameObjects[iSceneIndex]) {
		if (iter.second != nullptr)
			SafeRelease(iter.second);
		cout << iter.first << " < deleted Layer Address is " << iter.second << "\n";

	}

	m_pGameObjects[iSceneIndex].clear();
	m_pNameObjects[iSceneIndex].clear();
	m_pUUIDObjects[iSceneIndex].clear();

}
