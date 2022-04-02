#pragma once
#include "Enemy.h"
#include "StateMachine.h"

BEGIN(Client)
class CMonHp;
class CDarkKnight : public CEnemy
{
public:
	enum STATE { Walk, SK_SIDESLASH2, SK_RAISING2, SK_SLASH2, SK_STING2, PHASE2_START, PHASE2_LOOP, PHASE2_END, SK_SIDESLASH, SK_SHIELDATTACK, SK_JUMPATTACK, RUN, GROGGY,
	ENDTRYBATTLE, IDLE, IDLE_BATTLE, GETUP, DOWN_LOOP, DOWN_DEADBODY, DOWN, DMG1, DMG2, DIE, DEADBODY, SLASH, STING, STATE_END };

private:
	explicit CDarkKnight();
	explicit CDarkKnight(CGameObject* pObj);
	virtual ~CDarkKnight() = default;

public:
	static CDarkKnight* Create(CGameObject* pObj, _float3 position = _float3{ 0.f,0.f,0.f });
	virtual void Free() override;

public:
	virtual void GetDamage(_vector sourceLocation) {}

public:
	HRESULT Initialize(_float3 position);
	virtual void Update(_double dDeltaTime);
	virtual void LateUpdate(_double dDeltaTime);
	virtual void Render();

public:
	void SetGameObject(CGameObject* obj);
	void SetTransform(CTransform* trans);
	void SetCollision(CCollider* colli);
	void SetNavigation(CNavigation* nav);
	void SetModel(CModel* model);
	void SettingCollider(CCollider* col);
	void SetBasicCollider(CBasicCollider* BasicCol);
	void SetStat(CStat* pstat);
	void SetAttTarget(CGameObject* obj);
	void SetAttack(_double dDeltaTime);
	void SetMonHp(CMonHp* hp);

private:
	void RotateBody(_double deltaTime);
	void SetUp_AnimIndex(_uint Index) { m_pModel->SetUp_AnimationIndex(Index); }

private:
	void SetWolfState(STATE _pState) { m_eState = _pState; }
	void SetBehaviorDelay() { m_fBehaviorDelay = (rand() % 5 + 5) * 0.1f; }

	void ChaseTarget(_double deltaTime, _float3 vTargetPos);
private:
	void StateUpdate(_double dDeltaTime);
	void BehaviorUpdate(_double dDeltaTime);
	void CheckAnimFinish();

	void Update_Trail(_double dDeltaTime);
	void Create_Trail();
	void Hit();
private:
	PxVec3 PlayerDirection = PxVec3(0.f, 0.f, 0.f);

private:
	CStat*				m_pStat = nullptr;
	CTransform*			m_pTargetTransform = nullptr;
	CTransform*			m_pRenderTransform = nullptr;
	CBasicCollider*		m_pOBBCom = nullptr;
	CBasicCollider*		m_pOBB = nullptr;
	CMonHp*	m_pMonHp = nullptr;

private:
	_float	m_fSpeed = 15.0f;
	_bool	m_bMove = true;

	// Dissolve
	_bool	m_bDissolve = false;
	_float	m_fDissolveAcc = 0.f;

	//trail
	CVIBuffer_Trail*		m_pTrailBuffer = nullptr;
	_float4x4				m_wpBoneMatrix;
	_bool					m_DrawTrail = false;

private:
	_float3 m_vCreatePos = { 0.f, 0.f, 0.f };
	_float3 m_vDestPos = { 0.f, 0.f, 0.f };


	_float m_fBehaviorDuration = 0.f;
	_float m_fBehaviorTime = 0.f;
	_float m_fBehaviorDelay = 0.f;
	_float m_fStunDelay = 40.f;
	_float m_fDist = 0.f;
	_float m_fSpeed = 0.5f;
private:
	_bool m_bCombat = false;
	_bool m_bBehavior = false;
	_bool m_bPhase2 = false;
private:
	STATE	m_eState = STATE_END;
	STATE	m_eCurState = STATE_END;


};
END
