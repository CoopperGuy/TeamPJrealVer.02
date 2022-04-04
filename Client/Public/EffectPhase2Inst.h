#pragma once

#include "Engine.h"
#include "BasicEffect.h"

BEGIN(Client)

class CEffectPahse2Inst final :
	public CBasicEffect
{
private:
	explicit CEffectPahse2Inst();
	virtual ~CEffectPahse2Inst() = default;
public:
	virtual HRESULT Initialize(CTransform* pTransform);
	virtual void Update(_double deltaTime) ;
	virtual void LateUpdate(_double deltaTime) ;
	virtual void Render();

public:
	static CEffectPahse2Inst* Create(CTransform* pTransform);
	virtual void Free() override;

private:
	_double m_dDeadTime = 0;
	CVIBuffer_RectInstance* m_pRectInstCom = nullptr;	
};

END