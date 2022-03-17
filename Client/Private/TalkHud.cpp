#include "stdafx.h"
#include "..\Public\TalkHud.h"

USING(Client)

CTalkHud::CTalkHud()
{
}

HRESULT CTalkHud::Initailze(CGameObject * pArg)
{
	if (pArg != nullptr) {
		m_pTarget = pArg;
	}
	m_pThisUI = dynamic_cast<CEmptyUI*>(CEngine::GetInstance()->FindGameObjectWithName(0, "TalkDesk"));
	list<CGameObject*> child = m_pThisUI->GetChildren();
	m_pTextDesk = static_cast<CText*>(child.front()->GetComponent("Com_Text"));
	
	return S_OK;
}

void CTalkHud::Update(_double deltaTime)
{
	m_pTextDesk->SetString("gadf");
}

void CTalkHud::LateUpdate(_double deltaTime)
{
}

void CTalkHud::Render()
{
}

void CTalkHud::StartConversation(_uint npcID, _uint questID, _uint questProgress)
{
	CEngine::GetInstance()->GetTalk(npcID + questProgress);
}

CTalkHud * CTalkHud::Create(CGameObject * pTarget)
{
	CTalkHud*	obj = new CTalkHud();
	if (FAILED(obj->Initailze(pTarget))) {
		SAFE_RELEASE(obj);
		return nullptr;
	}
	return obj;
}

void CTalkHud::Free()
{
}

