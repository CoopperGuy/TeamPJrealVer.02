#include "stdafx.h"
#include "Client_Struct.h"
#include "..\Public\EffectSwordAura.h"
#include "Obb.h"

USING(Client)

CEffectSwordAura::CEffectSwordAura()
{
}

CEffectSwordAura * CEffectSwordAura::Create(void * pArg, CTransform* pTransform, _float fAngle)
{
	CEffectSwordAura*		pInstance = new CEffectSwordAura();

	if (FAILED(pInstance->Initialize(pArg, pTransform, fAngle)))
	{
		MSG_BOX("Failed to Create CEffectSwordAura");
		SafeRelease(pInstance);
		return nullptr;
	}

	return pInstance;
}


HRESULT CEffectSwordAura::Initialize(void* pArg, CTransform* pTransform, _float fAngle)
{
	if (pArg != nullptr) 
	{
		m_pGameObject = (CGameObject*)pArg;
		if (m_pGameObject == nullptr)
			return E_FAIL;
								
		m_pTransform = static_cast<CTransform*>(m_pGameObject->GetComponent("Com_Transform"));
		
		_vector vPosition = pTransform->GetState(CTransform::STATE_POSITION);
		_vector vLook = pTransform->GetState(CTransform::STATE_LOOK) * -1.f;
		_vector vAxisY = XMVectorSet(0.f, 1.f, 0.f, 0.f);
		_vector vRight;
		_vector vUp;

		vLook = XMVector3Normalize(vLook);
		vRight = XMVector3Normalize(XMVector3Cross(vAxisY, vLook));
		vUp = XMVector3Normalize(XMVector3Cross(vLook, vRight));
		vPosition = vPosition + vLook * 0.1f + XMVectorSet(0.f, 0.3f, 0.f, 0.f);
		_matrix RotationMatrix = XMMatrixRotationAxis(vLook, XMConvertToRadians(fAngle));
		
		m_pTransform->SetState(CTransform::STATE_RIGHT, XMVector4Transform(vRight * m_pTransform->GetScale(CTransform::STATE_RIGHT), RotationMatrix));
		m_pTransform->SetState(CTransform::STATE_UP, XMVector4Transform(vUp * m_pTransform->GetScale(CTransform::STATE_UP), RotationMatrix));
		m_pTransform->SetState(CTransform::STATE_LOOK, XMVector4Transform(vLook * m_pTransform->GetScale(CTransform::STATE_LOOK), RotationMatrix));
		m_pTransform->SetState(CTransform::STATE_POSITION, vPosition);

		m_pObb = CObb::Create(_float3(0.f, 0.f, 0.f), _float3(1.f, 1.f, 1.f), 120.f, Engine::ID::MONSTER_EFFECT, 3.f);
		
	}
	return S_OK;
}

void CEffectSwordAura::Update(_double deltaTime)
{
	if (m_bDead)
		return;

	if (!m_pGameObject)
		return;

	m_dDeadTime += deltaTime;

	m_pTransform->GoStraight(deltaTime * -3.0);

	m_pObb->SetSize(_float3(1.8f, 0.2f, 0.5f));
	_matrix ObbTransform = XMMatrixTranslation(0.f, 0.f, -0.5f) * m_pTransform->GetWorldMatrix();
	m_pObb->SetMatrix(ObbTransform);

}

void CEffectSwordAura::LateUpdate(_double deltaTime)
{
	if (m_dDeadTime >= 3.0)
	{
		this->SetDead();
		m_pGameObject->SetDead();
	}
}

void CEffectSwordAura::Render()
{
}


void CEffectSwordAura::Free()
{
	__super::Free();
}
