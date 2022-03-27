#pragma once
#include "BasicEffect.h"

BEGIN(Client)

class CBomb_Center : public CBasicEffect
{
private:
	explicit CBomb_Center();
	virtual ~CBomb_Center() = default;
private:
	virtual HRESULT Initialize(CEmptyEffect* pThis, CTransform* pTarget, class CElement_Bomb& pBomb);
	virtual void Update(_double dDeltaTime) override;
	virtual void LateUpdate(_double dDeltaTime) override;
private:
	CEmptyEffect*	m_pThis		   = nullptr;
	CTransform*		m_pEffectTrans = nullptr;
	class CElement_Bomb* m_pBomb   = nullptr;
	list<CTransform*> m_Child;
private:
	_float3			m_vScale;
	_float			m_fScale = 1.f;
public:
	static CBasicEffect* Create(CEmptyEffect* pThis, CTransform* _pTarget, class CElement_Bomb& pBomb);
	virtual void Free();
};

END