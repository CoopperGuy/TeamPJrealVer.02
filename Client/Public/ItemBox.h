#pragma once
#include "IScriptObject.h"
#include "Engine.h"
BEGIN(Client)
class CItemBox :
	public IScriptObject
{
public:
	CItemBox();
	virtual ~CItemBox() = default;

public:
	virtual HRESULT Initailze(CGameObject * pArg, _vector pos);
	virtual void Update(_double deltaTime) override;
	virtual void LateUpdate(_double deltaTime) override;

private:
	CEmptyGameObject*	m_pItemBox = nullptr;
	CEmptyGameObject*	m_pPlayer = nullptr;
	CEmptyGameObject*	m_pAlretUI = nullptr;
	CTransform*			m_pTransform = nullptr;
	CModel*				m_pModel = nullptr;
	class CItemDropEffect*    m_pItemDropEff = nullptr;

	_bool				m_bGetItem = false;
	
	_vector				m_vMyPos = {};

	// Dissolve
	_bool	m_bDissolve = false;
	_float	m_fDissolveAcc = 0.f;
	_float  m_fDissolveDelay = 1.5f;

	_double deaddt = 0.0;


public:
	static CItemBox*	Create(CGameObject*	pTarget = nullptr, _vector pos = {0.f,0.f,0.f});
	virtual void Free() override;
};

END