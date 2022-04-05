#include "stdafx.h"
#include "..\Public\EAFireBall.h"
#include "EffectFireBall.h"
#include "BasicCollider.h"
#include "Transform.h"
#include "EffectEAFire.h"
USING(Client)

CEAFireBall::CEAFireBall()
{
}

HRESULT CEAFireBall::Initailze(CGameObject * pArg, _matrix pos)
{
	m_pEAFireBall = (CGameObject*)pArg;
	if (m_pEAFireBall == nullptr)
		return E_FAIL;

	m_pTransform = static_cast<CTransform*>(m_pEAFireBall->GetComponent("Com_Transform"));
	m_pTransform->SetMatrix(pos);


	m_pOBB = static_cast<CBasicCollider*>(m_pEAFireBall->GetComponent("Com_OBB"));

	m_pStat = static_cast<CStat*>(m_pEAFireBall->GetComponent("Com_Stat"));
	m_pOBB->p_States = (CBasicCollider::STATES_ATK);


	_vector mypos = m_pTransform->GetState(CTransform::STATE_POSITION);
	//auto EffectFireBall = CEngine::GetInstance()->AddGameObjectToPrefab(CEngine::GetInstance()->GetCurSceneNumber(), "Prototype_Effect_Fire", "E_EAFire");
	//CEngine::GetInstance()->AddScriptObject(CEffectFireBall::Create(EffectFireBall, mypos), CEngine::GetInstance()->GetCurSceneNumber());

	CGameObject* pPlayer = CEngine::GetInstance()->FindGameObjectWithName(SCENE_STATIC, "Player");
	m_pTargetTransform = dynamic_cast<CTransform*>(pPlayer->GetComponent("Com_Transform"));
	m_Targetpos = m_pTargetTransform->GetState(CTransform::STATE_POSITION);

	if (XMVectorGetY(m_Targetpos) >= 0.f)
		m_Targetpos = XMVectorSetY(m_Targetpos, 0.f);

	LookAt(m_pTargetTransform->GetState(CTransform::STATE_POSITION));


	auto EffectFireBall = CEngine::GetInstance()->AddGameObjectToPrefab(CEngine::GetInstance()->GetCurSceneNumber(), "Prototype_Effect_FireBall", "E_EAFireBall");
	CEngine::GetInstance()->AddScriptObject(m_pEffFire = CEffectEAFire::Create(EffectFireBall, mypos), CEngine::GetInstance()->GetCurSceneNumber());


	CEngine::GetInstance()->PlaySoundW("WaterEAAtt6.mp3", CHANNELID::ENEMY12);

	if (m_pEffFire == nullptr)
		return E_FAIL;

	return S_OK;
}

void CEAFireBall::Update(_double deltaTime)
{
	if (m_bDead)
		true;

	/*if (!m_pOBB->Get_isHit()) {*/
	if (0.1f <= XMVectorGetY(m_pTransform->GetState(CTransform::STATE_POSITION))) {
		_vector		vPosition = m_pTransform->GetState(CTransform::STATE_POSITION);

		_vector		vDirection = m_Targetpos - vPosition;

		vPosition += XMVector3Normalize(vDirection) * 3.f * (_float)deltaTime;

		m_pTransform->SetState(CTransform::STATE_POSITION, vPosition);

		_matrix myPos = m_pTransform->GetWorldMatrix();
		myPos = Remove_ScaleRotation(m_pTransform->GetWorldMatrix());


		test += deltaTime;


		if (m_pEffFire)
			m_pEffFire->Set_Pos(vPosition);

		//makedt += deltaTime;
		//if (makedt >= 0.15)
		//{
		//	//auto EffectFireBall = CEngine::GetInstance()->AddGameObjectToPrefab(CEngine::GetInstance()->GetCurSceneNumber(), "Prototype_Effect_Fire", "E_EAFire");
		//	//CEngine::GetInstance()->AddScriptObject(CEffectFireBall::Create(EffectFireBall, vPosition), CEngine::GetInstance()->GetCurSceneNumber());
		//	auto EffectFireBall = CEngine::GetInstance()->AddGameObjectToPrefab(CEngine::GetInstance()->GetCurSceneNumber(), "Prototype_Effect_FireBall", "E_EAFireBall");
		//	CEngine::GetInstance()->AddScriptObject(CEffectEAFire::Create(EffectFireBall, vPosition), CEngine::GetInstance()->GetCurSceneNumber());
		//	makedt = 0;
		//}
	}
	else
	{
		m_bDead = true;
	}
	//if (m_pOBB->Get_isHit())
	//	m_bDead = true;

	//if (XMVectorGetY(m_Targetpos) >= XMVectorGetY(m_pTransform->GetState(CTransform::STATE_POSITION)))
	//	m_bDead = true;


}

void CEAFireBall::LateUpdate(_double deltaTime)
{
	if (m_pOBB->Get_isHit())
	{
		CEngine::GetInstance()->PlaySoundW("FlogasMeteoDrop.mp3", CHANNELID::ENEMY16);

		if (m_pEffFire)
			m_pEffFire->SetDead();
		this->SetDead();
		m_pEAFireBall->SetDead();

	}

	//if(XMVectorGetY(m_Targetpos) >= XMVectorGetY(m_pTransform->GetState(CTransform::STATE_POSITION)))
	//{

	//	CEngine::GetInstance()->PlaySoundW("FlogasMeteoDrop.mp3", CHANNELID::MAPOBJ);

	//	if (m_pEffFire)
	//		m_pEffFire->SetDead();
	//	this->SetDead();
	//	m_pEAFireBall->SetDead();
	//}

	if(m_bDead)
	{
		CEngine::GetInstance()->PlaySoundW("FlogasMeteoDrop.mp3", CHANNELID::ENEMY17);


		if (m_pEffFire)
			m_pEffFire->SetDead();
		this->SetDead();
		m_pEAFireBall->SetDead();
	}
}


CEAFireBall * CEAFireBall::Create(CGameObject * pTarget, _matrix pos)
{
	CEAFireBall*	obj = new CEAFireBall();
	if (FAILED(obj->Initailze(pTarget, pos))) {
		SAFE_RELEASE(obj);
		return nullptr;
	}
	return obj;
}

void CEAFireBall::Free()
{
}

void CEAFireBall::LookAt(_fvector vTargetPos)
{
	_vector		vDirection = vTargetPos - m_pTransform->GetState(CTransform::STATE_POSITION);

	_vector vUp = m_pTransform->GetState(CTransform::STATE_UP);			//	y축 // 외적으로 방향백터를 구하기위해서 그리고 좌우로만 바뀌지 y축은 안바뀌니까
	_vector	vRight = XMVector3Cross(vUp, vDirection);		//

	vRight = XMVector3Normalize(vRight) * m_pTransform->GetScale(CTransform::STATE_RIGHT);	//위에서 외적한 right는 스케일이 깨져있어서 원래 사용하던 right를 대입해주자 
	_vector		vLook = XMVector3Cross(vRight, vUp);			// 위에서 바꿔준 항등행렬과 y축을 외적하기 
	vLook = XMVector3Normalize(vLook) * m_pTransform->GetScale(CTransform::STATE_LOOK);

	m_pTransform->SetState(CTransform::STATE_RIGHT, vRight);
	m_pTransform->SetState(CTransform::STATE_LOOK, vLook);
}

_fmatrix CEAFireBall::Remove_ScaleRotation(_fmatrix TransformMatrix)
{
	_matrix			NonRotateMatrix = XMMatrixIdentity();

	NonRotateMatrix.r[3] = TransformMatrix.r[3];

	return NonRotateMatrix;
}