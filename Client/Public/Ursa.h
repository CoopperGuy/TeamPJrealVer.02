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
		IDLE02, IDLE01, IDLE_CB, RUN, CB_Start, ROAR_Casting, DASH_ATT, L_SLASH, R_SLASH, Combo_1Start, Combo_1Hold, Combo_1, Combo_1End, Combo_2Start, Combo_2End,
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

private:
	void Empty_queue();
	void Adjust_Dist(_double dDeltaTime);
	void Checking_Phase(_double dDeltaTime);
	void Execute_Pattern(_double dDeltaTime);
	void First_Phase(_double dDeltaTime);
	void Second_Phase(_double dDeltaTime);
	void Third_Phase(_double dDeltaTime);
	//void Last_Phase(_double dDeltaTime);
	void SetUp_Combo();
	void Checking_Finished();
	void TestAnimation(Ursa eState);
	void Roar();
	void SetRotate();
	void OrganizeEffect(_double dDeltaTime);
	void Hit(_double dDeltaTime);
	void Create_Trail();
	void CatchUpToLook(_double dDeltaTime);
	PxVec3 OriginShift();


	_float SetDistance();
	
	_bool None_Combat();
	_bool IsGravity();

	_fmatrix Remove_Scale(_fmatrix _srcmatrix);
	_fmatrix Remove_Rotation(_fmatrix TransformMatrix);
	_fmatrix Remove_ScaleRotation(_fmatrix TransformMatrix);
private:
	PxVec3 PlayerDirection = PxVec3(0.f, 0.f, 0.f);

private:
	class CStat* m_pStat				 = nullptr;
	class CTransform* m_pTargetTransform = nullptr;
	class CTransform* m_pRenderTransform = nullptr;
	CBasicCollider* m_pOBBCom			 = nullptr;
	CMonHp*	m_pMonHp					 = nullptr;
	CBasicCollider*		m_pOBB			= nullptr;
	CBasicCollider*		m_pLeftWeapon	= nullptr;
	CBasicCollider*		m_pRightWeapon	= nullptr;
	CBasicCollider*		m_pHead			= nullptr;

	class CEffectBlood* m_pBlood = nullptr;
	class CEffectUrsaShoulder* m_pUrsaShoulder = nullptr;
	class CEffectUrsaWind*		m_pUrsaWind = nullptr;
private:
	Ursa m_eState = Ursa_END;
	queue<Ursa> m_QueState;
	
	_float3 m_vTargetToLook;
	_float3 m_vCenterPos;

	_float m_fDist		     = 0.f;
	_float m_fJumpSpeed		 = 0.f;
	_float m_fSpeed			 = 0.098f;
	_float m_fMKB = 0.f;

	_double m_dPatternTime	 = 0.0;
	_double m_dWheelWindTime = 0.0;
	_double m_dDropRockMkdt = 0;
	_double m_dTest = 0.0;
	_double m_LerpTime = 0.0;
	_double m_MakeWind = 0.0;

	_uint m_iComboIndex = 0;
	_uint m_iFirst		= 0;
	_uint m_iSec		= 0;
	_uint m_iThir		= 0;
	_uint m_iLast		= 0;

	_uint m_iMakeDust = 0;

	_bool m_bCombat[Phase_End] = {};
	_bool m_bDeadMotion		   = false;
	_bool m_bCenter			   = false;
	_bool m_bFar			   = false;
	_bool m_bSuperFar		   = false;
	_bool m_bClose			   = false;
	_bool m_bMove			   = false;
	_bool m_bCB				   = false;
	_bool m_bFinishBlow		   = false;
	_bool m_bDelay			   = false;
	_bool m_bRoar			   = false;
	_bool m_bWheelWind		   = false;
	_bool m_bSkillDelay		   = false;
	_bool m_bAddRand		   = false;

	// Dissolve
	_bool m_bDissolve = false;
	_float m_fDissolveAcc = 0.f;


	int value = 0;
private:
	CVIBuffer_Trail*		m_pRightTrailBuffer = nullptr;
	CVIBuffer_Trail*		m_pLeftTrailBuffer = nullptr;

	_float4x4			m_RightwpBoneMatrix;
	_float4x4			m_LeftwpBoneMatrix;
	_bool				m_DrawTrail = false;


	CBasicCollider::STATES	m_eRightSTATES = CBasicCollider::STATES::STATES_END;
	CBasicCollider::STATES	m_eHeadSTATES = CBasicCollider::STATES::STATES_END;
};
END
