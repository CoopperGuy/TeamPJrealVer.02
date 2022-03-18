#include "stdafx.h"
#include "..\Public\WaterEA.h"
#include "EffectEAFire.h"
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
	case Client::CWaterEA::ATT:
		m_pModel->SetUp_AnimationIndex(0);
		if (makeEffect) {
			auto EffectEAFire = CEngine::GetInstance()->AddGameObjectToPrefab(CEngine::GetInstance()->GetCurSceneNumber(), "Prototype_Effect_EA_Att_Fire", "Effect_EA_Att_Fire");
			CEngine::GetInstance()->AddScriptObject(m_pEffEAFire = CEffectEAFire::Create(EffectEAFire), CEngine::GetInstance()->GetCurSceneNumber());
			makeEffect = false;
		}
		if (keyFrame >= 0 && keyFrame <= 23)
		{

			_matrix worlmatrix = m_pTransform->GetWorldMatrix();
			_matrix handbone = m_pModel->Get_BoneWithoutOffset("BN_Finger_04");
			handbone = Remove_ScaleRotation(handbone * m_pTransform->GetWorldMatrix());
			if (m_pEffEAFire) {
				m_pEffEAFire->SetMatrix(handbone);
				m_pEffEAFire->SetScale(_float3{ start,start ,0.f });
			}
		}

		if (keyFrame >= 23 && keyFrame <= 31)
			m_pEffEAFire->SetScale(_float3{ float(start += plus),float(start += plus) ,0.f });
		else
			start = 0.2f;

		if (keyFrame >= 40)
		{
			if (m_pEffEAFire)
				m_pEffEAFire->SetObj();
			m_dIdleTime = 0;
			m_eState = IDLE;
			makeFireball = true;
			m_dmake = 0.f;
			fireballmake = 0;
		}
		break;
	case Client::CWaterEA::IDLE: {
		LookPlayer();
		m_pTransform->RotateAxis(_vector{ 0.f,1.f,0.f }, 90.f);
		start = 0.4f;
		m_dIdleTime += dDeltaTime;
		m_dmake += dDeltaTime;
		m_pModel->SetUp_AnimationIndex(1);
		int irand = rand() % 5;
		irand += 10;
		if (m_dIdleTime > irand) {
			m_eState = ATT;
			makeEffect = true;
		}
		if (keyFrame >= 8) {
			if (m_dmake >= 0.27f && fireballmake < 8) {
				fireballmake += 1;
				auto EffectFireBall = CEngine::GetInstance()->AddGameObjectToPrefab(CEngine::GetInstance()->GetCurSceneNumber(), "Prototype_Effect_FireBall", "E_FireBall");
				CEngine::GetInstance()->AddScriptObject(m_pEffFireball = CEffectFireBall::Create(EffectFireBall), CEngine::GetInstance()->GetCurSceneNumber());
				_vector pos = m_pTransform->GetState(CTransform::STATE_POSITION);
				pos = XMVectorSetY(pos, XMVectorGetY(pos) + 0.1f);
				pos = XMVectorSetZ(pos, XMVectorGetZ(pos) - 0.0f);
				m_pEffFireball->SetTransform(pos);
	/*				_matrix worlmatrix = m_pTransform->GetWorldMatrix();
				_matrix handbone = m_pModel->Get_BoneWithoutOffset("Bip01-Head");
				handbone = Remove_ScaleRotation(handbone * m_pTransform->GetWorldMatrix());
				if(m_pEffFireball)
					m_pEffFireball->SetMatrix(handbone);*/
				//m_pEffFireball->SetScale(_float3{ 0.5f,0.5f ,0.f });
				//makeFireball = false;
				m_dmake = 0;
			}
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

	_vector vUp = m_pTransform->GetState(CTransform::STATE_UP);			//	y�� // �������� ������͸� ���ϱ����ؼ� �׸��� �¿�θ� �ٲ��� y���� �ȹٲ�ϱ�
	_vector	vRight = XMVector3Cross(vUp, vDirection);		//

	vRight = XMVector3Normalize(vRight) * m_pTransform->GetScale(CTransform::STATE_RIGHT);	//������ ������ right�� �������� �����־ ���� ����ϴ� right�� ���������� 
	_vector		vLook = XMVector3Cross(vRight, vUp);			// ������ �ٲ��� �׵���İ� y���� �����ϱ� 
	vLook = XMVector3Normalize(vLook) * m_pTransform->GetScale(CTransform::STATE_LOOK);

	m_pTransform->SetState(CTransform::STATE_RIGHT, vRight);
	m_pTransform->SetState(CTransform::STATE_LOOK, vLook);
}