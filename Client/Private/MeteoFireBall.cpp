#include "stdafx.h"
#include "..\Public\MeteoFireBall.h"
#include "EffectMeteoFireBall.h"
#include "BasicCollider.h"
#include "Transform.h"
#include "EffectMagic.h"
#include "EffectMeteoFire.h"
#include "EffectMeteoExpolRing.h"
#include "EffectSmoke.h"
#include "EffectMagicAf.h"
#include "EffectMeteoExpolFire.h"
#include "EventCheck.h"
USING(Client)

CMeteoFireBall::CMeteoFireBall()
{
}

HRESULT CMeteoFireBall::Initailze(CGameObject * pArg, _vector pos)
{
	m_pEAFireBall = (CGameObject*)pArg;
	if (m_pEAFireBall == nullptr)
		return E_FAIL;

	m_pTransform = static_cast<CTransform*>(m_pEAFireBall->GetComponent("Com_Transform"));

	m_arrivePos = pos;
	int random = rand() % 2;

	if (random == 0)
		pos = XMVectorSetX(pos, XMVectorGetX(pos) * -1);

	pos = XMVectorSetY(pos, 2.f + float(rand() % 3));

	m_pTransform->SetState(CTransform::STATE_POSITION, pos);


	m_pOBB = static_cast<CBasicCollider*>(m_pEAFireBall->GetComponent("Com_OBB"));

	m_pStat = static_cast<CStat*>(m_pEAFireBall->GetComponent("Com_Stat"));
	m_pStat->SetSTATE(CStat::STATES_ATK);


	mypos = m_pTransform->GetState(CTransform::STATE_POSITION);

	CGameObject* pPlayer = CEngine::GetInstance()->FindGameObjectWithName(SCENE_STATIC, "Player");
	m_pTargetTransform = dynamic_cast<CTransform*>(pPlayer->GetComponent("Com_Transform"));
	playerpos = m_pTargetTransform->GetState(CTransform::STATE_POSITION);

	posy = XMVectorGetY(mypos);

	SafeAddRef(m_pOBB);

	return S_OK;
}

void CMeteoFireBall::Update(_double deltaTime)
{
	if (m_bDead)
		true;

	/*if (!m_pOBB->Get_isHit()) {*/
	if (0.1 <= XMVectorGetY(m_pTransform->GetState(CTransform::STATE_POSITION))) {
		mypos = m_pTransform->GetState(CTransform::STATE_POSITION);

		mypos = m_pTransform->GetState(CTransform::STATE_POSITION);

		_vector		vDirection = m_arrivePos - mypos;

		mypos += XMVector3Normalize(vDirection) * _float(rand() % 8 + 3) * (_float)deltaTime;

		m_pTransform->SetState(CTransform::STATE_POSITION, mypos);
		
		//if (m_pMeteo)
		//	m_pMeteo->Set_Pos(mypos);

		makedt += deltaTime;
		if (makedt >= 0.2)
		{
			_vector mypos = m_pTransform->GetState(CTransform::STATE_POSITION);
			auto EffectFireBall = CEngine::GetInstance()->AddGameObjectToPrefab(CEngine::GetInstance()->GetCurSceneNumber(), "Prototype_Effect_Meteo", "E_Meteo");
			CEngine::GetInstance()->AddScriptObject(CEffectMeteoFireBall::Create(EffectFireBall, &mypos), CEngine::GetInstance()->GetCurSceneNumber());
			makedt = 0;
		}
	}
	else
		m_bDead = true;

	if (m_pOBB->Get_isHit())
		m_bDead = true;




}

void CMeteoFireBall::LateUpdate(_double deltaTime)
{
	if (m_bDead)
	{
		//if (makeDeadEff) {

			//CEventCheck::GetInstance()->ShakeCamera(CCamera_Fly::SHAKE::SHAKE_ING, 4.f, 0.08f);

			//auto EffectSmoke = CEngine::GetInstance()->AddGameObjectToPrefab(CEngine::GetInstance()->GetCurSceneNumber(), "Prototype_Effect_Smoke", "E_Smoke");
			//CEngine::GetInstance()->AddScriptObject(CEffectSmoke::Create(EffectSmoke, mypos), CEngine::GetInstance()->GetCurSceneNumber());

			//auto EffectRing = CEngine::GetInstance()->AddGameObjectToPrefab(CEngine::GetInstance()->GetCurSceneNumber(), "Prototype_Effect_MeteoEnd", "E_MeteoEnd");
			//CEngine::GetInstance()->AddScriptObject(CEffectMeteoExpolRing::Create(EffectRing, mypos), CEngine::GetInstance()->GetCurSceneNumber());

			//auto EffectAf = CEngine::GetInstance()->AddGameObjectToPrefab(CEngine::GetInstance()->GetCurSceneNumber(), "Prototype_Effect_MeteoDropAf", "E_MeteoDropAfter");
			//CEngine::GetInstance()->AddScriptObject(CEffectMagicAf::Create(EffectAf, mypos), CEngine::GetInstance()->GetCurSceneNumber());

			//auto EffectFireEnd = CEngine::GetInstance()->AddGameObjectToPrefab(CEngine::GetInstance()->GetCurSceneNumber(), "Prototype_Effect_Fire_explosion", "E_MeteoExplosion");
			//CEngine::GetInstance()->AddScriptObject(CEffectMeteoExpolFire::Create(EffectFireEnd, mypos), CEngine::GetInstance()->GetCurSceneNumber());
		
		//	makeDeadEff = false;
			
	//	}
		if (m_pMeteo)
			m_pMeteo->SetDead();

		this->SetDead();
		m_pEAFireBall->SetDead();
	}
}


CMeteoFireBall * CMeteoFireBall::Create(CGameObject * pTarget, _vector pos)
{
	CMeteoFireBall*	obj = new CMeteoFireBall();
	if (FAILED(obj->Initailze(pTarget, pos))) {
		SAFE_RELEASE(obj);
		return nullptr;
	}
	return obj;
}

void CMeteoFireBall::Free()
{
	SafeRelease(m_pOBB);
}

void CMeteoFireBall::LookAt(_fvector vTargetPos)
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

_fmatrix CMeteoFireBall::Remove_ScaleRotation(_fmatrix TransformMatrix)
{
	_matrix			NonRotateMatrix = XMMatrixIdentity();

	NonRotateMatrix.r[3] = TransformMatrix.r[3];

	return NonRotateMatrix;
}