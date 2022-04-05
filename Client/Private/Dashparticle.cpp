#include "stdafx.h"
#include "..\Public\Dashparticle.h"
#include "EventCheck.h"
#include "Obb.h"

USING(Client)

CDashparticle::CDashparticle()
{
}

HRESULT CDashparticle::Initialize(CEmptyEffect* pThis)
{
	m_pThis = pThis;
	m_pParticle = static_cast<CVIBuffer_RectInstance*>(m_pThis->GetComponent("Com_RectInstance"));

	return S_OK;
}

void CDashparticle::Update(_double dDeltaTime)
{
	m_DurationDelta += (_float)dDeltaTime;
}

void CDashparticle::LateUpdate(_double dDeltaTime)
{

	if (m_DurationDelta >= m_pParticle->Get_LifeTime())
	{
		this->SetDead();
		m_pThis->SetDead();
	}
}



CBasicEffect * CDashparticle::Create(CEmptyEffect* pThis)
{
	CDashparticle*		pInstance = new CDashparticle();

	if (FAILED(pInstance->Initialize(pThis)))
	{
		MSG_BOX("Failed to Create CDashparticle");
		SafeRelease(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CDashparticle::Free()
{
}
