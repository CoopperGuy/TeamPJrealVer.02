#pragma once

#include "Engine.h"
#include "BasicEffect.h"

BEGIN(Client)

class CEffectUrsaDust final :
	public CBasicEffect
{
private:
	explicit CEffectUrsaDust();
	virtual ~CEffectUrsaDust() = default;
public:
	virtual HRESULT Initialize(void* pArg);
	virtual HRESULT Initialize(void * pArg, _vector pos);
	virtual void Update(_double deltaTime) ;
	virtual void LateUpdate(_double deltaTime) ;
	virtual void Render();

public:
	static CEffectUrsaDust* Create(void*	pTarget);
	static CEffectUrsaDust * Create(void * pArg, _vector pos);
	virtual void Free() override;

private:
	CComponent* m_pComponent = nullptr;

	_double m_dDeadTime = 0;

	_float3 Startscail = {};
	_float ScailX = 0.f;
	_float ScailY = 0.f;
};

END