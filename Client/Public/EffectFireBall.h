#pragma once

#include "Engine.h"
#include "BasicEffect.h"

BEGIN(Client)
class CObb;
class CEffectFireBall final :
	public CBasicEffect
{
private:
	explicit CEffectFireBall();
	virtual ~CEffectFireBall() = default;

public:
	virtual HRESULT Initialize(void* pArg);
	virtual void Update(_double deltaTime);
	virtual void LateUpdate(_double deltaTime);
	virtual void Render();
public:
	static CEffectFireBall* Create(void*	pTarget);
	virtual void Free() override;

	void LookPlayer();


	void SetFadeInOut(CGameObject * pObj);
	void SetTransform(_vector pPos) { m_pTransform->SetState(CTransform::STATE_POSITION, pPos); }
	void SetMatrix(_matrix pWorldMatrix) { m_pTransform->SetMatrix(pWorldMatrix); }
	void SetScale(_float3 scail) { m_pTransform->SetScale(scail); }

public:
	void SetObj() { m_bSet = true; }
private:

	CTransform* m_pTargetTransform = nullptr;

	_bool	m_bSetFadeOut = true;
	_bool m_bSet = false;

	_vector StartPos = {};
	_vector StartScale = {};


	_float3  m_fScale = {};
	_float3 m_fScrollX = {};
	_float3 m_fScrollY = {};


	_float PlusScale = 0.01f;
	_float m_fMaxScail = 0.2f;
	_float m_fDeadTime = 0.f;

	CGameObject* pAttachObj = nullptr;

private:
//	CObb*	m_pOBB = nullptr;

};

END