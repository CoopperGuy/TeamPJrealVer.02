#pragma once
#include "Engine.h"
#include "Player.h"
#include "Base.h"

BEGIN(Client)
class CIdle_Player;
class CWalk_Player;
class CRun_Player;
class CAtt_Player;
class CPlayer;

enum class Player_State 
{
	WakeUp_Bad, Normal_Idle1, Normal_Idle2, Evade, Walk, Run, Sprint_ing, Sprint_end, Run_Stop, Jump, Fall, Fall_ing, Land_Stop, Land_Run, Sprint_Fall,
	Sprint_Fall_ing, Sprint_Land_Run, ML_Start, ML_ing, ML_End, LL_Start, LL_ing, LL_End, Interact_Relam1, Interact_Relam2, Interact_Relam3,
	Hit_F, Hit_B, Stand_Die, LBCombo1, LBCombo2, LBCombo3, LBCombo4_0, LBCombo4_1, RBCombo1, RBCombo2, RBCombo3, RBCombo4, LB2Combo1, LB2Combo2, LB2Combo3,
	LB3Combo1_0, LB3Combo1_1, LB3Combo2, LB3Combo3, LB3Combo4, Leap_Start, Leap_End, WhirlWind_Start, WhirlWind_ing, WhirlWind_End, WarCry, Chop_Start, Chop_ing1,
	Chop_ing2, Chop_End, Weapon_On, Weapon_Off, CB_Idle, CBEvade, CBWalk, CBRun, CBSprint_ing, CBRun_Stop, CBJump, CBFall, CBFall_ing, CBLand_Stop, CBLand_Run, Dead,
	KnockDown_Start, KnockDown_Ing,GetUp, Stun_Start, Stun_Ing, Stun_End, Player_End
};

class CStateMachine : public CBase
{
public:
	virtual ~CStateMachine();

public:
	virtual void Enter(CPlayer& pPlayer) = 0;
	virtual CStateMachine* Input(CPlayer& pPlayer) = 0;
	virtual void Update(_double dDeltaTime, CPlayer& pPlayer) = 0;
	virtual void End(CPlayer& pPlayer) = 0;

public:
	virtual void Free() override;
	CurState Get_State() { return m_eState; }
	_bool Get_Cancel() { return m_bCancel; }

	void Set_Cancel() { m_bCancel = false; }
protected:
	CurState m_eState;
	_bool m_bCancel = false;
};

END