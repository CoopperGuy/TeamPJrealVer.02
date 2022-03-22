#include "stdafx.h"
#include "Client_Struct.h"
#include "..\Public\EffectSmoke.h"
#include "EmptyEffect.h"
#include "Obb.h"
USING(Client)

CEffectSmoke::CEffectSmoke()
{
}

CEffectSmoke * CEffectSmoke::Create(void * pArg, _vector pos)
{
	CEffectSmoke*		pInstance = new CEffectSmoke();

	if (FAILED(pInstance->Initialize(pArg, pos)))
	{
		MSG_BOX("Failed to Create CEffectSmoke");
		SafeRelease(pInstance);
		return nullptr;
	}

	return pInstance;
}


HRESULT CEffectSmoke::Initialize(void* pArg, _vector pos)
{
	if (pArg != nullptr) {
		m_pGameObject = (CGameObject*)pArg;
		if (m_pGameObject == nullptr)
			return E_FAIL;

		m_pTransform = static_cast<CTransform*>(m_pGameObject->GetComponent("Com_Transform"));

		pos = XMVectorSetY(pos, XMVectorGetY(pos) + 0.0f);


		m_pTransform->SetState(CTransform::STATE_POSITION, pos);

	}
	return S_OK;
}

void CEffectSmoke::Update(_double deltaTime)
{
	if (m_bDead)
		return;

	if (!m_pGameObject)
		return;

	deaddt += deltaTime;
	_matrix viewInverse = XMMatrixInverse(nullptr, CEngine::GetInstance()->GetTransform(CPipeline::D3DTS_VIEW));
	_float4x4 newWorld;
	_float4x4 world = m_pTransform->GetMatrix();
	_vector scale, rotation, position;
	XMMatrixDecompose(&scale, &rotation, &position, m_pTransform->GetWorldMatrix());
	XMStoreFloat4x4(&newWorld, viewInverse);
	memcpy(newWorld.m[3], world.m[3], sizeof(_float3));
	m_pTransform->SetMatrix(XMMatrixScalingFromVector(scale) * XMLoadFloat4x4(&newWorld));
}


void CEffectSmoke::LateUpdate(_double deltaTime)
{

	if (deaddt >= 0.3)
	{
		this->SetDead();
		m_pGameObject->SetDead();
	}
	if (static_cast<CEmptyEffect*>(m_pGameObject)->GetSpriteEnd())
	{
		this->SetDead();
		m_pGameObject->SetDead();
	}
}

void CEffectSmoke::Render()
{
}


void CEffectSmoke::Free()
{
	__super::Free();
}

