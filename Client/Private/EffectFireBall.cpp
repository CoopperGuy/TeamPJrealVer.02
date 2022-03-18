#include "stdafx.h"
#include "Client_Struct.h"
#include "..\Public\EffectFireBall.h"
#include "EmptyEffect.h"
#include "Obb.h"
USING(Client)

CEffectFireBall::CEffectFireBall()
{
}

CEffectFireBall * CEffectFireBall::Create(void * pArg)
{
	CEffectFireBall*		pInstance = new CEffectFireBall();

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Create CEffectFireBall");
		SafeRelease(pInstance);
		return nullptr;
	}

	return pInstance;
}


HRESULT CEffectFireBall::Initialize(void* pArg)
{
	if (pArg != nullptr) {

		m_pGameObject = (CGameObject*)pArg;
		if (m_pGameObject == nullptr)
			return E_FAIL;
		m_pTransform = static_cast<CTransform*>(m_pGameObject->GetComponent("Com_Transform"));
		StartPos = m_pTransform->GetState(CTransform::STATE_POSITION);

		SetFadeInOut(m_pGameObject);


		CGameObject* pPlayer = CEngine::GetInstance()->FindGameObjectWithName(SCENE_STATIC, "Player");
		m_pTargetTransform = dynamic_cast<CTransform*>(pPlayer->GetComponent("Com_Transform"));

		LookPlayer();
		//CGameObject* pTargetObj = CEngine::GetInstance()->FindGameObjectWithName(CEngine::GetInstance()->GetCurSceneNumber(), "O_WaterEA");
		//if (pTargetObj == nullptr)
		//	return E_FAIL;
		//CTransform* pTargetTransform = static_cast<CTransform*>(pTargetObj->GetComponent("Com_Transform"));
		//m_pTransform = static_cast<CTransform*>(m_pGameObject->GetComponent("Com_Transform"));
		//_vector pos = pTargetTransform->GetState(CTransform::STATE_POSITION);
		//pos = XMVectorSetY(pos, XMVectorGetY(pos) + 0.2f);
		//m_pTransform->SetState(CTransform::STATE_POSITION, pos);
		//m_fScale = { m_pTransform->GetScale(CTransform::STATE_RIGHT) , m_pTransform->GetScale(CTransform::STATE_UP) , m_pTransform->GetScale(CTransform::STATE_LOOK) };
		//CStat* stat = static_cast<CStat*>(pTargetObj->GetComponent("Com_Stat"));
		////pos ,size ,  dmg,collisiontpye, duration;
		//m_pOBB = CObb::Create(pos, XMLoadFloat3(&m_fScale), stat->GetStatInfo().atk, ID::MONSTER_EFFECT, 100.f, nullptr);
	}
	return S_OK;
}

void CEffectFireBall::Update(_double deltaTime)
{
	if (m_bDead)
		return;

	if (!m_pGameObject)
		return;
	//m_pTransform->RotateAxis(_vector{ 0.f,0.f,1.f }, -90.f);

	LookPlayer();

	if (m_pGameObject)
	{
		//m_pTransform->SetScale(_float3{ 0.5f,0.5f,0.f });
		m_pTransform->GoStraight(deltaTime*0.5f);
		m_pTransform->GoDown(deltaTime*0.2f);

		//if (0 >= XMVectorGetY(m_pTransform->GetState(CTransform::STATE_POSITION)))
		//	SetFadeInOut(m_pGameObject);
		//m_pTransform->SetUpRotation(_vector{0.f,0.1f,0.f},90.f);
	}

	if (0.15f>=static_cast<CEmptyEffect*>(m_pGameObject)->GetFadeAlpha())
	{
		m_bDead = true;
	}
}


void CEffectFireBall::LateUpdate(_double deltaTime)
{
	if (m_bDead)
	{
		this->SetDead();
		m_pGameObject->SetDead();
		//m_pOBB->SetupDead();
	}
}

void CEffectFireBall::Render()
{
}

void CEffectFireBall::SetFadeInOut(CGameObject * pObj)
{
	static_cast<CEmptyEffect*>(pObj)->SetFadeOutEnable(true);
	static_cast<CEmptyEffect*>(pObj)->SetFadeOutStartTime(0.f);
	static_cast<CEmptyEffect*>(pObj)->SetFadeOutDuration(1.5f);
	static_cast<CEmptyEffect*>(pObj)->SetEffectDuration(1.5f);
}

void CEffectFireBall::Free()
{
	__super::Free();
}

void CEffectFireBall::LookPlayer()
{
	_vector		vDirection = m_pTargetTransform->GetState(CTransform::STATE_POSITION) - m_pTransform->GetState(CTransform::STATE_POSITION);

	_vector vUp = m_pTransform->GetState(CTransform::STATE_UP);			//	y축 // 외적으로 방향백터를 구하기위해서 그리고 좌우로만 바뀌지 y축은 안바뀌니까
	_vector	vRight = XMVector3Cross(vUp, vDirection);		//

	vRight = XMVector3Normalize(vRight) * m_pTransform->GetScale(CTransform::STATE_RIGHT);	//위에서 외적한 right는 스케일이 깨져있어서 원래 사용하던 right를 대입해주자 
	_vector		vLook = XMVector3Cross(vRight, vUp);			// 위에서 바꿔준 항등행렬과 y축을 외적하기 
	vLook = XMVector3Normalize(vLook) * m_pTransform->GetScale(CTransform::STATE_LOOK);

	m_pTransform->SetState(CTransform::STATE_RIGHT, vRight);
	m_pTransform->SetState(CTransform::STATE_LOOK, vLook);
}