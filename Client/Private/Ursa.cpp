#include "stdafx.h"
#include "..\Public\Ursa.h"
#include "BasicCollider.h"
#include "Transform.h"
#include "MonHp.h"

#include "Obb.h"

#pragma region MyRegion
#include "EffectSoilDust.h"
#include "EffectUrsaDust.h"
#include "EffectBlood.h"
#include "EffectBloodDecal.h"
#include "EffectUrsaShoulder.h"
#include "EffectUrsaWind.h"
#include "EffectDustWh.h"
#include "EffectUrsaPajang.h"
#include "EffectUrsaPajangMesh.h"
#include "EffectSoilDecal.h"
#include "EffectRockDecal.h"
#include "EffectRing.h"
#pragma endregion

#include "DropRock.h"
#include "DropRockSmall.h"
#include "EventCheck.h"


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
	m_pRenderTransform = static_cast<CTransform*>(m_pGameObject->GetComponent("Com_RenderTransform"));
	m_pModel = static_cast<CModel*>(m_pGameObject->GetComponent("Com_Model"));
	m_pCollider = static_cast<CCollider*>(m_pGameObject->GetComponent("Com_Collider"));
	if (m_pCollider)
		m_pController = m_pCollider->GetController();
	m_pStat = static_cast<CStat*>(m_pGameObject->GetComponent("Com_Stat"));
	m_pOBB = static_cast<CBasicCollider*>(m_pGameObject->GetComponent("Com_OBB"));
	m_pLeftWeapon = static_cast<CBasicCollider*>(m_pGameObject->GetComponent("Com_OBB1"));
	m_pRightWeapon = static_cast<CBasicCollider*>(m_pGameObject->GetComponent("Com_OBB2"));
	m_pHead = static_cast<CBasicCollider*>(m_pGameObject->GetComponent("Com_OBB3"));
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
	m_eRightSTATES = CBasicCollider::STATES_IDEL;
	m_eHeadSTATES = CBasicCollider::STATES_IDEL;
	m_eState = IDLE01;
	m_pModel->SetUp_AnimationIndex((_uint)m_eState);
	m_pMonHp = CMonHp::Create(m_pGameObject);
	XMStoreFloat3(&m_vCenterPos, m_pTransform->GetState(CTransform::STATE_POSITION));

	Create_Trail();

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

	Checking_Phase(dDeltaTime);
	Execute_Pattern(dDeltaTime);

	//if(!m_bWheelWind && !m_bRoar)
	Checking_Finished();


	if (CEngine::GetInstance()->Get_DIKDown(DIK_7))
		m_eState = IDLE01;
	if (CEngine::GetInstance()->Get_DIKDown(DIK_8))
		m_eState = ROAR_Start;
	if (CEngine::GetInstance()->Get_DIKDown(DIK_9))
		m_eState = Flying_End;
	//m_eState = DASH_ATTSpeedup;



	if (m_bCB)
		SetUp_Combo();

	m_pModel->SetUp_AnimationIndex((_uint)m_eState);
	m_pLeftWeapon->p_States = m_eCurSTATES;
	m_pRightWeapon->p_States = m_eRightSTATES;
	m_pHead->p_States = m_eHeadSTATES;
	if (m_pCollider)
		PxExtendedVec3 footpos = m_pCollider->GetController()->getFootPosition();

	PxControllerFilters filters;
	if (IsGravity()) {
		m_fJumpSpeed -= _float(m_fSpeed * (_float)dDeltaTime);
		m_pController->move(PxVec3(0.0f, m_fJumpSpeed, 0.f), 0.01f, PxF32(dDeltaTime), filters);
	}
	else {
		m_fJumpSpeed = 0.f;
	}

	Hit(dDeltaTime);
	OrganizeEffect(dDeltaTime);
	if (!None_Combat())
		CatchUpToLook(dDeltaTime);
}

void CUrsa::LateUpdate(_double dDeltaTime)
{
	__super::LateUpdate(dDeltaTime);

	m_pModel->Play_Animation(dDeltaTime);

	_matrix OffsetMatrix = XMMatrixIdentity();
	XMMatrixTranslation(0.f, 0.05f, -0.4f);

	DirectX::XMStoreFloat4x4(&m_RightwpBoneMatrix, m_pModel->Get_BoneWithoutOffset("BN_Axe_R"));
	DirectX::XMStoreFloat4x4(&m_LeftwpBoneMatrix, m_pModel->Get_BoneWithoutOffset("BN_Axe_L"));

	if (m_pRightTrailBuffer)
		m_pRightTrailBuffer->Update(dDeltaTime, OffsetMatrix * XMLoadFloat4x4(&m_RightwpBoneMatrix) * XMLoadFloat4x4(&m_pRenderTransform->GetMatrix()));
	if (m_pLeftTrailBuffer)
		m_pLeftTrailBuffer->Update(dDeltaTime, OffsetMatrix * XMLoadFloat4x4(&m_LeftwpBoneMatrix) * XMLoadFloat4x4(&m_pRenderTransform->GetMatrix()));
	//m_pRightTrailBuffer->SetIsActive(true);
	//m_pLeftTrailBuffer->SetIsActive(true);


	if (m_bDissolve)
	{
		m_fDissolveAcc += (_float)dDeltaTime * 0.5f;
		if (m_fDissolveAcc > 1.f)
			m_fDissolveAcc = 1.f;

		m_pModel->SetDissolve(m_fDissolveAcc);
	}
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
			m_pController->move(vDir * 1.f * (_float)dDeltaTime, 0.0001f, (_float)dDeltaTime, nullptr);
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
			m_bMove = true;
			m_bClose = false;
			m_bSuperFar = false;
		}
	}
	else
	{
		if (!m_bFar && !m_bSuperFar)
		{
			m_bClose = true;
			m_bMove = false;
			m_bSuperFar = false;
		}
	}

	if (m_bMove && !m_bFinishBlow)
	{
		if (!m_bWheelWind)
			m_eState = RUN;

		vLook = XMVectorLerp(vLook, vTargetLook, 0.5f);
		vLook = XMVectorSetY(vLook, 0.f);
		m_pTransform->SetLook(vLook);
		memcpy(&vDir, &vLook, sizeof(_float3));


		m_pController->move(vDir * 1.0f * (_float)dDeltaTime, 0.0001f, (_float)dDeltaTime, nullptr);
	}
}

