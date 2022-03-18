#pragma once
#include "IScriptObject.h"
#include "Engine.h"
BEGIN(Client)
class CEAFireBall :
	public IScriptObject
{
public:
	CEAFireBall();
	virtual ~CEAFireBall() = default;

public:
	virtual HRESULT Initailze(CGameObject * pArg, _matrix pos);
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

	class CEffectFireBall* m_pEffFireball = nullptr;

	_vector				m_pos = {};

	_bool makeFB = false;

	_double makedt = 0;


private:
	CBasicCollider*	m_pOBB = nullptr;
	CStat*		m_pStat = nullptr;

public:
	static CEAFireBall*	Create(CGameObject*	pTarget, _matrix pos);
	virtual void Free() override;
};

END