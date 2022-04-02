#include "stdafx.h"
#include "..\Public\DarkKnight.h"
#include "StateMachine.h"
#include "BasicCollider.h"
#include "Transform.h"
#include "MonHp.h"
BEGIN(Client)
void CreateWolfThread(CDarkKnight* obj, _float3 pos) {
	CGameObject*	pGameObject = CEngine::GetInstance()->SpawnPrefab("WolfBoss");
	CTransform* trans = static_cast<CTransform*>(pGameObject->GetComponent("Com_Transform"));
	CCollider* m_pCollider = static_cast<CCollider*>(pGameObject->GetComponent("Com_Collider"));
	CStat* pStat = static_cast<CStat*>(pGameObject->GetComponent("Com_Stat"));
	CGameObject* pPlayer = CEngine::GetInstance()->FindGameObjectWithName(SCENE_STATIC, "Player");
	CBasicCollider * m_pOBB = static_cast<CBasicCollider*>(pGameObject->GetComponent("Com_OBB"));

	CEngine::GetInstance()->AddScriptObject(obj, CEngine::GetInstance()->GetCurSceneNumber());

	obj->SetGameObject(pGameObject);
	obj->SetTransform(trans);
	trans->SetState(CTransform::STATE_POSITION, XMLoadFloat3(&pos));
	obj->SetModel(static_cast<CModel*>(pGameObject->GetComponent("Com_Model")));
	obj->SetCollision(m_pCollider);
	obj->SettingCollider(m_pCollider);
	obj->SetStat(pStat);
	obj->SetAttTarget(pPlayer);
	if (m_pOBB)
		obj->SetBasicCollider(m_pOBB);

	//pGameObject->AddColliderList(pGameObject);
}
END

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
	//std::thread name(CreateWolfThread, this , position);
	//name.join();

	//CGameObject* pPlayer = CEngine::GetInstance()->FindGameObjectWithName(SCENE_STATIC, "Player");
	//m_pTargetPos = dynamic_cast<CTransform*>(pPlayer->GetComponent("Com_Transform"));
	//WolfIdle = true;

	m_bBehavior = false;

	m_pGameObject = CEngine::GetInstance()->FindGameObjectWithName(CEngine::GetInstance()->GetCurSceneNumber(), "WolfBoss");
	if (m_pGameObject == nullptr)
		return E_FAIL;

	m_pTransform = static_cast<CTransform*>(m_pGameObject->GetComponent("Com_Transform"));
	m_pModel = static_cast<CModel*>(m_pGameObject->GetComponent("Com_Model"));
	m_pNavigation = static_cast<CNavigation*>(m_pGameObject->GetComponent("Com_Navigation"));
	m_pCollider = static_cast<CCollider*>(m_pGameObject->GetComponent("Com_Collider"));
	if (m_pCollider)
	{
		m_pController = m_pCollider->GetController();
		//m_pController->getActor()->userData = this;
	}
	m_pStat = static_cast<CStat*>(m_pGameObject->GetComponent("Com_Stat"));


	CGameObject* pTargetObj = CEngine::GetInstance()->FindGameObjectWithName(SCENE_STATIC, "Player");
	m_pTargetTransform = static_cast<CTransform*>(pTargetObj->GetComponent("Com_Transform"));
		
	m_pModel->SetAnimationLoop((_uint)WOLFSTATE::Firing_105_R_Start, false, true);
	m_pModel->SetAnimationLoop((_uint)WOLFSTATE::Firing_105_R_End, false, true);
	m_pModel->SetAnimationLoop((_uint)WOLFSTATE::Firing_105_R, false, true);
	m_pModel->SetAnimationLoop((_uint)WOLFSTATE::Firing_105_L_Start, false, true);
	m_pModel->SetAnimationLoop((_uint)WOLFSTATE::Firing_105_L_End, false, true);
	m_pModel->SetAnimationLoop((_uint)WOLFSTATE::Firing_105_L, false, true);
	m_pModel->SetAnimationLoop((_uint)WOLFSTATE::Firing_103_Start, false, false);
	m_pModel->SetAnimationLoop((_uint)WOLFSTATE::Firing_103_Short, false, true);
	m_pModel->SetAnimationLoop((_uint)WOLFSTATE::Firing_103, false, true);
	m_pModel->SetAnimationLoop((_uint)WOLFSTATE::Firing_102, false, true);
	m_pModel->SetAnimationLoop((_uint)WOLFSTATE::Firing_101, false, false);
	m_pModel->SetAnimationLoop((_uint)WOLFSTATE::Die, false, true);
	m_pModel->SetAnimationLoop((_uint)WOLFSTATE::Combo108_Back, false, true);
	m_pModel->SetAnimationLoop((_uint)WOLFSTATE::Combo108, false, true);
	m_pModel->SetAnimationLoop((_uint)WOLFSTATE::Combo107_R, false, true);
	m_pModel->SetAnimationLoop((_uint)WOLFSTATE::Combo106_L, false, true);
	m_pModel->SetAnimationLoop((_uint)WOLFSTATE::Combo105_Back, false, true);
	m_pModel->SetAnimationLoop((_uint)WOLFSTATE::Combo105, false, true);
	m_pModel->SetAnimationLoop((_uint)WOLFSTATE::Combo102_R_Start, false, true);
	m_pModel->SetAnimationLoop((_uint)WOLFSTATE::Combo102_R, false, true);
	m_pModel->SetAnimationLoop((_uint)WOLFSTATE::Combo101_L_Start, false, true);
	m_pModel->SetAnimationLoop((_uint)WOLFSTATE::Combo101_L, false, true);

	m_pModel->SetUp_AnimationIndex(m_eState);

	m_pMonHp = CMonHp::Create(m_pGameObject);

	return S_OK;
}

