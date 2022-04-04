#include "stdafx.h"
#include "Client_Struct.h"
#include "..\Public\EffectPhase2Inst.h"

USING(Client)

CEffectPahse2Inst::CEffectPahse2Inst()
{
}

CEffectPahse2Inst * CEffectPahse2Inst::Create(CTransform* pTransform)
{
	CEffectPahse2Inst*		pInstance = new CEffectPahse2Inst();

	if (FAILED(pInstance->Initialize(pTransform)))
	{
		MSG_BOX("Failed to Create CEffectPahse2Inst");
		SafeRelease(pInstance);
		return nullptr;
	}

	return pInstance;
}


HRESULT CEffectPahse2Inst::Initialize(CTransform* pTransform)
{
	if (pTransform != nullptr)
	{
		_float3 vPos;
		XMStoreFloat3(&vPos, pTransform->GetState(CTransform::STATE_POSITION));
		_matrix pTrans = XMMatrixTranslation(vPos.x, vPos.y + 0.2f, vPos.z);
		
		CGameObject* pEffect = CEngine::GetInstance()->AddGameObjectToPrefab(CEngine::GetInstance()->GetCurSceneNumber(), "Prototype_Effect_Phase2Inst", "E_Phase2Inst", &pTrans);

		m_pGameObject = pEffect;
		if (m_pGameObject == nullptr)
			return E_FAIL;

		m_pRectInstCom = static_cast<CVIBuffer_RectInstance*>(m_pGameObject->GetComponent("Com_RectInstance"));
		if (m_pRectInstCom == nullptr)
			return E_FAIL;
	}	

	return S_OK;
}

void CEffectPahse2Inst::Update(_double deltaTime)
{
	if (m_bDead)
		return;

	if (!m_pGameObject)
		return;

	m_dDeadTime += deltaTime;
}

void CEffectPahse2Inst::LateUpdate(_double deltaTime)
{	
	if (m_dDeadTime >= m_pRectInstCom->Get_LifeTime())
	{
		this->SetDead();
		m_pGameObject->SetDead();
	}
}

void CEffectPahse2Inst::Render()
{
}


void CEffectPahse2Inst::Free()
{
	__super::Free();
}
