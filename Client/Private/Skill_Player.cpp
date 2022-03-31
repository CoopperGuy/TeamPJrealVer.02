#include "stdafx.h"
#include "..\Public\Skill_Player.h"


CSkill_Player::CSkill_Player()
{
	m_eState = CurState::Skill;
}

void CSkill_Player::Enter(CPlayer & pPlayer)
{
	if (pPlayer.Get_Down() && !pPlayer.Get_SuperArmor())
	{
		pPlayer.SetUp_AnimIndex((_uint)Player_State::KnockDown_Start);
		return;
	}
	
	if (CEngine::GetInstance()->Get_DIKDown(DIK_2))
	{
		if (m_iCurrentAniIndex == 2)
			m_bCancel = true;
		m_iCurrentAniIndex = 2;
	}
	else if (CEngine::GetInstance()->Get_DIKDown(DIK_4))
	{
		if (m_iCurrentAniIndex == 4)
			m_bCancel = true;
		m_iCurrentAniIndex = 4;
	}


	if (pPlayer.Get_Skill() != -1)
	{
		if (!m_bDuring)
		{
			m_vecSkill.push_back(pPlayer.Get_Skill());
			m_iStartIndex = pPlayer.Get_Skill();
		}
		if (!m_bCancel)
			m_bDuring = true;

		if (m_bFinished)
		{
			m_bDuring = false;
			m_bFinished = false;
		}
	}
	else
		m_bDuring = false;

	if (m_vecSkill.size() > 0 && m_iCurIndex == 0)
		pPlayer.SetUp_AnimIndex(m_vecSkill[m_iCurIndex]);
	
}

CStateMachine * CSkill_Player::Input(CPlayer & pPlayer)
{
	if (pPlayer.Get_Down())
	{
		if (!pPlayer.Get_SuperArmor())
			m_bCancel = true;
	}
	if (!m_bDuring)
	{
		m_iCurIndex = 0;
		return pPlayer.GetState(CurState::IDLE);
	}
	if (m_bCancel)
	{
		m_bDuring = false;
		m_SkillTime = 0.f;
		m_bLoop = false;
		m_bFinished = true;
		return pPlayer.GetState(CurState::IDLE);
	}
	
	return pPlayer.GetState(CurState::Skill);
}

void CSkill_Player::Update(_double dDeltaTime, CPlayer & pPlayer)
{
	pPlayer.Set_AnimSpeed(1.f);
	_uint iFinished = 0;

	if (m_bDuring)
	{
		m_SkillTime += dDeltaTime;
		if (m_iStartIndex == (_uint)Player_State::Leap_Start)
		{
			m_iStartIndex = 0;
			m_vecSkill.push_back((_uint)Player_State::Leap_End);
		}
		if (m_iStartIndex == (_uint)Player_State::WhirlWind_Start)
		{
			m_iStartIndex = 0;
			m_bLoop = true;
			m_vecSkill.push_back((_uint)Player_State::WhirlWind_ing);
			m_vecSkill.push_back((_uint)Player_State::WhirlWind_End);
		}
		if (m_iStartIndex == (_uint)Player_State::WarCry)
		{
			m_iStartIndex = 0;
		}
		if (m_iStartIndex == (_uint)Player_State::Chop_Start)
		{
			m_iStartIndex = 0;
			m_bLoop = true;
			m_vecSkill.push_back((_uint)Player_State::Chop_ing1);
			m_vecSkill.push_back((_uint)Player_State::Chop_ing2);
			m_vecSkill.push_back((_uint)Player_State::Chop_End);
		}
		iFinished = (_uint)m_vecSkill.size();
	}

	if (m_bLoop)
	{
		if (pPlayer.m_pModel->Get_isFinished((_uint)Player_State::WhirlWind_Start) || pPlayer.m_pModel->Get_isFinished((_uint)Player_State::Chop_Start))
		{
			m_iCurIndex = 1;
			pPlayer.SetUp_AnimIndex(m_vecSkill[m_iCurIndex]);
		}
		if (m_SkillTime > 5.f)
		{
			if (m_vecSkill[0] == (_uint)Player_State::Chop_Start)
				m_iCurIndex = 2;
		}
		if (m_SkillTime > 6.f)
		{
			if (pPlayer.m_pModel->Get_isFinished())
			{
				pPlayer.SetUp_AnimIndex(m_vecSkill[++m_iCurIndex]);
				m_SkillTime = 0.f;
				m_bLoop = false;
			}
		}
	}
	else
	{
		if (pPlayer.m_pModel->Get_isFinished())
		{
			if (iFinished != 0)
			{
				if (pPlayer.m_pModel->Get_isFinished(m_vecSkill[m_iCurIndex]))
				{
					if (m_iCurIndex < iFinished - 1)
						pPlayer.SetUp_AnimIndex(m_vecSkill[++m_iCurIndex]);
					else if (m_iCurIndex >= iFinished - 1)
						m_bFinished = true;
				}
			}
		}
	}
	
}

void CSkill_Player::End(CPlayer & pPlayer)
{
	if (m_bFinished)
	{
		if(m_bCancel)
			m_bFinished = false;

		m_SkillTime = 0.f;
		m_iCurrentAniIndex = 0;
		m_vecSkill.clear();
	}

}

void CSkill_Player::Free()
{
	__super::Free();
}
