#include "stdafx.h"
#include "..\Public\Gold.h"

USING(Client)

CGold::CGold()
{
}

HRESULT CGold::Initailze(CGameObject * pArg)
{
	m_pGold = static_cast<CText*>(CEngine::GetInstance()->FindGameObjectWithName(SCENE_STATIC, "Gold")->GetComponent("Com_Text"));
	return S_OK;
}

void CGold::Update(_double deltaTime)
{
}

void CGold::LateUpdate(_double deltaTime)
{
}

void CGold::Render()
{
}

void CGold::UpdateGoldString(string str)
{
	m_pGold->SetString(str);
}

CGold * CGold::Create(CGameObject * pTarget)
{
	CGold*	obj = new CGold();
	if (FAILED(obj->Initailze(pTarget))) {
		SAFE_RELEASE(obj);
		return nullptr;
	}
	return obj;
}

void CGold::Free()
{
}

