#pragma once

#include "Character.h"
#include "StateMachine.h"
#include "BasicCollider.h"
//BEGIN(Engine)
//class CBasicCollider;
//END

BEGIN(Client)

class CEnemy : public CCharacter
{
	enum class ANIM_ENEMY { IDLE, ATK, ENEMY_END };
public:
	explicit CEnemy();
	explicit CEnemy(CGameObject* pObj);
	virtual ~CEnemy() = default;
public:
	static CEnemy* Create(CGameObject* pObj);
	virtual void Free() override;
public:
	HRESULT Initialize();
	virtual void Update(_double deltaTime);
	virtual void LateUpdate(_double dDeltaTime);
	virtual void Render();
public:
	void SetInitialPosition(_vector position);
	void SetActive(_bool result) { m_pGameObject->SetActive(result); }
	void SetExplosion(_bool result) { m_bExplosion = result; }
public:
	_bool Get_Destination() { return m_bDestination; }
protected:
	CTransform*		m_pTargetTransform = nullptr;
protected:
	string			m_Timer = "";
protected:
	_float			m_fSpeedFactor = 40.f;

protected:
	BONEDESC*		m_pHandBone = nullptr;
	CGameObject*	m_pWeapon = nullptr;
protected:
	_float			m_fDissolveCutoff = 0.f;
protected:
	_vector			m_velocity = XMVectorZero();
	_vector			m_curVelocity = XMVectorZero();
private:
	CStateMachine*	m_pState = nullptr;

protected:
	_bool		m_bDestination = false;
	_bool		m_bExplosion = false;
	CBasicCollider::STATES	m_eCurSTATES = CBasicCollider::STATES::STATES_END;

};

END