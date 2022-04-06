#include "stdafx.h"
#include "..\Public\Att_Player.h"
#include "Idle_Player.h"
CAtt_Player::CAtt_Player()
{
	m_eState = CurState::ATT;
}

void CAtt_Player::Enter(CPlayer & pPlayer)
{
	if (pPlayer.Get_Hit() && !pPlayer.Get_OnlyDown())
			pPlayer.SetUp_AnimIndex((_uint)Player_State::Hit_F);
	
	else if (pPlayer.Get_Down() && !pPlayer.Get_SuperArmor())
		pPlayer.SetUp_AnimIndex((_uint)Player_State::KnockDown_Start);
	//if (m_pEngine->IsKeyDown(VK_LBUTTON)) {
	//	testHud->CreateComboIcon(CComboHud::COMBOPOS_TOP, false);
	//}
	//if (m_pEngine->IsKeyDown(VK_RBUTTON)) {
	//	testHud->CreateComboIcon(CComboHud::COMBOPOS_BOT, true, true);
	//}
	else if (pPlayer.Get_Evade())
		pPlayer.m_pModel->SetUp_AnimationIndex((_uint)Player_State::CBEvade);
	else if (pPlayer.Get_RBComboIndex() >= 0)
	{
		switch (pPlayer.Get_RBComboIndex())
		{
		case 0:
			pPlayer.m_pModel->SetUp_AnimationIndex((_uint)Player_State::RBCombo1);
			m_iCurIndex = (_uint)Player_State::RBCombo1;
			break;
		case 1:
			pPlayer.m_pModel->SetUp_AnimationIndex((_uint)Player_State::RBCombo2);
			m_iCurIndex = (_uint)Player_State::RBCombo2;
			break;
		case 2:
			pPlayer.m_pModel->SetUp_AnimationIndex((_uint)Player_State::RBCombo3);
			m_iCurIndex = (_uint)Player_State::RBCombo3;
			break;
		case 3:
			pPlayer.m_pModel->SetUp_AnimationIndex((_uint)Player_State::RBCombo4);
			m_iCurIndex = (_uint)Player_State::RBCombo4;
			break;
		}
	}
	else if (pPlayer.Get_LBComboIndex() >= 0)
	{
		switch (pPlayer.Get_LBComboIndex())
		{
		case 0:
			pPlayer.SetUp_AnimIndex((_uint)Player_State::LBCombo1);
			break;
		case 1:
			pPlayer.SetUp_AnimIndex((_uint)Player_State::LBCombo2);
			break;
		case 2:
			pPlayer.SetUp_AnimIndex((_uint)Player_State::LBCombo3);
			break;
		case 3:
			pPlayer.SetUp_AnimIndex((_uint)Player_State::LBCombo4_0);
			break;
		case 4:
			pPlayer.SetUp_AnimIndex((_uint)Player_State::LBCombo4_1);
			break;
		}
	}
}

CStateMachine * CAtt_Player::Input(CPlayer & pPlayer)
{
	if (pPlayer.Get_Hit() && !pPlayer.Get_OnlyDown() 
		|| pPlayer.Get_Down() && !pPlayer.Get_SuperArmor())
		return	pPlayer.GetState(CurState::IDLE);
	if (pPlayer.m_pModel->Get_isFinished() || pPlayer.Get_LBComboIndex() < 0 && pPlayer.Get_RBComboIndex() < 0)
	{
		if (pPlayer.Get_LBComboIndex() >= 3)
			return pPlayer.GetState(CurState::ATT);

		return	pPlayer.GetState(CurState::IDLE);
	}
	return pPlayer.GetState(CurState::ATT);
}

void CAtt_Player::Update(_double dDeltaTime, CPlayer & pPlayer)
{
	//LB Combo
	pPlayer.Set_AnimSpeed(1.f);

	if (pPlayer.Get_LBComboIndex() >= 4)
	{
		if (pPlayer.Get_LBComboIndex() >= 5)
			pPlayer.isFinish_Combo();
	}
	else if (pPlayer.Get_RBComboIndex() >= 3)
	{
		if (pPlayer.Get_RBComboIndex() >= 4)
			pPlayer.isFinish_Combo();
	}
}

void CAtt_Player::End(CPlayer& pPlayer)
{
	if (pPlayer.Get_Hit())
	{
		if (pPlayer.Get_Down())
		{
			pPlayer.isFinish_Combo();
		}
	}
}

void CAtt_Player::Free()
{
	__super::Free();
}
