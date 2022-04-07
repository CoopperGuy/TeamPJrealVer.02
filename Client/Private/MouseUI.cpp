#include "stdafx.h"
#include "..\Public\MouseUI.h"

USING(Client)

CMouseUI::CMouseUI()
{
}

HRESULT CMouseUI::Initailze(CGameObject * pArg)
{
	CEngine*	pEngine = CEngine::GetInstance();

	m_pThisUI =static_cast<CEmptyUI*>(pEngine->FindGameObjectWithName(0, "MouseCursor"));
	pEngine->AddScriptObject(this, 0);
	return S_OK;
}

void CMouseUI::Update(_double deltaTime)
{
	if (g_Menu || g_AnotherMenu)
		m_pThisUI->SetActive(true);
	else
		m_pThisUI->SetActive(false);

	POINT		ptMouse;
	GetCursorPos(&ptMouse);
	ScreenToClient(g_hWnd, &ptMouse);
	m_pThisUI->SetPosition(ptMouse.x, ptMouse.y);
}

void CMouseUI::LateUpdate(_double deltaTime)
{
}

void CMouseUI::Render()
{
}

CMouseUI * CMouseUI::Create(CGameObject * pTarget)
{
	CMouseUI*	obj = new CMouseUI();
	if (FAILED(obj->Initailze(pTarget))) {
		SAFE_RELEASE(obj);
		return nullptr;
	}
	return obj;
}

void CMouseUI::Free()
{
}

