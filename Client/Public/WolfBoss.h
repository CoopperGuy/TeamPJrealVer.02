#pragma once
#include "Enemy.h"
#include "StateMachine.h"

BEGIN(Client)
class CMonHp;
class CWolfBoss : public CEnemy
{
public:
	enum WOLFSTATE { Walk, Firing_105_R_Start, Firing_105_R_End, Firing_105_R, Firing_105_L_Start, Firing_105_L_End, Firing_105_L, Firing_103_Start, Firing_103_Short, Firing_103, Firing_102, Firing_101, 
	Run, Passout, Idle, Idle_Battle, Die, DeadBody, Combo108_Back, Combo108, Combo107_R, Combo106_L, Combo105_Back, Combo105, Combo102_R_Start, Combo102_R, Combo101_L_Start, Combo101_L, STATE_END };
	//µ¿¹° ½ÃÃ¼¸Ô´Â°Å, Ç® ³¿»õ Å¯Å¯, ±×³É ¼­ÀÖ´Â°Å, °È±â, ¶Ù±â, À§Çù, Á¡ÇÁ°ø°Ý, Á÷Áø°ø°Ý, ÇÇÇØ, Á×´Â¸ð¼Ç, Á×Àº ÈÄ 
private:
	explicit CWolfBoss();
	explicit CWolfBoss(CGameObject* pObj);
	virtual ~CWolfBoss() = default;

public:
	static CWolfBoss* Create(CGameObject* pObj, _float3 position = _float3{ 0.f,0.f,0.f });
	virtual void Free() override;

public:
	virtual void GetDamage(_vector sourceLocation) {}

public:
	HRESULT Initialize(_float3 position);
	virtual void Update(_double dDeltaTime);
	virtual void LateUpdate(_double dDeltaTime);
	virtual void Render();

public:
	void RotateBody(_double deltaTime);
	void SetUp_AnimIndex(_uint Index) { m_pModel->SetUp_AnimationIndex(Index); }

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
public:
	void SetWolfState(WOLFSTATE _pState) { m_eState = _pState; }
	WOLFSTATE GetWolfState() { return m_eState; }
	void SetBehaviorDelay() { m_fBehaviorDelay = (rand() % 5 + 5) * 0.1f; }

	void WolfLookPlayer();
	void SetAtt();
	void SetIdle();

private:
	void StateUpdate(_double dDeltaTime);
	void BehaviorUpdate(_double dDeltaTime);
	void CheckAnimFinish();

private:
	PxVec3 PlayerDirection = PxVec3(0.f, 0.f, 0.f);

private:
	class CStat* m_pStat = nullptr;
	class CTransform* m_pTargetTransform = nullptr;
	CBasicCollider* m_pOBBCom = nullptr;
	CMonHp*	m_pMonHp = nullptr;

private:
	_float m_fSpeed = 15.0f;
	_bool m_bMove = true;

private:
	_float m_fBehaviorDuration = 0.f;
	_float m_fBehaviorDelay = 0.f;
	_float m_fStunDelay = 40.f;
private:
	_bool WolfAtt = false;
	_bool WolfIdle = false;
	_bool m_bBehavior = false;

private:
	WOLFSTATE	m_eState = Idle;
	WOLFSTATE	m_eCurState = Idle;	


};
END
