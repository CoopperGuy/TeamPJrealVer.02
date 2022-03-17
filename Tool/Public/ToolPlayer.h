#pragma once
#include "ToolCharacter.h"
//#include "StateMachine.h"

BEGIN(Tool)

class CToolPlayer : public CToolCharacter
{
	enum  Player_State
	{
		WakeUp_Bad, Normal_Idle1, Normal_Idle2, Evade, Walk, Run, Sprint_ing, Sprint_end, Run_Stop, Jump, Fall, Fall_ing, Land_Stop, Land_Run, Sprint_Fall,
		Sprint_Fall_ing, Sprint_Land_Run, ML_Start, ML_ing, ML_End, LL_Start, LL_ing, LL_End, Interact_Relam1, Interact_Relam2, Interact_Relam3,
		Hit_F, Hit_B, Stand_Die, LBCombo1, LBCombo2, LBCombo3, LBCombo4_0, LBCombo4_1, RBCombo1, RBCombo2, RBCombo3, RBCombo4, LB2Combo1, LB2Combo2, LB2Combo3,
		LB3Combo1_0, LB3Combo1_1, LB3Combo2, LB3Combo3, LB3Combo4, Leap_Start, Leap_End, WhirlWind_Start, WhirlWind_ing, WhirlWind_End, WarCry, Chop_Start, Chop_ing1,
		Chop_ing2, Chop_End, Weapon_On, Weapon_Off, CB_Idle, CBEvade, CBWalk, CBRun, CBSprint_ing, CBRun_Stop, CBJump, CBFall, CBFall_ing, CBLand_Stop, CBLand_Run, Dead, Player_End
	};

	enum LOOKDIR { LOOK_FRONT, LOOK_BACK, LOOK_LEFT, LOOK_RIGHT, LOOK_END };

	enum LBCOMBO {L_1COM, L_2COM, L_3COM};
private:
	explicit CToolPlayer();
	explicit CToolPlayer(CGameObject* pObj);
	virtual ~CToolPlayer() = default;

public:
	static CToolPlayer* Create(CGameObject* pObj);
	virtual void Free() override;

public:
	virtual void GetDamage(_vector sourceLocation) {}

public:
	HRESULT Initialize();
	virtual void Update(_double dDeltaTime);
	virtual void LateUpdate(_double dDeltaTime);
	virtual void Render();

public:
	void RotateBody(_double deltaTime);
	void SetUp_AnimIndex(_uint Index) { m_pModel->SetUp_AnimationIndex(Index); }

	const _bool Get_MoveState() const { return m_bMoveState; }
	const _bool Get_Combat() const { return m_bCB; }

private:
	void PlayerMove(_double dDeltaTime);
	void PlayerJump(_double dDeltaTime);
	void PlayerLookDirection(LOOKDIR _estate);

private:
	PxVec3 PlayerDirection = PxVec3(0.f, 0.f, 0.f);
	LOOKDIR	eLookDir = LOOK_LEFT;
	_float fStateAngle = 0;

private:
private:
	_bool m_bJump = false;
	_float m_fJumpSpeed = 0.f;
	_float m_fSpeed = 15.0f;
	_bool m_bMoveState = true;

	Player_State Pstate = Normal_Idle1;
	_double IdleStateDt = 0.f;
	_bool Find = true;

	_bool MakeNavigationCom = false;
	//class CStateMachine* m_pState = nullptr;

	_bool m_bCB = false;
	_double m_dCombatTime = 0.f;
	_uint m_iIndex = 0;


};
END
