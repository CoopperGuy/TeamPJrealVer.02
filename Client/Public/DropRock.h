#pragma once
#include "IScriptObject.h"
#include "Engine.h"
BEGIN(Client)
class CDropRock :
	public IScriptObject
{
public:
	CDropRock();
	virtual ~CDropRock() = default;

public:
	virtual HRESULT Initailze(CGameObject * pArg, _vector pos);
	virtual void Update(_double deltaTime) override;
	virtual void LateUpdate(_double deltaTime) override;

private:
	CGameObject*		m_pGameObject = nullptr;
	CCollider*			m_pCollider = nullptr;
	CTransform*			m_pTransform = nullptr;

	_vector MyPos = {};


	_float PosX = 0.f;
	_float PosY = 0.f;
	_float PosZ = 0.f;
	_float StartPosY = 0.f;
	_float StartPosX = 0.f;

	_float Speed = 3.f;

	_float Time = 0.f;
public:
	static CDropRock*	Create(CGameObject*	pArg, _vector pos = {0.f,0.f,0.f});
	virtual void Free() override;
};

END