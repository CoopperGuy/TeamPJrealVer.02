#include "stdafx.h"
#include "..\Public\Skull.h"
#include "StateMachine.h"
#include "BasicCollider.h"
#include "Transform.h"
#include "MonHp.h"

USING(Client)

CSkull::CSkull(CGameObject* pObj)
	: CEnemy(pObj)
{
}


CSkull * CSkull::Create(CGameObject * pObj, _float3 position)
{
	CSkull*		pInstance = new CSkull(pObj);

	if (FAILED(pInstance->Initialize(position)))
	{
		MSG_BOX("Failed to Create CSkull");
		SafeRelease(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CSkull::Free()
{
	__super::Free();

}

HRESULT CSkull::Initialize(_float3 position)
{
	m_bBehavior = false;

	m_pGameObject = CEngine::GetInstance()->FindGameObjectWithName(CEngine::GetInstance()->GetCurSceneNumber(), "Skull");
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
	m_pWeaponOBB = static_cast<CBasicCollider*>(m_pGameObject->GetComponent("Com_OBB1"));

	CGameObject* pTargetObj = CEngine::GetInstance()->FindGameObjectWithName(SCENE_STATIC, "Player");
	m_pTargetTransform = static_cast<CTransform*>(pTargetObj->GetComponent("Com_Transform"));
		
	m_pModel->SetAnimationLoop((_uint)STATE::SK_TWICESLASH, false, true);
	m_pModel->SetAnimationLoop((_uint)STATE::SK_DOWNSLASH, false, true);	
	m_pModel->SetAnimationLoop((_uint)STATE::DMG_F, false, true);
	m_pModel->SetAnimationLoop((_uint)STATE::DMG_B, false, true);
	m_pModel->SetAnimationLoop((_uint)STATE::DIE, false, true);
	m_pModel->SetAnimationLoop((_uint)STATE::SLASH, false, true);
	m_pModel->SetAnimationLoop((_uint)STATE::STING, false, true);
	
	m_eState = IDLE;
	m_pModel->SetUp_AnimationIndex(m_eState);
	//m_pMonHp = CMonHp::Create(m_pGameObject);

	Create_Trail();

	if (m_pTransform)
		XMStoreFloat3(&m_vCreatePos, m_pTransform->GetState(CTransform::STATE_POSITION));

	return S_OK;
}

void CSkull::Update(_double dDeltaTime)
{
	if (!m_pGameObject)
		return;

	if (!m_pGameObject->IsActive())
		return;
	
	__super::Update(dDeltaTime);

	/*if (CEngine::GetInstance()->IsKeyDown('R'))
	{
		_uint i = (_uint)m_eState;
		++i;
		if ((_uint)STATE_END == i)
			i = 0;
		m_eState = (STATE)i;
		m_pModel->SetUp_AnimationIndex(m_eState);

	}*/
	Hit();
	
	StateUpdate(dDeltaTime);
	CheckAnimFinish();
	BehaviorUpdate(dDeltaTime);


	if (m_pCollider) {
		PxExtendedVec3 footpos = m_pCollider->GetController()->getFootPosition();
	}
	
	//fall down
	PxControllerFilters filters;
	m_pController->move(PxVec3(0.0f, -0.1f, 0.f), 0.01f, PxF32(1.f / dDeltaTime), filters);
}
void CSkull::LateUpdate(_double dDeltaTime)
{
	__super::LateUpdate(dDeltaTime);
	
	m_pModel->SetUp_AnimationIndex(m_eState);
	m_pModel->Play_Animation(dDeltaTime);
	
	Update_Trail(dDeltaTime);

	if (m_bDissolve)
	{
		m_fDissolveAcc += (_float)dDeltaTime * 0.5f;
		if (m_fDissolveAcc > 1.f)
			m_fDissolveAcc = 1.f;

		m_pModel->SetDissolve(m_fDissolveAcc);
	}
}

void CSkull::Render()
{
}

void CSkull::RotateBody(_double deltaTime)
{
}

void CSkull::SetGameObject(CGameObject* obj)
{
	m_pGameObject = obj;
}

void CSkull::SetTransform(CTransform * trans)
{
	m_pTransform = trans;
}

void CSkull::SetCollision(CCollider * colli)
{
	m_pCollider = colli;
}

void CSkull::SetNavigation(CNavigation * nav)
{
	m_pNavigation = nav;
}

void CSkull::SetModel(CModel * model)
{
	m_pModel = model;
}

void CSkull::SettingCollider(CCollider * col)
{
	if (col)
	{
		m_pController = col->GetController();
		//m_pController->getActor()->userData = this;
		PxShape* shape = nullptr;
		m_pController->getActor()->getShapes(&shape, 1);
		PxFilterData filterData;
		filterData.word0 = CPxManager::GROUP1;
		filterData.word1 = CPxManager::GROUP4;
		filterData.word2 = CPxManager::GROUP3;
		shape->setQueryFilterData(filterData);
	}
}

void CSkull::SetBasicCollider(CBasicCollider * BasicCol)
{
	//if (BasicCol)
	//{
	if (BasicCol == m_pGameObject->GetComponent("Com_OBB"))
		m_pOBBCom = BasicCol;
	//}
}

void CSkull::SetStat(CStat * pstat)
{
	m_pStat = pstat;
}

void CSkull::SetAttTarget(CGameObject * obj)
{
	m_pTargetTransform = static_cast<CTransform*>(obj->GetComponent("Com_Transform"));
}

void CSkull::SetMonHp(CMonHp * hp)
{
	m_pMonHp = hp;
}

void CSkull::StateUpdate(_double dDeltaTime)
{
	if (m_bDeadBody == true || m_bBehavior == true || m_pTargetTransform == nullptr)
		return;

	/*if (CEngine::GetInstance()->IsKeyDown('R'))
		m_bCombat = !m_bCombat;*/

	_int iRand = rand() % 100;
	m_fDist = Calculation_DistanceToPlayer();
	
	if (m_fDist <= 3.f && m_fDist >= m_AttackRange)
	{
		m_fBehaviorTime = (rand() % 10 + 5) * 0.1f;
		m_eState = RUN;
	}
	else if (m_fDist < 1.f)
	{
		if (iRand < 10)
			m_eState = SK_TWICESLASH;
		else if (iRand < 20)
			m_eState = SK_DOWNSLASH;
		else if (iRand < 60)
			m_eState = STING;
		else 
			m_eState = SLASH;

		SetLookPlayer();
		m_bBehavior = true;
	}
}

void CSkull::BehaviorUpdate(_double dDeltaTime)
{
	/*if (m_bBehavior == false)
		return;*/
	_uint keyFrame = m_pModel->GetCurrentKeyFrame();
	m_pTrailBuffer->SetIsActive(false);
	m_pWeaponOBB->p_States = CBasicCollider::STATES::STATES_IDEL;

	switch (m_eState)
	{
	case Client::CSkull::SK_TWICESLASH:
		if (keyFrame >= 15 && keyFrame <= 35)
		{
			m_pWeaponOBB->p_States = CBasicCollider::STATES::STATES_ATK;

			m_pTrailBuffer->SetIsActive(true);
		}
		break;
	case Client::CSkull::SK_DOWNSLASH:
		if (keyFrame >= 15 && keyFrame <= 30)
		{
			m_pWeaponOBB->p_States = CBasicCollider::STATES::STATES_ATK;

			m_pTrailBuffer->SetIsActive(true);
		}
		break;
	case Client::CSkull::RUN:
	{		
		m_fBehaviorTime -= (_float)dDeltaTime;

		_float3 vTargetPos;
		XMStoreFloat3(&vTargetPos, m_pTargetTransform->GetState(CTransform::STATE_POSITION));
		ChaseTarget(dDeltaTime, vTargetPos);
		break;
	}	
	case Client::CSkull::IDLE:
		break;	
	case Client::CSkull::DMG_F:
		break;
	case Client::CSkull::DMG_B:
		break;
	case Client::CSkull::DIE:
		break;
	case Client::CSkull::DEADBODY:
		m_fDissolveDelay -= (_float)dDeltaTime;
		if (m_fDissolveDelay <= 0.f)
		{
			m_fDissolveAcc += (_float)dDeltaTime * 0.3f;
			if (m_fDissolveAcc > 1.f)
				m_fDissolveAcc = 1.f;
			m_pModel->SetDissolve(m_fDissolveAcc);
		}
		break;
	case Client::CSkull::SLASH:
		if (keyFrame >= 0 && keyFrame <= 15)
		{
			m_pWeaponOBB->p_States = CBasicCollider::STATES::STATES_ATK;

			m_pTrailBuffer->SetIsActive(true);
		}
		break;
	case Client::CSkull::STING:
		if (keyFrame >= 30 && keyFrame <= 50)
		{
			m_pWeaponOBB->p_States = CBasicCollider::STATES::STATES_ATK;

			m_pTrailBuffer->SetIsActive(true);
		}
		break;
	case Client::CSkull::STATE_END:		
		break;
	default:
		break;
	}
	
	if (m_fAttackDelay > 0.f)
	{
		m_fAttackDelay -= (_float)dDeltaTime;

		if (m_fAttackDelay <= 0.f)
		{
			m_fAttackDelay = 0.f;
			m_bBehavior = false;
		}
	}
}

void CSkull::CheckAnimFinish()
{
	if (m_pModel->Get_isFinished())
	{
		switch (m_eState)
		{
		case Client::CSkull::SK_TWICESLASH:
			SetAttackDelay();
			m_eState = IDLE;
			break;
		case Client::CSkull::SK_DOWNSLASH:
			SetAttackDelay();
			m_eState = IDLE;
			break;
		case Client::CSkull::RUN:
		{
			if (m_fBehaviorTime <= 0.f)
			{
				m_eState = IDLE;
				m_fBehaviorTime = 0.f;
				m_bBehavior = false;
				break;
			}

			_vector vDis = XMVector3Length(m_pTargetTransform->GetState(CTransform::STATE_POSITION) - m_pTransform->GetState(CTransform::STATE_POSITION));
			if (XMVectorGetX(vDis) < m_AttackRange)
			{
				m_bBehavior = false;
				m_eState = IDLE;
			}
			break;
		}
		case Client::CSkull::IDLE:
			break;
		case Client::CSkull::DMG_F:
			m_bBehavior = false;
			m_eState = IDLE;
			break;
		case Client::CSkull::DMG_B:
			m_bBehavior = false;
			m_eState = IDLE;
			break;
		case Client::CSkull::DIE:
			m_eState = DEADBODY;
			break;
		case Client::CSkull::DEADBODY:
			break;
		case Client::CSkull::SLASH:
			SetAttackDelay();
			m_eState = IDLE;
			break;
		case Client::CSkull::STING:
			SetAttackDelay();
			m_eState = IDLE;
			break;
		case Client::CSkull::STATE_END:
			break;
		default:
			break;
		}
	}
}

void CSkull::Update_Trail(_double dDeltaTime)
{
	_matrix OffsetMatrix = XMMatrixIdentity();
	XMMatrixTranslation(-0.03f, 0.17f, 0.005f);

	XMStoreFloat4x4(&m_wpBoneMatrix, m_pModel->Get_BoneWithoutOffset("BN_Weapon_R_01"));

	if (m_pTrailBuffer)
		m_pTrailBuffer->Update(dDeltaTime, OffsetMatrix * XMLoadFloat4x4(&m_wpBoneMatrix) * XMLoadFloat4x4(&m_pRenderTransform->GetMatrix()));
}

void CSkull::Create_Trail()
{
	CGameObject* pTrail = CEngine::GetInstance()->AddGameObject(CEngine::GetInstance()->GetCurSceneNumber(), "Prototype_EmptyEffect", "Kinght_Trail");
	if (pTrail == nullptr)
		return;

	CEmptyEffect* pEffect = static_cast<CEmptyEffect*>(pTrail);

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
	pEffect->SetOffsetColor(_float4(1.f, 1.f, 1.f, 0.f));
	
	TRAILDESC TrailDesc;
	TrailDesc.vHighOffset = { 0.f, 0.2f, 0.f };
	TrailDesc.vLowOffset = { 0.f, 0.f, 0.f };

	pTrail->AddComponent(0, "Prototype_VIBuffer_Trail", "Com_Trail", &TrailDesc);
	m_pTrailBuffer = static_cast<CVIBuffer_Trail*>(pTrail->GetComponent("Com_Trail"));
}

void CSkull::Hit()
{
	if (m_bDeadBody == false && m_pStat->GetStatInfo().hp <= 0.f)
	{
		m_bDeadBody = true;
		m_eState = DIE;
	}
	
	if (m_pOBB->Get_isHit() == true)
	{
		if (m_bCombat == false)
			m_bCombat = true;
			
		if (m_eState == IDLE || m_eState == RUN)
		{
			_vector vLook = XMVector3Normalize(m_pTransform->GetState(CTransform::STATE_LOOK));
			_vector vDestLook = XMVector3Normalize(m_pTargetTransform->GetState(CTransform::STATE_POSITION) - m_pTransform->GetState(CTransform::STATE_POSITION));

			if (XMVectorGetX(XMVector3Dot(vLook, vDestLook)) < 0.f)
				m_eState = DMG_B;
			else
				m_eState = DMG_F;
			m_bBehavior = true;
		}		
	}	
}

_float CSkull::Calculation_DistanceToPlayer()
{
	_vector vPosition = m_pTransform->GetState(CTransform::STATE_POSITION);
	_vector vTargetPostion = m_pTargetTransform->GetState(CTransform::STATE_POSITION);
	
	return XMVectorGetX(XMVector3Length(vTargetPostion - vPosition));
}

void CSkull::SetLookPlayer()
{
	_vector vLook = XMVector3Normalize(m_pTargetTransform->GetState(CTransform::STATE_POSITION) - m_pTransform->GetState(CTransform::STATE_POSITION));
	m_pTransform->SetLook(vLook);
}

void CSkull::ChaseTarget(_double deltaTime, _float3 vTargetPos)
{
	_float fScale = m_pTransform->GetScale(CTransform::STATE_RIGHT);
	_vector vPosition = m_pTransform->GetState(CTransform::STATE_POSITION);
	vPosition = XMVectorSetY(vPosition, 0.f);
	_vector vTargetPostion = XMVectorSet(vTargetPos.x, vTargetPos.y, vTargetPos.z, 1.f);
	vTargetPostion = XMVectorSetY(vTargetPostion, 0.f);

	_vector vLook = XMVector3Normalize(m_pTransform->GetState(CTransform::STATE_LOOK));
	_vector vDestLook = XMVector3Normalize(vTargetPostion - vPosition);
	_vector vAxisY = XMVectorSet(0.f, 1.f, 0.f, 0.f);
	_vector vUp;
	_vector vRight;

	if (XMVectorGetX(XMVector3Dot(vLook, vDestLook)) < 0.f)
		vLook = XMVectorLerp(vLook, vDestLook, 0.7f);
	else
		vLook = XMVectorLerp(vLook, vDestLook, 0.1f);
	vRight = XMVector3Cross(vAxisY, vLook);
	vUp = XMVector3Cross(vLook, vRight);
	
	_float3 vDir;
	XMStoreFloat3(&vDir, XMVector3Normalize(vLook));
	m_pController->move(PxVec3(vDir.x, vDir.y, vDir.z) * m_fSpeed * (_float)deltaTime, 0.0001f, (_float)deltaTime, nullptr);

	m_pTransform->SetState(CTransform::STATE_RIGHT, XMVector3Normalize(vRight) * fScale);
	m_pTransform->SetState(CTransform::STATE_UP, XMVector3Normalize(vUp) * fScale);
	m_pTransform->SetState(CTransform::STATE_LOOK, XMVector3Normalize(vLook) * fScale);
}