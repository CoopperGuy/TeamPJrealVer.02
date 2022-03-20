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
	virtual HRESULT Initialize(void* pArg, _vector* pos);
	virtual void Update(_double deltaTime);
	virtual void LateUpdate(_double deltaTime);
	virtual void Render();
public:
	static CEffectFireBall* Create(void * pArg, _vector* pos);
	virtual void Free() override;

public:
	void SetPos(_matrix _pmatrix);
	void Set_Pos(_vector pPos);
	_bool GetDead() { return m_bDead; }

private:

	CGameObject* pAttachObj = nullptr;

	_double deaddt = 0;
	//CTransform* m_pTargetTransform = nullptr;

	//_bool	m_bSetFadeOut = true;
	//_bool m_bSet = false;

	//_float3 StartPos = {};
	//_vector StartScale = {};


	//_float3  m_fScale = {};
	//_float3 m_fScrollX = {};
	//_float3 m_fScrollY = {};


	//_float PlusScale = 0.01f;
	//_float m_fMaxScail = 0.2f;
	//_float m_fDeadTime = 0.f;


	//int numrand = 0.f;


private:
//	CObb*	m_pOBB = nullptr;

};

END