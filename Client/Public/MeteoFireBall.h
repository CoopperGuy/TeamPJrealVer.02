#pragma once
#include "IScriptObject.h"
#include "Engine.h"
BEGIN(Client)
class CMeteoFireBall :
	public IScriptObject
{
public:
	CMeteoFireBall();
	virtual ~CMeteoFireBall() = default;

public:
	virtual HRESULT Initailze(CGameObject * pArg, _vector pos);
	virtual void Update(_double deltaTime) override;
	virtual void LateUpdate(_double deltaTime) override;

public:
	void LookAt(_fvector vTargetPos);

public:
	_fmatrix Remove_ScaleRotation(_fmatrix TransformMatrix);

private:
	CGameObject*	m_pEAFireBall = nullptr;
	CEmptyGameObject*	m_pPlayer = nullptr;
	CTransform* m_pTargetTransform = nullptr;
	CTransform*			m_pTransform = nullptr;

	class CEffectMeteoFireBall* m_pMeteo = nullptr;
	class CEffectSmokeBall* m_pEffFireball = nullptr;


	_bool makeFB = false;


	_double makedt = 0;

	_float posy = 0.f;

	_vector				mypos = {};
	_vector playerpos = {};


	_vector m_arrivePos = {};

private:
	CBasicCollider*	m_pOBB = nullptr;
	CStat*		m_pStat = nullptr;

public:
	static CMeteoFireBall*	Create(CGameObject*	pTarget, _vector pos);
	virtual void Free() override;
};

END