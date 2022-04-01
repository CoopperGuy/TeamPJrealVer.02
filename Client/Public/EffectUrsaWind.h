#pragma once
#include "BasicEffect.h"

BEGIN(Client)

class CEffectUrsaWind :
	public CBasicEffect
{
private:
	explicit CEffectUrsaWind();
	virtual ~CEffectUrsaWind() = default;
private:
	virtual HRESULT Initialize(void * pArg, _vector pos);
	virtual void Update(_double deltaTime) override;
	virtual void LateUpdate(_double deltaTime) override;

public:
	void SetPos(_vector pos);
private:
	CEmptyEffect*	m_pThisEffect = nullptr;
	CTransform*		m_pEffectTrans = nullptr;
	CGameObject*  m_pUrsa = nullptr;
private:
	CTransform*		m_pPlayerTrans = nullptr;
private:
	_double			m_Duration = 0.5f;
	_double			m_DurationDelta = 0;
public:
	static CEffectUrsaWind* Create(void * pArg, _vector pos);
	virtual void Free();
};

END