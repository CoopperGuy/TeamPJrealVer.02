#include "stdafx.h"
#include "..\Public\El_Flogas.h"
#include "Flogas.h"


USING(Client)

CEl_Flogas::CEl_Flogas()
{
}

CEl_Flogas::CEl_Flogas(CGameObject* pObj)
{
}

HRESULT CEl_Flogas::Initialize(string name)
{
	m_ScriptName = name;

	m_pGameObject = CEngine::GetInstance()->FindGameObjectWithName(CEngine::GetInstance()->GetCurSceneNumber(), m_ScriptName);
	if (m_pGameObject == nullptr)
		return E_FAIL;

	m_pTransform = static_cast<CTransform*>(m_pGameObject->GetComponent("Com_Transform"));
	m_pModel = static_cast<CModel*>(m_pGameObject->GetComponent("Com_Model"));
	XMStoreFloat3(&m_vOriginPos, m_pTransform->GetState(CTransform::STATE_POSITION));
	m_pCollider = static_cast<CCollider*>(m_pGameObject->GetComponent("Com_Collider"));
	if (m_pCollider)
		m_pController = m_pCollider->GetController();
	CGameObject* pTargetObj = CEngine::GetInstance()->FindGameObjectWithName(CEngine::GetInstance()->GetCurSceneNumber(), "Flogas");
	m_pTargetTransform = static_cast<CTransform*>(pTargetObj->GetComponent("Com_Transform"));
	XMStoreFloat3(&m_vTargetPos, m_pTargetTransform->GetState(CTransform::STATE_POSITION));
	m_pModel->SetUp_AnimationIndex(0);
	m_pGameObject->SetActive(false);
	return S_OK;
}

void CEl_Flogas::Update(_double dDeltaTime)
{
	_vector vTargetPos, vPos, vDist;
	_float fLen;
	
	vTargetPos = XMLoadFloat3(&m_vTargetPos);
	vPos = m_pTransform->GetState(CTransform::STATE_POSITION);
	vDist = vTargetPos - vPos;
	fLen = XMVectorGetX(XMVector3Length(vDist));
	if (m_pGameObject->GetActive())
	{
		PxVec3 vDir = PxVec3(0.f, 0.f, 0.f);
		if (!m_bDestination)
		{
			if (fLen > 1.f)
			{
				m_pModel->SetUp_AnimationIndex(0);
				vDist = XMVectorSetY(vDist, 0.f);
				m_pTransform->SetLook(vDist);
				memcpy(&vDir, &XMVector3Normalize(vDist), sizeof(_float3));
				m_pController->move(vDir * 0.4f * (_float)dDeltaTime, 0.f, (_float)dDeltaTime, nullptr);
			}
			else
				m_bDestination = true;
		}
		else
		{
			m_pModel->SetUp_AnimationIndex(1);
			if (m_bExplosion)
			{
				//터지는 이펙트 추가//
				m_pGameObject->SetActive(false);
				m_bDestination = false;
				m_bExplosion = false;
			}
		}
	}
	else
	{
		m_pTransform->SetState(CTransform::STATE_POSITION, XMLoadFloat3(&m_vOriginPos));
		m_pCollider->SetPosition(m_vOriginPos);
	}
}

void CEl_Flogas::LateUpdate(_double dDeltaTime)
{
	m_pModel->Play_Animation(dDeltaTime * 1.5f);
}

void CEl_Flogas::Render()
{
}

CEl_Flogas * CEl_Flogas::Create(string name, CGameObject* pObj)
{
	CEl_Flogas*		pInstance = new CEl_Flogas(pObj);

	if (FAILED(pInstance->Initialize(name)))
	{
		MSG_BOX("Failed to Create CEl_Flogas");
		SafeRelease(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CEl_Flogas::Free()
{
	__super::Free();
}
