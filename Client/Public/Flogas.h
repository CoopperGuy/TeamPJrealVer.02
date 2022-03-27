#pragma once
#include "Enemy.h"
#include "StateMachine.h"

BEGIN(Client)
class CMonHp;
class CTownPortal;
class CFlogas : public CEnemy
{
public:
	enum Flogas { IDLE, WALK, RUN, R_Slash, L_Slash, THRUST, SWORDTHROWING_START, SWORDTHROWING_LOOP, SWORDTHROWING_END, FIREWAVE,FIREFIST
		,FLYING_END2, FLYING, FOOTHAMMER,STICKSWORD1, STICKSWORD2, STICKSWORD3, STICKSWORDCutScene, STUN,DMG_F,DMG_B, DIE, DEADBODY,FLYING_START,FLYING_ING,FLYING_END, Flogas_END };

private:
	explicit CFlogas();
	explicit CFlogas(CGameObject* pObj);
	virtual ~CFlogas() = default;

public:
	static CFlogas* Create(CGameObject* pObj, _float3 position = _float3{ 0.f,0.f,0.f });
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
	void Create_Trail();
	void Set_TrailOnOff();

private:
	void Empty_queue();
	void InCombat(_double dDeltaTime);
	void EndState(Flogas eState, _double dDeltaTime);
	void RandomPattern();
	void Adjust_Dist(_double dDeltaTime);
	void SpecialPattern(_double dDeltaTime);
	void OverChase(_double dDeltaTime);
	
	void SecondCombat(_double dDeltaTime);
	void Flying(_double dDeltaTime);
	
	void OrganizeEffect(Flogas eState);
	void Hit();

	_float SetDistance();
	_bool OriginShift(_double dDeltaTime);

private:
	PxVec3 PlayerDirection = PxVec3(0.f, 0.f, 0.f);

private:
	class CStat* m_pStat					= nullptr;
	class CTransform* m_pTargetTransform	= nullptr;
	CBasicCollider* m_pOBBCom				= nullptr;
	CMonHp*	m_pMonHp						= nullptr;
	CEnemy* m_pElement[4] = {};


	list<class CWaterEA*> m_pWaterEA = {};
	class CEffectFly* m_pEffFly = nullptr;
	class CEffectGrab* m_pEffGrab = nullptr;
	class CFlyLight* m_pEffFlyLight = nullptr;
	class CEffectPajang* m_pEffPajang = nullptr;
	class CEffectFlyLaser* m_pEffFlyLaser = nullptr;
	class CEffectBlackhole* m_pEffBlackhole = nullptr;
	class CEffectSwordRing* m_pEffSwordRing = nullptr;

private:
	Flogas m_eState = Flogas_END;
	
	_bool m_bStartBattle	= false;
	_bool m_bClose			= false;
	_bool m_bMove			= false;
	_bool m_bAtt			= false;
	_bool m_bDelay			= false;
	_bool m_bOverChase		= false;
	_bool m_bSpecialAtt		= false;
	_bool m_bSpecialStart	= false;
	_bool m_bDeadMotion		= false;
	_bool m_bMeteor			= false;
	_bool m_bPhaseSecond	= false;
	_bool m_bFly			= true;
	_bool m_bCenter			= false;
	_bool m_bSecondAtt		= false;
	_bool m_bChanged		= false;
	_bool m_bMakeEffect = false;
	_bool m_bMakeFirewave = false;
	_bool m_bMakeBlood = false;

	_float m_fDist			 = 0.f;
	_double m_dSpecialDelay  = 3.f;
	_double m_dDelayTime	 = 0.f;
	_double m_dChaseTime	 = 0.f;
	_double m_dFlyTime		 = 0.f;
	_double m_dChangeTime	 = 0.f;
	_double m_dAniSpeed		 = 1.f;
	
	_uint m_iLastIndex		 = 0;
	_uint m_iFlyingCount	 = 0;
	_uint m_iOnePatternCount = 0;
	_uint m_iMakeMeteo = 0;
	_uint m_iBlood = 0;


	_float3 m_vTargetToLook;
	_float3 m_vCenterPos;
	queue<Flogas> m_QueState;
	
	_double m_TimeAcc = 0.0;
	CGameObject* pEffectObj = nullptr;
private:
	CGameObject*		m_pTrail = nullptr;
	CVIBuffer_Trail*	m_pTrailBuffer = nullptr;
	_float4x4			m_wpBoneMatrix;
	_bool				m_DrawTrail = false;
	CTownPortal*		m_pPortal = nullptr;
	CBasicCollider*		m_pOBB = nullptr;
};
END
