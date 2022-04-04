#pragma once
#include "Enemy.h"
#include "StateMachine.h"

BEGIN(Client)
class CMonHpVIBuffer;
class CWolf : public CEnemy
{
public:
	enum WOLFSTATE { EAT, IDLE0, IDLE1, WALK, RUN, THREATEN, ZTTACK, STRAIGHTATACK, DAMAGE, DIE, DEADBODY, THREATEN2, STATEEND };
	//µ¿¹° ½ÃÃ¼¸Ô´Â°Å, Ç® ³¿»õ Å¯Å¯, ±×³É ¼­ÀÖ´Â°Å, °È±â, ¶Ù±â, À§Çù, Á¡ÇÁ°ø°Ý, Á÷Áø°ø°Ý, ÇÇÇØ, Á×´Â¸ð¼Ç, Á×Àº ÈÄ 
private:
	explicit CWolf();
	explicit CWolf(CGameObject* pObj);
	virtual ~CWolf() = default;

public:
	static CWolf* Create(CGameObject * pObj, _float3 position = _float3{ 0.f,0.f,0.f });
	virtual void Free() override;

public:
	virtual void GetDamage(_vector sourceLocation) {}

public:
	HRESULT Initialize(_float3 position);
	virtual void Update(_double dDeltaTime);
	virtual void LateUpdate(_double dDeltaTime);
	virtual void Render();
	
private:
	void  SetUpAnimation();


public:
	void RotateBody(_double deltaTime);
	void SetUp_AnimIndex(_uint Index) { m_pModel->SetUp_AnimationIndex(Index); }
	void WolfAttflow(_double dDeltaTime);

private:
	WOLFSTATE	m_pWolfState = IDLE0;
	WOLFSTATE	m_pCurState = IDLE0;

private:
	_float DistanceCalculate(_vector _me, _vector _target);
	void WolfStateUpdate(_double dDeltaTime);
public:
	WOLFSTATE GetWolfState() { return m_pWolfState; }

	_float Gethp();
	

	_bool dead() { return m_bDead; }
public:
	void SetWolfState(WOLFSTATE _pState) { m_pWolfState = _pState; }
	void WolfSetAni(_double dDeltaTime);
	void SetHit() { m_pWolfState = DAMAGE; }
	void WolfLookPlayer();

	void SetAtt();
	void SetIdle();
	void Set_HpBar(CMonHpVIBuffer* _hp);
	void Gravity(_double dDeltaTime);

	_fmatrix Remove_ScaleRotation(_fmatrix TransformMatrix);




private:
	PxVec3 PlayerDirection = PxVec3(0.f, 0.f, 0.f);
private:
	_bool m_bMove = true;
	_bool WolfAtt = false;
	_bool WolfIdle = false;
	_bool m_isEnd = false;
	_bool m_isdeadAni = false;
	_bool m_bDIE = false;

	_double m_dAttDelta = 0.0;

	_vector vTargetPos = {};
private:
	_float m_fJumpSpeed = 0.f;
	_float m_fSpeed = 15.0f;
	_float3 m_vTargetToLook;

	_int  m_iBlood =0;

	_vector mypos = {};
	_vector PlayerPos = {};
private:
	class CStat* m_pStat = nullptr;
	class CTransform* m_pTargetTransform = nullptr;
	CMonHpVIBuffer*	m_pHpBar = nullptr;
private:
	CBasicCollider* m_pOBBCom = nullptr;
	CBasicCollider* m_pWeaponOBB = nullptr;



};
END
