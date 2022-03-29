#pragma once
#include "BasicEffect.h"

BEGIN(Client)
class CObb;
class CImpactBeam :
	public CBasicEffect
{
private:
	explicit CImpactBeam();
	virtual ~CImpactBeam() = default;
private:
	virtual HRESULT Initialize(CEmptyEffect* _pThisEffect, CGameObject* _pPlayer);
	virtual void Update(_double deltaTime) override;
	virtual void LateUpdate(_double deltaTime) override;
private:
	CEmptyEffect*	m_pThisEffect = nullptr;
	CTransform*		m_pEffectTrans = nullptr;
private:
	CObb*			m_pObb = nullptr;
private:
	vector<CTransform*>	m_Child;
private:
	_double			m_Duration = 0.5;
	_double			m_DurationDelta = 0;
public:
	static CImpactBeam* Create(CEmptyEffect* _pThisEffect, CGameObject* _pPlayerEffect);
	virtual void Free();
};

END