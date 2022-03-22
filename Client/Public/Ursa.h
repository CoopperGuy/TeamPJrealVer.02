#pragma once
#include "Enemy.h"
#include "StateMachine.h"

BEGIN(Client)
class CMonHp;
class CUrsa : public CEnemy
{
public:
	enum Ursa 
	{
		IDLE02, IDLE01, IDLE_CB, RUN, CB_START, CASTING, DASH_ATT, L_SLASH, R_SLASH, Combo_1Start, Combo_1Hold, Combo_1, Combo_1End, Combo_2Start, Combo_2End,
		Combo_3Start, Combo_3End, Combo_4Start, Combo_4End, Big_SLASH, AXE_STAMP, PUMMEL_1,PUMMEL_2, ROAR_ING,ROAR_End, DASH_ATTSpeedup, WHEELWIND_Start, WHEELWIND_Ing,
		WHEELWIND_End, ROAR_Start, HIT, DIE, DEADBODY,qqq,wwww,eee,err,assd,wwewe,qyyw,Flying_Start, Flying_Land, Flying_End, Ursa_END
	};
	enum CombatPhase {First, Second, Third, Last, Phase_End};

private:
	explicit CUrsa();
	explicit CUrsa(CGameObject* pObj);
	virtual ~CUrsa() = default;

public:
	static CUrsa* Create(CGameObject* pObj, _float3 position = _float3{ 0.f,0.f,0.f });
	virtual void Free() override;

public:
	virtual void GetDamage(_vector sourceLocation) {}

public:
	HRESULT Initialize(_float3 position);
	virtual void Update(_double dDeltaTime);
	virtual void LateUpdate(_double dDeltaTime);
	virtual void Render();

public:
	void SetUp_AnimIndex(_uint Index) { m_pModel->SetUp_AnimationIndex(Index); }
	
private:
	void Empty_queue();

	void Adjust_Dist(_double dDeltaTime);
	void Checking_Phase(_double dDeltaTime);
	void Execute_Pattern(_double dDeltaTime);
	void First_Phase(_double dDeltaTime);
	void Second_Phase(_double dDeltaTime);
	void Third_Phase(_double dDeltaTime);
	void Last_Phase(_double dDeltaTime);
	void SetUp_Combo();
	void Checking_Finished();
	_float SetDistance();

private:
	PxVec3 PlayerDirection = PxVec3(0.f, 0.f, 0.f);

private:
	class CStat* m_pStat				 = nullptr;
	class CTransform* m_pTargetTransform = nullptr;
	CBasicCollider* m_pOBBCom			 = nullptr;
	CMonHp*	m_pMonHp					 = nullptr;

private:
	Ursa m_eState = Ursa_END;
	
	_float3 m_vTargetToLook;
	_float3 m_vCenterPos;
	queue<Ursa> m_QueState;

	_float m_fDist = 0.f;
	_double m_dPatternTime = 0.0;

	_uint m_iComboIndex = 0;

	_bool m_bCombat[Phase_End] = {};
	_bool m_bFar			   = false;
	_bool m_bClose			   = false;
	_bool m_bMove			   = false;
	_bool m_bCB				   = false;
	
};
END
