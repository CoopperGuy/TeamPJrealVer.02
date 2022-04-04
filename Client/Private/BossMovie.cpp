#include "stdafx.h"
#include "..\Public\BossMovie.h"

USING(Client)

CBossMovie::CBossMovie()
{
}

HRESULT CBossMovie::Initailze(CGameObject * pArg)
{
	CEngine*	pEngine = CEngine::GetInstance();

	m_pThisUI = static_cast<CEmptyUI*>(pEngine->SpawnPrefab("BossMovie"));

	return S_OK;
}

void CBossMovie::Update(_double deltaTime)
{
}

void CBossMovie::LateUpdate(_double deltaTime)
{
	m_LifeDelta += deltaTime;
	if (m_LifeDelta > m_LifeTime) {
		this->SetDead();
		m_pThisUI->SetDead();
	}
}

void CBossMovie::Render()
{
}

CBossMovie * CBossMovie::Create(CGameObject * pTarget)
{
	CBossMovie*	obj = new CBossMovie();
	if (FAILED(obj->Initailze(pTarget))) {
		SAFE_RELEASE(obj);
		return nullptr;
	}
	return obj;
}

void CBossMovie::Free()
{
}

