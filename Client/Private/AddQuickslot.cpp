#include "stdafx.h"
#include "..\Public\AddQuickslot.h"
#include "VIBuffer_RectUI.h"
#include "EventCheck.h"
USING(Client)

CAddQuickslot::CAddQuickslot()
{
}

HRESULT CAddQuickslot::Initailze(CGameObject * pArg)
{
	list<class CGameObject*> regiQuick = CEngine::GetInstance()->GetGameObjectInLayer((_uint)SCENE_STATIC, "RegistQuickSlot");

	auto iter = regiQuick.begin();
	m_pThisUI = static_cast<CEmptyUI*>((*iter));
	iter++;
	m_pCloseButton = static_cast<CEmptyUI*>((*iter));

	list<CGameObject*> child = m_pThisUI->GetChildren();
	for (auto& c : child) {
		m_vecBuffer.emplace_back(static_cast<CVIBuffer_RectUI*>(c->GetComponent("Com_VIBuffer")));
	}
	CEventCheck::GetInstance()->SetAddQuick(this);
	CEngine::GetInstance()->AddScriptObject(this, SCENE_STATIC);
 	return S_OK;
}

void CAddQuickslot::Update(_double deltaTime)
{
	if (m_pCloseButton->IsHovered()) {
		if (CEngine::GetInstance()->IsMouseDown(0)) {
			m_pThisUI->SetActive(false);
			m_bisOpened = false;
		}
	}
	if (CEngine::GetInstance()->Get_DIKDown(VK_ESCAPE)) {
		m_bisOpened = false;
		m_pThisUI->SetActive(false);
	}
	if (m_bisOpened) {
		m_pThisUI->SetActive(true);
	}
	else {
		m_pThisUI->SetActive(false);
	}
}

void CAddQuickslot::LateUpdate(_double deltaTime)
{
}

void CAddQuickslot::Render()
{
}

void CAddQuickslot::UpdateIcon(string path, _uint idx)
{
	if(path != "")
		m_vecBuffer[idx]->UpdateTexture(path, CVIBuffer_RectUI::TEXTURE_DIFFUSE);
}

CAddQuickslot * CAddQuickslot::Create(CGameObject * pTarget)
{
	CAddQuickslot*	obj = new CAddQuickslot();
	if (FAILED(obj->Initailze(pTarget))) {
		SAFE_RELEASE(obj);
		return nullptr;
	}
	return obj;
}

void CAddQuickslot::Free()
{
}
