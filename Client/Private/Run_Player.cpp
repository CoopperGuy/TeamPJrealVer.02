#include "stdafx.h"
#include "..\Public\Run_Player.h"
#include "Skill_Player.h"

USING(Client)

CRun_Player::CRun_Player()
{
	m_eState = CurState::RUN;
}

void CRun_Player::Enter(CPlayer & pPlayer)
{
	if (pPlayer.Get_Hit())
	{
		if (pPlayer.Get_Down())
			pPlayer.SetUp_AnimIndex((_uint)Player_State::KnockDown_Start);
		else
			pPlayer.SetUp_AnimIndex((_uint)Player_State::Hit_F);
	}
	else if (CEngine::GetInstance()->IsKeyPressed(VK_SHIFT))
	{
		if (pPlayer.Get_Combat())
			pPlayer.SetUp_AnimIndex((_uint)Player_State::CBSprint_ing);
		else
			pPlayer.SetUp_AnimIndex((_uint)Player_State::Sprint_ing);
	}
	else
	{
		if (pPlayer.Get_Combat())
		{
			if (!pPlayer.Get_Evade())
				pPlayer.SetUp_AnimIndex((_uint)Player_State::CBRun);
			else
				pPlayer.SetUp_AnimIndex((_uint)Player_State::CBEvade);
		}
		else if (pPlayer.Get_Evade())
			pPlayer.SetUp_AnimIndex((_uint)Player_State::Evade);
		else
			pPlayer.SetUp_AnimIndex((_uint)Player_State::Run);
	}
}

CStateMachine * CRun_Player::Input(CPlayer & pPlayer)
{
	if (pPlayer.Get_Hit())
		return	pPlayer.GetState(CurState::IDLE);

	CSkill_Player* pSkill = static_cast<CSkill_Player*>(pPlayer.GetState(CurState::Skill));

	if (!CEngine::GetInstance()->IsKeyPressed('W') && !CEngine::GetInstance()->IsKeyPressed('A')
		&& !CEngine::GetInstance()->IsKeyPressed('S') && !CEngine::GetInstance()->IsKeyPressed('D'))
		return pPlayer.GetState(CurState::IDLE);
	
	if (!pPlayer.Get_During())
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
	if (!CEngine::GetInstance()->IsKeyPressed(VK_SHIFT))
	{
		if (!pSkill->Get_Cancel() && pPlayer.GetUsableSkill())
		{
			if (CEngine::GetInstance()->Get_DIKDown(DIK_1) || CEngine::GetInstance()->Get_DIKDown(DIK_2)
				|| CEngine::GetInstance()->Get_DIKDown(DIK_3) || CEngine::GetInstance()->Get_DIKDown(DIK_4))
				return pPlayer.GetState(CurState::Skill);
		}
	}

	pSkill->Set_Cancel();

	if (!pPlayer.Get_MoveState())
		return pPlayer.GetState(CurState::RUN);
	else
		return pPlayer.GetState(CurState::WALK);
		
}

void CRun_Player::Update(_double dDeltaTime, CPlayer & pPlayer)
{
}

void CRun_Player::End(CPlayer& pPlayer)
{
}

void CRun_Player::Free()
{
	__super::Free();
}
