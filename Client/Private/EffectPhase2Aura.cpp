#include "stdafx.h"
#include "Client_Struct.h"
#include "..\Public\EffectPhase2Aura.h"

USING(Client)

CEffectPhase2Aura::CEffectPhase2Aura()
{
}

CEffectPhase2Aura * CEffectPhase2Aura::Create(void * pArg, CTransform* pTransform)
{
	CEffectPhase2Aura*		pInstance = new CEffectPhase2Aura();

	if (FAILED(pInstance->Initialize(pArg, pTransform)))
	{
		MSG_BOX("Failed to Create CEffectPhase2Aura");
		SafeRelease(pInstance);
		return nullptr;
	}

	return pInstance;
}


HRESULT CEffectPhase2Aura::Initialize(void* pArg, CTransform* pTransform)
{
	if (pArg != nullptr) 
	{
		m_pGameObject = (CGameObject*)pArg;
		if (m_pGameObject == nullptr)
			return E_FAIL;
								
		m_pTransform = static_cast<CTransform*>(m_pGameObject->GetComponent("Com_Transform"));
		
		_vector vPosition = pTransform->GetState(CTransform::STATE_POSITION);
		XMVectorSetY(vPosition, 0.f);

		m_pTransform->SetState(CTransform::STATE_POSITION, vPosition);

		CEngine::GetInstance()->PlaySoundW("DarkKnight_Phase2.wav", ENEMY05);
	}
		

	return S_OK;
}

void CEffectPhase2Aura::Update(_double deltaTime)
{
	if (m_bDead)
		return;

	if (!m_pGameObject)
		return;

	m_dDeadTime += deltaTime;

	_matrix RotationMatrix = XMMatrixRotationAxis(XMVector3Normalize(m_pTransform->GetState(CTransform::STATE_UP)), XMConvertToRadians((_float)deltaTime * 4.f));
	m_pTransform->SetState(CTransform::STATE_RIGHT, XMVector4Transform(m_pTransform->GetState(CTransform::STATE_RIGHT), RotationMatrix));
	m_pTransform->SetState(CTransform::STATE_UP, XMVector4Transform(m_pTransform->GetState(CTransform::STATE_UP), RotationMatrix));
	m_pTransform->SetState(CTransform::STATE_LOOK, XMVector4Transform(m_pTransform->GetState(CTransform::STATE_LOOK), RotationMatrix));

}

void CEffectPhase2Aura::LateUpdate(_double deltaTime)
{	
	if (m_dDeadTime >= static_cast<CEmptyEffect*>(m_pGameObject)->GetEffectDuration())
	{
		this->SetDead();
		m_pGameObject->SetDead();
	}
}

void CEffectPhase2Aura::Render()
{
}


void CEffectPhase2Aura::Free()
{
	__super::Free();
}
