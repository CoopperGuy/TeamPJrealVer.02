#include "stdafx.h"
#include "..\Public\ConsumItemHud.h"
#include "Item.h"
#include "EmptyUI.h"
#include "EventCheck.h"

BEGIN(Client)
_int threadNum = 0;
void CheckConsumItem(CConsumItemHud* script) {
	if (threadNum < 0)
		return;
	threadNum--;
	script->CheckItems();
	threadNum++;
}

END
USING(Client)
static string itemImagePath = "../../Assets/UITexture/Item/";

CConsumItemHud::CConsumItemHud()
{
}

HRESULT CConsumItemHud::Initailze(CGameObject * pArg)
{
	if (pArg != nullptr) {
		m_pTarget = pArg;
		SafeAddRef(m_pTarget);
	}
	m_pThisUI = dynamic_cast<CEmptyUI*>(CEngine::GetInstance()->FindGameObjectWithName(0, "ConsumItemList"));
//#ifdef _DEBUG
//	for (int i = 0; i < 5; i++) {
//		m_pConsumItemList.emplace_back(dynamic_cast<CEmptyUI*>(CEngine::GetInstance()->SpawnPrefab("ConsumItemList0")));
//		m_fCurYSize += 65.f;
//	}
//	
//	_float x = 390.f;
//	_float y = 0.f;
//	_int i = 0;
//	for (auto& iter : m_pConsumItemList) {
//		y += m_constYScale;
//		iter->SetPosition(x, y);
//		m_pThisUI->AddChild(iter);
//		i++;
//	}
//#endif

	return S_OK;
}

void CConsumItemHud::Update(_double deltaTime)
{
	Scroll(deltaTime);
	_uint i = 0;
	for (auto& iter : m_pConsumItemList) {
		if (iter.first->IsHovered()) {
			if(CEngine::GetInstance()->IsMouseDown(0))
				m_iCurSelected = i;
		}
		i++;
	}



	if (m_pThisUI->IsActive()) {
		if (CEventCheck::GetInstance()->GetBackPackState() == CEventCheck::BACK_CONSUM) {
			if (CEngine::GetInstance()->Get_DIKDown(DIK_SPACE)) {
				m_bSelectInputIndex = true;
				CEventCheck::GetInstance()->OpenAddQuickSlot(m_bSelectInputIndex);
			}
		}
	}
	else {
		m_iCurSelected = -1;
	}
}

void CConsumItemHud::LateUpdate(_double deltaTime)
{
	_uint checkIdx = 0;
	for (auto& iter : m_pConsumItemList) {
		if (checkIdx == m_iCurSelected) {
			iter.first->SetIsSelected(true);
		}
		else {
			iter.first->SetIsSelected(false);
		}
		checkIdx++;
	}
}

void CConsumItemHud::Render()
{
}

void CConsumItemHud::AddConsumItem(CEmptyUI * itemUI)
{
	_float x = m_fAddItemPosX;
	list<CGameObject*>	child = itemUI->GetChildren();
	auto& iter = child.begin();
	ITEMSUB	itemSub;
	itemSub.name = dynamic_cast<CText*>((*iter)->GetComponent("Com_Text"));
	iter++;
	itemSub.amount = dynamic_cast<CText*>((*iter)->GetComponent("Com_Text"));
	iter++;
	itemSub._image = dynamic_cast<CVIBuffer_RectUI*>((*iter)->GetComponent("Com_VIBuffer"));


	m_pConsumItemList.emplace_back(make_pair(itemUI,itemSub));
	m_fCurYSize += m_constYScale;
	itemUI->SetPosition(x, m_fCurYSize + 120.f);
	m_pThisUI->AddChild(itemUI);

}

void CConsumItemHud::RemoveConsumItem(_uint idx)
{
	auto iter = m_pConsumItemList.begin();
	for (_uint i = 0; i < idx; i++) {
		iter++;
	}
	(*iter).first->RemoveParent();
	iter = m_pConsumItemList.erase(iter);
	m_fCurYSize -= m_constYScale;

	for (; iter != m_pConsumItemList.end(); iter++) {
		_float2 pos = (*iter).first->GetPosition();
		pos.y -= m_constYScale;
		(*iter).first->SetPosition(pos.x, pos.y);
	}



}

void CConsumItemHud::AddQuickSlot()
{
}

void CConsumItemHud::CheckItems()
{
	_int	uiNumber = (_int)m_pConsumItemList.size();
	if (!m_pInven)
		return;
	_int	invenNumber = (_int)m_pInven->GetInvenTypeSize(ITEMTYPE::CONSUM);
	if (uiNumber < invenNumber) {
		for (_int i = 0; i < invenNumber - uiNumber; i++) {
			AddConsumItem(dynamic_cast<CEmptyUI*>(CEngine::GetInstance()->SpawnPrefab("U_ItemList0")));
		}
	}
	if (uiNumber > invenNumber) {
		for (_int i = 0; i < uiNumber - invenNumber; i++) {
			m_pConsumItemList.back().first->SetDead();
			m_pConsumItemList.pop_back();
		}
	}

	INVEN itemList = m_pInven->GetInventoryByType(ITEMTYPE::CONSUM);
	_uint i = 0;
	for (auto& iter : itemList) {
		ITEMINFO info = iter.second->GetItempInfo();
		m_pConsumItemList[i].second.name->SetString(iter.first);
		m_pConsumItemList[i].second.amount->SetString(to_string(info.numOfItem));
		string path = itemImagePath + info.imageName + ".dds";
		m_pConsumItemList[i].second._image->UpdateTexture(path, CVIBuffer_RectUI::TEXTURE_DIFFUSE);
		i++;
	}

}

void CConsumItemHud::LinkInven(CInventory * _inven)
{
	m_pInven = _inven;
}

void CConsumItemHud::Scroll(_double deltaTime)
{
	_float fScrollSpd = 500.f;
	if (m_pThisUI->IsActive()) {
		std::thread checking(CheckConsumItem, this);
		checking.detach();
		if (CEngine::GetInstance()->GetMouseMoveValue().z > 0) {
			_float2 pos = m_pThisUI->GetTransformOffst();
			if (pos.y >= 0.f) {
				pos.y -= fScrollSpd*(_float)deltaTime;
			}
			else {
				pos.y = 0.f;
			}
			m_pThisUI->SetTransformOffst(pos.x, pos.y);
		}
		else if (CEngine::GetInstance()->GetMouseMoveValue().z < 0) {
			_float2 pos = m_pThisUI->GetTransformOffst();
			if (pos.y <= m_fCurYSize - m_constYScale) {
				pos.y += fScrollSpd*(_float)deltaTime;
			}
			else {
				pos.y = m_fCurYSize - m_constYScale;
			}
			m_pThisUI->SetTransformOffst(pos.x, pos.y);
		}
		else {
			_float2 offest = m_pThisUI->GetTransformOffst();
			m_pThisUI->SetTransformOffst(offest.x, offest.y);
		}
	}
	else {
		m_pThisUI->SetTransformOffst(0.f, 0.f);
	}
}

void CConsumItemHud::SetAddPosition(_float x)
{
	m_fAddItemPosX = x;
}

string CConsumItemHud::GetSelectedItemName()
{
	return m_pConsumItemList[m_iCurSelected].second.name->GetText();
}


CConsumItemHud * CConsumItemHud::Create(CGameObject * pTarget)
{
	CConsumItemHud*	obj = new CConsumItemHud();
	if (FAILED(obj->Initailze(pTarget))) {
		SAFE_RELEASE(obj);
		return nullptr;
	}
	return obj;
}

void CConsumItemHud::Free()
{
}

