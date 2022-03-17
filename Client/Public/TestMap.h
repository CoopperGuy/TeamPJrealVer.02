#pragma once
#include "Character.h"
#include "StateMachine.h"

BEGIN(Client)
class CTestMap : public CCharacter
{
private:
	explicit CTestMap();
	explicit CTestMap(CGameObject* pObj);
	virtual ~CTestMap() = default;

public:
	static CTestMap* Create(CGameObject* pObj);
	virtual void Free() override;

public:
	HRESULT Initialize();
	virtual void Update(_double dDeltaTime);
	virtual void LateUpdate(_double dDeltaTime);
	virtual void Render();

private:
	CTransform*			m_pTransformCom = nullptr;
	CRenderer*			m_pRendererCom = nullptr;
	CModel*				m_pModelCom = nullptr;
	CGameObject*		m_pgameObject = nullptr;
	CCapsuleCollider*	m_pCapsuleCollider = nullptr;

private:
	PxVec3 PlayerDirection = PxVec3(0.f, 0.f, 0.f);

private:
private:
	bool m_bJump = false;
	_float m_fJumpSpeed = 0.f;
	_float m_fSpeed =15.0f;

};
END
