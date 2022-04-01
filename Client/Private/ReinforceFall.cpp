#include "stdafx.h"
#include "..\Public\ReinforceFall.h"


CReinforceFall::CReinforceFall()
{
}

HRESULT CReinforceFall::Initailze(CGameObject * pArg)
{
	m_pThisUI = static_cast<CEmptyUI*>(CEngine::GetInstance()->SpawnPrefab("U_ReinforceFail"));
	CEngine::GetInstance()->AddScriptObject(this, CEngine::GetInstance()->GetCurSceneNumber());
	return S_OK;
}

void CReinforceFall::Update(_double deltaTime)
{
}

void CReinforceFall::LateUpdate(_double deltaTime)
{
	m_LifeDelta += deltaTime;
	if (m_LifeTime < m_LifeDelta) {
		m_LifeDelta = 0;
		this->SetDead();
		m_pThisUI->SetDead();
	}
}

void CReinforceFall::Render()
{
}

CReinforceFall * CReinforceFall::Create(CGameObject * pTarget)
{
	CReinforceFall*	obj = new CReinforceFall();
	if (FAILED(obj->Initailze(pTarget))) {
		SAFE_RELEASE(obj);
		return nullptr;
	}
	return obj;
}

void CReinforceFall::Free()
{
}

