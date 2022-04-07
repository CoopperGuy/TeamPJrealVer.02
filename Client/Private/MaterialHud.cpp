#include "stdafx.h"
#include "..\Public\MaterialHud.h"
#include "EventCheck.h"
BEGIN(Client)
void CheckMaterialItem(CMaterialHud* script) {
	if (script->GetThreadNum() < 0)
		return;
	script->MinusThread();
	script->AddThread();
}
END

USING(Client)
static string itemImagePath = "../../Assets/UITexture/Item/";

CMaterialHud::CMaterialHud()
{
}

HRESULT CMaterialHud::Initailze(CGameObject * pArg)
{
	if (pArg != nullptr) {
		m_pTarget = pArg;
	}
	m_pThisUI = dynamic_cast<CEmptyUI*>(CEngine::GetInstance()->FindGameObjectWithName(0, "MaterialItemList0"));
//
//#ifdef _DEBUG
//	for (int i = 0; i < 3; i++) {
//		m_pMaterialList.emplace_back(dynamic_cast<CEmptyUI*>(CEngine::GetInstance()->SpawnPrefab("ItemList0")));
//		m_fCurYSize += 65.f;
//	}
//	_float x = 390.f;
//	_float y = 0.f;
//	_int i = 0;
//	for (auto& iter : m_pMaterialList) {
//		y += m_constYScale;
//		iter->SetPosition(x, y);
//		m_pThisUI->AddChild(iter);
//		i++;
//	}
//#endif


	return S_OK;
}

void CMaterialHud::Update(_double deltaTime)
{
	_uint i = 0;



	if (m_pThisUI->IsActive()) 
	{
		for (auto& iter : m_pMaterialList) 
		{
			if (iter.first->isFristEnter())
			{
				CEngine::GetInstance()->StopSound(CHANNELID::UI02);
				CEngine::GetInstance()->PlaySoundW("ItemHover.mp3", CHANNELID::UI02);
			}
			if (iter.first->IsHovered()) 
			{
				if (CEngine::GetInstance()->Get_MouseButtonStateDown(CInput_Device::MOUSEBUTTONSTATE::MBS_LBUTTON))
				{
					CEngine::GetInstance()->StopSound(CHANNELID::UI03);
					CEngine::GetInstance()->PlaySoundW("MaterialSelect.mp3", CHANNELID::UI03);
					m_iCurSelected = i;
				}
			}
			i++;
		}
	}
	else {
		m_iCurSelected = -1;
	}

	Scroll(deltaTime);

}

void CMaterialHud::LateUpdate(_double deltaTime)
{
	_uint checkIdx = 0;
	for (auto& iter : m_pMaterialList) {
		if (checkIdx == m_iCurSelected) {
			iter.first->SetIsSelected(true);
		}
		else {
			iter.first->SetIsSelected(false);
		}
		checkIdx++;
	}
}

void CMaterialHud::Render()
{
}

void CMaterialHud::AddMaterialItem(CEmptyUI * item)
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

	m_pMaterialList.emplace_back(make_pair(item, itemSub));
	m_fCurYSize += m_constYScale;
	item->SetPosition(x, m_fCurYSize + 120.f);
	m_pThisUI->AddChild(item);
}

void CMaterialHud::RemoveMaterialItem(_uint idx)
{
	auto iter = m_pMaterialList.begin();
	for (_uint i = 0; i < idx; i++) {
		iter++;
	}
	(*iter).first->RemoveParent();
	(*iter).first->SetDead();
	iter = m_pMaterialList.erase(iter);
	m_fCurYSize -= m_constYScale;

	for (; iter != m_pMaterialList.end(); iter++) {
		_float2 offset = (*iter).first->GetTransformOffst();
		offset.y -= m_constYScale;
		(*iter).first->SetTransformOffst(offset.x, offset.y);
	}


}

void CMaterialHud::LinkInven(CInventory * inven)
{
	m_pInven = inven;
}

void CMaterialHud::CheckItems()
{
	_int	uiNumber = (_int)m_pMaterialList.size();
	_int	invenNumber = (_int)m_pInven->GetInvenTypeSize(ITEMTYPE::MATERIAL);
	if (uiNumber < invenNumber) {
		for (_int i = 0; i < invenNumber - uiNumber; i++) {
			AddMaterialItem(dynamic_cast<CEmptyUI*>(CEngine::GetInstance()->SpawnPrefab("U_ItemList0")));
		}
	}
	if (uiNumber > invenNumber) {
		for (_int i = 0; i < uiNumber - invenNumber; i++) {
			m_pMaterialList.back().first->SetDead();
			m_pMaterialList.pop_back();
		}
	}

	INVEN itemList = m_pInven->GetInventoryByType(ITEMTYPE::MATERIAL);
	_uint i = 0;
	for (auto& iter : itemList) {
		ITEMINFO info = iter.second->GetItempInfo();

		m_pMaterialList[i].second.name->SetString(iter.first);
		m_pMaterialList[i].second.amount->SetString(to_string(info.numOfItem));
		string path = itemImagePath + info.imageName + ".dds";
		m_pMaterialList[i].second._image->UpdateTexture(path, CVIBuffer_RectUI::TEXTURE_DIFFUSE);
		i++;
	}
}

void CMaterialHud::Scroll(_double deltaTime)
{
	_float fScrollSpd = 500.f;
	if (m_pThisUI->IsActive()) {
		this->CheckItems();
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
		if (CEngine::GetInstance()->GetMouseMoveValue().z < 0) {
			_float2 pos = m_pThisUI->GetTransformOffst();
			if (pos.y <= m_fCurYSize - m_constYScale) {
				pos.y += fScrollSpd*(_float)deltaTime;
			}
			else {
				pos.y = m_fCurYSize - m_constYScale;
			}
			m_pThisUI->SetTransformOffst(pos.x, pos.y);
		}
	}
	else {
		m_pThisUI->SetTransformOffst(0.f, 0.f);
	}
}

CItem * CMaterialHud::GetSelectedItem()
{
	string _name = m_pMaterialList[m_iCurSelected].second.name->GetText();
	CItem* _item = m_pInven->GetItemByName(_name, ITEMTYPE::MATERIAL);
	return _item;
}

void CMaterialHud::SetAddPosition(_float x)
{
	m_fAddItemPosX = x;
}

CMaterialHud * CMaterialHud::Create(CGameObject * pTarget)
{
	CMaterialHud*	obj = new CMaterialHud();
	if (FAILED(obj->Initailze(pTarget))) {
		SAFE_RELEASE(obj);
		return nullptr;
	}
	return obj;
}

void CMaterialHud::Free()
{
}

