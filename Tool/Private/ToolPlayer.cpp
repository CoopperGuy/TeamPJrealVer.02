#include "stdafx.h"
#include "..\Public\ToolPlayer.h"

USING(Tool)

CToolPlayer::CToolPlayer(CGameObject* pObj)
	: CToolCharacter(pObj)
{
}


CToolPlayer * CToolPlayer::Create(CGameObject * pObj)
{
	CToolPlayer*		pInstance = new CToolPlayer(pObj);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed to Create CToolPlayer");
		SafeRelease(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CToolPlayer::Free()
{
	__super::Free();
}

HRESULT CToolPlayer::Initialize()
{

	m_pGameObject = CEngine::GetInstance()->FindGameObjectWithName(CEngine::GetInstance()->GetCurSceneNumber(), "Player");
	if (nullptr == m_pGameObject)
		return E_NOINTERFACE;

	m_pTransform = dynamic_cast<CTransform*>(m_pGameObject->GetComponent("Com_Transform"));
	m_pModel = dynamic_cast<CModel*>(m_pGameObject->GetComponent("Com_Model"));
	m_pNavigation = dynamic_cast<CNavigation*>(m_pGameObject->GetComponent("Com_Navigation"));

	m_pModel->SetUp_AnimationIndex(1, ANIM_TYPE::NONE, false);

	m_pModel->SetAnimationLoop((_uint)Player_State::LBCombo1, false);

	return S_OK;
}

void CToolPlayer::Update(_double dDeltaTime)
{
	if (!m_pGameObject)
		return;
	if (CEngine::GetInstance()->IsKeyDown('9'))
	{
		++m_iIndex;
		m_pModel->SetUp_AnimationIndex(m_iIndex);
	}

	if (CEngine::GetInstance()->IsKeyDown(VK_OEM_2))
	{
		if (!m_bMoveState)
			m_bMoveState = true;
		else
			m_bMoveState = false;
	}
	if (CEngine::GetInstance()->IsKeyDown(VK_LBUTTON))
	{
		m_dCombatTime = 0.f;
		m_bCB = true;
		Pstate = LBCombo1;
	}

	IdleStateDt += dDeltaTime;
	if (IdleStateDt > 0.3 && !m_bCB)
		Pstate = Normal_Idle1; /*m_pModel->SetUp_AnimationIndex(1);*/

	if (m_bCB)
	{
		m_dCombatTime += dDeltaTime;
		Pstate = LBCombo1;
	}


	if (m_bCB && m_pModel->Get_isFinished())
	{
		m_bCB = false;
		m_dCombatTime = 0.f;
	}


	//PlayerJump(dDeltaTime);
	PlayerMove(dDeltaTime);

}


void CToolPlayer::LateUpdate(_double dDeltaTime)
{
	m_pModel->Play_Animation(dDeltaTime);

}

void CToolPlayer::Render()
{
}


void CToolPlayer::PlayerMove(_double dDeltaTime)
{
	_float fSpeed = 0.f;

	if (CEngine::GetInstance()->IsKeyPressed(VK_UP))
	{
		IdleStateDt = 0.f;
		PlayerLookDirection(LOOKDIR::LOOK_FRONT);
		Pstate = Run;
		m_pTransform->GoRight(-dDeltaTime);
	}
	if (CEngine::GetInstance()->IsKeyPressed(VK_DOWN))
	{
		IdleStateDt = 0.f;
		PlayerLookDirection(LOOKDIR::LOOK_BACK);
		Pstate = Run;
		m_pTransform->GoLeft(dDeltaTime);
	}
	if (CEngine::GetInstance()->IsKeyPressed(VK_LEFT))
	{
		IdleStateDt = 0.f;
		PlayerLookDirection(LOOKDIR::LOOK_LEFT);
		Pstate = Run;
		m_pTransform->GoRight(-dDeltaTime);
	}
	if (CEngine::GetInstance()->IsKeyPressed(VK_RIGHT))
	{
		IdleStateDt = 0.f;
		PlayerLookDirection(LOOKDIR::LOOK_RIGHT);
		Pstate = Run;
		m_pTransform->GoRight(-dDeltaTime);
	}

	if (CEngine::GetInstance()->IsKeyDown(VK_SHIFT)) //Sprint
	{
		//vDir = PxVec3(0.1f, 0.f, 0.f);
		//fSpeed = 0.01f;
	}
	if (CEngine::GetInstance()->IsKeyDown(VK_CONTROL)) // Jump
	{
		m_bJump = true;
		m_fJumpSpeed = 15.f;
	}

	if (CEngine::GetInstance()->IsKeyDown(VK_CONTROL)) // Evade
	{
		//vDir = PxVec3(0.1f, 0.f, 0.f);
	}
	//m_pController->move(vDir, fSpeed, 1.f / dDeltaTime, nullptr);

	m_pModel->SetUp_AnimationIndex(Pstate);
}

void CToolPlayer::PlayerJump(_double dDeltaTime)
{
	PxControllerFilters filters;
	if (m_bJump)
	{
		if (m_fJumpSpeed >= 0)
		{
			m_fJumpSpeed -= _float(m_fSpeed * dDeltaTime);
		}
		else
		{
			m_bJump = false;
		}

		m_pController->move(PxVec3(0.f, 1.f * m_fJumpSpeed * (_float)dDeltaTime, 0.f), 0.01f, PxF32(1.f / dDeltaTime), filters);
	}
	else
	{
		m_fJumpSpeed -= _float(m_fSpeed * (_float)dDeltaTime);
		m_pController->move(PxVec3(0.0f, m_fJumpSpeed * (_float)dDeltaTime, 0.f), 0.01f, PxF32(1.f / dDeltaTime), filters);
	}
}
void CToolPlayer::PlayerLookDirection(LOOKDIR _estate)
{
	if (_estate == LOOK_FRONT) //angle : 0
	{//앞 방향-90
		fStateAngle = 90.f;

		if (eLookDir == LOOK_BACK)
			fStateAngle += 180.f;
		else if (eLookDir == LOOK_LEFT)
			fStateAngle += 90.f;

		else if (eLookDir == LOOK_RIGHT)
			fStateAngle -= 90.f;

	}
	else if (_estate == LOOK_BACK)
	{
		fStateAngle = -90;

		if (eLookDir == LOOK_FRONT)
			fStateAngle += 180.f;
		else if (eLookDir == LOOK_LEFT)
			fStateAngle += 90.f;
		else if (eLookDir == LOOK_RIGHT)
			fStateAngle -= 90.f;

	}
	else if (_estate == LOOK_LEFT)
	{//왼쪽 방향  기본
		fStateAngle = 0;

		if (eLookDir == LOOK_FRONT)
			fStateAngle += 90.f;
		else if (eLookDir == LOOK_BACK)
			fStateAngle -= 90.f;
		else if (eLookDir == LOOK_RIGHT)
			fStateAngle -= 180.f;

	}
	else if (_estate == LOOK_RIGHT)
	{
		fStateAngle = -180;

		if (eLookDir == LOOK_FRONT)
			fStateAngle -= 90.f;
		else if (eLookDir == LOOK_BACK)
			fStateAngle += 90.f;
		else if (eLookDir == LOOK_LEFT)
			fStateAngle += 180.f;

	}

	m_pTransform->SetUpRotation(m_pTransform->GetState(CTransform::STATE_UP), fStateAngle);
	eLookDir = _estate;
}