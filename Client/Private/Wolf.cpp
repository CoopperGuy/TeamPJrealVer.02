#include "stdafx.h"
#include "..\Public\Wolf.h"
#include "StateMachine.h"
#include "BasicCollider.h"
#include "Transform.h"
#include "MonHpVIBuffer.h"
#include "EffectBlood.h"
#include "EffectBloodDecal.h"
CWolf::CWolf(CGameObject* pObj)
	: CEnemy(pObj)
{
}


CWolf * CWolf::Create(CGameObject * pObj, _float3 position)
{
	CWolf*		pInstance = new CWolf(pObj);

	if (FAILED(pInstance->Initialize(position)))
	{
		MSG_BOX("Failed to Create CWolf");
		SafeRelease(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CWolf::Free()
{
	__super::Free();
	//if (m_pHpBar)
	//	m_pHpBar->SetUpDead();
}

HRESULT CWolf::Initialize(_float3 position)
{
	m_pGameObject = CEngine::GetInstance()->AddGameObjectToPrefab(CEngine::GetInstance()->GetCurSceneNumber(), "Prototype_GameObecjt_Wolf", "O_Wolf");
	m_pModel = static_cast<CModel*>(m_pGameObject->GetComponent("Com_Model"));

	CEngine::GetInstance()->AddScriptObject(this, CEngine::GetInstance()->GetCurSceneNumber());
	m_pTransform = static_cast<CTransform*>(m_pGameObject->GetComponent("Com_Transform"));
	m_pOBBCom = static_cast<CBasicCollider*>(m_pGameObject->GetComponent("Com_OBB"));
	m_pWeaponOBB = static_cast<CBasicCollider*>(m_pGameObject->GetComponent("Com_OBB1"));
	m_pCollider = static_cast<CCollider*>(m_pGameObject->GetComponent("Com_Collider"));
	m_pController = m_pCollider->GetController();
	m_pStat = static_cast<CStat*>(m_pGameObject->GetComponent("Com_Stat"));
	m_eCurSTATES = CBasicCollider::STATES_IDEL;
	m_pWeaponOBB->p_States = m_eCurSTATES;
	m_pTransform->SetState(CTransform::STATE_POSITION, _vector{ position.x,3.f,position.z });
	m_pCollider->SetPosition(_float3(position.x, position.y, position.z));

	SetUpAnimation();
	m_pCurState = IDLE0;
	SetIdle();


	m_pHpBar = CMonHpVIBuffer::Create(m_pGameObject);

	//CEngine::GetInstance()->AddScriptObject(this, CEngine::GetInstance()->GetCurSceneNumber());

	CGameObject* pPlayer = CEngine::GetInstance()->FindGameObjectWithName(SCENE_STATIC, "Player");
	m_pTargetTransform = dynamic_cast<CTransform*>(pPlayer->GetComponent("Com_Transform"));

	return S_OK;
}

void CWolf::Update(_double dDeltaTime)
{
	if (m_bDead)
		return;


	if (!m_pGameObject)
		return;

	if (!m_pGameObject->IsActive())
		return;


	if (m_pStat->GetStatInfo().hp <= 0) {
		m_pWolfState = DIE;
		WolfAtt = false;
		WolfIdle = false;
	}
	__super::Update(dDeltaTime);

	WolfStateUpdate(dDeltaTime);

	mypos = m_pTransform->GetState(CTransform::STATE_POSITION);

	if (WolfAtt)
	{
		if (m_pWolfState == IDLE0)
			m_pWolfState = THREATEN;
		WolfAttflow(dDeltaTime);
	}

	if (m_pOBBCom->Get_isHit()) {
		m_pWolfState = DAMAGE;
	}

	//SetAttack(dDeltaTime);
}
void CWolf::LateUpdate(_double dDeltaTime)
{
	if (m_pStat->GetStatInfo().hp <= 0) {
		m_pWolfState = DIE;
		//m_bDead = true;
	}

	if (!m_bDead) {
		if (m_bMove)
		{
			PxVec3 vDir = PxVec3(0.f, 0.f, 0.f);
			PxControllerFilters filters;
			_vector vLook{}, vTargetLook{};
			_vector vDis;

			//if (m_bLook) {
			_vector vTargetPos, vPos;
			vPos = m_pTransform->GetState(CTransform::STATE_POSITION);
			vDis = PlayerPos - vPos;
			vDis = XMVectorSetY(vDis, 0.f);
			m_pTransform->SetLook(vDis);

			_float tempX = XMVectorGetX(vDis);
			_float tempZ = XMVectorGetZ(vDis);
			if (tempX < 0)
				tempX = tempX*-1.f;
			if (tempZ < 0)
				tempZ = tempZ*-1.f;


			memcpy(&vDir, &XMVector3Normalize(vDis), sizeof(_float3));
			WolfLookPlayer();
			m_pController->move(vDir * 1.8f * (_float)dDeltaTime, 0.f, (_float)dDeltaTime, nullptr);

		}
		WolfSetAni(dDeltaTime);
		//WolfStateUpdate(dDeltaTime);
	}

	if (m_pWolfState == DIE)
	{

		CEngine::GetInstance()->PlaySoundW("WolfDie.mp3", CHANNELID::ENEMY13);

		if (m_pModel->Get_isFinished()) {

			this->SetDead();
			m_pGameObject->SetDead();
			m_pCollider->ReleaseController();
			if (m_pHpBar) {
				m_pHpBar->SetUpDead();
			}
		}
	}

}

void CWolf::Render()
{
}

void CWolf::SetUpAnimation()
{
	m_pModel->SetAnimationLoop((_uint)WOLFSTATE::EAT, true, false);
	m_pModel->SetAnimationLoop((_uint)WOLFSTATE::IDLE0, true, false);
	m_pModel->SetAnimationLoop((_uint)WOLFSTATE::IDLE1, true, false);
	m_pModel->SetAnimationLoop((_uint)WOLFSTATE::WALK, true, false);
	m_pModel->SetAnimationLoop((_uint)WOLFSTATE::RUN, true, false);
	m_pModel->SetAnimationLoop((_uint)WOLFSTATE::THREATEN, false, false); // 위협
	m_pModel->SetAnimationLoop((_uint)WOLFSTATE::ZTTACK, false, true);
	m_pModel->SetAnimationLoop((_uint)WOLFSTATE::STRAIGHTATACK, false, true);
	m_pModel->SetAnimationLoop((_uint)WOLFSTATE::DAMAGE, false, false);
	m_pModel->SetAnimationLoop((_uint)WOLFSTATE::DIE, false, false);
	m_pModel->SetAnimationLoop((_uint)WOLFSTATE::DEADBODY, false, false);
}

void CWolf::RotateBody(_double deltaTime)
{
}

void CWolf::WolfAttflow(_double dDeltaTime)
{

	if (m_pStat->GetStatInfo().hp <= 0)
		return;

	//cout << m_pStat->GetStatInfo().hp << endl;

	_vector PlayerTF = m_pTargetTransform->GetState(CTransform::STATE_POSITION);
	_vector TargetDistance = PlayerTF - m_pTransform->GetState(CTransform::STATE_POSITION);

	_uint keyFrame = m_pModel->GetCurrentKeyFrame();

	if (XMVectorGetZ(TargetDistance) <= 0 || XMVectorGetX(TargetDistance) <= 0)
		TargetDistance = TargetDistance*-1;

	_float dis = 1.f;
	switch (m_pWolfState)
	{
	case Client::CWolf::THREATEN2: {
		m_bMove = false;
		m_dAttDelta += dDeltaTime;
		if (m_dAttDelta >=1.2) {
			m_pWolfState = THREATEN;
		}
		break;
	}
	case Client::CWolf::THREATEN: {
		m_dAttDelta = 0;

		if (m_pStat->GetStatInfo().hp <= 0) {
			m_pWolfState = DIE;
			return;
		}
		//CEngine::GetInstance()->PlaySoundW("WolfTHREATEN.mp3", CHANNELID::ENEMY10);

		WolfLookPlayer();
		m_bMove = false;

		if (m_pModel->Get_isFinished()) {
			if (XMVectorGetX(TargetDistance) > dis || XMVectorGetZ(TargetDistance) > dis)
				m_pWolfState = RUN;
			else
				m_pWolfState = STRAIGHTATACK;

			m_dAttDelta = 0.f;
		}
		break;
	}
	case Client::CWolf::STRAIGHTATACK:


		if (keyFrame == 0)
			PlayerPos = m_pTargetTransform->GetState(CTransform::STATE_POSITION);
		
		if (keyFrame >= 41 && keyFrame <= 43)
			m_pWeaponOBB->p_States = CBasicCollider::STATES_ATK;
		else
			m_pWeaponOBB->p_States = CBasicCollider::STATES_IDEL;


		if (m_pModel->Get_isFinished()) {
			WolfLookPlayer();
			m_bMove = false; 
			m_pWolfState = THREATEN2;
		}
		else {
			m_bMove = true;
			if (keyFrame >= 42)
			{
				CEngine::GetInstance()->PlaySoundW("WolfAtt.mp3", CHANNELID::ENEMY16);
			}

		}
		break;
	case Client::CWolf::DAMAGE: {
		if (m_iBlood < 1) {
			m_iBlood += 1;
			m_pWeaponOBB->p_States = CBasicCollider::STATES_IDEL;
			CEngine::GetInstance()->PlaySoundW("WolfHit.mp3", CHANNELID::ENEMY17);
			//CEngine::GetInstance()->SetVolume(0.2f, CHANNELID::ENEMY10);

			_matrix Translation;
			_int random = rand() % 2;
			random += 1;
			Translation = XMMatrixTranslation(XMVectorGetX(m_pTransform->GetState(CTransform::STATE_POSITION)), XMVectorGetY(m_pTransform->GetState(CTransform::STATE_POSITION)) + ((float)random*0.1f), XMVectorGetZ(m_pTransform->GetState(CTransform::STATE_POSITION)));
			Translation = m_pTransform->Remove_Scale(Translation);

			CGameObject* EffectBlood = CEngine::GetInstance()->AddGameObjectToPrefab(CEngine::GetInstance()->GetCurSceneNumber(), "Prototype_Effect_IIBlood", "E_IIBlood", &Translation);
			CEngine::GetInstance()->AddScriptObject(CEffectBlood::Create(EffectBlood), CEngine::GetInstance()->GetCurSceneNumber());
			CGameObject* EffectBloodDecal = CEngine::GetInstance()->AddGameObjectToPrefab(CEngine::GetInstance()->GetCurSceneNumber(), "Prototype_Effect_BloodDecal", "E_BloodDecal");
			CEngine::GetInstance()->AddScriptObject(CEffectBloodDecal::Create(EffectBloodDecal, m_pTransform->GetState(CTransform::STATE_POSITION)), CEngine::GetInstance()->GetCurSceneNumber());

		}
		m_bMove = false;
		if (m_pModel->Get_isFinished()) {
			m_pWolfState = THREATEN;
			SetAtt();
		}
	}
								break;
	case Client::CWolf::RUN:
	{
		PlayerPos = m_pTargetTransform->GetState(CTransform::STATE_POSITION);
		m_pWeaponOBB->p_States = CBasicCollider::STATES_IDEL;

		m_bMove = true;

		if (XMVectorGetX(TargetDistance) >= 1.f || XMVectorGetZ(TargetDistance) >= 1.f)
			m_bMove = true;
		else {
			m_pWolfState = THREATEN;
			m_bMove = false;
		}
		break;
	}
	}
}

_float CWolf::DistanceCalculate(_vector _me, _vector _target)
{
	_float fdistance = 0.f;


	return fdistance;
}

void CWolf::WolfStateUpdate(_double dDeltaTime)
{
	_vector PlayerTF = m_pTargetTransform->GetState(CTransform::STATE_POSITION);

	_vector TargetDistance = PlayerTF - m_pTransform->GetState(CTransform::STATE_POSITION);

	_float dis = 2.f;


	if (m_pWolfState != DAMAGE) {
		if (m_pStat->GetStatInfo().maxHp > m_pStat->GetStatInfo().hp)
		{
			SetAtt();
			m_iBlood = 0;
		}
		else {
			if (WolfIdle && m_pWolfState != DIE && m_pWolfState != DEADBODY) {
				SetIdle();
				m_pWolfState = IDLE0;
				m_pWeaponOBB->p_States = CBasicCollider::STATES_IDEL;
				m_bMove = false;
			}
		}
	}
}

_float CWolf::Gethp()
{
	if (m_pStat->GetStatInfo().hp <= 0)
		return -1.f;

	return m_pStat->GetStatInfo().hp;
}

void CWolf::WolfSetAni(_double dDeltaTime)
{
	if (m_pCurState != m_pWolfState)
	{
		switch (m_pWolfState)
		{
		case Client::CWolf::EAT:
			m_pModel->SetUp_AnimationIndex(EAT);
			SetIdle();
			break;
		case Client::CWolf::IDLE0:
			m_pModel->SetUp_AnimationIndex(IDLE0);
			SetIdle();
			break;
		case Client::CWolf::IDLE1:
			m_pModel->SetUp_AnimationIndex(IDLE1);
			SetIdle();
			break;
		case Client::CWolf::WALK:
			m_pModel->SetUp_AnimationIndex(WALK);
			SetIdle();
			break;
		case Client::CWolf::RUN:
			m_pModel->SetUp_AnimationIndex(RUN);
			SetIdle();
			break;
		case Client::CWolf::THREATEN:
			m_pModel->SetUp_AnimationIndex(THREATEN);
			SetAtt();
			break;
		case Client::CWolf::ZTTACK:
			m_pModel->SetUp_AnimationIndex(ZTTACK);
			SetAtt();
			break;
		case Client::CWolf::STRAIGHTATACK:
			m_pModel->SetUp_AnimationIndex(STRAIGHTATACK);
			SetAtt();
			break;
		case Client::CWolf::DAMAGE:
			m_pModel->SetUp_AnimationIndex(DAMAGE);
			break;
		case Client::CWolf::DIE:
			m_pModel->SetUp_AnimationIndex(DIE);
			break;
		case Client::CWolf::DEADBODY:
			m_pModel->SetUp_AnimationIndex(DEADBODY);
			break;
		case Client::CWolf::THREATEN2:
			m_pModel->SetUp_AnimationIndex(THREATEN);
			SetAtt();
			break;
		}
		m_pCurState = m_pWolfState;
	}

	m_pModel->Play_Animation(dDeltaTime);
	m_pModel->SetUp_AnimationIndex(m_pCurState);
	Gravity(dDeltaTime);
}

void CWolf::WolfLookPlayer()
{
	_vector		vDirection = m_pTargetTransform->GetState(CTransform::STATE_POSITION) - m_pTransform->GetState(CTransform::STATE_POSITION);

	_vector vUp = m_pTransform->GetState(CTransform::STATE_UP);			//	y축 // 외적으로 방향백터s를 구하기위해서 그리고 좌우로만 바뀌지 y축은 안바뀌니까
	_vector	vRight = XMVector3Cross(vUp, vDirection);		//

	vRight = XMVector3Normalize(vRight) * m_pTransform->GetScale(CTransform::STATE_RIGHT);	//위에서 외적한 right는 스케일이 깨져있어서 원래 사용하던 right를 대입해주자 
	_vector		vLook = XMVector3Cross(vRight, vUp);			// 위에서 바꿔준 항등행렬과 y축을 외적하기 
	vLook = XMVector3Normalize(vLook) * m_pTransform->GetScale(CTransform::STATE_LOOK);

	m_pTransform->SetState(CTransform::STATE_RIGHT, vRight);
	m_pTransform->SetState(CTransform::STATE_LOOK, vLook);

	m_pTransform->RotateAxis(_vector{ 0.f,1.f,0.f }, 90.f);
}
void CWolf::SetAtt()
{
	WolfAtt = true;
	WolfIdle = false;
}

void CWolf::SetIdle()
{
	WolfAtt = false;
	WolfIdle = true;
}

void CWolf::Set_HpBar(CMonHpVIBuffer * _hp)
{
	m_pHpBar = _hp;
}

void CWolf::Gravity(_double dDeltaTime)
{
	PxControllerFilters filters;
	if (m_pController)
		m_pController->move(PxVec3(0.0f, -1.f, 0.f), 0.01f, PxF32(1.f / dDeltaTime), filters);
}
_fmatrix CWolf::Remove_ScaleRotation(_fmatrix TransformMatrix)
{
	_matrix			NonRotateMatrix = XMMatrixIdentity();

	NonRotateMatrix.r[3] = TransformMatrix.r[3];

	return NonRotateMatrix;
}