void CDarkKnight::Update(_double dDeltaTime)
{
	if (!m_pGameObject)
		return;

	if (!m_pGameObject->IsActive())
		return;

	//if (m_pStat->GetStatInfo().hp <= 0)
	//	m_pGameObject->SetDead();

	__super::Update(dDeltaTime);
	StateUpdate(dDeltaTime);
	CheckAnimFinish();
	m_pModel->SetUp_AnimationIndex(m_eState);
	BehaviorUpdate(dDeltaTime);

	if (m_pCollider) {
		PxExtendedVec3 footpos = m_pCollider->GetController()->getFootPosition();
		//cout << "wolf king foot pos " << footpos.x << "\t" << footpos.y << "\t" << footpos.z << "\n";
	}
	/*if (m_pTargetPos)
	{
		WolfSetAni(dDeltaTime);
		WolfStateUpdate(dDeltaTime);
	}*/

	//fall down
	PxControllerFilters filters;
	m_pController->move(PxVec3(0.0f, -0.1f, 0.f), 0.01f, PxF32(1.f / dDeltaTime), filters);


}
void CDarkKnight::LateUpdate(_double dDeltaTime)
{
	__super::LateUpdate(dDeltaTime);
	if (m_pStat) {
		if (CEngine::GetInstance()->Get_DIKDown(DIK_F10)) {
			this->SetDead();
			m_pGameObject->SetDead();
		}
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
	if (!WolfAtt)
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
	if (m_pTargetTransform == nullptr)
	{
		m_eState = Idle;
		return;
	}

	if (CEngine::GetInstance()->IsKeyDown('R'))
		WolfAtt = !WolfAtt;

	if (WolfAtt == true && m_bBehavior == false)
	{	
		_vector vPosition = m_pTransform->GetState(CTransform::STATE_POSITION);
		_vector vTargetPostion = m_pTargetTransform->GetState(CTransform::STATE_POSITION);
		_float fDist = XMVectorGetX(XMVector3Length(vTargetPostion - vPosition));
		_float fMinDist = 2.f;
		_float fMaxDist = 5.f;
		_int iNumRand = -1;

		if (fMaxDist > fDist && fDist > fMinDist)
		{
			iNumRand = rand() % 100;
			
			if (iNumRand >= 80)
			{
				m_eState = Firing_103_Start;
				m_pTransform->SetLook(XMVector3Normalize(vTargetPostion - vPosition));			
			}
			else if (iNumRand >= 0)
			{
				m_eState = Run;
				m_fBehaviorDuration = 1.f;
			}		

			m_bBehavior = true;
		}
		else if (fDist <= fMinDist)
		{
			iNumRand = rand() % 100;
			
			if (iNumRand >= 95)
			{
				m_eState = Firing_105_L_Start;
			}
			else if (iNumRand >= 90)
			{
				m_eState = Firing_105_R_Start;
			}
			else if (iNumRand >= 45)
			{
				m_eState = Combo105;
			}
			else if (iNumRand >= 0)
			{
				m_eState = Combo108;
			}

			if (m_fStunDelay == 0.f)
			{
				m_eState = Firing_101;
			}
			else if (m_fStunDelay > 0.f)
			{
				m_fStunDelay -= (_float)dDeltaTime;
				if (m_fStunDelay <= 0.f)
					m_fStunDelay = 0.f;
			}

			m_pTransform->SetLook(XMVector3Normalize(vTargetPostion - vPosition));
			m_bBehavior = true;
		}
	}	
}

void CDarkKnight::BehaviorUpdate(_double dDeltaTime)
{
	if (m_eState == Run)
	{
		_vector vPosition = m_pTransform->GetState(CTransform::STATE_POSITION);
		_vector vTargetPostion = m_pTargetTransform->GetState(CTransform::STATE_POSITION);
		_float3 vDia;
		_float fDist = XMVectorGetX(XMVector3Length(vTargetPostion - vPosition));
		_float fMinDist = 0.5f;
		_float fMaxDist = 5.f;

		m_fBehaviorDuration -= (_float)dDeltaTime;
		if (m_fBehaviorDuration <= 0.f || fDist <= fMinDist)
		{
			m_eState = Idle_Battle;
			m_bBehavior = false;
		}
		else
		{
			XMStoreFloat3(&vDia, XMVector3Normalize(vTargetPostion - vPosition));
			m_pTransform->SetLook(XMVector3Normalize(vTargetPostion - vPosition));
			m_pController->move(PxVec3(vDia.x, vDia.y, vDia.z) * 0.03f, 0.01f, 1.f / (_float)dDeltaTime, nullptr);
		}
	}

	if (m_fBehaviorDelay > 0.f)
	{
		m_fBehaviorDelay -= (_float)dDeltaTime;

		if (m_fBehaviorDelay <= 0.f)
		{
			m_fBehaviorDelay = 0.f;
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
		case Combo105:
			m_eState = Combo105_Back;
			break;
		case Combo105_Back:
			m_eState = Idle_Battle;
			SetBehaviorDelay();
			break;
		case Combo108:
			m_eState = Combo108_Back;
			break;
		case Combo108_Back:
			m_eState = Idle_Battle;
			SetBehaviorDelay();
			break;
		case Firing_103_Start:
			m_eState = Firing_103_Short;
			break;
		case Firing_103_Short:
			m_eState = Idle_Battle;
			SetBehaviorDelay();
			break;
		case Firing_105_L_Start:
			m_eState = Firing_105_L;
			break;
		case Firing_105_L:
			m_eState = Firing_105_L_End;
			break;
		case Firing_105_L_End:
			m_eState = Idle_Battle;
			SetBehaviorDelay();
			break;
		case Firing_105_R_Start:
			m_eState = Firing_105_R;
			break;
		case Firing_105_R:
			m_eState = Firing_105_R_End;
			break;
		case Firing_105_R_End:
			m_eState = Idle_Battle;
			SetBehaviorDelay();
			break;
		case Firing_101:
			m_eState = Idle_Battle;
			m_fStunDelay = 40.f;
			SetBehaviorDelay();
			break;
		}
	}
}

void CDarkKnight::WolfLookPlayer()
{
	_vector		vDirection = m_pTargetTransform->GetState(CTransform::STATE_POSITION) - m_pTransform->GetState(CTransform::STATE_POSITION);

	_vector vUp = m_pTransform->GetState(CTransform::STATE_UP);			//	y축 // 외적으로 방향백터를 구하기위해서 그리고 좌우로만 바뀌지 y축은 안바뀌니까
	_vector	vRight = XMVector3Cross(vUp, vDirection);		//

	vRight = XMVector3Normalize(vRight) * m_pTransform->GetScale(CTransform::STATE_RIGHT);	//위에서 외적한 right는 스케일이 깨져있어서 원래 사용하던 right를 대입해주자 
	_vector		vLook = XMVector3Cross(vRight, vUp);			// 위에서 바꿔준 항등행렬과 y축을 외적하기 
	vLook = XMVector3Normalize(vLook) * m_pTransform->GetScale(CTransform::STATE_LOOK);

	m_pTransform->SetState(CTransform::STATE_RIGHT, vRight);
	m_pTransform->SetState(CTransform::STATE_LOOK, vLook);
}
void CDarkKnight::SetAtt()
{
	WolfAtt = true;
	WolfIdle = false;
}

void CDarkKnight::SetIdle()
{
	WolfAtt = false;
	WolfIdle = true;
}


