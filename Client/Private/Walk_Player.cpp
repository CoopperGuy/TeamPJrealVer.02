#include "stdafx.h"
#include "..\Public\Walk_Player.h"
#include "Skill_Player.h"

USING(Client)

CWalk_Player::CWalk_Player()
{
	m_eState = CurState::WALK;
}

void CWalk_Player::Enter(CPlayer & pPlayer)
{
	if (pPlayer.Get_Combat())
	{
		if (!pPlayer.Get_Evade())
			pPlayer.SetUp_AnimIndex((_uint)Player_State::CBWalk);
		else
			pPlayer.SetUp_AnimIndex((_uint)Player_State::CBEvade);
	}
	else if (pPlayer.Get_Evade())
		pPlayer.SetUp_AnimIndex((_uint)Player_State::Evade);
	else
		pPlayer.SetUp_AnimIndex((_uint)Player_State::Walk);
	
}

CStateMachine* CWalk_Player::Input(CPlayer& pPlayer)
{
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
				else
					return pPlayer.GetState(CurState::RUN);
			}
		}
		else
		{
			if (CEngine::GetInstance()->IsMouseDown(1))
			{
				if (!pPlayer.Get_Evade())
					return pPlayer.GetState(CurState::ATT);
				else
					return pPlayer.GetState(CurState::RUN);
			}
		}
	}
	if (CEngine::GetInstance()->IsKeyPressed(VK_CONTROL))
	{
		if (!pPlayer.Get_During() && !pPlayer.Get_Evade())
			return pPlayer.GetState(CurState::JUMP);
	}

	if (CEngine::GetInstance()->IsKeyPressed(VK_CONTROL) && !pPlayer.Get_During())
		return pPlayer.GetState(CurState::JUMP);
	
	if (!CEngine::GetInstance()->IsKeyPressed(VK_SHIFT))
	{
		if (!pSkill->Get_Cancel())
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

void CWalk_Player::Update(_double DeltaTime, CPlayer& pPlayer)
{
}

void CWalk_Player::End(CPlayer& pPlayer)
{
}

void CWalk_Player::Free()
{
	__super::Free();
}

