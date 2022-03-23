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

	m_pModel->SetAnimationLoop((_uint)Ursa::CB_START, false);
	m_pModel->SetAnimationLoop((_uint)Ursa::DASH_ATT, false, true);
	m_pModel->SetAnimationLoop((_uint)Ursa::L_SLASH, false);
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
	m_fDist = SetDistance();


	if (m_bCombat[First])
	{
		if (!m_bCB)
			Adjust_Dist(dDeltaTime);
	}
	Checking_Phase(dDeltaTime);
	Execute_Pattern(dDeltaTime);

	if (CEngine::GetInstance()->Get_DIKDown(DIK_P))
		m_bCombat[First] = true;
	if (m_pCollider) 
		PxExtendedVec3 footpos = m_pCollider->GetController()->getFootPosition();

	m_pModel->SetUp_AnimationIndex((_uint)m_eState);
	Checking_Finished();
	if(m_bCB)
		SetUp_Combo();
	m_pStat->SetSTATE(m_eCurSTATES);
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
	_int Drawing = rand() % 100;

	if (m_fDist >= 6.5f)
	{
		m_bMove	 = true;
		m_bFar	 = true;
		m_bClose = false;
	}
	else if (m_fDist >= 1.f)
	{
		m_bMove	 = true;
		m_bClose = false;
	}
	else
	{
		m_bClose = true;
		m_bMove  = false;
	}

	_vector vLook, vTargetLook;
	vLook = m_pTransform->GetState(CTransform::STATE_LOOK);
	vTargetLook = XMLoadFloat3(&m_vTargetToLook);
	PxVec3 vDir = PxVec3(0.f, 0.f, 0.f);
	PxControllerFilters filters;
	if (m_bMove)
	{
		m_eState = RUN;
	
		vLook = XMVectorLerp(vLook, vTargetLook, 0.5f);
		vLook = XMVectorSetY(vLook, 0.f);
		m_pTransform->SetLook(vLook);
		memcpy(&vDir, &vLook, sizeof(_float3));
		m_pController->move(vDir * 0.5f * (_float)dDeltaTime, 0.f, (_float)dDeltaTime, nullptr);
	}
}

void CUrsa::Checking_Phase(_double dDeltaTime)
{
	_float Max = m_pStat->GetStatInfo().maxHp;
	if (m_pStat->GetStatInfo().hp < Max)
	{
		if(!m_bCB)
			Adjust_Dist(dDeltaTime);
		m_bCombat[First] = true;
		if (m_pStat->GetStatInfo().hp < Max * 0.7f)
		{
			memset(m_bCombat, false, sizeof(m_bCombat));
			m_bCombat[Second] = true;
			if (m_pStat->GetStatInfo().hp < Max * 0.4f)
			{
				memset(m_bCombat, false, sizeof(m_bCombat));
				m_bCombat[Third] = true;
				if (m_pStat->GetStatInfo().hp < Max * 0.1f)
					memset(m_bCombat, false, sizeof(m_bCombat));
					m_bCombat[Last] = true;
			}
		}
	}
	
}

void CUrsa::Execute_Pattern(_double dDeltaTime)
{
	if (m_bFar)
	{
		if (!m_bCombat[Last])
			m_eState = DASH_ATT;
		else
			m_eState = DASH_ATTSpeedup;

		if (m_pModel->Get_isFinished((_uint)m_eState))
			m_bFar = false;

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

void CUrsa::First_Phase(_double dDeltaTime)
{
	if (m_QueState.empty())
	{
		++m_iComboIndex;
		m_bCB = true;
		if (m_iComboIndex > 3)
			m_iComboIndex = 0;
	}
}

void CUrsa::Second_Phase(_double dDeltaTime)
{
}

void CUrsa::Third_Phase(_double dDeltaTime)
{
}

void CUrsa::SetUp_Combo()
{
	if (m_bCombat[First])
	{
		if (m_QueState.empty() && m_bCB)
		{
			switch (m_iComboIndex)
			{
			case 1:
			m_QueState.push(Combo_1Start);
			m_QueState.push(Combo_1Hold);
			m_QueState.push(Combo_1);
			m_QueState.push(Combo_1End);
			m_QueState.push(Combo_2Start);
			m_QueState.push(Combo_2End);
			m_eState = m_QueState.front();
			m_QueState.pop();
			break;
			case 2:
			m_QueState.push(Combo_1Start);
			m_QueState.push(Combo_1Hold);
			m_QueState.push(Combo_1);
			m_QueState.push(Combo_1End);
			m_QueState.push(R_SLASH);
			m_eState = m_QueState.front();
			m_QueState.pop();
			break;
			case 3:
			Empty_queue();
			m_eState = Big_SLASH;
			break;
			default:
				break;
			}
		}
	}
	else if (m_bCombat[Second])
	{

	}
	else if (m_bCombat[Third])
	{

	}
}

void CUrsa::Checking_Finished()
{
	if (m_pModel->Get_isFinished())
	{
		if (m_QueState.empty())
			m_bCB = false;
		if (!m_QueState.empty())
		{
			m_eState = m_QueState.front();
			m_QueState.pop();
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