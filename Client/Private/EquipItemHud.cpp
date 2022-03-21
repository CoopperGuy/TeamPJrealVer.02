#include "stdafx.h"
#include "..\Public\EquipItemHud.h"
#include "EventCheck.h"
BEGIN(Client)
void CheckEquipItem(CEquipItemHud* script) {
	if (script->GetThreadNum() < 0)
		return;
	script->MinusThread();
	script->CheckItems();
	script->AddThread();
}
END

USING(Client)
static string itemImagePath = "../../Assets/UITexture/Item/";

CEquipItemHud::CEquipItemHud()
{
}

HRESULT CEquipItemHud::Initailze(CGameObject * pArg)
{
	if (pArg != nullptr) {
		m_pTarget = pArg;
	}
	m_pThisUI = dynamic_cast<CEmptyUI*>(CEngine::GetInstance()->FindGameObjectWithName(0, "EquipItemList"));
	list<CGameObject*> category = CEngine::GetInstance()->GetGameObjectInLayer(0, "EquipCategory");

	for (auto& iter : category) {
		m_pEquipTagList.emplace_back(static_cast<CEmptyUI*>(iter));
	}

	for (_uint i = 0; i < (_uint)EQUIPTYPE::TYPE_END; i++) {
		m_fCurYSize[i] = 0.f;
	}
//#ifdef _DEBUG
//	for (int j = 0; j <  (_int)EQUIPTYPE::TYPE_END; j++) {
//		for (int i = 0; i < j+3; i++) {
//			m_pEquipItemList[j].emplace_back(dynamic_cast<CEmptyUI*>(CEngine::GetInstance()->SpawnPrefab("ItemList0")));
//			m_fCurYSize[j] += 65.f;
//		}
//	}
//	for (int i = 0; i < (_int)EQUIPTYPE::TYPE_END; i++) {
//		_float x = 390.f;
//		_float y = 0.f;
//		for (auto& iter : m_pEquipItemList[i]) {
//			y += m_constYScale;
//			iter->SetPosition(x, y);
//			m_pThisUI->AddChild(iter);
//		}
//	}
//#endif
//

	

	return S_OK;
}

void CEquipItemHud::Update(_double deltaTime)
{
	_uint selectIdx = 0;
	for (auto& iter : m_pEquipTagList) {
		if (iter->IsHovered()) {
			if (CEngine::GetInstance()->IsMouseDown(0)) {
				m_iCurSelectedTag = selectIdx;
				m_pThisUI->SetTransformOffst(0.f, 0.f);
				for (int i = 0; i < m_pEquipTagList.size(); i++) {
					if (i != selectIdx) {
						m_pEquipTagList[i]->SetIsSelected(false);
					}
				}
			}
		}
	
		selectIdx++;
	}

	for (_uint i = 0; i < (_int)EQUIPTYPE::TYPE_END; i++) {
		for (auto& iter : m_pEquipItemList[i]) {
			iter.first->SetisRender(false);
		}
	}
	for (auto& iter : m_pEquipItemList[m_iCurSelectedTag]) {
		iter.first->SetisRender(true);
		if (CEventCheck::GetInstance()->GetBackPackState() == CEventCheck::BACK_EQUIP) {
			if (iter.first->IsHovered()) {
				if (CEngine::GetInstance()->Get_DIKDown(DIK_SPACE)) {
					CEventCheck::GetInstance()->SetUpEquip(iter.second.name->GetText());
				}
			}
		}
	}
	Scroll(deltaTime);
}

void CEquipItemHud::LateUpdate(_double deltaTime)
{
}

void CEquipItemHud::Render()
{
}

void CEquipItemHud::AddEquipItem(CEmptyUI * item, EQUIPTYPE _category)
{
	_float x = m_fAddItemPosX;
	list<CGameObject*>	child = item->GetChildren();
	auto& iter = child.begin();
	ITEMSUB	itemSub;
	itemSub.name = dynamic_cast<CText*>((*iter)->GetComponent("Com_Text"));
	iter++;
	itemSub.amount = dynamic_cast<CText*>((*iter)->GetComponent("Com_Text"));
	iter++;
	itemSub._image = dynamic_cast<CVIBuffer_RectUI*>((*iter)->GetComponent("Com_VIBuffer"));

	m_pEquipItemList[(_int)_category].emplace_back(make_pair(item, itemSub));
	m_fCurYSize[(_int)_category] += m_constYScale;
	item->SetPosition(x, m_fCurYSize[(_int)_category] + 120.f);
	m_pThisUI->AddChild(item);
}

