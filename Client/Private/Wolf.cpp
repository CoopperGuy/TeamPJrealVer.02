#include "stdafx.h"
#include "..\Public\Wolf.h"
#include "StateMachine.h"
#include "BasicCollider.h"
#include "Transform.h"
#include "MonHpVIBuffer.h"

CWolf::CWolf(CGameObject* pObj)
	: CEnemy(pObj)
{
}


CWolf * CWolf::Create(CGameObject * pObj, _float3 position)
{
	CWolf*		pInstance = new CWolf(pObj);

	if (FAILED(pInstance->Initialize(position)))
	{
		MSG_BOX("Failed to Create CWolf");
		SafeRelease(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CWolf::Free()
{
	__super::Free();
	m_pHpBar->SetDead();

}

HRESULT CWolf::Initialize(_float3 position)
{
	m_pGameObject = CEngine::GetInstance()->AddGameObjectToPrefab(CEngine::GetInstance()->GetCurSceneNumber(), "Prototype_GameObecjt_Wolf", "O_Wolf");
	m_pModel = static_cast<CModel*>(m_pGameObject->GetComponent("Com_Model"));
	m_pTransform = static_cast<CTransform*>(m_pGameObject->GetComponent("Com_Transform"));
	m_pOBBCom = static_cast<CBasicCollider*>(m_pGameObject->GetComponent("Com_OBB"));
	m_pCollider = static_cast<CCollider*>(m_pGameObject->GetComponent("Com_Collider"));
	m_pController = m_pCollider->GetController();
	m_pStat = static_cast<CStat*>(m_pGameObject->GetComponent("Com_Stat"));
	m_pStat->SetSTATE(CStat::STATES_IDEL);

	m_pTransform->SetState(CTransform::STATE_POSITION, _vector{ position.x,0.f,position.z });
	m_pCollider->SetPosition(_float3(0, 0, 0));

	SetUpAnimation();
	m_pCurState = IDLE0;
	SetIdle();


	m_pHpBar = CMonHpVIBuffer::Create(m_pGameObject);

	//CEngine::GetInstance()->AddScriptObject(this, CEngine::GetInstance()->GetCurSceneNumber());

	CGameObject* pPlayer = CEngine::GetInstance()->FindGameObjectWithName(SCENE_STATIC, "Player");
	m_pTargetTransform = dynamic_cast<CTransform*>(pPlayer->GetComponent("Com_Transform"));

	return S_OK;
}

void CWolf::Update(_double dDeltaTime)
{
	if (m_bDead)
		return;

	if (m_pCurState == DIE || m_pWolfState == DIE)
	{
		m_bDead = true;
		return;
	}
	if (!m_pGameObject)
		return;

	if (!m_pGameObject->IsActive())
		return;

	__super::Update(dDeltaTime);

	WolfStateUpdate(dDeltaTime);

	if (WolfAtt)
	{
		m_pStat->SetSTATE(CStat::STATES_ATK);
		WolfAttflow(dDeltaTime);
	}
	//SetAttack(dDeltaTime);
}
void CWolf::LateUpdate(_double dDeltaTime)
{

	if (!m_bDead) {
		if (m_bMove)
		{
			_vector vTargetPos, vPos;
			vTargetPos = m_pTargetTransform->GetState(CTransform::STATE_POSITION);
			vPos = m_pTransform->GetState(CTransform::STATE_POSITION);
			XMStoreFloat3(&m_vTargetToLook, vTargetPos - vPos);

			_vector vLook, vTargetLook;
			vLook = m_pTransform->GetState(CTransform::STATE_LOOK);
			vTargetLook = XMLoadFloat3(&m_vTargetToLook);


			PxVec3 vDir = PxVec3(0.f, 0.f, 0.f);
			PxControllerFilters filters;
			vLook = XMVectorLerp(vLook, vTargetLook, 0.5f);
			vLook = XMVectorSetY(vLook, 0.f);
			m_pTransform->SetLook(vLook);
			memcpy(&vDir, &vLook, sizeof(_float3));
			m_pController->move(vDir * 1.5f * dDeltaTime, 0.f, (_float)dDeltaTime, nullptr);
		}
		WolfSetAni(dDeltaTime);
		//WolfStateUpdate(dDeltaTime);
	}

	if (m_pStat->GetStatInfo().hp <= 0)
	{
		m_pModel->SetUp_AnimationIndex(DIE);

		if (m_pHpBar)
			m_pHpBar->SetDead();

		/*if (m_pModel->Get_isFinished()) {*/
			m_pModel->Play_Animation(0);
			this->SetDead();
			m_pGameObject->SetDead();
			m_pCollider->ReleaseController();
		//}
	}

}

void CWolf::Render()
{
}

void CWolf::SetUpAnimation()
{
	m_pModel->SetAnimationLoop((_uint)WOLFSTATE::EAT, true, false);
	m_pModel->SetAnimationLoop((_uint)WOLFSTATE::IDLE0, true, false);
	m_pModel->SetAnimationLoop((_uint)WOLFSTATE::IDLE1, true, false);
	m_pModel->SetAnimationLoop((_uint)WOLFSTATE::WALK, true, false);
	m_pModel->SetAnimationLoop((_uint)WOLFSTATE::RUN, true, false);
	m_pModel->SetAnimationLoop((_uint)WOLFSTATE::THREATEN, false, false); // 위협
	m_pModel->SetAnimationLoop((_uint)WOLFSTATE::ZTTACK, false, true);
	m_pModel->SetAnimationLoop((_uint)WOLFSTATE::STRAIGHTATACK, false, true);
	m_pModel->SetAnimationLoop((_uint)WOLFSTATE::DAMAGE, false, false);
	m_pModel->SetAnimationLoop((_uint)WOLFSTATE::DIE, false, false);
	m_pModel->SetAnimationLoop((_uint)WOLFSTATE::DEADBODY, false, false);
}

void CWolf::RotateBody(_double deltaTime)
{
}

void CWolf::SetAttack(_double dDeltaTime)
{
	//if (!WolfAtt)
		//return;

	_vector vLook, vRight, vWolfLook;
	vLook = m_pTargetTransform->GetState(CTransform::STATE_LOOK);
	vRight = m_pTargetTransform->GetState(CTransform::STATE_RIGHT);
	vWolfLook = m_pTransform->GetState(CTransform::STATE_LOOK);
	PxVec3 vDir = PxVec3(0.f, 0.f, 0.f);
	PxControllerFilters filters;
	_float fSpeed = 0.f;
	_vector vUp = m_pTransform->GetState(CTransform::STATE::STATE_UP);


	_vector WolfLook = m_pTransform->GetState(CTransform::STATE_LOOK);
	WolfLook = XMVectorLerp(WolfLook, vRight, 0.4f);
	WolfLook = XMVectorSetY(WolfLook, 0.f);
	memcpy(&vDir, &vWolfLook, sizeof(PxVec3));
	fSpeed = 0.01f;

	m_pTransform->SetLook(vWolfLook);

}

void CWolf::WolfAttflow(_double dDeltaTime)
{
	_uint keyFrame = m_pModel->GetCurrentKeyFrame();

	switch (m_pWolfState)
	{
	case Client::CWolf::IDLE0: {
		m_bMove = false;
		m_pWolfState = THREATEN;
		break;
	}
	case Client::CWolf::THREATEN: {
		WolfLookPlayer();
		m_bMove = false;
		if (m_pModel->Get_isFinished()) {
			m_pWolfState = STRAIGHTATACK;
		}
		break;
	}
	case Client::CWolf::ZTTACK:
		if (m_pModel->Get_isFinished()) {
			WolfLookPlayer();
			m_pWolfState = THREATEN;
		}
		else {
			if (keyFrame >= 30 && keyFrame <= 50)
				m_bMove = true;
			else {
				WolfLookPlayer();
				m_bMove = false;
			}
		}
		break;
	case Client::CWolf::STRAIGHTATACK:
		if (m_pModel->Get_isFinished()) {
			WolfLookPlayer();
			m_pWolfState = THREATEN;
		}
		else {
			if (keyFrame >= 30 && keyFrame <= 45)
				m_bMove = true;
			else {
				WolfLookPlayer();
				m_bMove = false;
			}
		}
		break;
	case Client::CWolf::DAMAGE:
		m_bMove = false;
		if (m_pModel->Get_isFinished())
			m_pWolfState = RUN;
		break;
	case Client::CWolf::RUN: {
		//m_bMove = true;
		break;
	}
	}
}

_float CWolf::DistanceCalculate(_vector _me, _vector _target)
{
	_float fdistance = 0.f;


	return fdistance;
}

void CWolf::WolfStateUpdate(_double dDeltaTime)
{
	_vector PlayerTF = m_pTargetTransform->GetState(CTransform::STATE_POSITION);

	_vector TargetDistance = PlayerTF - m_pTransform->GetState(CTransform::STATE_POSITION);

	_float dis = 1.2f;

	if (m_pOBBCom->Get_isHit())
		m_pWolfState == DAMAGE;

	if (m_pWolfState == DAMAGE)
	{
		if (m_pModel->Get_isFinished())
		{
			m_pWolfState = THREATEN;
		}
	}

	if (m_pWolfState != DAMAGE) {
		if (XMVectorGetX(TargetDistance) > dis || XMVectorGetZ(TargetDistance) > dis)
			SetIdle();
		else {
			SetAtt();
			//	WolfLookPlayer();
		}
		if (WolfIdle && m_pWolfState != DIE && m_pWolfState != DEADBODY) {
			SetIdle();
			m_pWolfState = IDLE0;
			m_pStat->SetSTATE(CStat::STATES_IDEL);
			m_bMove = false;
		}
	}
}

void CWolf::WolfSetAni(_double dDeltaTime)
{
	if (m_pCurState != m_pWolfState)
	{
		switch (m_pWolfState)
		{
		case Client::CWolf::EAT:
			m_pModel->SetUp_AnimationIndex(EAT);
			SetIdle();
			break;
		case Client::CWolf::IDLE0:
			m_pModel->SetUp_AnimationIndex(IDLE0);
			SetIdle();
			break;
		case Client::CWolf::IDLE1:
			m_pModel->SetUp_AnimationIndex(IDLE1);
			SetIdle();
			break;
		case Client::CWolf::WALK:
			m_pModel->SetUp_AnimationIndex(WALK);
			SetIdle();
			break;
		case Client::CWolf::RUN:
			m_pModel->SetUp_AnimationIndex(RUN);
			SetIdle();
			break;
		case Client::CWolf::THREATEN:
			m_pModel->SetUp_AnimationIndex(THREATEN);
			SetAtt();
			break;
		case Client::CWolf::ZTTACK:
			m_pModel->SetUp_AnimationIndex(ZTTACK);
			SetAtt();
			break;
		case Client::CWolf::STRAIGHTATACK:
			m_pModel->SetUp_AnimationIndex(STRAIGHTATACK);
			SetAtt();
			break;
		case Client::CWolf::DAMAGE:
			m_pModel->SetUp_AnimationIndex(DAMAGE);
			break;
		case Client::CWolf::DIE:
			m_pModel->SetUp_AnimationIndex(DIE);
			break;
		case Client::CWolf::DEADBODY:
			m_pModel->SetUp_AnimationIndex(DEADBODY);
			break;
		}
		m_pCurState = m_pWolfState;
	}

	m_pModel->Play_Animation(dDeltaTime);
	m_pModel->SetUp_AnimationIndex(m_pCurState);
	Gravity(dDeltaTime);
}

void CWolf::WolfLookPlayer()
{
	_vector		vDirection = m_pTargetTransform->GetState(CTransform::STATE_POSITION) - m_pTransform->GetState(CTransform::STATE_POSITION);

	_vector vUp = m_pTransform->GetState(CTransform::STATE_UP);			//	y축 // 외적으로 방향백터s를 구하기위해서 그리고 좌우로만 바뀌지 y축은 안바뀌니까
	_vector	vRight = XMVector3Cross(vUp, vDirection);		//

	vRight = XMVector3Normalize(vRight) * m_pTransform->GetScale(CTransform::STATE_RIGHT);	//위에서 외적한 right는 스케일이 깨져있어서 원래 사용하던 right를 대입해주자 
	_vector		vLook = XMVector3Cross(vRight, vUp);			// 위에서 바꿔준 항등행렬과 y축을 외적하기 
	vLook = XMVector3Normalize(vLook) * m_pTransform->GetScale(CTransform::STATE_LOOK);

	m_pTransform->SetState(CTransform::STATE_RIGHT, vRight);
	m_pTransform->SetState(CTransform::STATE_LOOK, vLook);

	m_pTransform->RotateAxis(_vector{ 0.f,1.f,0.f }, 90.f);
}
void CWolf::SetAtt()
{
	WolfAtt = true;
	WolfIdle = false;
}

void CWolf::SetIdle()
{
	WolfAtt = false;
	WolfIdle = true;
}

void CWolf::Set_HpBar(CMonHpVIBuffer * _hp)
{
	m_pHpBar = _hp;
}

void CWolf::Gravity(_double dDeltaTime)
{
	PxControllerFilters filters;
	if (m_pController)
		m_pController->move(PxVec3(0.0f, -1.f, 0.f), 0.01f, PxF32(1.f / dDeltaTime), filters);
}
