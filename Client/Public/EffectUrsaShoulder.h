#pragma once

#include "Engine.h"
#include "BasicEffect.h"

BEGIN(Client)

class CEffectUrsaShoulder final :
	public CBasicEffect
{
private:
	explicit CEffectUrsaShoulder();
	virtual ~CEffectUrsaShoulder() = default;

public:
	virtual HRESULT Initialize(void* pArg, _matrix pos);
	virtual void Update(_double deltaTime) ;
	virtual void LateUpdate(_double deltaTime) ;
	virtual void Render();
public:
	static CEffectUrsaShoulder* Create(void*pTarget, _matrix pos);
	virtual void Free() override;

public:
	void Set_Matrix(_matrix pos) { m_pTransform->SetMatrix(pos); }


private:
	_double Deaddt = 0;



};

END