#include "stdafx.h"
#include "Client_Struct.h"
#include "..\Public\EffectMeteoFireBall.h"
#include "EmptyEffect.h"
#include "EffectMeteoExpolRing.h"
#include "EffectMeteoFire.h"
#include "EffectSmoke.h"
#include "EffectMeteoExpolFire.h"
USING(Client)

CEffectMeteoFireBall::CEffectMeteoFireBall()
{
}

CEffectMeteoFireBall * CEffectMeteoFireBall::Create(void * pArg, _vector* pos)
{
	CEffectMeteoFireBall*		pInstance = new CEffectMeteoFireBall();

	if (FAILED(pInstance->Initialize(pArg, pos)))
	{
		MSG_BOX("Failed to Create CEffectMeteoFireBall");
		SafeRelease(pInstance);
		return nullptr;
	}

	return pInstance;
}


HRESULT CEffectMeteoFireBall::Initialize(void* pArg, _vector* pos)
{
	if (pArg != nullptr) {

		m_pGameObject = (CGameObject*)pArg;
		if (m_pGameObject == nullptr)
			return E_FAIL;

		m_pTransform = static_cast<CTransform*>(m_pGameObject->GetComponent("Com_Transform"));

		m_pTransform->SetState(CTransform::STATE_POSITION, *pos);

		startposy = XMVectorGetY(m_pTransform->GetState(CTransform::STATE_POSITION));

	}
	return S_OK;
}

void CEffectMeteoFireBall::Update(_double deltaTime)
{
	if (m_bDead)
		return;

	if (!m_pGameObject)
		return;

	//static_cast<CEmptyEffect*>(m_pGameObject)->SetSpritMaxNum(64.f);


	posy = XMVectorGetY(m_pTransform->GetState(CTransform::STATE_POSITION));
	pos = m_pTransform->GetState(CTransform::STATE_POSITION);
	deaddt += deltaTime;
	
	//if (deaddt >=0.8f)
	//{
	//	deaddt = 0;
	//	m_bDead = true;
	//}

	m_dMakeFB += deltaTime;
	_vector vPosition = m_pTransform->GetState(CTransform::STATE_POSITION);
	if (m_dMakeFB >= 0.15)
	{
		auto EffectSmoke = CEngine::GetInstance()->AddGameObjectToPrefab(CEngine::GetInstance()->GetCurSceneNumber(), "Prototype_Effect_Smoke", "E_Smoke");
		CEngine::GetInstance()->AddScriptObject(CEffectSmoke::Create(EffectSmoke, vPosition), CEngine::GetInstance()->GetCurSceneNumber());
		m_dMakeFB = 0;
	}

	_matrix viewInverse = XMMatrixInverse(nullptr, CEngine::GetInstance()->GetTransform(CPipeline::D3DTS_VIEW));
	_float4x4 newWorld;
	_float4x4 world = m_pTransform->GetMatrix();
	_vector scale, rotation, position;
	XMMatrixDecompose(&scale, &rotation, &position, m_pTransform->GetWorldMatrix());
	XMStoreFloat4x4(&newWorld, viewInverse);
	memcpy(newWorld.m[3], world.m[3], sizeof(_float3));
	m_pTransform->SetMatrix(XMMatrixScalingFromVector(scale) * XMLoadFloat4x4(&newWorld));
}


void CEffectMeteoFireBall::LateUpdate(_double deltaTime)
{
	if (m_bDead)
	{
		this->SetDead();
		m_pGameObject->SetDead();
	}

	if (static_cast<CEmptyEffect*>(m_pGameObject)->GetSpriteEnd())
	{
		this->SetDead();
		m_pGameObject->SetDead();
	}


	if (0.1 >= XMVectorGetY(m_pTransform->GetState(CTransform::STATE_POSITION)))
	{
		this->SetDead();
		m_pGameObject->SetDead();
	}
}

void CEffectMeteoFireBall::Render()
{
}


void CEffectMeteoFireBall::Free()
{
	__super::Free();
}

void CEffectMeteoFireBall::SetPos(_matrix _pmatrix)
{
	if (m_pTransform != nullptr)
	{
		m_pTransform->SetMatrix(_pmatrix);
	}
}

void CEffectMeteoFireBall::Set_Pos(_vector pPos)
{
	_vector postemp = {};
	memcpy(&postemp, &pos, sizeof(XMVECTOR));

	m_pTransform->SetState(CTransform::STATE_POSITION, postemp);
}
