#include "stdafx.h"
#include "..\Public\WaterEA.h"
#include "EffectEAFire.h"
#include "EAFireBall.h"
#include "Obb.h"
#include "EffectFireBall.h"

CWaterEA::CWaterEA()
{
}

CWaterEA::CWaterEA(CGameObject* rhs)
	:CEnemy(rhs)
{
}

HRESULT CWaterEA::Initialize(_float3 position)
{
	m_pGameObject = CEngine::GetInstance()->AddGameObjectToPrefab(CEngine::GetInstance()->GetCurSceneNumber(), "Prototype_GameObecjt_WaterEA", "O_WaterEA");
	m_pModel = static_cast<CModel*>(m_pGameObject->GetComponent("Com_Model"));
	m_pTransform = static_cast<CTransform*>(m_pGameObject->GetComponent("Com_Transform"));
	m_pTransform->SetState(CTransform::STATE_POSITION, _vector{ -6.5f,0.3f,-1.f });

	CEngine::GetInstance()->AddScriptObject(this, CEngine::GetInstance()->GetCurSceneNumber());

	m_pModel->SetAnimationLoop((_uint)STATE::ATT, false, false);
	m_pModel->SetAnimationLoop((_uint)STATE::IDLE, true, false);

	CGameObject* pPlayer = CEngine::GetInstance()->FindGameObjectWithName(SCENE_STATIC, "Player");
	m_pTargetTransform = dynamic_cast<CTransform*>(pPlayer->GetComponent("Com_Transform"));

	//pos ,size ,  dmg,collisiontpye, duration;

	m_eState = IDLE;
	return S_OK;
}

void CWaterEA::Update(_double dDeltaTime)
{
	if (m_bDead)
		return;

	Set_State(dDeltaTime);


}

void CWaterEA::LateUpdate(_double dDeltaTime)
{
	if (m_bDead)
	{
		this->SetDead();
		m_pGameObject->SetDead();
	}

	m_pModel->Play_Animation(dDeltaTime * m_dAniIndex);


}

void CWaterEA::Render()
{

}


void CWaterEA::Set_State(_double dDeltaTime)
{
	_uint keyFrame = m_pModel->GetCurrentKeyFrame();
	switch (m_eState)
	{
	case Client::CWaterEA::ATT: {
		m_pModel->SetUp_AnimationIndex(0);
		m_dmake += dDeltaTime;
		_matrix worlmatrix = m_pTransform->GetWorldMatrix();
		_matrix handbone = m_pModel->Get_BoneWithoutOffset("BN_Finger_04");

		if (keyFrame >= 22 && keyFrame <= 23)
		{
			if (fireballmake <= 2  ) {
				fireballmake += 1;
				handbone = Remove_ScaleRotation(handbone * m_pTransform->GetWorldMatrix());

				auto EffectFireBall = CEngine::GetInstance()->AddGameObjectToPrefab(CEngine::GetInstance()->GetCurSceneNumber(), "Prototype_GameObecjt_EAFire", "O_EAFire");
				CEngine::GetInstance()->AddScriptObject(CEAFireBall::Create(EffectFireBall, handbone), CEngine::GetInstance()->GetCurSceneNumber());
			}
		}
		if (keyFrame >= 40)
		{
			m_dIdleTime = 0;
			m_eState = IDLE;
		}
	}
	break;
	case Client::CWaterEA::IDLE: {
		LookPlayer();
		m_pTransform->RotateAxis(_vector{ 0.f,1.f,0.f }, 90.f);
		m_dIdleTime += dDeltaTime;
		m_pModel->SetUp_AnimationIndex(1);
		int irand = rand() % 5;
		irand += 5;
		if (m_dIdleTime > irand) {
			m_eState = ATT;
			fireballmake = 0;
			makeEffect = true;
		}
		break;
	}
	}
}

CWaterEA * CWaterEA::Create(CGameObject * pObj, _float3 position)
{
	CWaterEA*		pInstance = new CWaterEA(pObj);
	if (FAILED(pInstance->Initialize(position)))
	{
		MSG_BOX("Failed to Create WaterEA");
		SafeRelease(pInstance);
		return nullptr;
	}
	return pInstance;
}

void CWaterEA::Free()
{

}
_fmatrix CWaterEA::Remove_ScaleRotation(_fmatrix TransformMatrix)
{
	_matrix			NonRotateMatrix = XMMatrixIdentity();

	NonRotateMatrix.r[3] = TransformMatrix.r[3];

	return NonRotateMatrix;
}

void CWaterEA::LookPlayer()
{
	_vector		vDirection = m_pTargetTransform->GetState(CTransform::STATE_POSITION) - m_pTransform->GetState(CTransform::STATE_POSITION);

	_vector vUp = m_pTransform->GetState(CTransform::STATE_UP);			//	y축 // 외적으로 방향백터s를 구하기위해서 그리고 좌우로만 바뀌지 y축은 안바뀌니까
	_vector	vRight = XMVector3Cross(vUp, vDirection);		//

	vRight = XMVector3Normalize(vRight) * m_pTransform->GetScale(CTransform::STATE_RIGHT);	//위에서 외적한 right는 스케일이 깨져있어서 원래 사용하던 right를 대입해주자 
	_vector		vLook = XMVector3Cross(vRight, vUp);			// 위에서 바꿔준 항등행렬과 y축을 외적하기 
	vLook = XMVector3Normalize(vLook) * m_pTransform->GetScale(CTransform::STATE_LOOK);

	m_pTransform->SetState(CTransform::STATE_RIGHT, vRight);
	m_pTransform->SetState(CTransform::STATE_LOOK, vLook);
}