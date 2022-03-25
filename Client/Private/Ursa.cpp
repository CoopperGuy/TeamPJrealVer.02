#include "stdafx.h"
#include "..\Public\Ursa.h"
#include "BasicCollider.h"
#include "Transform.h"
#include "MonHp.h"

#include "Obb.h"


USING(Client)

CUrsa::CUrsa(CGameObject* pObj)
	: CEnemy(pObj)
{
}


CUrsa * CUrsa::Create(CGameObject * pObj, _float3 position)
{
	CUrsa*		pInstance = new CUrsa(pObj);

	if (FAILED(pInstance->Initialize(position)))
	{
		MSG_BOX("Failed to Create CUrsa");
		SafeRelease(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CUrsa::Free()
{
	__super::Free();
}

HRESULT CUrsa::Initialize(_float3 position)
{
	m_pGameObject = CEngine::GetInstance()->FindGameObjectWithName(CEngine::GetInstance()->GetCurSceneNumber(), "Ursa");
	if (m_pGameObject == nullptr)
		return E_FAIL;

	m_pTransform = static_cast<CTransform*>(m_pGameObject->GetComponent("Com_Transform"));
	m_pModel = static_cast<CModel*>(m_pGameObject->GetComponent("Com_Model"));
	m_pCollider = static_cast<CCollider*>(m_pGameObject->GetComponent("Com_Collider"));
	if (m_pCollider)
		m_pController = m_pCollider->GetController();
	m_pStat = static_cast<CStat*>(m_pGameObject->GetComponent("Com_Stat"));

	CGameObject* pTargetObj = CEngine::GetInstance()->FindGameObjectWithName(SCENE_STATIC, "Player");
	m_pTargetTransform = static_cast<CTransform*>(pTargetObj->GetComponent("Com_Transform"));

	m_pModel->SetAnimationLoop((_uint)Ursa::CB_Start, false);
	m_pModel->SetAnimationLoop((_uint)Ursa::DASH_ATT, false, true);
	m_pModel->SetAnimationLoop((_uint)Ursa::L_SLASH, false, true);
	m_pModel->SetAnimationLoop((_uint)Ursa::R_SLASH, false, true);
	m_pModel->SetAnimationLoop((_uint)Ursa::Combo_1Start, false);
	m_pModel->SetAnimationLoop((_uint)Ursa::Combo_1, false, true);
	m_pModel->SetAnimationLoop((_uint)Ursa::Combo_1End, false);
	m_pModel->SetAnimationLoop((_uint)Ursa::Combo_2Start, false, true);
	m_pModel->SetAnimationLoop((_uint)Ursa::Combo_2End, false, true);
	m_pModel->SetAnimationLoop((_uint)Ursa::Combo_3Start, false, true);
	m_pModel->SetAnimationLoop((_uint)Ursa::Combo_3End, false, true);
	m_pModel->SetAnimationLoop((_uint)Ursa::Combo_4Start, false, true);
	m_pModel->SetAnimationLoop((_uint)Ursa::Combo_4End, false, true);
	m_pModel->SetAnimationLoop((_uint)Ursa::Big_SLASH, false, true);
	m_pModel->SetAnimationLoop((_uint)Ursa::AXE_STAMP, false, true);
	m_pModel->SetAnimationLoop((_uint)Ursa::PUMMEL_1, false);
	m_pModel->SetAnimationLoop((_uint)Ursa::PUMMEL_2, false);
	m_pModel->SetAnimationLoop((_uint)Ursa::ROAR_End, false, true);
	m_pModel->SetAnimationLoop((_uint)Ursa::DASH_ATTSpeedup, false, true);
	m_pModel->SetAnimationLoop((_uint)Ursa::WHEELWIND_Start, false);
	m_pModel->SetAnimationLoop((_uint)Ursa::WHEELWIND_End, false);
	m_pModel->SetAnimationLoop((_uint)Ursa::ROAR_Start, false);
	m_pModel->SetAnimationLoop((_uint)Ursa::HIT, false);
	m_pModel->SetAnimationLoop((_uint)Ursa::DIE, false);
	m_pModel->SetAnimationLoop((_uint)Ursa::DEADBODY, false);
	m_pModel->SetAnimationLoop((_uint)Ursa::Flying_Start, false);
	m_pModel->SetAnimationLoop((_uint)Ursa::Flying_Land, false, true);
	m_pModel->SetAnimationLoop((_uint)Ursa::Flying_End, false);

	m_eState = IDLE01;
	m_pModel->SetUp_AnimationIndex((_uint)m_eState);
	m_pMonHp = CMonHp::Create(m_pGameObject);
	XMStoreFloat3(&m_vCenterPos, m_pTransform->GetState(CTransform::STATE_POSITION));

	return S_OK;
}

void CUrsa::Update(_double dDeltaTime)
{
	if (!m_pGameObject)
		return;

	if (!m_pGameObject->IsActive())
		return;
	//if (m_pStat->GetStatInfo().hp <= 0)
	//	m_pGameObject->SetDead();

	__super::Update(dDeltaTime);

	m_fDist = SetDistance();

	//TestAnimation(Flying_End);
	Checking_Phase(dDeltaTime);
	//if (m_bCombat[First])
	//{
	//	if (!m_bCB)
	//		Adjust_Dist(dDeltaTime);
	//}


	if (CEngine::GetInstance()->Get_DIKDown(DIK_P))
		m_bCombat[First] = true;
	if (CEngine::GetInstance()->Get_DIKDown(DIK_O))
		Roar();
	if (CEngine::GetInstance()->Get_DIKDown(DIK_I))
	{
		m_bCombat[Second] = true;
		m_bCombat[First] = false;
	}
	Execute_Pattern(dDeltaTime);

	/*if(!m_bWheelWind && !m_bRoar)*/
		Checking_Finished();

	if(m_bCB)
		SetUp_Combo();
	m_pModel->SetUp_AnimationIndex((_uint)m_eState);
	m_pStat->SetSTATE(m_eCurSTATES);
	if (m_pCollider) 
		PxExtendedVec3 footpos = m_pCollider->GetController()->getFootPosition();

	//fall down
	/*PxControllerFilters filters;
	m_pController->move(PxVec3(0.0f, -0.1f, 0.f), 0.01f, PxF32(1.f / dDeltaTime), filters);*/
}

void CUrsa::LateUpdate(_double dDeltaTime)
{
	__super::LateUpdate(dDeltaTime);

	m_pModel->Play_Animation(dDeltaTime);

}

void CUrsa::Render()
{
}

void CUrsa::Empty_queue()
{
	if (!m_QueState.empty())
	{
		_uint size = (_uint)m_QueState.size();
		for (_uint i = 0; i < size; ++i)
			m_QueState.pop();
	}
}

void CUrsa::Adjust_Dist(_double dDeltaTime)
{
	_vector vLook, vTargetLook;
	vLook = m_pTransform->GetState(CTransform::STATE_LOOK);
	vTargetLook = XMLoadFloat3(&m_vTargetToLook);
	PxVec3 vDir = PxVec3(0.f, 0.f, 0.f);
	PxControllerFilters filters;
	if (m_bRoar)
	{
		m_bFar = false;
		m_bMove = false;
		m_bClose = false;
		vDir = OriginShift();
		if (!m_bCenter)
			m_pController->move(vDir * 1.f * dDeltaTime, 0.0001f, (_float)dDeltaTime, nullptr);

	}
	else if (m_bWheelWind)
	{
		if (m_eState == WHEELWIND_Ing)
		{
			if (m_fDist >= 1.f)
			{
				m_bMove = true;
				m_bClose = false;
			}
			else
				m_bClose = true;
		}
	}
	else if (m_fDist >= 3.f)
	{
		m_bSuperFar = true;
		m_bFar = false;
		m_bMove = false;
		m_bClose = false;
	}
	else if (m_fDist >= 2.f)
	{
		if (!m_bSuperFar)
		{
			m_bFar = true;
			m_bSuperFar = false;
			m_bMove = false;
			m_bClose = false;
		}
	}
	else if (m_fDist >= 1.f)
	{
		if (!m_bFar && !m_bSuperFar)
		{
			m_bMove		= true;
			m_bClose	= false;
			m_bSuperFar = false;
		}
	}
	else
	{
		if (!m_bFar && !m_bSuperFar)
		{
			m_bClose	= true;
			m_bMove		= false;
			m_bSuperFar = false;
		}
	}

	if (m_bMove && !m_bFinishBlow)
	{
		if(!m_bWheelWind)
			m_eState = RUN;

		vLook = XMVectorLerp(vLook, vTargetLook, 0.5f);
		vLook = XMVectorSetY(vLook, 0.f);
		m_pTransform->SetLook(vLook);
		memcpy(&vDir, &vLook, sizeof(_float3));
		
		m_pController->move(vDir * 1.f * dDeltaTime, 0.0001f, (_float)dDeltaTime, nullptr);

	}
}

void CUrsa::Checking_Phase(_double dDeltaTime)
{
	_float Max = m_pStat->GetStatInfo().maxHp;
	if (m_bFinishBlow)
	{
		if(m_bDelay)
			m_dPatternTime += dDeltaTime;
		
		if (m_pModel->Get_isFinished())
		{
			if(!m_bDelay)
				SetRotate();
			m_bDelay = true;
			m_eState = IDLE_CB;
		}
		
		if (m_dPatternTime > 0.5)
		{
			m_dPatternTime = 0.0;
			m_bSuperFar	   = false;
			m_bFar		   = false;
			m_bCB		   = false;
			m_bClose	   = false;
			m_bDelay	   = false;
			m_bRoar		   = false;
			m_bCenter	   = false;
			m_bFinishBlow  = false;
		}
	}
	if (m_pStat->GetStatInfo().hp < Max)
	{

		if (m_pStat->GetStatInfo().hp <= 0)
		{
			memset(m_bCombat, false, sizeof(m_bCombat));
			m_bDeadMotion = true;
		}
		else
		{
			if (!m_bCB || m_bWheelWind)
				Adjust_Dist(dDeltaTime);
			if (m_iFirst == 0)
				Roar();
			++m_iFirst;
			m_bCombat[First] = true;
			if (m_pStat->GetStatInfo().hp < Max * 0.7f)
			{
				if (m_iSec == 0)
					Roar();
				++m_iSec;
				memset(m_bCombat, false, sizeof(m_bCombat));
				m_bCombat[Second] = true;
				if (m_pStat->GetStatInfo().hp < Max * 0.4f)
				{
					if (m_iThir == 0)
					{
						Roar();
						m_bWheelWind = true;
					}
					++m_iThir;
					memset(m_bCombat, false, sizeof(m_bCombat));
					m_bCombat[Third] = true;
					if (m_pStat->GetStatInfo().hp < Max * 0.1f)
						if (m_iLast == 0)
							Roar();
					++m_iLast;
					m_bCombat[Last] = true;
				}
			}
		}
	}
}

void CUrsa::Execute_Pattern(_double dDeltaTime)
{
	if(m_bDeadMotion)
	{
		Empty_queue();
		if (m_pMonHp)
		{
			m_pMonHp->SetRelease();
			m_pMonHp = nullptr;
		}
		m_eState = DIE;
		if (m_pModel->Get_isFinished(DIE))
			m_eState = DEADBODY;
	}
	else
	{
		if (m_bRoar)
		{
			if (m_QueState.empty() && !m_bFinishBlow)
			{
				++m_iComboIndex;
				m_bCB = true;
				if (m_iComboIndex == 1)
					m_iComboIndex = 0;
			}
		}
		else if (m_bSuperFar)
		{
			if (m_QueState.empty() && !m_bFinishBlow)
			{
				m_QueState.push(Flying_Start);
				m_QueState.push(Flying_Land);
				m_QueState.push(Flying_End);

				_vector vTargetLook = XMLoadFloat3(&m_vTargetToLook);
				vTargetLook = XMVectorSetY(vTargetLook, 0.f);
				m_pTransform->SetLook(vTargetLook);
			}
		}
		else if (m_bFar)
		{
			if (m_QueState.empty() && !m_bFinishBlow)
			{
				if (!m_bCombat[Last])
					m_QueState.push(DASH_ATT);
				else
					m_QueState.push(DASH_ATTSpeedup);

				_vector vTargetLook = XMLoadFloat3(&m_vTargetToLook);
				vTargetLook = XMVectorSetY(vTargetLook, 0.f);
				m_pTransform->SetLook(vTargetLook);
			}
		}
		else if (m_bWheelWind)
		{
			if (m_QueState.empty() && !m_bFinishBlow)
			{
				m_QueState.push(WHEELWIND_Start);
				m_QueState.push(WHEELWIND_Ing);
				m_QueState.push(WHEELWIND_End);
				m_eState = m_QueState.front();
				m_QueState.pop();
			}
			else if (m_pModel->Get_isFinished())
			{
				if (m_eState == WHEELWIND_Start)
				{
					m_eState = m_QueState.front();
					m_QueState.pop();
					m_bSkillDelay = true;
				}
			}

			if (m_bSkillDelay)
				m_dWheelWindTime += dDeltaTime;

			if (m_dWheelWindTime > 5.0)
			{
				m_dWheelWindTime = 0.0;
				m_bSkillDelay = false;
				m_bWheelWind = false;
			}
		}
		else
		{
			if (m_bClose)
			{
				if (m_bCombat[First])
					First_Phase(dDeltaTime);

				else if (m_bCombat[Second])
					Second_Phase(dDeltaTime);

				else if (m_bCombat[Third])
					Third_Phase(dDeltaTime);
			}
		}
	}
}

void CUrsa::First_Phase(_double dDeltaTime)
{
	if (m_QueState.empty() && !m_bFinishBlow)
	{
		++m_iComboIndex;
		m_bCB = true;
		if (m_iComboIndex > 3)
			m_iComboIndex = 0;

	}
}

void CUrsa::Second_Phase(_double dDeltaTime)
{
	if (m_QueState.empty() && !m_bFinishBlow)
	{
		++m_iComboIndex;
		m_bCB = true;
		if (m_iComboIndex > 5)
		{
			m_iComboIndex = 0;
		}
	}
}

void CUrsa::Third_Phase(_double dDeltaTime)
{
	if (m_bWheelWind)
	{
		if (m_QueState.empty() && !m_bFinishBlow)
		{
			m_QueState.push(WHEELWIND_Start);
			m_QueState.push(WHEELWIND_Ing);
			m_QueState.push(WHEELWIND_End);
			m_eState = m_QueState.front();
			m_QueState.pop();
		}
		else if (m_pModel->Get_isFinished())
		{
			if (m_eState == WHEELWIND_Start)
			{
				m_eState = m_QueState.front();
				m_QueState.pop();
				m_bSkillDelay = true;
			}
		}

		if (m_bSkillDelay)
			m_dWheelWindTime += dDeltaTime;

		if (m_dWheelWindTime > 5.0)
		{
			m_dWheelWindTime = 0.0;
			m_bSkillDelay = false;
			m_bWheelWind = false;
		}
	}
	else if (m_QueState.empty() && !m_bFinishBlow)
	{
		++m_iComboIndex;
		m_bCB = true;
		if (m_bAddRand)
		{
			if (m_iComboIndex > 6)
			{
				m_iComboIndex = 0;
				m_bWheelWind = true;
				m_bAddRand = false;
			}
		}
		else
		{
			if (m_iComboIndex > 5)
			{
				m_iComboIndex = 0;
				m_bWheelWind = true;
			}
		}
	}
}

void CUrsa::SetUp_Combo()
{
	_vector vTargetLook = XMLoadFloat3(&m_vTargetToLook);
	_int Drawing = rand() % 100;
	if (m_bRoar)
	{
		m_bCB = false;
		m_bFinishBlow = false;
		m_iComboIndex = 0;
		if (m_bCombat[First])
		{
			m_QueState.push(ROAR_Casting);
		}
		else
		{
			m_QueState.push(ROAR_Start);
			m_QueState.push(ROAR_ING);
			m_QueState.push(ROAR_ING);
			m_QueState.push(ROAR_End);
		}
	}
	if (m_QueState.empty())
	{
		if (!m_bFinishBlow)
		{
			if (m_bCombat[First])
			{
				switch (m_iComboIndex)
				{
				case 1:
					m_QueState.push(Combo_1Start);
					m_QueState.push(Combo_1Hold);
					m_QueState.push(Combo_1);
					m_QueState.push(Combo_2Start);
					m_QueState.push(Combo_2End);
					m_eState = m_QueState.front();
					m_QueState.pop();
					vTargetLook = XMVectorSetY(vTargetLook, 0.f);
					m_pTransform->SetLook(vTargetLook);
					break;
				case 2:
					m_QueState.push(Combo_1Start);
					m_QueState.push(Combo_1Hold);
					m_QueState.push(Combo_1);
					m_QueState.push(R_SLASH);
					m_eState = m_QueState.front();
					m_QueState.pop();
					vTargetLook = XMVectorSetY(vTargetLook, 0.f);
					m_pTransform->SetLook(vTargetLook);
					break;
				case 3:
					m_QueState.push(Big_SLASH);
					vTargetLook = XMVectorSetY(vTargetLook, 0.f);
					m_pTransform->SetLook(vTargetLook);
					break;
				default:
					break;
				}

			}
			else if (m_bCombat[Second])
			{
				switch (m_iComboIndex)
				{
				case 1:
					m_QueState.push(Combo_1Start);
					m_QueState.push(Combo_1Hold);
					m_QueState.push(Combo_1);
					m_QueState.push(Combo_2Start);
					m_QueState.push(Combo_3Start);
					m_QueState.push(Combo_3End);
					m_eState = m_QueState.front();
					m_QueState.pop();
					vTargetLook = XMVectorSetY(vTargetLook, 0.f);
					m_pTransform->SetLook(vTargetLook);
					break;
				case 2:
					m_QueState.push(Combo_1Start);
					m_QueState.push(Combo_1Hold);
					m_QueState.push(Combo_1);
					m_QueState.push(R_SLASH);
					m_eState = m_QueState.front();
					m_QueState.pop();
					vTargetLook = XMVectorSetY(vTargetLook, 0.f);
					m_pTransform->SetLook(vTargetLook);
					break;
				case 3:
					m_QueState.push(Combo_1Start);
					m_QueState.push(Combo_1Hold);
					m_QueState.push(Combo_1);
					m_QueState.push(L_SLASH);
					m_eState = m_QueState.front();
					m_QueState.pop();
					vTargetLook = XMVectorSetY(vTargetLook, 0.f);
					m_pTransform->SetLook(vTargetLook);
					break;
				case 4:
					m_QueState.push(Big_SLASH);
					vTargetLook = XMVectorSetY(vTargetLook, 0.f);
					m_pTransform->SetLook(vTargetLook);
					break;
				case 5:
					m_QueState.push(AXE_STAMP);
					vTargetLook = XMVectorSetY(vTargetLook, 0.f);
					m_pTransform->SetLook(vTargetLook);
					break;
				default:
					break;
				}
			}
			else if (m_bCombat[Third])
			{
				if (Drawing <= 30)
					m_bAddRand = true;
				else
					m_bAddRand = false;
				switch (m_iComboIndex)
				{
				case 1:
					m_QueState.push(Combo_1Start);
					m_QueState.push(Combo_1Hold);
					m_QueState.push(Combo_1);
					m_QueState.push(R_SLASH);
					m_eState = m_QueState.front();
					m_QueState.pop();
					vTargetLook = XMVectorSetY(vTargetLook, 0.f);
					m_pTransform->SetLook(vTargetLook);
					break;
				case 2:
					m_QueState.push(Combo_1Start);
					m_QueState.push(Combo_1Hold);
					m_QueState.push(Combo_1);
					m_QueState.push(L_SLASH);
					m_eState = m_QueState.front();
					m_QueState.pop();
					vTargetLook = XMVectorSetY(vTargetLook, 0.f);
					m_pTransform->SetLook(vTargetLook);
					break;
				case 3:
					m_QueState.push(Combo_1Start);
					m_QueState.push(Combo_1Hold);
					m_QueState.push(Combo_1);
					m_QueState.push(Combo_2Start);
					m_QueState.push(Combo_3Start);
					m_QueState.push(Combo_4Start);
					m_QueState.push(Combo_4End);
					m_eState = m_QueState.front();
					m_QueState.pop();
					vTargetLook = XMVectorSetY(vTargetLook, 0.f);
					m_pTransform->SetLook(vTargetLook);
					break;
				case 4:
					if(m_bAddRand)
						m_QueState.push(Big_SLASH);
					else
						m_QueState.push(AXE_STAMP);
					vTargetLook = XMVectorSetY(vTargetLook, 0.f);
					m_pTransform->SetLook(vTargetLook);
					break;
				case 5:
					if (m_bAddRand)
						m_QueState.push(AXE_STAMP);
					else
					{
						m_QueState.push(PUMMEL_1);
						m_QueState.push(PUMMEL_2);
					}
					vTargetLook = XMVectorSetY(vTargetLook, 0.f);
					m_pTransform->SetLook(vTargetLook);
					break;
				case 6:
					if (m_bAddRand)
					{
						m_QueState.push(PUMMEL_1);
						m_QueState.push(PUMMEL_2);
						vTargetLook = XMVectorSetY(vTargetLook, 0.f);
						m_pTransform->SetLook(vTargetLook);
					}
					break;
				default:
					break;
				}
			}	
		}
	}
}

void CUrsa::Checking_Finished()
{
	if (m_bRoar)
	{
		if (m_bCenter)
		{
			if (m_pModel->Get_isFinished())
			{
				if (!m_QueState.empty())
				{
					m_eState = m_QueState.front();
					m_QueState.pop();

				}
				if (!None_Combat())
				{
					if (m_QueState.empty())
						m_bFinishBlow = true;
				}
			}
		}
	}
	else
	{
		if (m_pModel->Get_isFinished())
		{
			if (!m_bWheelWind)
			{
				if (!m_QueState.empty())
				{
					m_eState = m_QueState.front();
					m_QueState.pop();

				}
			}
			if (!None_Combat())
			{
				if (m_QueState.empty())
					m_bFinishBlow = true;
			}
		}
		else if (m_bFar && m_bSuperFar)
		{
			if (!m_QueState.empty())
			{
				m_eState = m_QueState.front();
				m_QueState.pop();

			}
			if (!None_Combat())
			{
				if (m_QueState.empty())
					m_bFinishBlow = true;
			}
		}
	}
}

_float CUrsa::SetDistance()
{
	_vector vTargetPos, vPos;
	vTargetPos = m_pTargetTransform->GetState(CTransform::STATE_POSITION);
	vPos = m_pTransform->GetState(CTransform::STATE_POSITION);
	XMStoreFloat3(&m_vTargetToLook, vTargetPos - vPos);
	return XMVectorGetX(XMVector3Length(vTargetPos - vPos));
}

_bool CUrsa::None_Combat()
{
	if (m_eState == IDLE01 || m_eState == IDLE02
		|| m_eState == IDLE_CB || m_eState == RUN || m_eState == CB_Start)
		return true;

	else
		return false;
}

void CUrsa::TestAnimation(Ursa eState)
{
	if(CEngine::GetInstance()->Get_DIKDown(DIK_O))
		m_eState = eState;

	if (m_pModel->Get_isFinished())
		m_eState = IDLE01;
}

void CUrsa::Roar()
{
	Empty_queue();
	if (m_eState == Combo_1Start || m_eState == Combo_1Hold || m_eState == Combo_1)
	{
		m_QueState.push(Combo_1End);
	}
	else if (m_eState == Combo_2Start)
	{
		m_QueState.push(Combo_2End);
	}
	else if (m_eState == Combo_3Start)
	{
		m_QueState.push(Combo_3End);
	}
	else if (m_eState == Combo_4Start)
	{
		m_QueState.push(Combo_4End);
	}

	m_bRoar = true;
}

void CUrsa::SetRotate()
{
	_vector vRight = m_pTransform->GetState(CTransform::STATE_RIGHT);
	if(m_eState == R_SLASH)
	{
		m_pTransform->SetLook(vRight);
	}
	else if (m_eState == L_SLASH)
	{
		m_pTransform->SetLook(-vRight);
	}
}

PxVec3 CUrsa::OriginShift()
{
	_vector vCenter;
	PxVec3 vDir = PxVec3(0.f, 0.f, 0.f);
	PxControllerFilters filters;
	vCenter = (XMLoadFloat3(&m_vCenterPos) - m_pTransform->GetState(CTransform::STATE_POSITION));
	_float Length = XMVectorGetX(XMVector3Length(vCenter));

	if (Length > 0.05f)
	{
		if (!m_bCenter)
		{
			m_eState = RUN;
			vCenter = XMVectorSetY(vCenter, 0.f);
			m_pTransform->SetLook(vCenter);
			memcpy(&vDir, &XMVector3Normalize(vCenter), sizeof(_float3));
		}
	}
	else
	{
		if (!m_bCenter)
		{
			//m_eState = IDLE_CB;
			m_pTransform->SetLook(XMLoadFloat3(&m_vTargetToLook));
		}
		m_bCenter = true;
	}

	return vDir;
}
