#include "stdafx.h"
#include "..\Public\MeteoFireBall.h"
#include "EffectMeteoFireBall.h"
#include "BasicCollider.h"
#include "Transform.h"

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


	_int  startrand = rand() % 2;
	if( 0 ==  startrand)
		m_pTransform->SetState(CTransform::STATE_POSITION, _vector{ float(rand() % 8) ,2.f , XMVectorGetZ(pos) - float(rand() % 5)  });
	else
		m_pTransform->SetState(CTransform::STATE_POSITION, _vector{ float(rand() % 8) * -1 ,2.f , XMVectorGetZ(pos) + float(rand() % 5) });




	m_pOBB = static_cast<CBasicCollider*>(m_pEAFireBall->GetComponent("Com_OBB"));

	m_pStat = static_cast<CStat*>(m_pEAFireBall->GetComponent("Com_Stat"));
	m_pStat->SetSTATE(CStat::STATES_ATK);


	mypos = m_pTransform->GetState(CTransform::STATE_POSITION);
	auto EffectFireBall = CEngine::GetInstance()->AddGameObjectToPrefab(CEngine::GetInstance()->GetCurSceneNumber(), "Prototype_Effect_FireBall", "E_FireBall");
	CEngine::GetInstance()->AddScriptObject(m_pEffFireball = CEffectMeteoFireBall::Create(EffectFireBall, &mypos), CEngine::GetInstance()->GetCurSceneNumber());

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
	if (0 <= XMVectorGetY(m_pTransform->GetState(CTransform::STATE_POSITION))) {
		_vector		vPosition = m_pTransform->GetState(CTransform::STATE_POSITION);

		/*)vector		vDirection = m_pTargetTransform->GetState(CTransform::STATE_POSITION) - playerpos;*/
		
		
		posy -= deltaTime * (rand()%4+1);

		mypos =XMVectorSetY(mypos, posy);

		m_pTransform->SetState(CTransform::STATE_POSITION, mypos);


		makedt += deltaTime;
		if (makedt >= 0.15)
		{
			_vector mypos = m_pTransform->GetState(CTransform::STATE_POSITION);
			auto EffectFireBall = CEngine::GetInstance()->AddGameObjectToPrefab(CEngine::GetInstance()->GetCurSceneNumber(), "Prototype_Effect_Meteo", "E_Meteo");
			CEngine::GetInstance()->AddScriptObject(m_pEffFireball = CEffectMeteoFireBall::Create(EffectFireBall, &mypos), CEngine::GetInstance()->GetCurSceneNumber());
			makedt = 0;
		}
	}
	else
	{
		if (m_pEffFireball)
			m_pEffFireball->SetDead();
		m_bDead = true;
	}


}

void CMeteoFireBall::LateUpdate(_double deltaTime)
{
	if (m_bDead)
	{

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