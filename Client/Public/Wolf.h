#pragma once
#include "Enemy.h"
#include "StateMachine.h"

BEGIN(Client)
class CMonHpVIBuffer;
class CWolf : public CEnemy
{
public:
	enum WOLFSTATE { EAT, IDLE0, IDLE1, WALK, RUN, THREATEN, ZTTACK, STRAIGHTATACK, DAMAGE, DIE, DEADBODY, STATEEND };
	//���� ��ü�Դ°�, Ǯ ���� ůů, �׳� ���ִ°�, �ȱ�, �ٱ�, ����, ��������, ��������, ����, �״¸��, ���� �� 
private:
	explicit CWolf();
	explicit CWolf(CGameObject* pObj);
	virtual ~CWolf() = default;

public:
	static CWolf* Create(CGameObject* pObj, _float3 position = _float3{ 0.f,0.f,0.f });
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

	void SetAttack(_double dDeltaTime);

private:
	WOLFSTATE	m_pWolfState = IDLE0;
	WOLFSTATE	m_pCurState = IDLE0;

private:
	_float DistanceCalculate(_vector _me, _vector _target);
	void WolfStateUpdate(_double dDeltaTime);
public:
	WOLFSTATE GetWolfState() { return m_pWolfState; }

public:
	void SetWolfState(WOLFSTATE _pState) { m_pWolfState = _pState; }

	void WolfSetAni(_double dDeltaTime);
	void SetHit() { m_pWolfState = DAMAGE; }
	void WolfLookPlayer();

	void SetAtt();
	void SetIdle();
	void Set_HpBar(CMonHpVIBuffer* _hp);
	void Gravity(_double dDeltaTime);

private:
	PxVec3 PlayerDirection = PxVec3(0.f, 0.f, 0.f);
private:
	_bool m_bJump = false;
	_bool m_bMove = true;
	_bool WolfAtt = false;
	_bool WolfIdle = false;
	_bool m_isEnd = false;
	_bool m_isdeadAni = false;
private:
	_float m_fJumpSpeed = 0.f;
	_float m_fSpeed = 15.0f;
private:
	class CStat* m_pStat = nullptr;
	class CTransform* m_pTargetTransform = nullptr;
	CMonHpVIBuffer*	m_pHpBar = nullptr;
private:
	CBasicCollider* m_pOBBCom = nullptr;



};
END
