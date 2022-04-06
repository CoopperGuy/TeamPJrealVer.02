#include "stdafx.h"
#include "..\Public\ImpactBeam.h"
#include "Obb.h"
USING(Client)

CImpactBeam::CImpactBeam()
{
}

HRESULT CImpactBeam::Initialize(CEmptyEffect * _pThisEffect, CGameObject * _pPlayer)
{
	m_pThisEffect = _pThisEffect;
	m_pTargetObj = _pPlayer;
	m_Child.reserve(2);
	CTransform* pPlayerTrans = static_cast<CTransform*>(m_pTargetObj->GetComponent("Com_Transform"));
	m_pEffectTrans = static_cast<CTransform*>(m_pThisEffect->GetComponent("Com_Transform"));

	_vector position = XMVector3TransformCoord(XMVectorSet(0.f, 0.f, 0.5f, 1.f), pPlayerTrans->GetWorldMatrix());
	//m_pEffectTrans->SetMatrix(pPlayerTrans->GetWorldMatrix());
	m_pEffectTrans->SetState(CTransform::STATE_POSITION, position);

	list<CGameObject*>	child = m_pThisEffect->GetChildren();
	for (auto& iter : child) {
		m_Child.emplace_back(static_cast<CTransform*>(iter->GetComponent("Com_LocalTransform")));
	}
	m_pObb = CObb::Create(position, XMVectorSet(1.f, 1.f, 1.f, 0.f), 50.f, ID::PLAYER_EFFECT, (_float)m_Duration);
	return S_OK;
}

void CImpactBeam::Update(_double deltaTime)
{
	_float delta = (_float)m_DurationDelta;
	m_pEffectTrans->SetScale(_float3(delta * 1.25f + 0.5f, delta * 2.f + 0.5f, delta * 1.25f + 0.5f));
	m_Child[0]->SetScale(_float3(delta * 7.f, 0.5f, delta * 7.f));
	m_Child[1]->SetScale(_float3(delta * 1.5f + 0.5f, delta * 1.5f + 0.5f, delta * 1.5f + 0.5f));

}

void CImpactBeam::LateUpdate(_double deltaTime)
{
	if (m_DurationDelta > 0)
		m_pObb->Idle();
	m_DurationDelta += deltaTime;
	if (m_DurationDelta > m_Duration) {
		this->SetDead();
		m_pThisEffect->SetDead();
		//m_pObb->SetupDead();
	}
}

CImpactBeam * CImpactBeam::Create(CEmptyEffect * _pThisEffect, CGameObject * _pPlayerEffect)
{
	CImpactBeam*		pInstance = new CImpactBeam();
	if (FAILED(pInstance->Initialize(_pThisEffect, _pPlayerEffect)))
	{
		MSG_BOX("Failed to Create CImpactBeam");
		SafeRelease(pInstance);
		return nullptr;
	}
	return pInstance;
}

void CImpactBeam::Free()
{
}


