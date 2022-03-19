#pragma once

#include "Engine.h"
#include "BasicEffect.h"

BEGIN(Client)
class CObb;
class CEffectMeteoFireBall final :
	public CBasicEffect
{
private:
	explicit CEffectMeteoFireBall();
	virtual ~CEffectMeteoFireBall() = default;

public:
	virtual HRESULT Initialize(void* pArg, _vector* pos);
	virtual void Update(_double deltaTime);
	virtual void LateUpdate(_double deltaTime);
	virtual void Render();
public:
	static CEffectMeteoFireBall* Create(void * pArg, _vector* pos);
	virtual void Free() override;

public:
	void SetPos(_matrix _pmatrix);
	void Set_Pos(_vector pPos);
	_bool GetDead() { return m_bDead; }

private:

	CGameObject* pAttachObj = nullptr;

	_float deaddt = 0;
	float posy = 0.f;
	_vector pos = {};
	_float startposy = 0.f;
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