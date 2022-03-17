#pragma once
#include "IScriptObject.h"
#include "Engine.h"
BEGIN(Client)
class CSpawn :
	public IScriptObject
{
private:
	explicit CSpawn();
	virtual ~CSpawn() = default;
public:
	virtual HRESULT Initailze(CGameObject * pArg);
	virtual void Update(_double deltaTime) override;
	virtual void LateUpdate(_double deltaTime) override;
	virtual void Render();
public:
	void SpawnObjects();
public:
	void SetCreateCnt(_int cnt) { m_iCreateCnt = cnt; }
	void SetIsSpawned(_bool _tf) { m_bisSpawned = _tf; }
public:
	_int GetCreateCnt() { return m_iCreateCnt; }
	_bool GetIsSpawned() { return m_bisSpawned; }
private:
	CEmptyGameObject*	m_pThis = nullptr;
	CSpawner*			m_pSpawn = nullptr;
	CTransform*			m_pSpawnPos = nullptr;
private:
	_int	m_iCreateCnt = 1;
	_double m_SpawnDelta = 0;
	_double m_SpawnTime = 0;
private:
	_bool	m_bisSpawned = false;
private:
	IScriptObject*	m_iSpawnScript = nullptr;
public:
	static CSpawn* Create(CGameObject*	pTarget = nullptr);
	virtual void Free() override;
};

END