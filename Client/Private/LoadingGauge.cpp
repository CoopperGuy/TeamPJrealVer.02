#include "stdafx.h"
#include "..\Public\LoadingGauge.h"

USING(Client)

CLoadingGauge::CLoadingGauge()
{
}

HRESULT CLoadingGauge::Initailze(CGameObject * pArg)
{
	if (pArg != nullptr) {
		pTarget = pArg;
	}

	m_pThisUI = static_cast<CEmptyUI*>(CEngine::GetInstance()->FindGameObjectWithName(CEngine::GetInstance()->GetCurSceneNumber(), "LoadGuage"));

	m_pVIBuffer = static_cast<CVIBuffer_RectUI*>(m_pThisUI->GetComponent("Com_VIBuffer"));
	list<CGameObject*> child = m_pThisUI->GetChildren();
	auto& iter = child.begin();
	m_child.emplace_back(static_cast<CEmptyUI*>(*iter));
	iter++;
	m_pText = static_cast<CText*>((*iter)->GetComponent("Com_Text"));
	return S_OK;
}

void CLoadingGauge::Update(_double deltaTime)
{
	string percentage = to_string((_int)GetPercentage()) + "%";
	m_pText->SetString(percentage);
}

void CLoadingGauge::LateUpdate(_double deltaTime)
{
	if (m_fDegree >= XMConvertToRadians(360.f)) {
		m_fDegree = XMConvertToRadians(360.f);
	}
	m_child[0]->SetDegree(m_fDegree);
}

void CLoadingGauge::Render()
{
}

void CLoadingGauge::AddPercentage(_float num)
{
	m_fDegree += num * XMConvertToRadians(3.6f);
}

CLoadingGauge * CLoadingGauge::Create(CGameObject * pTarget)
{
	CLoadingGauge*	obj = new CLoadingGauge();
	if (FAILED(obj->Initailze(pTarget))) {
		SAFE_RELEASE(obj);
		return nullptr;
	}
	return obj;
}

void CLoadingGauge::Free()
{
}


