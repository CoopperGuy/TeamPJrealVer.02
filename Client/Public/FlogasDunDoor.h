#pragma once
#include "IScriptObject.h"
#include "Engine.h"
BEGIN(Client)
class CFlogasDunDoor :
	public IScriptObject
{
public:
	CFlogasDunDoor();
	virtual ~CFlogasDunDoor() = default;

public:
	virtual HRESULT Initailze(CGameObject * pArg);
	virtual void Update(_double deltaTime) override;
	virtual void LateUpdate(_double deltaTime) override;

private:
	CEmptyGameObject*	m_pFlogasDunDoor = nullptr;
	CEmptyGameObject*	m_pPlayer = nullptr;
	CEmptyGameObject*	m_pAlretUI = nullptr;
	CCollider*			m_pCollider = nullptr;
	CTransform*			m_pTransform = nullptr;

	_bool				m_bOpenDoor = false;

	_vector pFlogasDunDoor = {};

	_float MaxHight = 2.f;
	_float MinHight = 0.f;
	_float doorY;


public:
	static CFlogasDunDoor*	Create(CGameObject*	pTarget = nullptr);
	virtual void Free() override;
};

END