void CUrsa::Checking_Phase(_double dDeltaTime)
{
	_float Max = m_pStat->GetStatInfo().maxHp;
	if (m_bFinishBlow)
	{
		if (m_bDelay)
			m_dPatternTime += dDeltaTime;

		if (m_pModel->Get_isFinished())
		{
			if (!m_bDelay)
				SetRotate();
			m_bDelay = true;
			m_eState = IDLE_CB;
		}

		if (m_dPatternTime > 0.5)
		{
			m_dPatternTime = 0.0;
			m_bSuperFar = false;
			m_bFar = false;
			m_bCB = false;
			m_bClose = false;
			m_bDelay = false;
			m_bRoar = false;
			m_bCenter = false;
			m_bFinishBlow = false;
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
	if (m_bDeadMotion)
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
					m_LerpTime = 0.0;
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
					m_LerpTime = 0.0;
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
					m_LerpTime = 0.0;
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
					m_LerpTime = 0.0;
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
					m_LerpTime = 0.0;
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
					m_LerpTime = 0.0;
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
					m_LerpTime = 0.0;
					m_QueState.push(Big_SLASH);
					vTargetLook = XMVectorSetY(vTargetLook, 0.f);
					m_pTransform->SetLook(vTargetLook);
					break;
				case 5:
					m_LerpTime = 0.0;
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
					m_LerpTime = 0.0;
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
					m_LerpTime = 0.0;
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
					m_LerpTime = 0.0;
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
					m_LerpTime = 0.0;
					if (m_bAddRand)
						m_QueState.push(Big_SLASH);
					else
						m_QueState.push(AXE_STAMP);
					vTargetLook = XMVectorSetY(vTargetLook, 0.f);
					m_pTransform->SetLook(vTargetLook);
					break;
				case 5:
					m_LerpTime = 0.0;
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
						m_LerpTime = 0.0;
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
					m_LerpTime = 0.0;
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
				m_LerpTime = 0.0;
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
				m_LerpTime = 0.0;
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
	m_vTargetToLook.y = 0.f;
	return XMVectorGetX(XMVector3Length(XMLoadFloat3(&m_vTargetToLook)));
}

_bool CUrsa::None_Combat()
{
	if (m_eState == IDLE01 || m_eState == IDLE02
		|| m_eState == IDLE_CB || m_eState == RUN || m_eState == CB_Start)
		return true;

	else
		return false;
}

_bool CUrsa::IsGravity()
{
	_vector vUrsaPos = m_pTransform->GetState(CTransform::STATE_POSITION);
	_vector vRayDir = XMVectorSet(0.f, -1.f, 0.f, 0.f);
	PxRaycastBuffer buf;
	PxQueryFilterData filterData;
	filterData.data.word1 = CPxManager::GROUP4;
	filterData.data.word2 = CPxManager::GROUP4;
	filterData.flags |= PxQueryFlag::eANY_HIT;
	filterData.flags |= PxQueryFlag::ePREFILTER;
	_bool isCollied = false;
	PxRigidActor* actor = m_pController->getActor();

	if (CEngine::GetInstance()->Raycast(vUrsaPos, vRayDir, 0.06f, buf, filterData, &CPxQueryFilters(actor, CPxManager::GROUP4)))
	{
		if (buf.getAnyHit(0).distance <= 0.0f)
		{
			//cout << "playe collid with terrain\n";
			isCollied = false;
		}
		else {
			//cout << "playe no collid with terrain \n";
			isCollied = true;
		}
	}
	else {
		//	cout << "playe no collid with terrain \n";
		isCollied = true;
	}
	return isCollied;
}

void CUrsa::TestAnimation(Ursa eState)
{
	if (CEngine::GetInstance()->Get_DIKDown(DIK_O))
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
	if (m_eState == R_SLASH)
	{
		m_pTransform->SetLook(vRight);
	}
	else if (m_eState == L_SLASH)
	{
		m_pTransform->SetLook(-vRight);
	}
}


void CUrsa::OrganizeEffect(_double dDeltaTime)
{
	_uint keyFrame = m_pModel->GetCurrentKeyFrame();
	_vector pos = m_pTransform->GetState(CTransform::STATE_POSITION);
	_matrix UrsaAxeR = m_pModel->Get_BoneWithoutOffset("BN_Axe_R");
	UrsaAxeR = Remove_ScaleRotation(UrsaAxeR* m_pTransform->GetWorldMatrix());

	_matrix UrsaAxeL = m_pModel->Get_BoneWithoutOffset("BN_Axe_L");
	UrsaAxeL = Remove_ScaleRotation(UrsaAxeL* m_pTransform->GetWorldMatrix());

	_matrix RemoveWorldMatrix = m_pTransform->Remove_Scale(m_pTransform->GetWorldMatrix());

	m_pRightTrailBuffer->SetIsActive(false);
	m_pLeftTrailBuffer->SetIsActive(false);
	m_eCurSTATES = CBasicCollider::STATES_IDEL;
	m_eRightSTATES = CBasicCollider::STATES_IDEL;
	m_eHeadSTATES = CBasicCollider::STATES_IDEL;
	switch (m_eState)
	{
	case Client::CUrsa::IDLE02:
		break;
	case Client::CUrsa::IDLE01:
		break;
	case Client::CUrsa::IDLE_CB:
		break;
	case Client::CUrsa::RUN:
		if (keyFrame == 2 || keyFrame == 14)
		{
			CEngine::GetInstance()->StopSound(ENEMY20);
			CEngine::GetInstance()->StopSound(ENEMY21);
			CEngine::GetInstance()->PlaySoundW("Walk_Ursa.ogg", ENEMY20);
			CEngine::GetInstance()->PlaySoundW("WalkFloor_Ursa.ogg", ENEMY21);
		}
		break;
	case Client::CUrsa::CB_Start:
		if (keyFrame == 8)
			CEngine::GetInstance()->PlaySoundW("Roar_Ursa.mp3", ENEMY22);

		CEventCheck::GetInstance()->ShakeUpDown(5, 0.05f);
		//if (keyFrame <= 40)
		//	static_cast<CEmptyGameObject*>(m_pGameObject)->SetRimLight(true, DirectX::Colors::Red, 1.f);
		//else
		//	static_cast<CEmptyGameObject*>(m_pGameObject)->SetRimLight(false, DirectX::Colors::Red, 1.f);

		break;
	case Client::CUrsa::ROAR_Casting: {
		//if (keyFrame >= 47)
		//	static_cast<CEmptyGameObject*>(m_pGameObject)->SetRimLight(true, DirectX::Colors::Red, 1.f);
		//else
		//	static_cast<CEmptyGameObject*>(m_pGameObject)->SetRimLight(false, DirectX::Colors::Red, 1.f);
	}
	break;
	case Client::CUrsa::DASH_ATT: {
		if (keyFrame == 42)
			CEngine::GetInstance()->PlaySoundW("DashAtt_Ursa.mp3", ENEMY23);
		if (keyFrame == 62)
			CEngine::GetInstance()->PlaySoundW("WeakSwing_Ursa.mp3", ENEMY24);
		if (keyFrame >= 42 && keyFrame <= 70)
		{
			m_eHeadSTATES = CBasicCollider::STATES_ATK;
			if (keyFrame >= 62)
			{
				m_eHeadSTATES = CBasicCollider::STATES_IDEL;
				m_eCurSTATES = CBasicCollider::STATES_ATK;
				m_eRightSTATES = CBasicCollider::STATES_ATK;
			}
		}

		CEngine::GetInstance()->PlaySoundW("UrsaVoice03.ogg", CHANNELID::ENEMY10);
		m_eCurSTATES = CBasicCollider::STATES_ATK;


		if (keyFrame >= 50 && keyFrame <= 70)
		{
			m_pRightTrailBuffer->SetIsActive(true);
			m_pLeftTrailBuffer->SetIsActive(true);
		}
		_matrix ArmTwist = m_pModel->Get_BoneWithoutOffset("Bip01-RUpArmTwist");
		_matrix Offset = XMMatrixTranslation(-0.1f, 0.3f, 0.2f);
		ArmTwist = Remove_ScaleRotation(Offset * m_pTransform->GetWorldMatrix());

		if (keyFrame == 48 && m_iMakeDust < 1) {
			m_iMakeDust += 1;
			auto UrsaShoulder = CEngine::GetInstance()->AddGameObjectToPrefab(CEngine::GetInstance()->GetCurSceneNumber(), "Prototype_GameObecjt_UrsaShoulder", "E_UrsaShoulder");
			CEngine::GetInstance()->AddScriptObject(m_pUrsaShoulder = CEffectUrsaShoulder::Create(UrsaShoulder, ArmTwist), CEngine::GetInstance()->GetCurSceneNumber());
		}
		//else if (keyFrame == 63 && m_iMakeDust < 1) {
		//	m_iMakeDust += 1;
		//	auto DustWh = CEngine::GetInstance()->AddGameObjectToPrefab(CEngine::GetInstance()->GetCurSceneNumber(), "Prototype_GameObecjt_Ursa_DustWh", "E_Ursa_DustWh");
		//	CEngine::GetInstance()->AddScriptObject(CEffectDustWh::Create(DustWh, UrsaAxeL), CEngine::GetInstance()->GetCurSceneNumber());
		//}
		else
			m_iMakeDust = 0;

		if (keyFrame >= 48 && keyFrame <= 60) {
			if (m_pUrsaShoulder)
				m_pUrsaShoulder->Set_Matrix(ArmTwist);
		}

	}
								  break;
	case Client::CUrsa::L_SLASH:
		if (keyFrame >= 20 && keyFrame <= 29)
		{
			CEngine::GetInstance()->PlaySoundW("UrsaVoice01.ogg", CHANNELID::ENEMY10);

			m_pLeftTrailBuffer->SetIsActive(true);
			m_eCurSTATES = CBasicCollider::STATES_ATK;
		}
		break;
	case Client::CUrsa::R_SLASH:
		if (keyFrame >= 25 && keyFrame <= 34)
		{
			CEngine::GetInstance()->PlaySoundW("UrsaVoice01.ogg", CHANNELID::ENEMY10);

			m_pRightTrailBuffer->SetIsActive(true);
			m_eRightSTATES = CBasicCollider::STATES_ATK;
		}
		break;
	case Client::CUrsa::Combo_1Start:
		break;
	case Client::CUrsa::Combo_1Hold:
		CEngine::GetInstance()->PlaySoundW("WeakShout_Ursa.mp3", ENEMY24);
		break;
	case Client::CUrsa::Combo_1:
		if(keyFrame == 20)
			CEventCheck::GetInstance()->ShakeUpDown(2, 0.06f);

		if (keyFrame >= 16 && keyFrame <= 23)
			m_eCurSTATES = CBasicCollider::STATES_ATK;
		if (keyFrame >= 6 && keyFrame <= 27) {
			m_pRightTrailBuffer->SetIsActive(true);
			CEngine::GetInstance()->PlaySoundW("UrsaVoice02.ogg", CHANNELID::ENEMY10);
		}
		{
			if (keyFrame == 19 && m_iMakeDust < 1) {
				m_iMakeDust += 1;
				CEngine::GetInstance()->PlaySoundW("UrsaSoilDust.ogg", CHANNELID::ENEMY11);

				auto SoilDust = CEngine::GetInstance()->AddGameObjectToPrefab(CEngine::GetInstance()->GetCurSceneNumber(), "Prototype_Effect_Ursa_SoilDust", "E_Ursa_SoilDust");
				CEngine::GetInstance()->AddScriptObject(CEffectSoilDust::Create(SoilDust, UrsaAxeR), CEngine::GetInstance()->GetCurSceneNumber());

				for (int i = 0; i < 2; ++i) {
					pos = XMVectorSetY(pos, 0.1f);
					CGameObject* RockSmall = CEngine::GetInstance()->AddGameObjectToPrefab(CEngine::GetInstance()->GetCurSceneNumber(), "Prototype_GameObecjt_RockSmall", "O_RockSmall");
					CEngine::GetInstance()->AddScriptObject(CDropRockSmall::Create(RockSmall, pos), CEngine::GetInstance()->GetCurSceneNumber());
				}
			}
			else
				m_iMakeDust = 0;
		}
		{
			_uint random = rand() % 10;

			if (random >= 2)
			{
				{
					/*		if (keyFrame >= 82 && keyFrame <= 105)*/ {
						m_dDropRockMkdt += dDeltaTime;
						if (m_dDropRockMkdt >= 1.f) {
							CGameObject* Rock = CEngine::GetInstance()->AddGameObjectToPrefab(CEngine::GetInstance()->GetCurSceneNumber(), "Prototype_GameObecjt_Rock", "O_Rock");
							CEngine::GetInstance()->AddScriptObject(CDropRock::Create(Rock), CEngine::GetInstance()->GetCurSceneNumber());
							m_dDropRockMkdt = 0;
						}
					}
				}
			}
		}
		break;
	case Client::CUrsa::Combo_1End:
		break;
	case Client::CUrsa::Combo_2Start:
		if(keyFrame == 39)
			CEventCheck::GetInstance()->ShakeUpDown(2, 0.06f);

		if (keyFrame >= 36 && keyFrame <= 41) {
			CEngine::GetInstance()->PlaySoundW("UrsaVoice01.ogg", CHANNELID::ENEMY10);
			m_eCurSTATES = CBasicCollider::STATES_ATK;
		}
		if (keyFrame >= 24 && keyFrame <= 49)
			m_pLeftTrailBuffer->SetIsActive(true);
		{
			if (keyFrame == 39 && m_iMakeDust < 1) {
				m_iMakeDust += 1;
				CEngine::GetInstance()->PlaySoundW("UrsaSoilDust.ogg", CHANNELID::ENEMY11);

				auto SoilDust = CEngine::GetInstance()->AddGameObjectToPrefab(CEngine::GetInstance()->GetCurSceneNumber(), "Prototype_Effect_Ursa_SoilDust", "E_Ursa_SoilDust");
				CEngine::GetInstance()->AddScriptObject(CEffectSoilDust::Create(SoilDust, UrsaAxeL), CEngine::GetInstance()->GetCurSceneNumber());

				for (int i = 0; i < 2; ++i) {
					pos = XMVectorSetY(pos, 0.1f);
					CGameObject* RockSmall = CEngine::GetInstance()->AddGameObjectToPrefab(CEngine::GetInstance()->GetCurSceneNumber(), "Prototype_GameObecjt_RockSmall", "O_RockSmall");
					CEngine::GetInstance()->AddScriptObject(CDropRockSmall::Create(RockSmall, pos), CEngine::GetInstance()->GetCurSceneNumber());
				}
			}
			else
				m_iMakeDust = 0;
		}
		{
			_uint random = rand() % 10;

			if (random >= 2)
			{
				{
					/*		if (keyFrame >= 82 && keyFrame <= 105)*/ {
						m_dDropRockMkdt += dDeltaTime;
						if (m_dDropRockMkdt >= 1.f) {
							CGameObject* Rock = CEngine::GetInstance()->AddGameObjectToPrefab(CEngine::GetInstance()->GetCurSceneNumber(), "Prototype_GameObecjt_Rock", "O_Rock");
							CEngine::GetInstance()->AddScriptObject(CDropRock::Create(Rock), CEngine::GetInstance()->GetCurSceneNumber());
							m_dDropRockMkdt = 0;
						}
					}
				}
			}
		}

		break;
	case Client::CUrsa::Combo_2End:
		break;
	case Client::CUrsa::Combo_3Start:
		if (keyFrame >= 6 && keyFrame <= 20) {
			CEngine::GetInstance()->PlaySoundW("UrsaVoice03.ogg", CHANNELID::ENEMY10);
			m_eCurSTATES = CBasicCollider::STATES_ATK;
		}
		if (keyFrame >= 8)
		{
			m_pRightTrailBuffer->SetIsActive(true);
			m_pLeftTrailBuffer->SetIsActive(true);
		}
		break;
	case Client::CUrsa::Combo_3End:
		break;
	case Client::CUrsa::Combo_4Start:
		if (keyFrame >= 7 && keyFrame <= 20)
		{
			CEngine::GetInstance()->PlaySoundW("UrsaVoice04.ogg", CHANNELID::ENEMY10);
			m_eCurSTATES = CBasicCollider::STATES_ATK;
			m_pRightTrailBuffer->SetIsActive(true);
			m_pLeftTrailBuffer->SetIsActive(true);
		}
		break;
	case Client::CUrsa::Combo_4End:
		break;
	case Client::CUrsa::Big_SLASH:
		if (keyFrame >= 35 && keyFrame <= 50) {
			CEngine::GetInstance()->PlaySoundW("UrsaVoice02.ogg", CHANNELID::ENEMY10);
			m_pRightTrailBuffer->SetIsActive(true);
		}
		break;
	case Client::CUrsa::AXE_STAMP:
		if(keyFrame ==49)
			CEventCheck::GetInstance()->ShakeUpDown(5, 0.04f);

		if (keyFrame >= 34 && keyFrame <= 50)
		{
			m_pRightTrailBuffer->SetIsActive(true);
			m_pLeftTrailBuffer->SetIsActive(true);
		}
		if (keyFrame >= 40 && keyFrame <= 50)
			CEngine::GetInstance()->PlaySoundW("UrsaVoice03.ogg", CHANNELID::ENEMY10);
		{
			if (keyFrame == 47 && m_iMakeDust < 1) {
				m_iMakeDust += 1;
				CEngine::GetInstance()->PlaySoundW("UrsaSoilDust.ogg", CHANNELID::ENEMY11);

				auto SoilDust = CEngine::GetInstance()->AddGameObjectToPrefab(CEngine::GetInstance()->GetCurSceneNumber(), "Prototype_Effect_Ursa_SoilDust", "E_Ursa_SoilDust");
				CEngine::GetInstance()->AddScriptObject(CEffectSoilDust::Create(SoilDust, UrsaAxeL), CEngine::GetInstance()->GetCurSceneNumber());

				pos = XMVectorSetY(pos, 0.1f);
				CGameObject* RockSmall = CEngine::GetInstance()->AddGameObjectToPrefab(CEngine::GetInstance()->GetCurSceneNumber(), "Prototype_GameObecjt_RockSmall", "O_RockSmall");
				CEngine::GetInstance()->AddScriptObject(CDropRockSmall::Create(RockSmall, pos), CEngine::GetInstance()->GetCurSceneNumber());
			}
			else if (keyFrame == 49 && m_iMakeDust < 1) {
				m_iMakeDust += 1;
				CEngine::GetInstance()->PlaySoundW("UrsaSoilDust.ogg", CHANNELID::ENEMY12);
				auto SoilDust = CEngine::GetInstance()->AddGameObjectToPrefab(CEngine::GetInstance()->GetCurSceneNumber(), "Prototype_Effect_Ursa_SoilDust", "E_Ursa_SoilDust");
				CEngine::GetInstance()->AddScriptObject(CEffectSoilDust::Create(SoilDust, UrsaAxeR), CEngine::GetInstance()->GetCurSceneNumber());

				for (int i = 0; i < 2; ++i) {
					pos = XMVectorSetY(pos, 0.1f);
					CGameObject* RockSmall = CEngine::GetInstance()->AddGameObjectToPrefab(CEngine::GetInstance()->GetCurSceneNumber(), "Prototype_GameObecjt_RockSmall", "O_RockSmall");
					CEngine::GetInstance()->AddScriptObject(CDropRockSmall::Create(RockSmall, pos), CEngine::GetInstance()->GetCurSceneNumber());
				}
			}
			else
				m_iMakeDust = 0;
		}
		break;
	case Client::CUrsa::PUMMEL_1:
		if (keyFrame >= 10 && keyFrame <= 35)
			CEngine::GetInstance()->PlaySoundW("UrsaVoice05.ogg", CHANNELID::ENEMY10);

		break;
	case Client::CUrsa::PUMMEL_2:
		if (keyFrame >= 20 && keyFrame <= 80)
			CEngine::GetInstance()->PlaySoundW("UrsaVoice06.ogg", CHANNELID::ENEMY10);

		if (keyFrame == 100 && m_iMakeDust < 1)
		{
			CEventCheck::GetInstance()->ShakeUpDown(2, 0.06f);
			CEngine::GetInstance()->PlaySoundW("UrsaSoilDust.ogg", CHANNELID::ENEMY11);
			m_iMakeDust += 1;
			CGameObject* RockSmall = CEngine::GetInstance()->AddGameObjectToPrefab(CEngine::GetInstance()->GetCurSceneNumber(), "Prototype_GameObecjt_RockSmall", "O_RockSmall");
			CEngine::GetInstance()->AddScriptObject(CDropRockSmall::Create(RockSmall, pos, true), CEngine::GetInstance()->GetCurSceneNumber());
			auto SoilDust = CEngine::GetInstance()->AddGameObjectToPrefab(CEngine::GetInstance()->GetCurSceneNumber(), "Prototype_Effect_Ursa_SoilDust", "E_Ursa_SoilDust");
			CEngine::GetInstance()->AddScriptObject(CEffectSoilDust::Create(SoilDust, UrsaAxeL, false), CEngine::GetInstance()->GetCurSceneNumber());
		}
		else if (keyFrame == 109 && m_iMakeDust < 1)
		{
			CEventCheck::GetInstance()->ShakeUpDown(2, 0.06f);

			CEngine::GetInstance()->PlaySoundW("UrsaSoilDust.ogg", CHANNELID::ENEMY12);
			m_iMakeDust += 1;
			auto SoilDust = CEngine::GetInstance()->AddGameObjectToPrefab(CEngine::GetInstance()->GetCurSceneNumber(), "Prototype_Effect_Ursa_SoilDust", "E_Ursa_SoilDust", false);
			CEngine::GetInstance()->AddScriptObject(CEffectSoilDust::Create(SoilDust, UrsaAxeL, false), CEngine::GetInstance()->GetCurSceneNumber());
		}
		else if (keyFrame == 117 && m_iMakeDust < 1)
		{
			CEventCheck::GetInstance()->ShakeUpDown(2, 0.06f);

			CEngine::GetInstance()->PlaySoundW("UrsaSoilDust.ogg", CHANNELID::ENEMY13);

			CGameObject* RockSmall = CEngine::GetInstance()->AddGameObjectToPrefab(CEngine::GetInstance()->GetCurSceneNumber(), "Prototype_GameObecjt_RockSmall", "O_RockSmall");
			CEngine::GetInstance()->AddScriptObject(CDropRockSmall::Create(RockSmall, pos, true), CEngine::GetInstance()->GetCurSceneNumber());
			m_iMakeDust += 1;
			auto SoilDust = CEngine::GetInstance()->AddGameObjectToPrefab(CEngine::GetInstance()->GetCurSceneNumber(), "Prototype_Effect_Ursa_SoilDust", "E_Ursa_SoilDust", false);
			CEngine::GetInstance()->AddScriptObject(CEffectSoilDust::Create(SoilDust, UrsaAxeL, false), CEngine::GetInstance()->GetCurSceneNumber());
		}
		else if (keyFrame == 126 && m_iMakeDust < 1)
		{
			CEventCheck::GetInstance()->ShakeUpDown(2, 0.06f);

			CEngine::GetInstance()->PlaySoundW("UrsaSoilDust.ogg", CHANNELID::ENEMY14);

			m_iMakeDust += 1;
			auto SoilDust = CEngine::GetInstance()->AddGameObjectToPrefab(CEngine::GetInstance()->GetCurSceneNumber(), "Prototype_Effect_Ursa_SoilDust", "E_Ursa_SoilDust", false);
			CEngine::GetInstance()->AddScriptObject(CEffectSoilDust::Create(SoilDust, UrsaAxeL, false), CEngine::GetInstance()->GetCurSceneNumber());
		}
		else if (keyFrame == 135 && m_iMakeDust < 1)
		{
			CEventCheck::GetInstance()->ShakeUpDown(2, 0.06f);

			CGameObject* RockSmall = CEngine::GetInstance()->AddGameObjectToPrefab(CEngine::GetInstance()->GetCurSceneNumber(), "Prototype_GameObecjt_RockSmall", "O_RockSmall");
			CEngine::GetInstance()->AddScriptObject(CDropRockSmall::Create(RockSmall, pos, true), CEngine::GetInstance()->GetCurSceneNumber());
			CEngine::GetInstance()->PlaySoundW("UrsaSoilDust.ogg", CHANNELID::ENEMY15);

			m_iMakeDust += 1;
			auto SoilDust = CEngine::GetInstance()->AddGameObjectToPrefab(CEngine::GetInstance()->GetCurSceneNumber(), "Prototype_Effect_Ursa_SoilDust", "E_Ursa_SoilDust", false);
			CEngine::GetInstance()->AddScriptObject(CEffectSoilDust::Create(SoilDust, UrsaAxeL, false), CEngine::GetInstance()->GetCurSceneNumber());
		}
		else if (keyFrame == 144 && m_iMakeDust < 1)
		{
			CEventCheck::GetInstance()->ShakeUpDown(2, 0.06f);

			CEngine::GetInstance()->PlaySoundW("UrsaSoilDust.ogg", CHANNELID::ENEMY16);

			m_iMakeDust += 1;
			auto SoilDust = CEngine::GetInstance()->AddGameObjectToPrefab(CEngine::GetInstance()->GetCurSceneNumber(), "Prototype_Effect_Ursa_SoilDust", "E_Ursa_SoilDust", false);
			CEngine::GetInstance()->AddScriptObject(CEffectSoilDust::Create(SoilDust, UrsaAxeL, false), CEngine::GetInstance()->GetCurSceneNumber());
		}
		else
			m_iMakeDust = 0;


		break;
	case Client::CUrsa::ROAR_ING: {
		if (keyFrame == 17)
			CEngine::GetInstance()->PlaySoundW("UrsaFootSound.ogg", CHANNELID::ENEMY10);
		if (keyFrame == 40)
			CEngine::GetInstance()->PlaySoundW("UrsaVoice07.ogg", CHANNELID::ENEMY11);
		if (keyFrame == 46) {
			CEventCheck::GetInstance()->ShakeUpDown(1, 0.08f);

			CEngine::GetInstance()->PlaySoundW("UrsaFootSound2.ogg", CHANNELID::ENEMY12);
			CGameObject* RockSmall = CEngine::GetInstance()->AddGameObjectToPrefab(CEngine::GetInstance()->GetCurSceneNumber(), "Prototype_GameObecjt_RockSmall", "O_RockSmall");
			CEngine::GetInstance()->AddScriptObject(CDropRockSmall::Create(RockSmall, pos), CEngine::GetInstance()->GetCurSceneNumber());
			CGameObject* Eff = CEngine::GetInstance()->AddGameObjectToPrefab(CEngine::GetInstance()->GetCurSceneNumber(), "Prototype_GameObecjt_Ursa_ROARDecal", "E_ROARDecal");
			CEngine::GetInstance()->AddScriptObject(CEffectRockDecal::Create(Eff, pos), CEngine::GetInstance()->GetCurSceneNumber());
		}
		if (keyFrame == 60)
			CEngine::GetInstance()->PlaySoundW("UrsaVoice05.ogg", CHANNELID::ENEMY13);
	}
								  break;
	case Client::CUrsa::ROAR_End:
		//cout << keyFrame << endl;

		if (keyFrame >= 15 && keyFrame <= 59)
			CEngine::GetInstance()->PlaySoundW("UrsaVoice03.ogg", CHANNELID::ENEMY14);
		else
			CEngine::GetInstance()->StopSound( CHANNELID::ENEMY14);
		break;
	case Client::CUrsa::DASH_ATTSpeedup: {///////////////////////////////////////////////////////////////////////////////////
		if (keyFrame >= 15 && keyFrame <= 40)
		{
			CEngine::GetInstance()->PlaySoundW("UrsaVoice05.ogg", CHANNELID::ENEMY10);
			m_pRightTrailBuffer->SetIsActive(true);
			m_pLeftTrailBuffer->SetIsActive(true);
		}
		else
			CEngine::GetInstance()->StopSound(CHANNELID::ENEMY10);
		_matrix ArmTwist = m_pModel->Get_BoneWithoutOffset("Bip01-RUpArmTwist");
		_matrix Offset = XMMatrixTranslation(-0.1f, 0.3f, 0.2f);
		ArmTwist = Remove_ScaleRotation(Offset * m_pTransform->GetWorldMatrix());

		if (keyFrame == 14 && m_iMakeDust < 1) {
			m_iMakeDust += 1;
			auto UrsaShoulder = CEngine::GetInstance()->AddGameObjectToPrefab(CEngine::GetInstance()->GetCurSceneNumber(), "Prototype_GameObecjt_UrsaShoulder", "E_UrsaShoulder");
			CEngine::GetInstance()->AddScriptObject(m_pUrsaShoulder = CEffectUrsaShoulder::Create(UrsaShoulder, ArmTwist), CEngine::GetInstance()->GetCurSceneNumber());
		}
		/*else if (keyFrame == 28 && m_iMakeDust <= 1) {
			m_iMakeDust += 1;
			auto DustWh = CEngine::GetInstance()->AddGameObjectToPrefab(CEngine::GetInstance()->GetCurSceneNumber(), "Prototype_GameObecjt_Ursa_DustWh", "E_Ursa_DustWh");
			CEngine::GetInstance()->AddScriptObject(CEffectDustWh::Create(DustWh, UrsaAxeL), CEngine::GetInstance()->GetCurSceneNumber());
		}*/
		else
			m_iMakeDust = 0;

		if (keyFrame >= 14 && keyFrame <= 23) {
			if (m_pUrsaShoulder)
				m_pUrsaShoulder->Set_Matrix(ArmTwist);
		}

	}
										 break;
	case Client::CUrsa::WHEELWIND_Start:
		break;
	case Client::CUrsa::WHEELWIND_Ing: {
		CEngine::GetInstance()->PlaySoundW("UrsaWind.ogg", CHANNELID::ENEMY10);
		m_pRightTrailBuffer->SetIsActive(true);
		m_pLeftTrailBuffer->SetIsActive(true);

		CGameObject* UrsaWind = CEngine::GetInstance()->AddGameObjectToPrefab(CEngine::GetInstance()->GetCurSceneNumber(), "Prototype_GameObecjt_UrsaWind", "E_UrsaWinds");
		CEngine::GetInstance()->AddScriptObject(m_pUrsaWind = CEffectUrsaWind::Create(UrsaWind, pos), CEngine::GetInstance()->GetCurSceneNumber());
	}

	 break;
	case Client::CUrsa::WHEELWIND_End:
		if(keyFrame == 7)
			CEngine::GetInstance()->PlaySoundW("UrsaWind2.ogg", CHANNELID::ENEMY11);

		if (keyFrame <= 7)
		{
			m_pRightTrailBuffer->SetIsActive(true);
			m_pLeftTrailBuffer->SetIsActive(true);
		}
		break;
	case Client::CUrsa::ROAR_Start: {
		if (keyFrame == 20)
			CEngine::GetInstance()->PlaySoundW("UrsaRoarSound01.ogg", CHANNELID::ENEMY10);
		if (keyFrame == 45)
			CEngine::GetInstance()->PlaySoundW("UrsaVoice06.ogg", CHANNELID::ENEMY11);

		if (keyFrame >= 46 && keyFrame <= 107)
			static_cast<CEmptyGameObject*>(m_pGameObject)->SetRimLight(true, DirectX::Colors::Red, 10.f);
		else
			static_cast<CEmptyGameObject*>(m_pGameObject)->SetRimLight(false, DirectX::Colors::Red, 10.f);

		if (keyFrame == 80)
			CEventCheck::GetInstance()->ShakeCamera(CCamera_Fly::SHAKE::SHAKE_ING, 7, 0.04f);


		if (keyFrame == 86) {
			CGameObject* Eff = nullptr;

			Eff = CEngine::GetInstance()->AddGameObjectToPrefab(CEngine::GetInstance()->GetCurSceneNumber(), "Prototype_GameObecjt_Ursa_PajangDecal", "E_UrsaPajang");
			CEngine::GetInstance()->AddScriptObject(CEffectUrsaPajang::Create(Eff, pos), CEngine::GetInstance()->GetCurSceneNumber());

			Eff = CEngine::GetInstance()->AddGameObjectToPrefab(CEngine::GetInstance()->GetCurSceneNumber(), "Prototype_GameObecjt_Ursa_PajangMesh", "E_UrsaPajangMesh");
			CEngine::GetInstance()->AddScriptObject(CEffectUrsaPajangMesh::Create(Eff, pos), CEngine::GetInstance()->GetCurSceneNumber());

			Eff = CEngine::GetInstance()->AddGameObjectToPrefab(CEngine::GetInstance()->GetCurSceneNumber(), "Prototype_GameObecjt_Ursa_ROARDecal", "E_ROARDecal");
			CEngine::GetInstance()->AddScriptObject(CEffectRockDecal::Create(Eff, pos), CEngine::GetInstance()->GetCurSceneNumber());

			Eff = CEngine::GetInstance()->AddGameObjectToPrefab(CEngine::GetInstance()->GetCurSceneNumber(), "Prototype_GameObecjt_Ursa_Ring", "E_UrsRing");
			CEngine::GetInstance()->AddScriptObject(CEffectRing::Create(Eff, pos), CEngine::GetInstance()->GetCurSceneNumber());
		}

	}
									break;
	case Client::CUrsa::HIT:
		break;
	case Client::CUrsa::DIE:
		break;
	case Client::CUrsa::DEADBODY:
		m_bDissolve = true;
		break;
	case Client::CUrsa::Flying_Start:
		CEngine::GetInstance()->PlaySoundW("UrsaVoice02.ogg", CHANNELID::ENEMY13);

		break;
	case Client::CUrsa::Flying_Land:
		if (keyFrame >= 0 && keyFrame <= 30) {
			CEngine::GetInstance()->PlaySoundW("UrsaVoice08.ogg", CHANNELID::ENEMY14);

			m_pLeftTrailBuffer->SetIsActive(true);
		}
		if (keyFrame >= 23)
			m_pRightTrailBuffer->SetIsActive(true);

		break;
	case Client::CUrsa::Flying_End:
		if (keyFrame == 1 && m_iMakeDust <= 1) {
			CEventCheck::GetInstance()->ShakeUpDown(5, 0.08f);

			CEngine::GetInstance()->PlaySoundW("UrsaFlySound.ogg", CHANNELID::ENEMY16);

			m_iMakeDust += 1;
			auto SoilDust = CEngine::GetInstance()->AddGameObjectToPrefab(CEngine::GetInstance()->GetCurSceneNumber(), "Prototype_Effect_Ursa_SoilDust", "E_Ursa_SoilDust");
			CEngine::GetInstance()->AddScriptObject(CEffectSoilDust::Create(SoilDust, UrsaAxeL), CEngine::GetInstance()->GetCurSceneNumber());
			auto SoilDust2 = CEngine::GetInstance()->AddGameObjectToPrefab(CEngine::GetInstance()->GetCurSceneNumber(), "Prototype_Effect_Ursa_SoilDust", "E_Ursa_SoilDust");
			CEngine::GetInstance()->AddScriptObject(CEffectSoilDust::Create(SoilDust2, UrsaAxeR), CEngine::GetInstance()->GetCurSceneNumber());

			auto Eff = CEngine::GetInstance()->AddGameObjectToPrefab(CEngine::GetInstance()->GetCurSceneNumber(), "Prototype_GameObecjt_Ursa_ROARDecal", "E_ROARDecal");
			CEngine::GetInstance()->AddScriptObject(CEffectRockDecal::Create(Eff, pos), CEngine::GetInstance()->GetCurSceneNumber());

			for (int i = 0; i < 2; ++i) {
				pos = XMVectorSetY(pos, 0.1f);
				CGameObject* RockSmall = CEngine::GetInstance()->AddGameObjectToPrefab(CEngine::GetInstance()->GetCurSceneNumber(), "Prototype_GameObecjt_RockSmall", "O_RockSmall");
				CEngine::GetInstance()->AddScriptObject(CDropRockSmall::Create(RockSmall, pos), CEngine::GetInstance()->GetCurSceneNumber());
			}

			CEventCheck::GetInstance()->ShakeUpDown(5, 0.02f);
		}
		else
			m_iMakeDust = 0;
		break;
	case Client::CUrsa::Ursa_END:
		break;
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

_fmatrix CUrsa::Remove_Rotation(_fmatrix TransformMatrix)
{
	_matrix			NonRotateMatrix = XMMatrixIdentity();

	NonRotateMatrix.r[3] = TransformMatrix.r[3];

	return NonRotateMatrix;
}

_fmatrix CUrsa::Remove_Scale(_fmatrix _srcmatrix)
{
	_matrix			NonScaleMatrix = (_srcmatrix);

	NonScaleMatrix.r[0] = XMVector3Normalize(_srcmatrix.r[0]);
	NonScaleMatrix.r[1] = XMVector3Normalize(_srcmatrix.r[1]);
	NonScaleMatrix.r[2] = XMVector3Normalize(_srcmatrix.r[2]);

	return NonScaleMatrix;
}

_fmatrix CUrsa::Remove_ScaleRotation(_fmatrix _srcmatrix)
{
	_matrix			NonScaleMatrix = Remove_Rotation(_srcmatrix);

	NonScaleMatrix.r[0] = XMVector3Normalize(_srcmatrix.r[0]);
	NonScaleMatrix.r[1] = XMVector3Normalize(_srcmatrix.r[1]);
	NonScaleMatrix.r[2] = XMVector3Normalize(_srcmatrix.r[2]);

	return NonScaleMatrix;
}

void CUrsa::Hit(_double dDeltaTime)
{
	if (m_eState == DEADBODY || m_eState == DIE)
		return;


	if (m_pStat->GetStatInfo().hp <= 0)
		return;


	//m_fMKB += (_float)dDeltaTime;

	//if (m_fMKB >= 0.2f) {
	if (m_pOBB->Get_isHit()) {
		CEngine::GetInstance()->PlaySoundW("Blood.mp3", CHANNELID::ENEMY18);
		CEngine::GetInstance()->PlaySoundW("UrsaHit.ogg", CHANNELID::ENEMY17);

		_matrix Translation;
		_int random = rand() % 7;
		_int random2 = rand() % 5;
		_int random3 = rand() % 2;
		random += 1;
		if (random3 == 0)
			Translation = XMMatrixTranslation(XMVectorGetX(m_pTransform->GetState(CTransform::STATE_POSITION)) + ((float)random2*0.1f), XMVectorGetY(m_pTransform->GetState(CTransform::STATE_POSITION)) + ((float)random*0.1f), XMVectorGetZ(m_pTransform->GetState(CTransform::STATE_POSITION)));
		else
			Translation = XMMatrixTranslation(XMVectorGetX(m_pTransform->GetState(CTransform::STATE_POSITION)) + ((float)random2*-0.1f), XMVectorGetY(m_pTransform->GetState(CTransform::STATE_POSITION)) + ((float)random*0.1f), XMVectorGetZ(m_pTransform->GetState(CTransform::STATE_POSITION)));

		Translation = m_pTransform->Remove_Scale(Translation);

		for (int i = 0; i < 2; ++i) {

			CGameObject* EffectBlood = CEngine::GetInstance()->AddGameObjectToPrefab(CEngine::GetInstance()->GetCurSceneNumber(), "Prototype_Effect_IIBlood", "E_IIBlood", &Translation);
			CEngine::GetInstance()->AddScriptObject(m_pBlood = CEffectBlood::Create(EffectBlood), CEngine::GetInstance()->GetCurSceneNumber());

			//CGameObject* EffectBlood2 = CEngine::GetInstance()->AddGameObjectToPrefab(CEngine::GetInstance()->GetCurSceneNumber(), "Prototype_Effect_IBlood", "E_IBlood", &Translation);
			//CEngine::GetInstance()->AddScriptObject(CEffectBlood::Create(EffectBlood2), CEngine::GetInstance()->GetCurSceneNumber());
		}
		CGameObject* EffectBloodDecal = CEngine::GetInstance()->AddGameObjectToPrefab(CEngine::GetInstance()->GetCurSceneNumber(), "Prototype_Effect_BloodDecal", "E_BloodDecal");
		CEngine::GetInstance()->AddScriptObject(CEffectBloodDecal::Create(EffectBloodDecal, m_pTransform->GetState(CTransform::STATE_POSITION)), CEngine::GetInstance()->GetCurSceneNumber());

		m_fMKB = 0;

	}

}

void CUrsa::Create_Trail()
{
	CGameObject* pRightTrail = CEngine::GetInstance()->AddGameObject(CEngine::GetInstance()->GetCurSceneNumber(), "Prototype_EmptyEffect", "Ursa_Trail");
	if (pRightTrail == nullptr)
		return;

	CEmptyEffect* pEffect = static_cast<CEmptyEffect*>(pRightTrail);

	pEffect->SetPassIndex(3);
	//pEffect->SetTexture("../../Assets/Textures/Effect/Diffuse/nullImage.dds", CEmptyEffect::TEXTURE_DIFFUSE);
	pEffect->SetTexture("../../Assets/Textures/Effect/Mask/Trun_FX_Trail01_TEX_HKJ.jpg", CEmptyEffect::TEXTURE_MASK);
	pEffect->SetTexture("../../Assets/Textures/Effect/TrailDistortion/FX_FX_Dust_Aura01_NRM.tga", CEmptyEffect::TEXTURE_NOISE);


	pEffect->SetScrollSpeedX(_float3(0.5f, 0.5f, 0.f));
	pEffect->SetScrollSpeedY(_float3(0.f, 0.f, 0.f));
	pEffect->setDistortion(0, _float2(0.1f, 0.2f));
	pEffect->setDistortion(1, _float2(0.1f, 0.3f));
	pEffect->setDistortion(2, _float2(0.1f, 0.1f));
	pEffect->SetDistortionScale(4.f);
	pEffect->SetDistortionBias(1.f);

	CGameObject* pLeftTrail = CEngine::GetInstance()->AddGameObject(CEngine::GetInstance()->GetCurSceneNumber(), "Prototype_EmptyEffect", "Ursa_Trail");
	if (pLeftTrail == nullptr)
		return;

	pEffect = nullptr;
	pEffect = static_cast<CEmptyEffect*>(pLeftTrail);

	pEffect->SetPassIndex(3);
	//pEffect->SetTexture("../../Assets/Textures/Effect/Diffuse/nullImage.dds", CEmptyEffect::TEXTURE_DIFFUSE);
	pEffect->SetTexture("../../Assets/Textures/Effect/Mask/Trun_FX_Trail01_TEX_HKJ.jpg", CEmptyEffect::TEXTURE_MASK);
	pEffect->SetTexture("../../Assets/Textures/Effect/TrailDistortion/FX_FX_Dust_Aura01_NRM.tga", CEmptyEffect::TEXTURE_NOISE);

	pEffect->SetScrollSpeedX(_float3(0.5f, 0.5f, 0.f));
	pEffect->SetScrollSpeedY(_float3(0.f, 0.f, 0.f));
	pEffect->setDistortion(0, _float2(0.1f, 0.2f));
	pEffect->setDistortion(1, _float2(0.1f, 0.3f));
	pEffect->setDistortion(2, _float2(0.1f, 0.1f));
	pEffect->SetDistortionScale(4.f);
	pEffect->SetDistortionBias(1.f);

	XMStoreFloat4x4(&m_RightwpBoneMatrix, m_pModel->Get_BoneWithoutOffset("BN_Axe_R"));
	XMStoreFloat4x4(&m_LeftwpBoneMatrix, m_pModel->Get_BoneWithoutOffset("BN_Axe_L"));

	TRAILDESC TrailDesc;
	TrailDesc.vHighOffset = { 0.f, 0.f, -0.5f };
	TrailDesc.vLowOffset = { 0.f, 0.f, -0.2f };

	pRightTrail->AddComponent(0, "Prototype_VIBuffer_Trail", "Com_Trail", &TrailDesc);
	pLeftTrail->AddComponent(0, "Prototype_VIBuffer_Trail", "Com_Trail", &TrailDesc);
	m_pRightTrailBuffer = static_cast<CVIBuffer_Trail*>(pRightTrail->GetComponent("Com_Trail"));
	m_pLeftTrailBuffer = static_cast<CVIBuffer_Trail*>(pLeftTrail->GetComponent("Com_Trail"));
}

void CUrsa::CatchUpToLook(_double dDeltaTime)
{
	_vector vLook = m_pTransform->GetState(CTransform::STATE_LOOK);
	_vector vTargetToLook = XMLoadFloat3(&m_vTargetToLook);
	_float ratio = (_float)m_LerpTime / (_float)m_pModel->Get_AnimTime() * 0.3f;
	m_pTransform->SetLook(XMVectorLerp(vLook, vTargetToLook, ratio));
	m_LerpTime += dDeltaTime;
}

