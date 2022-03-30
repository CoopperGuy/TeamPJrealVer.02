#include "stdafx.h"
#include "..\Public\El_Flogas.h"
#include "Flogas.h"

#include "Element_Bomb.h"
#include "Bomb_Center.h"
#include "SpriteFire.h"
#include "EffectFlash.h"

#include "MonHpVIBuffer.h"
#include "BasicCollider.h"
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
	m_pOBB = static_cast<CBasicCollider*>(m_pGameObject->GetComponent("Com_OBB"));
	m_pModel->SetAnimationLoop((_uint)ELEMENT_STATE::DIE, false);
	m_pModel->SetAnimationLoop((_uint)ELEMENT_STATE::DEADBODY, false);
	if (m_pCollider)
		m_pController = m_pCollider->GetController();
	_float3 vPos = m_vOriginPos;
	vPos.y = -5.f;
	m_pCollider->SetPosition(vPos);
	m_pMonHp = CMonHpVIBuffer::Create(m_pGameObject);

	CGameObject* pTargetObj = CEngine::GetInstance()->FindGameObjectWithName(CEngine::GetInstance()->GetCurSceneNumber(), "Flogas");
	m_pTargetTransform = static_cast<CTransform*>(pTargetObj->GetComponent("Com_Transform"));
	XMStoreFloat3(&m_vTargetPos, m_pTargetTransform->GetState(CTransform::STATE_POSITION));
	XMStoreFloat3(&m_vEffectDist, m_pTransform->GetState(CTransform::STATE_POSITION) - XMLoadFloat3(&m_vTargetPos));
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
	{
		m_bHpzero = true;
	}
	if (m_pGameObject->GetActive())
	{
		if (m_bMove)
		{
			m_pCollider->SetPosition(m_vOriginPos);
			m_bMove = false;
		}
		PxVec3 vDir = PxVec3(0.f, 0.f, 0.f);
		if (!m_bDestination)
		{
			if (m_bHpzero)
			{
				m_bExplosion = true;
				m_bDestination = true;
			}
			else if (fLen > 1.f)
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
				if (m_bHpzero)
				{
					m_bDeadMotion = true;
					m_bExplosion = false;
					m_dExplosionTime = 0.0;
					m_pModel->SetUp_AnimationIndex(DIE);
				}
				else
				{
					if (m_dExplosionTime <= 0.0)
					{
						//CGameObject* pGameObject = CEngine::GetInstance()->AddGameObjectToPrefab(CEngine::GetInstance()->GetCurSceneNumber(), "Prototype_Effect_ElementBomb", "E_Element_Bomb");
						//CEngine::GetInstance()->AddScriptObject(CElement_Bomb::Create((CEmptyEffect*)pGameObject, m_pGameObject, *this), CEngine::GetInstance()->GetCurSceneNumber());
						//CEngine::GetInstance()->AddScriptObject(CElement_Bomb::Create((CEmptyEffect*)pGameObject, m_pGameObject, *this, 90.f), CEngine::GetInstance()->GetCurSceneNumber());
						CGameObject* pGameObject = CEngine::GetInstance()->AddGameObjectToPrefab(CEngine::GetInstance()->GetCurSceneNumber(), "Prototype_Effect_Bomb_Center", "E_Bomb_Center");
						CEngine::GetInstance()->AddScriptObject(CBomb_Center::Create((CEmptyEffect*)pGameObject, m_pTransform, *this), CEngine::GetInstance()->GetCurSceneNumber());
					}
					m_dExplosionTime += dDeltaTime;

					if (m_dExplosionTime > 3.0)
					{
						m_bDeadMotion = true;
						m_bExplosion = false;
						m_dExplosionTime = 0.0;
						m_pModel->SetUp_AnimationIndex(DIE);
					}
				}
			}
			else
			{
				if (m_bDeadMotion)
				{
					/*m_fScale -= (_float)dDeltaTime;
					m_pTransform->SetScale(_float3(m_fScale, m_fScale, m_fScale));*/
					if (!m_bHpzero)
					{
						if (m_pModel->GetCurrentKeyFrame() >= 1)
						{
							if (m_bCreateEffect)
							{
								m_dCreatTime += dDeltaTime;
								if (m_iEffectCount > 7)
								{
									m_bCreateEffect = false;
									m_iEffectCount = 0;
									m_dCreatTime = 0.0;
								}
								else if (m_dCreatTime > 0.2)
								{
									CGameObject* pGameObject = CEngine::GetInstance()->AddGameObjectToPrefab(CEngine::GetInstance()->GetCurSceneNumber(), "Prototype_E_Flash", "E_Flash");
									CEngine::GetInstance()->AddScriptObject(CEffectFlash::Create((CEmptyEffect*)pGameObject, m_pGameObject, *this), CEngine::GetInstance()->GetCurSceneNumber());
									m_dCreatTime = 0.f;
									++m_iEffectCount;
								}

							}
						}
					}
					if (m_pModel->Get_isFinished(DIE))
					{
						if (m_bHpzero)
						{
							_float3 vPos = {};
							XMStoreFloat3(&vPos, m_pTransform->GetState(CTransform::STATE_POSITION));
							vPos.y = -5.f;
							m_pModel->SetUp_AnimationIndex(DEADBODY);
							m_pStat->HpHeal(100.f);
							m_pGameObject->SetActive(m_pFlogas->Get_Flying());
							m_pCollider->SetPosition(vPos);
						}
						else
						{
							m_pModel->SetUp_AnimationIndex(DEADBODY);
							m_pGameObject->SetActive(false);
						}
					}
				}
			}
		}
	}
	else
	{
		m_pStat->HpHeal(100.f);
		m_fScale = 1.2f;
		m_bDeadMotion = false;
		m_bDestination = false;
		m_bExplosion = false;
		m_bMove = true;
		m_bCreateEffect = true;
		m_bHpzero = false;
		m_pTransform->SetMatrix(m_OriginWorld);
		_float3 vPos = m_vOriginPos;
		vPos.y = -5.f;
		m_pTransform->SetState(CTransform::STATE_POSITION, XMLoadFloat3(&vPos));
		m_pCollider->SetPosition(vPos);
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
