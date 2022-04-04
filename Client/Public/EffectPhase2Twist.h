#pragma once

#include "Engine.h"
#include "BasicEffect.h"

BEGIN(Client)

class CEffectPahse2Twist final :
	public CBasicEffect
{
private:
	explicit CEffectPahse2Twist();
	virtual ~CEffectPahse2Twist() = default;
public:
	virtual HRESULT Initialize(void* pArg, CTransform* pTransform, CModel* pModel);
	virtual void Update(_double deltaTime) ;
	virtual void LateUpdate(_double deltaTime) ;
	virtual void Render();

public:
	void End_Effect();

public:
	static CEffectPahse2Twist* Create(void* pTarget, CTransform* pTransform, CModel* pModel);
	virtual void Free() override;

private:
	CTransform* m_pTargetTransform = nullptr;
	CModel* m_pTargetModel = nullptr;
	
	_double m_dDeadTime = 0;

	_float	m_fInstanceSpawnDelay = 0.5f;
	_float	m_fInstanceSpawnTimeAcc = 0.f;
};

END