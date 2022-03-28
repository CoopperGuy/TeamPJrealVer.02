#include "stdafx.h"
#include "Client_Struct.h"
#include "..\Public\EffectFireBall.h"
#include "EmptyEffect.h"
#include "Obb.h"
USING(Client)

CEffectFireBall::CEffectFireBall()
{
}

CEffectFireBall * CEffectFireBall::Create(void * pArg, _vector pos)
{
	CEffectFireBall*		pInstance = new CEffectFireBall();

	if (FAILED(pInstance->Initialize(pArg, pos)))
	{
		MSG_BOX("Failed to Create CEffectFireBall");
		SafeRelease(pInstance);
		return nullptr;
	}

	return pInstance;
}


HRESULT CEffectFireBall::Initialize(void* pArg, _vector pos)
{
	if (pArg != nullptr) {

		m_pGameObject = (CGameObject*)pArg;
		if (m_pGameObject == nullptr)
			return E_FAIL;

		m_pTransform = static_cast<CTransform*>(m_pGameObject->GetComponent("Com_Transform"));
		_vector pPos = m_pTransform->GetState(CTransform::STATE_POSITION);

		m_pTransform->SetState(CTransform::STATE_POSITION, pos);

	}
	return S_OK;
}

void CEffectFireBall::Update(_double deltaTime)
{
	if (m_bDead)
		return;

	if (!m_pGameObject)
		return;


	_matrix viewInverse = XMMatrixInverse(nullptr, CEngine::GetInstance()->GetTransform(CPipeline::D3DTS_VIEW));
	_float4x4 newWorld;
	_float4x4 world = m_pTransform->GetMatrix();
	_vector scale, rotation, position;
	XMMatrixDecompose(&scale, &rotation, &position, m_pTransform->GetWorldMatrix());
	XMStoreFloat4x4(&newWorld, viewInverse);
	memcpy(newWorld.m[3], world.m[3], sizeof(_float3));
	m_pTransform->SetMatrix(XMMatrixScalingFromVector(scale) * XMLoadFloat4x4(&newWorld));

	//if (static_cast<CEmptyEffect*>(m_pGameObject)->GetSpriteEnd())
		//m_bDead = true;
}


void CEffectFireBall::LateUpdate(_double deltaTime)
{
	if (static_cast<CEmptyEffect*>(m_pGameObject)->GetSpriteEnd())
	{
		this->SetDead();
		m_pGameObject->SetDead();
	}
}

void CEffectFireBall::Render()
{
}


void CEffectFireBall::Free()
{
	__super::Free();
}

void CEffectFireBall::SetPos(_matrix _pmatrix)
{
	if (m_pTransform != nullptr)
	{
		m_pTransform->SetMatrix(_pmatrix);
	}
}

void CEffectFireBall::Set_Pos(_vector pPos)
{
}
