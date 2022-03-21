#pragma once

#include "Engine.h"
#include "BasicEffect.h"

BEGIN(Client)
class CObb;
class CEffectEAFire final :
	public CBasicEffect
{
private:
	explicit CEffectEAFire();
	virtual ~CEffectEAFire() = default;

public:
	virtual HRESULT Initialize(void* pArg);
	virtual void Update(_double deltaTime);
	virtual void LateUpdate(_double deltaTime);
	virtual void Render();
public:
	static CEffectEAFire* Create(void*	pTarget);
	virtual void Free() override;

public:
	void Set_Pos(_vector pos) { m_pTransform->SetState(CTransform::STATE_POSITION,pos);}
private:
	_double m_dMakeFB = 0;




};

END