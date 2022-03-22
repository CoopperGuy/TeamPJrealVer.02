#pragma once
#include "IScriptObject.h"
#include "Engine.h"
BEGIN(Client)

class CTargetOn :
	public IScriptObject
{
private:
	enum TARGETS{ TARGETS_UP, TARGETS_DOWN, TARGETS_LEFT, TARGETS_RIGHT, TARGETS_END};
private:
	explicit CTargetOn();
	virtual ~CTargetOn() = default;
public:
	virtual HRESULT Initailze(CGameObject* player,CGameObject * pArg);
	virtual void Update(_double deltaTime) override;
	virtual void LateUpdate(_double deltaTime) override;
	virtual void Render();
public:
	void ReleaseThisUI();
private:
	CEmptyGameObject*		m_pPlayer = nullptr;
	CEmptyGameObject*		m_pTarget = nullptr;
	CEmptyUI*				m_pThisUI = nullptr;
private:
	CRectTransform* m_pTargetTrans = nullptr;
private:
	vector<CEmptyUI*>	m_Child;

private:
	mutable	_float	m_fAimOffset = 15.f;
public:
	static CTargetOn* Create(CGameObject* player=nullptr ,CGameObject*	pTarget = nullptr);
	virtual void Free() override;
};

END