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
	void SetScale(_float3 scail) { m_pTransform->SetScale(scail); }

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


	_float PlusScale = 0.01f;
	_float m_fMaxScail = 0.2f;
	_float m_fDeadTime = 0.f;

	CGameObject* pAttachObj = nullptr;


};

END