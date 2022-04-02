//#pragma once
//#include "Enemy.h"
//#include "StateMachine.h"
//
//BEGIN(Client)
//class CMonHp;
//class CDarkKnight : public CEnemy
//{
//public:
//	enum WOLFSTATE { Walk, SK_SIDESLASH2, SK_RAISING2, SK_STING2, PHASE2_START, PHASE2_LOOP, PHASE2_END, SK_SIDESLASH, SK_SHIELDATTACK, SK_JUMPATTACK, RUN, GROGGY,
//	ENDTRYBATTLE, IDLE, IDLE_BATTLE, GETUP, DOWN_LOOP, DOWN_DEADBODY, DOWN, DMG1, DMG2, DIE, DEADBODY, SLASH, STING, STATE_END };
//	//µ¿¹° ½ÃÃ¼¸Ô´Â°Å, Ç® ³¿»õ Å¯Å¯, ±×³É ¼­ÀÖ´Â°Å, °È±â, ¶Ù±â, À§Çù, Á¡ÇÁ°ø°Ý, Á÷Áø°ø°Ý, ÇÇÇØ, Á×´Â¸ð¼Ç, Á×Àº ÈÄ 
//private:
//	explicit CDarkKnight();
//	explicit CDarkKnight(CGameObject* pObj);
//	virtual ~CDarkKnight() = default;
//
//public:
//	static CDarkKnight* Create(CGameObject* pObj, _float3 position = _float3{ 0.f,0.f,0.f });
//	virtual void Free() override;
//
//public:
//	virtual void GetDamage(_vector sourceLocation) {}
//
//public:
//	HRESULT Initialize(_float3 position);
//	virtual void Update(_double dDeltaTime);
//	virtual void LateUpdate(_double dDeltaTime);
//	virtual void Render();
//
//public:
//	void RotateBody(_double deltaTime);
//	void SetUp_AnimIndex(_uint Index) { m_pModel->SetUp_AnimationIndex(Index); }
//
//public:
//	void SetGameObject(CGameObject* obj);
//	void SetTransform(CTransform* trans);
//	void SetCollision(CCollider* colli);
//	void SetNavigation(CNavigation* nav);
//	void SetModel(CModel* model);
//	void SettingCollider(CCollider* col);
//	void SetBasicCollider(CBasicCollider* BasicCol);
//	void SetStat(CStat* pstat);
//	void SetAttTarget(CGameObject* obj);
//	void SetAttack(_double dDeltaTime);
//	void SetMonHp(CMonHp* hp);
//public:
//	void SetWolfState(WOLFSTATE _pState) { m_eState = _pState; }
//	WOLFSTATE GetWolfState() { return m_eState; }
//	void SetBehaviorDelay() { m_fBehaviorDelay = (rand() % 5 + 5) * 0.1f; }
//
//	void WolfLookPlayer();
//	void SetAtt();
//	void SetIdle();
//
//private:
//	void StateUpdate(_double dDeltaTime);
//	void BehaviorUpdate(_double dDeltaTime);
//	void CheckAnimFinish();
//
//private:
//	PxVec3 PlayerDirection = PxVec3(0.f, 0.f, 0.f);
//
//private:
//	class CStat* m_pStat = nullptr;
//	class CTransform* m_pTargetTransform = nullptr;
//	CBasicCollider* m_pOBBCom = nullptr;
//	CMonHp*	m_pMonHp = nullptr;
//
//private:
//	_float m_fSpeed = 15.0f;
//	_bool m_bMove = true;
//
//private:
//	_float m_fBehaviorDuration = 0.f;
//	_float m_fBehaviorDelay = 0.f;
//	_float m_fStunDelay = 40.f;
//private:
//	_bool WolfAtt = false;
//	_bool WolfIdle = false;
//	_bool m_bBehavior = false;
//
//private:
//	WOLFSTATE	m_eState = Idle;
//	WOLFSTATE	m_eCurState = Idle;	
//
//
//};
//END
