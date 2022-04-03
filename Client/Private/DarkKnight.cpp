#include "stdafx.h"
#include "..\Public\DarkKnight.h"
#include "StateMachine.h"
#include "BasicCollider.h"
#include "Transform.h"
#include "MonHp.h"

USING(Client)

CDarkKnight::CDarkKnight(CGameObject* pObj)
	: CEnemy(pObj)
{
}


CDarkKnight * CDarkKnight::Create(CGameObject * pObj, _float3 position)
{
	CDarkKnight*		pInstance = new CDarkKnight(pObj);

	if (FAILED(pInstance->Initialize(position)))
	{
		MSG_BOX("Failed to Create CDarkKnight");
		SafeRelease(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CDarkKnight::Free()
{
	__super::Free();

}

HRESULT CDarkKnight::Initialize(_float3 position)
{
	m_bBehavior = false;

	m_pGameObject = CEngine::GetInstance()->FindGameObjectWithName(CEngine::GetInstance()->GetCurSceneNumber(), "DarkKinght");
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

	CGameObject* pTargetObj = CEngine::GetInstance()->FindGameObjectWithName(SCENE_STATIC, "Player");
	m_pTargetTransform = static_cast<CTransform*>(pTargetObj->GetComponent("Com_Transform"));
		
	m_pModel->SetAnimationLoop((_uint)STATE::SK_SIDESLASH2, false, true);
	m_pModel->SetAnimationLoop((_uint)STATE::SK_RAISING2, false, true);
	m_pModel->SetAnimationLoop((_uint)STATE::SK_SLASH2, false, true);
	m_pModel->SetAnimationLoop((_uint)STATE::SK_STING2, false, true);
	m_pModel->SetAnimationLoop((_uint)STATE::PHASE2_START, false, true);
	m_pModel->SetAnimationLoop((_uint)STATE::PHASE2_END, false, true);
	m_pModel->SetAnimationLoop((_uint)STATE::SK_SIDESLASH, false, true);
	m_pModel->SetAnimationLoop((_uint)STATE::SK_SHIELDATTACK, false, false);
	m_pModel->SetAnimationLoop((_uint)STATE::SK_JUMPATTACK, false, true);
	m_pModel->SetAnimationLoop((_uint)STATE::ENDTRYBATTLE, false, true);
	m_pModel->SetAnimationLoop((_uint)STATE::GETUP, false, true);
	m_pModel->SetAnimationLoop((_uint)STATE::DOWN, false, false);
	m_pModel->SetAnimationLoop((_uint)STATE::DMG_F, false, true);
	m_pModel->SetAnimationLoop((_uint)STATE::DMG_B, false, true);
	m_pModel->SetAnimationLoop((_uint)STATE::DIE, false, true);
	m_pModel->SetAnimationLoop((_uint)STATE::SLASH, false, true);
	m_pModel->SetAnimationLoop((_uint)STATE::STING, false, true);
	
	m_eState = IDLE;
	m_pModel->SetUp_AnimationIndex(m_eState);
	m_pMonHp = CMonHp::Create(m_pGameObject);

	Create_Trail();

	if (m_pTransform)
		XMStoreFloat3(&m_vCreatePos, m_pTransform->GetState(CTransform::STATE_POSITION));

	return S_OK;
}

void CDarkKnight::Update(_double dDeltaTime)
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
void CDarkKnight::LateUpdate(_double dDeltaTime)
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

void CDarkKnight::Render()
{
}

void CDarkKnight::RotateBody(_double deltaTime)
{
}

void CDarkKnight::SetGameObject(CGameObject* obj)
{
	m_pGameObject = obj;
}

void CDarkKnight::SetTransform(CTransform * trans)
{
	m_pTransform = trans;
}

void CDarkKnight::SetCollision(CCollider * colli)
{
	m_pCollider = colli;
}

void CDarkKnight::SetNavigation(CNavigation * nav)
{
	m_pNavigation = nav;
}

void CDarkKnight::SetModel(CModel * model)
{
	m_pModel = model;
}

void CDarkKnight::SettingCollider(CCollider * col)
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

void CDarkKnight::SetBasicCollider(CBasicCollider * BasicCol)
{
	//if (BasicCol)
	//{
	if (BasicCol == m_pGameObject->GetComponent("Com_OBB"))
		m_pOBBCom = BasicCol;
	//}
}

void CDarkKnight::SetStat(CStat * pstat)
{
	m_pStat = pstat;
}

void CDarkKnight::SetAttTarget(CGameObject * obj)
{
	m_pTargetTransform = static_cast<CTransform*>(obj->GetComponent("Com_Transform"));
}

void CDarkKnight::SetAttack(_double dDeltaTime)
{
	if (!m_bCombat)
		return;

	_vector vLook, vRight, vWolfLook;
	vLook = m_pTargetTransform->GetState(CTransform::STATE_LOOK);
	vRight = m_pTargetTransform->GetState(CTransform::STATE_RIGHT);
	vWolfLook = m_pTransform->GetState(CTransform::STATE_LOOK);
	PxVec3 vDir = PxVec3(0.f, 0.f, 0.f);
	PxControllerFilters filters;
	_float fSpeed = 0.f;
	_vector vUp = m_pTransform->GetState(CTransform::STATE::STATE_UP);


	_vector WolfLook = m_pTransform->GetState(CTransform::STATE_LOOK);
	WolfLook = XMVectorLerp(WolfLook, vRight, 0.4f);
	WolfLook = XMVectorSetY(WolfLook, 0.f);
	memcpy(&vDir, &vWolfLook, sizeof(PxVec3));
	fSpeed = 0.01f;

	m_pTransform->SetLook(vWolfLook);
	m_pController->move(vDir * 0.1f * 0.5f, 0.01f, 1.f / (_float)dDeltaTime, nullptr);
}

void CDarkKnight::SetMonHp(CMonHp * hp)
{
	m_pMonHp = hp;
}

void CDarkKnight::StateUpdate(_double dDeltaTime)
{
	if (m_bDead == true || m_bBehavior == true || m_pTargetTransform == nullptr)
		return;

	/*if (CEngine::GetInstance()->IsKeyDown('R'))
		m_bCombat = !m_bCombat;*/

	_int iRand = rand() % 100;
	m_fDist = Calculation_DistanceToPlayer();

	if (m_bCombat == false)
	{
		if (iRand <= 90)
		{
			m_vDestPos = _float3(m_vCreatePos.x + (rand() % 80 - 40) * 0.1f, m_vCreatePos.y, m_vCreatePos.z + (rand() % 80 - 40) * 0.1f);
			m_fBehaviorTime = (rand() % 20 + 20) * 0.1f;
			m_eState = Walk;
		}
		else
		{
			m_fBehaviorTime = (rand() % 10 + 5)* 0.1f;
			m_eState = IDLE;
		}

		m_bBehavior = true;
	}
	else
	{		
		if (m_bPhase2 == false) //Phase1
		{
			if (m_fDist >= 1.f)
				m_eState = RUN;
			else
			{
				if (iRand < 10)
					m_eState = SK_SHIELDATTACK;
				else if (iRand < 20)
					m_eState = SK_SIDESLASH;
				else if (iRand < 60)
					m_eState = STING;
				else 
					m_eState = SLASH;

				SetLookPlayer();
			}
		}
		else //Phase2
		{
			if (m_fDist >= 1.f)
			{
				if (iRand < 70)
				{
					m_eState = RUN;
					m_fBehaviorTime = (rand() % 10 + 10) * 0.1f;
				}
				else if (iRand < 85)
					m_eState = SK_SIDESLASH2;
				else
					m_eState = SK_RAISING2;
			}
			else
			{
				if (iRand < 33)
					m_eState = SK_SHIELDATTACK;
				else if (iRand < 66)
					m_eState = SK_SLASH2;
				else
					m_eState = SK_STING2;
			}
			SetLookPlayer();
		}

		m_bBehavior = true;
	}	
}

void CDarkKnight::BehaviorUpdate(_double dDeltaTime)
{
	/*if (m_bBehavior == false)
		return;*/

	switch (m_eState)
	{
	case Client::CDarkKnight::Walk:
		m_fBehaviorTime -= (_float)dDeltaTime;	
		ChaseTarget(dDeltaTime, m_vDestPos);
		break;
	case Client::CDarkKnight::SK_SIDESLASH2:
		break;
	case Client::CDarkKnight::SK_RAISING2:
		break;
	case Client::CDarkKnight::SK_STING2:
		break;
	case Client::CDarkKnight::PHASE2_START:
		break;
	case Client::CDarkKnight::PHASE2_LOOP:
		m_fPhaseLoopTime -= (_float)dDeltaTime;		
		break;
	case Client::CDarkKnight::PHASE2_END:
		break;
	case Client::CDarkKnight::SK_SIDESLASH:
		break;
	case Client::CDarkKnight::SK_SHIELDATTACK:
		break;
	case Client::CDarkKnight::SK_JUMPATTACK:
		break;
	case Client::CDarkKnight::RUN:
	{
		if (m_bPhase2 == true)
			m_fBehaviorTime -= (_float)dDeltaTime;

		_float3 vTargetPos;
		XMStoreFloat3(&vTargetPos, m_pTargetTransform->GetState(CTransform::STATE_POSITION));
		ChaseTarget(dDeltaTime, vTargetPos);
		break;
	}
	case Client::CDarkKnight::GROGGY:
		break;
	case Client::CDarkKnight::ENDTRYBATTLE:
		break;
	case Client::CDarkKnight::IDLE:
		m_fBehaviorTime -= (_float)dDeltaTime;		
		break;
	case Client::CDarkKnight::IDLE_BATTLE:
		break;
	case Client::CDarkKnight::GETUP:
		break;
	case Client::CDarkKnight::DOWN_LOOP:
		break;
	case Client::CDarkKnight::DOWN_DEADBODY:
		m_fDissolveAcc += (_float)dDeltaTime * 0.5f;
		if (m_fDissolveAcc > 1.f)
			m_fDissolveAcc = 1.f;
		m_pModel->SetDissolve(m_fDissolveAcc);
		break;
	case Client::CDarkKnight::DOWN:
		break;
	case Client::CDarkKnight::DMG_F:
		break;
	case Client::CDarkKnight::DMG_B:
		break;
	case Client::CDarkKnight::DIE:
		break;
	case Client::CDarkKnight::DEADBODY:
		m_fDissolveAcc += (_float)dDeltaTime * 0.5f;
		if (m_fDissolveAcc > 1.f)
			m_fDissolveAcc = 1.f;
		m_pModel->SetDissolve(m_fDissolveAcc);
		break;
	case Client::CDarkKnight::SLASH:
		break;
	case Client::CDarkKnight::STING:
		break;
	case Client::CDarkKnight::STATE_END:
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

void CDarkKnight::CheckAnimFinish()
{
	if (m_pModel->Get_isFinished())
	{
		switch (m_eState)
		{
		case Client::CDarkKnight::Walk:
		{
			_vector vDis = XMVector3Length(XMLoadFloat3(&m_vDestPos) - m_pTransform->GetState(CTransform::STATE_POSITION));
			if (m_fBehaviorTime <= 0.f || XMVectorGetX(vDis) <= 0.5f)
			{
				m_fBehaviorTime = 0.f;
				m_bBehavior = false;
				m_eState = IDLE;
			}
			break;
		}
		case Client::CDarkKnight::SK_SIDESLASH2:
			SetAttackDelay();
			m_eState = IDLE_BATTLE;
			break;
		case Client::CDarkKnight::SK_RAISING2:
			SetAttackDelay();
			m_eState = IDLE_BATTLE;
			break;
		case Client::CDarkKnight::SK_SLASH2:
			SetAttackDelay();
			m_eState = IDLE_BATTLE;
			break;
		case Client::CDarkKnight::SK_STING2:
			SetAttackDelay();
			m_eState = IDLE_BATTLE;
			break;
		case Client::CDarkKnight::PHASE2_START:
			m_eState = PHASE2_LOOP;
			break;
		case Client::CDarkKnight::PHASE2_LOOP:
			if (m_fPhaseLoopTime <= 0.f)
				m_eState = PHASE2_END;
			break;
		case Client::CDarkKnight::PHASE2_END:
			m_bBehavior = false;
			m_eState = IDLE_BATTLE;
			break;
		case Client::CDarkKnight::SK_SIDESLASH:
			SetAttackDelay();
			m_eState = IDLE_BATTLE;
			break;
		case Client::CDarkKnight::SK_SHIELDATTACK:
			SetAttackDelay();
			m_eState = IDLE_BATTLE;
			break;
		case Client::CDarkKnight::SK_JUMPATTACK:
			break;
		case Client::CDarkKnight::RUN:
		{
			if (m_bPhase2 == true && m_fBehaviorTime <= 0.f)
			{
				m_fBehaviorTime = 0.f;
				m_bBehavior = false;
				break;
			}

			_vector vDis = XMVector3Length(m_pTargetTransform->GetState(CTransform::STATE_POSITION) - m_pTransform->GetState(CTransform::STATE_POSITION));
			if (XMVectorGetX(vDis) < 1.f)
			{
				m_bBehavior = false;
				m_eState = IDLE_BATTLE;
			}
			break;
		}
		case Client::CDarkKnight::GROGGY:
			break;
		case Client::CDarkKnight::ENDTRYBATTLE:
			break;
		case Client::CDarkKnight::IDLE:
			if (m_fBehaviorTime <= 0.f)
			{
				m_fBehaviorTime = 0.f;
				m_bBehavior = false;
			}
			break;
		case Client::CDarkKnight::IDLE_BATTLE:
			break;
		case Client::CDarkKnight::GETUP:
			break;
		case Client::CDarkKnight::DOWN_LOOP:
			break;
		case Client::CDarkKnight::DOWN_DEADBODY:
			break;
		case Client::CDarkKnight::DOWN:
			break;
		case Client::CDarkKnight::DMG_F:
			m_bBehavior = false;
			m_eState = IDLE_BATTLE;
			break;
		case Client::CDarkKnight::DMG_B:
			m_bBehavior = false;
			m_eState = IDLE_BATTLE;
			break;
		case Client::CDarkKnight::DIE:
			m_eState = DEADBODY;
			break;
		case Client::CDarkKnight::DEADBODY:
			break;
		case Client::CDarkKnight::SLASH:
			SetAttackDelay();
			m_eState = IDLE_BATTLE;
			break;
		case Client::CDarkKnight::STING:
			SetAttackDelay();
			m_eState = IDLE_BATTLE;
			break;
		case Client::CDarkKnight::STATE_END:
			break;
		default:
			break;
		}
	}
}

void CDarkKnight::Update_Trail(_double dDeltaTime)
{
	_matrix OffsetMatrix = XMMatrixIdentity();
	XMMatrixTranslation(-0.0f, 0.f, -0.33f);

	XMStoreFloat4x4(&m_wpBoneMatrix, m_pModel->Get_BoneWithoutOffset("Bip01-R-Hand"));

	if (m_pTrailBuffer)
		m_pTrailBuffer->Update(dDeltaTime, OffsetMatrix * XMLoadFloat4x4(&m_wpBoneMatrix) * XMLoadFloat4x4(&m_pRenderTransform->GetMatrix()));
}

void CDarkKnight::Create_Trail()
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
	pEffect->SetOffsetColor(_float4(0.8f, 0.f, 0.8f, 0.f));
		
	TRAILDESC TrailDesc;
	TrailDesc.vHighOffset = { 0.f, 0.f, -0.3f };
	TrailDesc.vLowOffset = { 0.f, 0.f, 0.f };

	pTrail->AddComponent(0, "Prototype_VIBuffer_Trail", "Com_Trail", &TrailDesc);
	m_pTrailBuffer = static_cast<CVIBuffer_Trail*>(pTrail->GetComponent("Com_Trail"));
}

void CDarkKnight::Hit()
{
	if (m_bDead = false && m_pStat->GetStatInfo().hp <= 0.f)
	{
		m_bDead = true;
		m_eState = DIE;
	}

	if (m_bPhase2 == false)
	{
		_float fMaxHp = m_pStat->GetStatInfo().maxHp;

		if (m_pStat->GetStatInfo().hp <= fMaxHp * 0.5f)
		{
			m_fSpeed = 1.3f;
			m_bPhase2 = true;
			m_bBehavior = true;
			m_eState = PHASE2_START;
		}
	}

	if (m_bPhase2 == false && m_pOBB->Get_isHit() == true)
	{
		if (m_bCombat == false)
		{
			m_fSpeed = 1.f;
			m_bCombat = true;
		}
				
		if (m_eState == IDLE || m_eState == IDLE_BATTLE || m_eState == Walk || m_eState == RUN)
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

_float CDarkKnight::Calculation_DistanceToPlayer()
{
	_vector vPosition = m_pTransform->GetState(CTransform::STATE_POSITION);
	_vector vTargetPostion = m_pTargetTransform->GetState(CTransform::STATE_POSITION);
	
	return XMVectorGetX(XMVector3Length(vTargetPostion - vPosition));
}

void CDarkKnight::SetLookPlayer()
{
	_vector vLook = XMVector3Normalize(m_pTargetTransform->GetState(CTransform::STATE_POSITION) - m_pTransform->GetState(CTransform::STATE_POSITION));
	m_pTransform->SetLook(vLook);
}

void CDarkKnight::ChaseTarget(_double deltaTime, _float3 vTargetPos)
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