#include "stdafx.h"
#include "..\Public\Idle_Player.h"
#include "Skill_Player.h"

USING(Client)

CIdle_Player::CIdle_Player()
{
	m_eState = CurState::IDLE;
}

void CIdle_Player::Enter(CPlayer & pPlayer)
{

	if (pPlayer.Get_Combat())
	{
		if (!pPlayer.Get_Evade())
			pPlayer.SetUp_AnimIndex((_uint)Player_State::CB_Idle);
		else
			pPlayer.SetUp_AnimIndex((_uint)Player_State::CBEvade);
	}
	else if (pPlayer.Get_Evade())
		pPlayer.SetUp_AnimIndex((_uint)Player_State::Evade);
	else
		pPlayer.SetUp_AnimIndex((_uint)Player_State::Normal_Idle1);

}

CStateMachine* CIdle_Player::Input(CPlayer& pPlayer)
{
	CSkill_Player* pSkill =static_cast<CSkill_Player*>(pPlayer.GetState(CurState::Skill));

	if (CEngine::GetInstance()->IsKeyPressed('W') || CEngine::GetInstance()->IsKeyPressed('A')
		|| CEngine::GetInstance()->IsKeyPressed('S') || CEngine::GetInstance()->IsKeyPressed('D'))
	{
		if (!pPlayer.Get_MoveState())
			return pPlayer.GetState(CurState::RUN);

		else
			return pPlayer.GetState(CurState::WALK);
	}
	if(!pPlayer.Get_During())
	{
		if (!pPlayer.Get_Evade())
		{
			if (pPlayer.Get_LBComboIndex() >= 0)
			{
				if (CEngine::GetInstance()->IsMouseDown(0) || pPlayer.Get_LBComboIndex() >= 3)
				{
					if (!pPlayer.Get_Evade())
						return pPlayer.GetState(CurState::ATT);
				}
			}
			else
			{
				if (CEngine::GetInstance()->IsMouseDown(1))
				{
					if (!pPlayer.Get_Evade())
						return pPlayer.GetState(CurState::ATT);
				}
			}
		}
	}
	if (!pSkill->Get_Cancel() && pPlayer.GetUsableSkill())
	{
		if (CEngine::GetInstance()->Get_DIKDown(DIK_1) || CEngine::GetInstance()->Get_DIKDown(DIK_2)
			|| CEngine::GetInstance()->Get_DIKDown(DIK_3) || CEngine::GetInstance()->Get_DIKDown(DIK_4))
			return pPlayer.GetState(CurState::Skill);
	}

	pSkill->Set_Cancel();

	return pPlayer.GetState(CurState::IDLE);
}

void CIdle_Player::Update(_double dDeltaTime, CPlayer& pPlayer)
{
	if (pPlayer.Get_LBComboIndex() >= 5)
		pPlayer.isFinish_Combo();
}

void CIdle_Player::End(CPlayer& pPlayer)
{
}

void CIdle_Player::Free()
{
	__super::Free();
}
