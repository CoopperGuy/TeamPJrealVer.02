#pragma once
#include "IScriptObject.h"
#include "Engine.h"
BEGIN(Client)
class CUrsaDunDoor :
	public IScriptObject
{
public:
	CUrsaDunDoor();
	virtual ~CUrsaDunDoor() = default;

public:
	virtual HRESULT Initailze(CGameObject * pArg);
	virtual void Update(_double deltaTime) override;
	virtual void LateUpdate(_double deltaTime) override;

private:
	CGameObject*	m_pUrsaDunDoor = nullptr;
	CGameObject*	m_pPlayer = nullptr;
	CGameObject*	m_pUrsa = nullptr;
	
	CTransform*			m_pTransform = nullptr;

	class CUrsa*	m_Ursa = nullptr;
	class CStat* UrsaStat = nullptr;

	list<class CWolf*> m_pWolf = {};

	_bool				m_bOpenDoor = false;
	_bool				m_bCloseDoor = false;

	_vector pUrsaDunDoor = {};

	_float MaxHight = 1.6f;
	_float MinHight = 0.f;
	_float doorY;

	_double closedoordt = 0;

	_uint make = 0;

	_bool WolfSet = false;


public:
	static CUrsaDunDoor*	Create(CGameObject*	pTarget = nullptr);
	virtual void Free() override;
};

END