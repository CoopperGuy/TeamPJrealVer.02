#include "EnginePCH.h"
#include "..\Public\BossMovie.h"

USING(Engine)

CBossMovie::CBossMovie()
{
}

HRESULT CBossMovie::Initailze(_int _type, CGameObject * pArg)
{
	CEngine*	pEngine = CEngine::GetInstance();

	m_pThisUI = static_cast<CEmptyUI*>(pEngine->SpawnPrefab("BossMovie"));
	pEngine->AddScriptObject(this,pEngine->GetCurSceneNumber());
	list<CGameObject*> _child = m_pThisUI->GetChildren();
	auto& iter = _child.front();

	if (_type == 0)
	{
		m_pName = static_cast<CVIBuffer_RectUI*>(iter->GetComponent("Com_VIBuffer"));
	}
	else
	{

	}
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

CBossMovie * CBossMovie::Create(_int _type, CGameObject * pTarget)
{
	CBossMovie*	obj = new CBossMovie();
	if (FAILED(obj->Initailze(_type,pTarget))) {
		SAFE_RELEASE(obj);
		return nullptr;
	}
	return obj;
}

void CBossMovie::Free()
{
}

