#include "stdafx.h"
#include "..\Public\El_Flogas.h"
#include "Flogas.h"
#include "Element_Bomb.h"

USING(Client)

CEl_Flogas::CEl_Flogas()
{
}

CEl_Flogas::CEl_Flogas(CGameObject * pObj)
	:CEnemy(pObj)
{
}

HRESULT CEl_Flogas::Initialize(string name, CFlogas * pObj)
{
	m_ScriptName = name;
	m_pFlogas = pObj;

	m_pGameObject = CEngine::GetInstance()->FindGameObjectWithName(CEngine::GetInstance()->GetCurSceneNumber(), m_ScriptName);
	if (m_pGameObject == nullptr)
		return E_FAIL;

	m_pTransform = static_cast<CTransform*>(m_pGameObject->GetComponent("Com_Transform"));
	m_pModel = static_cast<CModel*>(m_pGameObject->GetComponent("Com_Model"));
	XMStoreFloat3(&m_vOriginPos, m_pTransform->GetState(CTransform::STATE_POSITION));
	m_OriginWorld = m_pTransform->GetMatrix();
	m_pCollider = static_cast<CCollider*>(m_pGameObject->GetComponent("Com_Collider"));

	m_pModel->SetAnimationLoop((_uint)ELEMENT_STATE::DIE, false);
	if (m_pCollider)
		m_pController = m_pCollider->GetController();


	CGameObject* pTargetObj = CEngine::GetInstance()->FindGameObjectWithName(CEngine::GetInstance()->GetCurSceneNumber(), "Flogas");
	m_pTargetTransform = static_cast<CTransform*>(pTargetObj->GetComponent("Com_Transform"));
	XMStoreFloat3(&m_vTargetPos, m_pTargetTransform->GetState(CTransform::STATE_POSITION));
	m_pModel->SetUp_AnimationIndex(IDLE);
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
				m_pModel->SetUp_AnimationIndex(RUN);
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
			if (m_bExplosion)
			{
				m_pModel->SetUp_AnimationIndex(IDLE);
				if (m_dExplosionTime <= 0.0)
				{
					CGameObject* pGameObject = CEngine::GetInstance()->AddGameObjectToPrefab(CEngine::GetInstance()->GetCurSceneNumber(), "Prototype_Effect_ElementBomb", "E_Element_Bomb");
					CEngine::GetInstance()->AddScriptObject(CElement_Bomb::Create((CEmptyEffect*)pGameObject, m_pGameObject), CEngine::GetInstance()->GetCurSceneNumber());
				}
				m_dExplosionTime += dDeltaTime;

				if (m_dExplosionTime > 4.0)
				{
					m_bExplosion = false;
					m_dExplosionTime = 0.0;
					m_pModel->SetUp_AnimationIndex(DIE);
				}
			}
			else
			{
				m_pTransform->SetScale(_float3(1.5f - (_float)dDeltaTime, 1.5f - (_float)dDeltaTime, 1.5f - (_float)dDeltaTime));
				if (m_pModel->Get_isFinished())
				{
					m_bDestination = false;
					m_pModel->SetUp_AnimationIndex(DEADBODY);
					m_pGameObject->SetActive(false);
				}
			}
		}
	}
	else
	{
		m_pTransform->SetMatrix(m_OriginWorld);
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

CEl_Flogas * CEl_Flogas::Create(string name, CFlogas* pObj)
{
	CEl_Flogas*		pInstance = new CEl_Flogas();

	if (FAILED(pInstance->Initialize(name,pObj)))
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
