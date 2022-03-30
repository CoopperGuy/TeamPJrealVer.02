#include "stdafx.h"
#include "Client_Struct.h"
#include "..\Public\EffectRockDust.h"

USING(Client)

CEffectRockDust::CEffectRockDust()
{
}

CEffectRockDust * CEffectRockDust::Create(void * pArg)
{
	CEffectRockDust*		pInstance = new CEffectRockDust();

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Create CEffectRockDust");
		SafeRelease(pInstance);
		return nullptr;
	}

	return pInstance;
}


HRESULT CEffectRockDust::Initialize(void* pArg)
{
	if (pArg != nullptr) {

		m_pGameObject = (CGameObject*)pArg;
		if (m_pGameObject == nullptr)
			return E_FAIL;

		m_pTransform = static_cast<CTransform*>(m_pGameObject->GetComponent("Com_Transform"));
		m_pComponent = m_pGameObject->GetComponent("Com_RectInstance");


		RandomNum = rand() % 3;
	}
	return S_OK;
}

void CEffectRockDust::Update(_double deltaTime)
{
	if (m_bDead)
		return;

	if (!m_pGameObject)
		return;

	m_dDeadTime += deltaTime;

	_matrix viewInverse = XMMatrixInverse(nullptr, CEngine::GetInstance()->GetTransform(CPipeline::D3DTS_VIEW));
	_float4x4 newWorld;
	_float4x4 world = m_pTransform->GetMatrix();
	_vector scale, rotation, position;
	XMMatrixDecompose(&scale, &rotation, &position, m_pTransform->GetWorldMatrix());
	XMStoreFloat4x4(&newWorld, viewInverse);
	memcpy(newWorld.m[3], world.m[3], sizeof(_float3));
	m_pTransform->SetMatrix(XMMatrixScalingFromVector(scale) * XMLoadFloat4x4(&newWorld));


}

void CEffectRockDust::LateUpdate(_double deltaTime)
{
	//if (static_cast<CEmptyEffect*>(m_pGameObject)->GetEffectDuration()<= m_dDeadTime)
	if (static_cast<CVIBuffer_RectInstance*>(m_pComponent)->Get_LifeTime() <= m_dDeadTime)
	{
		this->SetDead();
		m_pGameObject->SetDead();
	}
}

void CEffectRockDust::Render()
{
}


void CEffectRockDust::Free()
{
	__super::Free();
}
