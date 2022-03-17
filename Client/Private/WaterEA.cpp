#include "stdafx.h"
#include "..\Public\WaterEA.h"
#include "EffectEAFire.h"

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
	m_pTransform->SetState(CTransform::STATE_POSITION, _vector{ 3.f,0.3f,-3.f });

	CEngine::GetInstance()->AddScriptObject(this, CEngine::GetInstance()->GetCurSceneNumber());

	m_pModel->SetAnimationLoop((_uint)STATE::ATT, false, false);
	m_pModel->SetAnimationLoop((_uint)STATE::IDLE, true, false);

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
		if (makeEffect) {
			auto EffectEAFire = CEngine::GetInstance()->AddGameObjectToPrefab(CEngine::GetInstance()->GetCurSceneNumber(), "Prototype_Effect_EA_Att_Fire", "Effect_EA_Att_Fire");
			CEngine::GetInstance()->AddScriptObject(m_pEffEAFire = CEffectEAFire::Create(EffectEAFire), CEngine::GetInstance()->GetCurSceneNumber());
			makeEffect = false;
		}
		m_pModel->SetUp_AnimationIndex(0);
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

		if (keyFrame >= 40)
		{
			if (m_pEffEAFire)
				m_pEffEAFire->SetObj();
			m_dIdleTime = 0;
			m_eState = IDLE;
		}
		break;
	case Client::CWaterEA::IDLE: {
		start = 0.2f;
		m_dIdleTime += dDeltaTime;
		m_pModel->SetUp_AnimationIndex(1);
		int irand = rand() % 5;
		irand += 10;
		if (m_dIdleTime > irand) {
			m_eState = ATT;
			makeEffect = true;
		}
	}break;
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
