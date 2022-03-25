#include "stdafx.h"
#include "Client_Struct.h"
#include "..\Public\EffectBlood.h"

USING(Client)

CEffectBlood::CEffectBlood()
{
}

CEffectBlood * CEffectBlood::Create(void * pArg, _vector pos)
{
	CEffectBlood*		pInstance = new CEffectBlood();

	if (FAILED(pInstance->Initialize(pArg, pos)))
	{
		MSG_BOX("Failed to Create CEffectBlood");
		SafeRelease(pInstance);
		return nullptr;
	}

	return pInstance;
}


HRESULT CEffectBlood::Initialize(void* pArg, _vector pos)
{
	if (pArg != nullptr) {

		m_pGameObject = (CGameObject*)pArg;
		if (m_pGameObject == nullptr)
			return E_FAIL;

		m_pTransform = static_cast<CTransform*>(m_pGameObject->GetComponent("Com_Transform"));
		pos = XMVectorSetY(pos, XMVectorGetY(pos) + 0.2f);

		int random = rand() % 3;
		float randomf = random / 10;
		pos = XMVectorSetX(pos, XMVectorGetX(pos) + random);
		pos = XMVectorSetY(pos, XMVectorGetY(pos) + randomf);
		pos = XMVectorSetZ(pos, XMVectorGetZ(pos) + random);

		Startscail.x = m_pTransform->GetScale(CTransform::STATE_RIGHT);
		Startscail.y = m_pTransform->GetScale(CTransform::STATE_UP);

		PosX = XMVectorGetX(m_pTransform->GetState(CTransform::STATE_POSITION));
		PosY =XMVectorGetY(m_pTransform->GetState(CTransform::STATE_POSITION));



		RandomNum = rand() % 3;
	}
	return S_OK;
}

void CEffectBlood::Update(_double deltaTime)
{
	if (m_bDead)
		return;

	if (!m_pGameObject)
		return;

	m_dDeadTime += deltaTime;

	_vector pos = m_pTransform->GetState(CTransform::STATE_POSITION);

	//PosX += cosf((RandomNum + 50) * 3.141592f / 180.f) * deltaTime;
	//PosY -= sinf((RandomNum + 50)* 3.141592f / 180.f) * deltaTime;

	//pos = XMVectorSetX(pos, PosX);
	//pos = XMVectorSetY(pos, PosY);
	
	m_pTransform->SetState(CTransform::STATE_POSITION, pos);
	//m_pTransform->SetScale(_float3(Startscail));

	_matrix viewInverse = XMMatrixInverse(nullptr, CEngine::GetInstance()->GetTransform(CPipeline::D3DTS_VIEW));
	_float4x4 newWorld;
	_float4x4 world = m_pTransform->GetMatrix();
	_vector scale, rotation, position;
	XMMatrixDecompose(&scale, &rotation, &position, m_pTransform->GetWorldMatrix());
	XMStoreFloat4x4(&newWorld, viewInverse);
	memcpy(newWorld.m[3], world.m[3], sizeof(_float3));
	m_pTransform->SetMatrix(XMMatrixScalingFromVector(scale) * XMLoadFloat4x4(&newWorld));

}

void CEffectBlood::LateUpdate(_double deltaTime)
{
	if (static_cast<CEmptyEffect*>(m_pGameObject)->GetEffectDuration()<= m_dDeadTime)
	{
		this->SetDead();
		m_pGameObject->SetDead();
	}
}

void CEffectBlood::Render()
{
}


void CEffectBlood::Free()
{
	__super::Free();
}
