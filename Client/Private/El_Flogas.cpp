#include "stdafx.h"
#include "..\Public\El_Flogas.h"
#include "Flogas.h"
#include "Element_Bomb.h"
#include "SpriteFire.h"
#include "EffectFlash.h"
#include "MonHp.h"

#include "Obb.h"

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
	if (name == "El_Flogas01")
	{
		m_bRight = true;
		m_bFront = true;
	}
	else if (name == "El_Flogas02")
	{
		m_bRight = false;
		m_bFront = true;
	}
	else if (name == "El_Flogas03")
	{
		m_bRight = true;
		m_bFront = false;
	}
	else if (name == "El_Flogas04")
	{
		m_bRight = false;
		m_bFront = false;
	}
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
	m_pStat = static_cast<CStat*>(m_pGameObject->GetComponent("Com_Stat"));
	m_pModel->SetAnimationLoop((_uint)ELEMENT_STATE::DIE, false);
	if (m_pCollider)
		m_pController = m_pCollider->GetController();
	m_pMonHp = CMonHp::Create(m_pGameObject);

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
	m_pStat->SetSTATE(m_eCurSTATES);
	vTargetPos = XMLoadFloat3(&m_vTargetPos);
	vPos = m_pTransform->GetState(CTransform::STATE_POSITION);
	vDist = vTargetPos - vPos;
	fLen = XMVectorGetX(XMVector3Length(vDist));
	if (m_pStat->GetStatInfo().hp < 0)
		m_bDead = true;

	if (m_pGameObject->GetActive())
	{
		PxVec3 vDir = PxVec3(0.f, 0.f, 0.f);
		if (!m_bMove)
		{
			if (fLen > 1.f)
			{
				if (!m_bDead)
				{
					m_pModel->SetUp_AnimationIndex(RUN);
					vDist = XMVectorSetY(vDist, 0.f);
					m_pTransform->SetLook(vDist);
					memcpy(&vDir, &XMVector3Normalize(vDist), sizeof(_float3));
					m_pController->move(vDir * 0.4f * (_float)dDeltaTime, 0.f, (_float)dDeltaTime, nullptr);
				}
				else
				{
					m_pModel->SetUp_AnimationIndex(DIE);
					m_fScale -= (_float)dDeltaTime;
					m_pTransform->SetScale(_float3(m_fScale, m_fScale, m_fScale));
					
					if (m_pModel->Get_isFinished(DIE))
					{
						_float3 vPos = {};
						XMStoreFloat3(&vPos, XMVectorSetY(m_pTransform->GetState(CTransform::STATE_POSITION), -5.f));
						m_pCollider->SetPosition(vPos);
						m_bMove = false;
						m_pModel->SetUp_AnimationIndex(DEADBODY);
						m_bDestination = true;
					}
				}
			
			}
			else
			{
				m_bMove = true;
				m_bDestination = true;
			}
		}
		else
		{
			if (m_bDead)
			{
				if(!m_pFlogas->Get_Flying())
					m_pGameObject->SetActive(false);
			}
			else if (m_bExplosion)
			{
				m_pModel->SetUp_AnimationIndex(IDLE);
				if (m_dExplosionTime <= 0.0)
				{
					CGameObject* pGameObject = CEngine::GetInstance()->AddGameObjectToPrefab(CEngine::GetInstance()->GetCurSceneNumber(), "Prototype_Effect_ElementBomb", "E_Element_Bomb");
					CEngine::GetInstance()->AddScriptObject(CElement_Bomb::Create((CEmptyEffect*)pGameObject, m_pGameObject,*this), CEngine::GetInstance()->GetCurSceneNumber());
				}
				m_dExplosionTime += dDeltaTime;
				m_fScale += (_float)dDeltaTime * 0.1f;
				m_pTransform->SetScale(_float3(m_fScale, m_fScale, m_fScale));
				if (m_dExplosionTime > 3.0)
				{
					m_bDeadMotion = true;
					m_bExplosion = false;
					m_dExplosionTime = 0.0;
					m_pModel->SetUp_AnimationIndex(DIE);
				}
			}
			else
			{
				if (m_bDeadMotion)
				{
					m_fScale -= (_float)dDeltaTime;
					m_pTransform->SetScale(_float3(m_fScale, m_fScale, m_fScale));
					if (m_pModel->Get_isFinished(DIE))
					{
						CGameObject* pGameObject = CEngine::GetInstance()->AddGameObjectToPrefab(CEngine::GetInstance()->GetCurSceneNumber(), "Prototype_E_Flash", "E_Flash");
						CEngine::GetInstance()->AddScriptObject(CEffectFlash::Create((CEmptyEffect*)pGameObject, m_pGameObject, *this), CEngine::GetInstance()->GetCurSceneNumber());
				
						m_bMove = false;
						m_pModel->SetUp_AnimationIndex(DEADBODY);
						m_pGameObject->SetActive(false);
					}
				}
			}
		}
	}
	else
	{
		m_fScale = 1.5f;
		m_bDeadMotion = false;
		m_bMove = false;
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
