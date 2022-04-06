#pragma once
#include "BasicEffect.h"

BEGIN(Client)
class CObb;
class CShoulerAtk :
	public CBasicEffect
{
private:
	explicit CShoulerAtk();
	virtual ~CShoulerAtk() = default;
private:
	virtual HRESULT Initialize(CEmptyEffect* _pThisEffect, CGameObject* _pPlayer);
	virtual void Update(_double deltaTime) override;
	virtual void LateUpdate(_double deltaTime) override;
private:
	CEmptyEffect*	m_pThisEffect = nullptr;
	CTransform*		m_pEffectTrans = nullptr;
private:
	CTransform*		m_pPlayerTrans = nullptr;
private:
	CObb*			m_pObb = nullptr;
private:
	_double			m_Duration = 0.2;
	_double			m_DurationDelta = 0;
public:
	static CBasicEffect* Create(CEmptyEffect* _pThisEffect, CGameObject* _pPlayerEffect);
	virtual void Free();
};

END