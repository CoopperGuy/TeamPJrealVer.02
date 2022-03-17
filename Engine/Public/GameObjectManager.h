#pragma once

#include "Base.h"
#include "EmptyGameObject.h"
#include "EmptyUI.h"
#include "EmptyEffect.h"
/* 객체들을 개발자가 원하는 기준으로 나누어서 보관한다.  */

BEGIN(Engine)
class CLayer;
class CBasicCollider;
class CGameObjectManager final : public CBase
{
	DECLARE_SINGLETON(CGameObjectManager)
private:
	CGameObjectManager();
	virtual ~CGameObjectManager() = default;
	virtual void Free() override;

public:
	_int	Update(_double dDeltaTime);
	_int	LateUpdate(_double dDeltaTime);
	void	Clear(_uint iSceneIndex);

public:
	// allocate the size of list
	HRESULT ReserveManager(_uint iNumScenes);
	HRESULT	AddPrototype(string sPrototypeTag, CGameObject* pPrototype);
	CGameObject* AddGameObject(_uint iSceneIndex, string sPrototypeTag, string pLayerTag, void* pArg);
	list<class CGameObject*> GetGameObjectInLayer(_uint iSceneIndex, string pLayerTag);
	unordered_map<string, CLayer*>* GetLayers() { return m_pGameObjects; }
	void AddGameObjectWithUUID(uint64_t uuid, CGameObject* pObj, _uint iSceneIndex) { m_pUUIDObjects[iSceneIndex].insert({ uuid, pObj }); }
	void AddGameObjectWithName(string name, CGameObject* pObj, _uint iSceneIndex) { m_pNameObjects[iSceneIndex].insert({ name, pObj }); }

	void AddOBBCollsionList(ID Type, CGameObject* _pGameObject, CBasicCollider* _obb);

public: // For.Prefab
	CGameObject* AddGameObjectToPrefab(_uint iSceneIndex, string pPrototypeTag, string pLayerTag, void* pArg);
	CGameObject* ClonePrototype(string pFindPrototypeTag, string pPrototypeTag, void * pArg);

public:
	CGameObject*	FindPrototype(string pPrototypeTag);
	CLayer*			FindLayer(_uint iSceneIndex, string pLayerTag);
	CGameObject*	FindGameObjectWithUUID(_uint iSceneIndex, uint64_t uuid);
	CGameObject*	FindGameObjectWithName(_uint iSceneIndex, string name);
	void CollsionCheck();
	void ClearList();

public:
	void			AddPrefab(string name, YAML::Node node);
	CGameObject*	SpawnPrefab(string name);
	HRESULT			CreatePrefab(string pPrototypeTag, string name);
private:
	_uint		m_iNumScenes = 0;

private:
	unordered_map<string, CGameObject*>			m_pPrototypes;
	unordered_map<string, YAML::Node>			m_pPrefabs;
	unordered_map<string, CGameObject*>			m_pPrefabPrototype;

	typedef unordered_map<string, CLayer*>		GAMEOBJECTS;
	GAMEOBJECTS*								m_pGameObjects = nullptr;

	unordered_map<uint64_t, CGameObject*>*		m_pUUIDObjects;
	unordered_map<string, CGameObject*>*		m_pNameObjects;

	list<pair<CGameObject*,CBasicCollider*>>	m_pColliderList[(_int)ID::IDEND+1];
	CBasicCollider* m_pOBB = nullptr;
};

END