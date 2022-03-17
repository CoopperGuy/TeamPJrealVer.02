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
	m_eState = IDLE;
	return S_OK;
}

void CWaterEA::Update(_double dDeltaTime)
{
	if (m_bDead)
		return;

	if (CEngine::GetInstance()->Get_DIKDown(DIK_8))
	{
		m_eState = IDLE;
		indexd = 1;
	}
	if (CEngine::GetInstance()->Get_DIKDown(DIK_9))
	{
		m_eState = ATT;
		indexd = 0;
		makeEffect = true;
	}

	if(m_pEffEAFire)
		Set_FirePos();


	m_pModel->SetUp_AnimationIndex(indexd);
	m_pModel->Play_Animation(dDeltaTime);
}

void CWaterEA::LateUpdate(_double dDeltaTime)
{
	if (m_bDead)
	{
		this->SetDead();
		m_pGameObject->SetDead();
	}
	if (makeEffect) {
		auto EffectEAFire = CEngine::GetInstance()->AddGameObjectToPrefab(CEngine::GetInstance()->GetCurSceneNumber(), "Prototype_Effect_EA_Att_Fire", "Effect_EA_Att_Fire");
		CEngine::GetInstance()->AddScriptObject(m_pEffEAFire = CEffectEAFire::Create(EffectEAFire), CEngine::GetInstance()->GetCurSceneNumber());
		_matrix handbone = m_pModel->Get_BoneWithoutOffset("BN_Finger_04");
		_matrix TossMatrix = handbone * m_pTransform->GetWorldMatrix();
		m_pEffEAFire->SetMatrix(TossMatrix);
		makeEffect = false;
	}

}

void CWaterEA::Render()
{

}

void CWaterEA::Set_FirePos()
{
	if (m_eState == ATT) {
		_uint keyFrame = m_pModel->GetCurrentKeyFrame();
		if (keyFrame >= 0 && keyFrame <= 23)
		{
			_matrix handbone = m_pModel->Get_BoneWithoutOffset("BN_Finger_04");
			_matrix TossMatrix = handbone * m_pTransform->GetWorldMatrix();
			m_pEffEAFire->SetMatrix(TossMatrix);
		}

		if (keyFrame >= 24)
		{
			//m_pEffEAFire->set
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