void CEquipItemHud::RemoveEquipItem(_uint idx, EQUIPTYPE _category)
{
	auto iter = m_pEquipItemList[(_int)_category].begin();
	for (_uint i = 0; i < idx; i++) {
		iter++;
	}
	(*iter).first->RemoveParent();
	iter = m_pEquipItemList[(_int)_category].erase(iter);
	m_fCurYSize[(_int)_category] -= m_constYScale;
	for (; iter != m_pEquipItemList[(_int)_category].end(); iter++) {
		_float2 pos = (*iter).first->GetPosition();
		pos.y += m_constYScale;
		(*iter).first->SetPosition(pos.x,pos.y);
	}


}

void CEquipItemHud::CheckItems()
{
	for (_int i = 0; i < (_int)EQUIPTYPE::TYPE_END; i++) {
		_int uiNumber = (_int)m_pEquipItemList[i].size();
		_int invenNumber = (_int)m_pInven->GetInvenTypeSize(ITEMTYPE::EQUIP, (EQUIPTYPE)i);
		if (uiNumber < invenNumber) {
			for (_int j = 0; j < (invenNumber - uiNumber); j++) {
				AddEquipItem(dynamic_cast<CEmptyUI*>(CEngine::GetInstance()->SpawnPrefab("U_ItemList0")), (EQUIPTYPE)i);
			}
		}

		if (uiNumber > invenNumber) {
			for (_int j = 0; j < uiNumber - invenNumber; j++) {
				m_pEquipItemList[i].back().first->SetDead();
				m_pEquipItemList[i].pop_back();
			}
		}
	}
	for (_int i = 0; i < (_int)EQUIPTYPE::TYPE_END; i++) {

		INVEN itemList = m_pInven->GetInventoryByType(ITEMTYPE::EQUIP,EQUIPTYPE(i));
		_uint idx = 0;
		for (auto& iter : itemList) {
			if (m_pEquipItemList[i].size() == itemList.size()) {
				ITEMINFO info = iter.second->GetItempInfo();
				m_pEquipItemList[i][idx].second.name->SetString(iter.first);
				m_pEquipItemList[i][idx].second.amount->SetString(to_string(info.numOfItem));
				string path = itemImagePath + info.imageName + ".dds";
				m_pEquipItemList[i][idx].second._image->UpdateTexture(path, CVIBuffer_RectUI::TEXTURE_DIFFUSE);

				idx++;
			}
		}
	}

}

void CEquipItemHud::LinkInven(CInventory * inven)
{
	m_pInven = inven;
}

void CEquipItemHud::Scroll(_double deltaTime)
{
	_float fScrollSpd = 500.f;
	if (m_pThisUI->IsActive()) {
		std::thread CheckingItem(CheckEquipItem, this);
		CheckingItem.detach();
		if (CEngine::GetInstance()->GetMouseMoveValue().z > 0) {
			_float2 pos = m_pThisUI->GetTransformOffst();
			if (pos.y >= 0) {
				pos.y -= fScrollSpd*(_float)deltaTime;
			}
			else {
				pos.y = 0;
			}
			m_pThisUI->SetTransformOffst(pos.x, pos.y);
		}
		if (CEngine::GetInstance()->GetMouseMoveValue().z < 0) {
			_float2 pos = m_pThisUI->GetTransformOffst();
			if (pos.y <= m_fCurYSize[m_iCurSelectedTag] - m_constYScale) {
				pos.y += fScrollSpd*(_float)deltaTime;
			}
			else {
				pos.y = m_fCurYSize[m_iCurSelectedTag] - m_constYScale;
			}
			m_pThisUI->SetTransformOffst(pos.x, pos.y);
		}
	}
	else {
		m_pThisUI->SetTransformOffst(0.f, 0.f);
	}
}

void CEquipItemHud::SetAddPosition(_float x)
{
	m_fAddItemPosX = x;
}


CEquipItemHud * CEquipItemHud::Create(CGameObject * pTarget)
{
	CEquipItemHud*	obj = new CEquipItemHud();
	if (FAILED(obj->Initailze(pTarget))) {
		SAFE_RELEASE(obj);
		return nullptr;
	}
	return obj;
}

void CEquipItemHud::Free()
{
}


