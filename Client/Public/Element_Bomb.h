#pragma once
#include "BasicEffect.h"

BEGIN(Client)

class CElement_Bomb : public CBasicEffect
{
private:
	explicit CElement_Bomb();
	virtual ~CElement_Bomb() = default;
private:
	virtual HRESULT Initialize(CEmptyEffect* pThis, CGameObject* pTargat, class CEl_Flogas& pObj);
	virtual void Update(_double dDeltaTime) override;
	virtual void LateUpdate(_double dDeltaTime) override;

public:
	_bool Get_Release() { return m_bRelease; }

private:
	CEmptyEffect*	m_pThis		   = nullptr;
	CTransform*		m_pEffectTrans = nullptr;
	class CEl_Flogas* m_pElement = nullptr;
private:
	_double			m_Duration = 3.0;
	_double			m_DurationDelta = 0.0;
	_float			m_fScale = 1.f;
	_bool			m_bRelease = false;
public:
	static CBasicEffect* Create(CEmptyEffect* pThis, CGameObject* pTargat, class CEl_Flogas& pObj);
	virtual void Free();
};

END