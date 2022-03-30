#pragma once

#include "Engine.h"
#include "BasicEffect.h"

BEGIN(Client)
class CObb;
class CEffectDropRock final :
	public CBasicEffect
{
private:
	explicit CEffectDropRock();
	virtual ~CEffectDropRock() = default;

public:
	virtual HRESULT Initialize(void* pArg, _vector pos);
	virtual void Update(_double deltaTime);
	virtual void LateUpdate(_double deltaTime);
	virtual void Render();
public:
	static CEffectDropRock* Create(void * pArg, _vector pos);
	virtual void Free() override;

public:
	void SetPos(_matrix _pmatrix);
	void Set_Pos(_vector pPos);
	_bool GetDead() { return m_bDead; }

private:

	CGameObject* pAttachObj = nullptr;

	_double deaddt = 0;


private:
//	CObb*	m_pOBB = nullptr;

};

END