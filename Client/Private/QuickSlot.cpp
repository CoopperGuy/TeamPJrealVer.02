#include "stdafx.h"
#include "..\Public\QuickSlot.h"
#include "Item.h"
#include "VIBuffer_RectUI.h"
#include "EventCheck.h"
#include "AddQuickslot.h"
#include "Player.h"
USING(Client)
static string itemImagePath = "../../Assets/UITexture/Item/";

CQuickSlot::CQuickSlot()
{
}

HRESULT CQuickSlot::Initailze(CGameObject * pArg)
{
	m_pThis =static_cast<CEmptyUI*>(CEngine::GetInstance()->FindGameObjectWithName(SCENE_STATIC, "QuickSlot0"));
	list<CGameObject*> child = m_pThis->GetChildren();
	_uint i = 0;
	for (auto& iter : child) {
		m_childVIBuffer.emplace_back(static_cast<CVIBuffer_RectUI*>(iter->GetComponent("Com_VIBuffer")));
		i++;
	}
	CAddQuickslot::Create(nullptr);
	return S_OK;
}

void CQuickSlot::Update(_double deltaTime)
{
}

void CQuickSlot::LateUpdate(_double deltaTime)
{
}

void CQuickSlot::Render()
{
}

void CQuickSlot::RegistItem(CItem * _item, _int _idx)
{
	m_iMyIdx = _idx;
	if (_item != nullptr) {
		string path = itemImagePath + _item->GetItempInfo().imageName + ".dds";
		m_childVIBuffer[QUICK_ICON]->UpdateTexture(path, CVIBuffer_RectUI::TEXTURE_DIFFUSE);
		m_CurItem = _item;
		CEventCheck::GetInstance()->SetAddQuickIcon(path, m_iMyIdx);
	}
}

void CQuickSlot::UseItem(CPlayer* _obj)
{
	if (m_CurItem != nullptr) {
		m_CurItem->UseItem();
		ITEMINFO info = m_CurItem->GetItempInfo();
		_obj->HealHp(info.hp);
		if (info.numOfItem == 0) {
			string path = itemImagePath + "nullImage.dds";
			m_childVIBuffer[QUICK_ICON]->UpdateTexture(path, CVIBuffer_RectUI::TEXTURE_DIFFUSE);
			CEventCheck::GetInstance()->SetAddQuickIcon(path, m_iMyIdx);
			m_CurItem = nullptr;
		}
	}
}

CQuickSlot * CQuickSlot::Create(CGameObject * pTarget)
{
	CQuickSlot*	obj = new CQuickSlot();
	if (FAILED(obj->Initailze(pTarget))) {
		SAFE_RELEASE(obj);
		return nullptr;
	}
	return obj;
}

void CQuickSlot::Free()
{
}

