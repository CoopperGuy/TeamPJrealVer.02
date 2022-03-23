#include "stdafx.h"
#include "Client_Struct.h"
#include "..\Public\EffectMeteoExpolRing.h"
#include "EmptyEffect.h"
#include "EffectMagicAf.h"
USING(Client)

CEffectMeteoExpolRing::CEffectMeteoExpolRing()
{
}

CEffectMeteoExpolRing * CEffectMeteoExpolRing::Create(void * pArg, _vector pos)
{
	CEffectMeteoExpolRing*		pInstance = new CEffectMeteoExpolRing();

	if (FAILED(pInstance->Initialize(pArg, pos)))
	{
		MSG_BOX("Failed to Create CEffectMeteoExpolRing");
		SafeRelease(pInstance);
		return nullptr;
	}

	return pInstance;
}


HRESULT CEffectMeteoExpolRing::Initialize(void* pArg, _vector pos)
{
	if (pArg != nullptr) {

		m_pGameObject = (CGameObject*)pArg;
		if (m_pGameObject == nullptr)
			return E_FAIL;

		m_pTransform = static_cast<CTransform*>(m_pGameObject->GetComponent("Com_Transform"));
		pos = XMVectorSetY(pos, XMVectorGetY(pos) + 0.2f);

		m_pTransform->SetState(CTransform::STATE_POSITION, pos);

		Startscail.x = m_pTransform->GetScale(CTransform::STATE_RIGHT);
		Startscail.y = m_pTransform->GetScale(CTransform::STATE_UP);

	}
	return S_OK;
}

void CEffectMeteoExpolRing::Update(_double deltaTime)
{
	if (m_bDead)
		return;

	if (!m_pGameObject)
		return;
	
	m_dDeadTime += deltaTime;

	Startscail.x += (_float)deltaTime*1.2 ;
	Startscail.y+= (_float)deltaTime *1.2;


	m_pTransform->SetScale(_float3(Startscail));

	if (m_pTransform->GetScale(CTransform::STATE_UP) >= 1.3f)
		m_bDead = true;

	_matrix viewInverse = XMMatrixInverse(nullptr, CEngine::GetInstance()->GetTransform(CPipeline::D3DTS_VIEW));
	_float4x4 newWorld;
	_float4x4 world = m_pTransform->GetMatrix();
	_vector scale, rotation, position;
	XMMatrixDecompose(&scale, &rotation, &position, m_pTransform->GetWorldMatrix());
	XMStoreFloat4x4(&newWorld, viewInverse);
	memcpy(newWorld.m[3], world.m[3], sizeof(_float3));
	m_pTransform->SetMatrix(XMMatrixScalingFromVector(scale) * XMLoadFloat4x4(&newWorld));
}


void CEffectMeteoExpolRing::LateUpdate(_double deltaTime)
{
	if (m_bDead)
	{
		this->SetDead();
		m_pGameObject->SetDead();
	}
}

void CEffectMeteoExpolRing::Render()
{
}


void CEffectMeteoExpolRing::Free()
{
	__super::Free();
}
