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


	void SetFadeInOut(CGameObject * pObj);

	void SetMatrix(_matrix pWorldMatrix) { m_pTransform->SetMatrix(pWorldMatrix); }

public:
	void SetObj() { m_bSet = true; }
private:
	_bool	m_bSetFadeOut = true;
	_bool m_bSet = false;

	_vector TargetPos = {};
	_vector StartScale = {};


	_float3  m_fScale = {};
	_float3 m_fScrollX = {};
	_float3 m_fScrollY = {};


	_float PlusScale = 1.f;
	_float plusscale = 1.f;
	_float m_fMaxScail = 2.f;
	_float m_fDeadTime = 0.f;

	CGameObject* pAttachObj = nullptr;

};

END