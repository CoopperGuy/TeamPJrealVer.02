#pragma once
#include "BasicEffect.h"

BEGIN(Client)

class CEffectFlash : public CBasicEffect
{
private:
	explicit CEffectFlash();
	virtual ~CEffectFlash() = default;

private:
	virtual HRESULT Initialize(CEmptyEffect* pThis, CGameObject* pTarget, class CEl_Flogas& pEl);
	virtual void Update(_double dDeltaTime) override;
	virtual void LateUpdate(_double dDeltaTime) override;

private:
	CEmptyEffect*	m_pThis		   = nullptr;
	CTransform*		m_pEffectTrans = nullptr;
	class CEl_Flogas* m_pEl		   = nullptr;

private:
	_float			m_DurationDelta = 0;
	_float			m_fScale = 0.f;

public:
	static CBasicEffect* Create(CEmptyEffect* pThis, CGameObject* _pTarget, class CEl_Flogas& pEl);
	virtual void Free();
};

END