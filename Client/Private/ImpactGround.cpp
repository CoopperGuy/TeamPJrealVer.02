#include "stdafx.h"
#include "..\Public\ImpactGround.h"

USING(Client)

CImpactGround::CImpactGround()
{
}

HRESULT CImpactGround::Initialize(CEmptyEffect * _pThisEffect, CGameObject * _pPlayer)
{
	m_pThisEffect = _pThisEffect;
	m_pTargetObj = _pPlayer;

	CTransform* pPlayerTrans = static_cast<CTransform*>(m_pTargetObj->GetComponent("Com_Transform"));
	m_pEffectTrans = static_cast<CTransform*>(m_pThisEffect->GetComponent("Com_Transform"));

	_vector position = XMVector3TransformCoord(XMVectorSet(0.f, 0.f, 0.5f, 1.f), pPlayerTrans->GetWorldMatrix());
	m_pEffectTrans->SetMatrix(XMMatrixRotationX(XMConvertToRadians(90.f)) * XMMatrixRotationY(XMConvertToRadians(90.f)) * pPlayerTrans->GetWorldMatrix());
	m_pEffectTrans->SetState(CTransform::STATE_POSITION, position);

	CModel* pModel = static_cast<CModel*>(m_pThisEffect->GetComponent("Com_Model"));
	if (pModel)
		m_ModelList.emplace_back(pModel);
	list<CGameObject*>	child = m_pThisEffect->GetChildren();
	for (auto& iter : child) {
		CModel* pModels = static_cast<CModel*>(iter->GetComponent("Com_Model"));
		if(pModels)
			m_ModelList.emplace_back(pModels);
	}
	return S_OK;
}

void CImpactGround::Update(_double deltaTime)
{
	m_pEffectTrans->SetScale(_float3((_float)m_DurationDelta + 1.4f, m_DurationDelta + 1.4f, (_float)m_DurationDelta + 1.4f));
}

void CImpactGround::LateUpdate(_double deltaTime)
{
	m_DurationDelta += deltaTime;
	if (m_DurationDelta > m_Duration) {
		this->SetDead();
		m_pThisEffect->SetDead();
	}
	for (auto& iter : m_ModelList) {
		
	}
}

CImpactGround * CImpactGround::Create(CEmptyEffect * _pThisEffect, CGameObject * _pPlayerEffect)
{
	CImpactGround*		pInstance = new CImpactGround();
	if (FAILED(pInstance->Initialize(_pThisEffect, _pPlayerEffect)))
	{
		MSG_BOX("Failed to Create CImpactGround");
		SafeRelease(pInstance);
		return nullptr;
	}
	return pInstance;
}

void CImpactGround::Free()
{
}


