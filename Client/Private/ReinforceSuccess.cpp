#include "stdafx.h"
#include "..\Public\ReinforceSuccess.h"

USING(Client)

CReinforceSuccess::CReinforceSuccess()
{
}

HRESULT CReinforceSuccess::Initailze(CGameObject * pArg)
{
	m_pThisUI = static_cast<CEmptyUI*>(CEngine::GetInstance()->SpawnPrefab("U_ReinforceSuccess"));
	CEngine::GetInstance()->AddScriptObject(this, CEngine::GetInstance()->GetCurSceneNumber());
	return S_OK;
}

void CReinforceSuccess::Update(_double deltaTime)
{
	m_pThisUI->p_Alpha = _float((m_LifeTime - m_LifeDelta) / m_LifeTime);
}

void CReinforceSuccess::LateUpdate(_double deltaTime)
{
	m_LifeDelta += deltaTime;
	if (m_LifeTime < m_LifeDelta) {
		m_LifeDelta = 0;
		this->SetDead();
		m_pThisUI->SetDead();
	}
}

void CReinforceSuccess::Render()
{
}

CReinforceSuccess * CReinforceSuccess::Create(CGameObject * pTarget)
{
	CReinforceSuccess*	obj = new CReinforceSuccess();
	if (FAILED(obj->Initailze(pTarget))) {
		SAFE_RELEASE(obj);
		return nullptr;
	}
	return obj;
}

void CReinforceSuccess::Free()
{
}

