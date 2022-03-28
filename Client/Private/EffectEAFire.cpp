#include "stdafx.h"
#include "Client_Struct.h"
#include "..\Public\EffectEAFire.h"
#include "EmptyEffect.h"
#include "EffectFireBall.h"

USING(Client)

CEffectEAFire::CEffectEAFire()
{
}

CEffectEAFire * CEffectEAFire::Create(void * pArg,_vector pos)
{
	CEffectEAFire*		pInstance = new CEffectEAFire();

	if (FAILED(pInstance->Initialize(pArg, pos)))
	{
		MSG_BOX("Failed to Create CEffectEAFire");
		SafeRelease(pInstance);
		return nullptr;
	}

	return pInstance;
}


HRESULT CEffectEAFire::Initialize(void* pArg,_vector pos)
{
	if (pArg != nullptr) {

		m_pGameObject = (CGameObject*)pArg;
		if (m_pGameObject == nullptr)
			return E_FAIL;
		m_pTransform = static_cast<CTransform*>(m_pGameObject->GetComponent("Com_Transform"));
		m_pTransform->SetState(CTransform::STATE_POSITION, pos);
	}
	return S_OK;
}

void CEffectEAFire::Update(_double deltaTime)
{
	if (m_bDead)
		return;

	if (!m_pGameObject)
		return;

	m_dMakeFB += deltaTime;
	dead += deltaTime;
	vPosition = m_pTransform->GetState(CTransform::STATE_POSITION);
	if (m_dMakeFB >= 0.15)
	{
		auto EffectFireBall = CEngine::GetInstance()->AddGameObjectToPrefab(CEngine::GetInstance()->GetCurSceneNumber(), "Prototype_Effect_Fire", "E_EAFire");
		CEngine::GetInstance()->AddScriptObject(CEffectFireBall::Create(EffectFireBall, vPosition), CEngine::GetInstance()->GetCurSceneNumber());
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


void CEffectEAFire::LateUpdate(_double deltaTime)
{
	if (0.1f >= XMVectorGetY(m_pTransform->GetState(CTransform::STATE_POSITION)))
	{
		this->SetDead();
		m_pGameObject->SetDead();
	}

	if (m_bDead)
	{
		this->SetDead();
		m_pGameObject->SetDead();
	}
}

void CEffectEAFire::Render()
{
}

void CEffectEAFire::Free()
{
	__super::Free();
}

void CEffectEAFire::Set_Pos(_vector pos)
{
	_vector postemp = {};
	memcpy(&postemp, &pos, sizeof(XMVECTOR));

	m_pTransform->SetState(CTransform::STATE_POSITION, postemp);
}
