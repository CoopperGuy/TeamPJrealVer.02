#pragma once

#include "Engine.h"
#include "BasicEffect.h"

BEGIN(Client)
class CObb;
class CEffectPajang final :
	public CBasicEffect
{
private:
	explicit CEffectPajang();
	virtual ~CEffectPajang() = default;

public:
	virtual HRESULT Initialize(void* pArg);
	virtual void Update(_double deltaTime) ;
	virtual void LateUpdate(_double deltaTime) ;
	virtual void Render();
public:
	static CEffectPajang* Create(void*	pTarget);
	virtual void Free() override;
	
	
	void SetFadeInOut(CGameObject * pObj);


public:
	void SetSlashR(_bool ptrue) { m_bSlashR = ptrue; }
private:
	_bool	m_bSetFadeOut= true;
	_bool m_bSlashR = false;

	_vector TargetPos = {};
	_vector StartScale = {};


	_float3  m_fScale = {};
	_float3 m_fScrollX = {};
	_float3 m_fScrollY = {};
	_float3	m_vScales;

	_float PlusScale = 0.1f;
	_float m_fMaxScail = 1.5f;
	_float m_fDeadTime = 0.f;

private:
	CObb*	m_pOBB = nullptr;

};

END