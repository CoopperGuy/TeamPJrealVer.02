#include "stdafx.h"
#include "..\Public\IdleState.h"
#include "WalkState.h"
#include "RunState.h"

CIdleState::CIdleState()
{
}

void CIdleState::Input(CStateMachine* pState, CPlayer* pPlayer)
{
	Update(pState, pPlayer);
}

void CIdleState::Update(CStateMachine* pState, CPlayer* pPlayer)
{
	if (CEngine::GetInstance()->IsKeyDown(VK_OEM_2))
	{
		if (m_bMoveState)
			m_bMoveState = false;
		else
			m_bMoveState = true;
	}
	if (CEngine::GetInstance()->IsKeyPressed('W') || CEngine::GetInstance()->IsKeyPressed('A')
		|| CEngine::GetInstance()->IsKeyPressed('S') || CEngine::GetInstance()->IsKeyPressed('D'))
	{
		if (m_bMoveState)
		{
			pState = CStateMachine::pWalk;
			pState->Input(pState, pPlayer);
		}
		else
		{
			pState = CStateMachine::pRun;
			pState->Input(pState, pPlayer);
		}
	}
	else
		pPlayer->SetUp_AnimIndex((_uint)Anim_State::Normal_Idle1);
	//if (CEngine::GetInstance()->IsKeyDown(VK_LBUTTON))
	//{
	//	//pState = *CStateMachine::Combo1;
	//	pState->Input(pState, pPlayer);
	//}
	//if (CEngine::GetInstance()->IsKeyDown(VK_SPACE))
	//{
	//	//pState = *CStateMachine::Evade;
	//	pState->Input(pState, pPlayer);
	//}
}
