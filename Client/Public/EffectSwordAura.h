#pragma once

#include "Engine.h"
#include "BasicEffect.h"

BEGIN(Client)

class CEffectSwordAura final :
	public CBasicEffect
{
private:
	explicit CEffectSwordAura();
	virtual ~CEffectSwordAura() = default;
public:
	virtual HRESULT Initialize(void* pArg, CTransform* pTransform, _float fAngle);
	virtual void Update(_double deltaTime) ;
	virtual void LateUpdate(_double deltaTime) ;
	virtual void Render();

public:
	static CEffectSwordAura* Create(void* pTarget, CTransform* pTransform, _float fAngle);
	virtual void Free() override;

private:
	class CObb* m_pObb = nullptr;

	_double m_dDeadTime = 0;

	_float3 Startscail = {};
	_float ScailX = 0.f;
	_float ScailY = 0.f;

	_float PosX = 0.f;
	_float PosY = 0.f;

	int RandomNum = 0;
};

END