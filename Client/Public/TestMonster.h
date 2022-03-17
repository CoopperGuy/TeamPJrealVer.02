#pragma once
#include "Enemy.h"
#include "Engine.h"
BEGIN(Client)

class CTestMonster :
	public CEnemy
{
public:
	explicit CTestMonster();
	explicit CTestMonster(CGameObject* pObj);
	virtual ~CTestMonster() = default;

public:
	HRESULT Initialize();
	virtual void Update(_double deltaTime);
	virtual void LateUpdate(_double dDeltaTime);
	virtual void Render();
private:
	CEmptyGameObject*		m_pThisObj = nullptr;
	CModel*					m_pThisModel = nullptr;
	CStat*					m_pThisStat = nullptr;
public:
	static CTestMonster* Create(CGameObject* pObj);
	virtual void Free() override;
};

END