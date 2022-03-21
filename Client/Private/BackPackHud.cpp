#include "stdafx.h"
#include "..\Public\BackPackHud.h"
#include "EquipItemHud.h"
#include "MaterialHud.h"
#include "ConsumItemHud.h"
#include "EventCheck.h"
USING(Client)

CBackPackHud::CBackPackHud()
{
}

HRESULT CBackPackHud::Initailze(CGameObject * pArg)
{
	if (pArg != nullptr) {
		m_pTarget = pArg;
	}
	m_pThisUI = dynamic_cast<CEmptyUI*>(CEngine::GetInstance()->FindGameObjectWithName(0, "Backpack_Hud"));
	list<CGameObject*> selectList = CEngine::GetInstance()->GetGameObjectInLayer(0, "BackPackSub");
	list<CGameObject*> buttonList = CEngine::GetInstance()->GetGameObjectInLayer(0, "BackPackSelect");

	for (auto& iter : selectList) {
		m_pBackPackList.emplace_back(dynamic_cast<CEmptyUI*>(iter));
	}
	for (auto& iter : buttonList) {
		m_pBackPackButton.emplace_back(dynamic_cast<CEmptyUI*>(iter));
	}
	m_pEquipHud = CEquipItemHud::Create();
	m_pMaterialHud = CMaterialHud::Create();
	m_pConsumHud = CConsumItemHud::Create();

	CEngine::GetInstance()->AddScriptObject(m_pEquipHud, 0);
	CEngine::GetInstance()->AddScriptObject(m_pMaterialHud, 0);
	CEngine::GetInstance()->AddScriptObject(m_pConsumHud, 0);

	CEventCheck::GetInstance()->SetBackPackHud(this);

	return S_OK;
}

void CBackPackHud::Update(_double deltaTime)
{
	_uint idx = 0;
	

	if (m_pInven != nullptr) {
		m_pEquipHud->LinkInven(m_pInven);
		m_pMaterialHud->LinkInven(m_pInven);
		m_pConsumHud->LinkInven(m_pInven);
	}

	if (m_pThisUI->IsActive()) {
		for (auto& iter : m_pBackPackButton) {
			iter->SetCorrectYSize(0.15f);
			if (iter->IsHovered()) {
				if (CEngine::GetInstance()->IsMouseDown(0)) {
					m_iCurSelected = idx;
				}
			}

			if (idx == m_iCurSelected) {
				m_pBackPackList[idx]->SetActive(true);
				iter->SetIsSelected(true);
			}
			else {
				m_pBackPackList[idx]->SetActive(false);
				iter->SetIsSelected(false);
			}
			
			idx++;
		}

		if (m_pConsumHud->GetAddQuickSlotOpen()) {
			_int idx = -1;
			if (CEngine::GetInstance()->Get_DIKDown(DIK_Z)) {
				idx = 0;
			}else if (CEngine::GetInstance()->Get_DIKDown(DIK_X)) {
				idx = 1;
			}
			else if (CEngine::GetInstance()->Get_DIKDown(DIK_C)) {
				idx = 2;
			}
			else if (CEngine::GetInstance()->Get_DIKDown(DIK_V)) {
				idx = 3;
			}
			if (idx > -1) {
				string name = m_pConsumHud->GetSelectedItemName();
				CItem* item = m_pInven->GetItemByName(name, ITEMTYPE::CONSUM);
				CEventCheck::GetInstance()->AddItemAtQuickSlot(item, idx);
				m_pConsumHud->SetAddQuickSlotOpen(false);
			}
						
		}
	}
	else {
		for (auto& iter : m_pBackPackList) {
			iter->SetActive(false);
		}
	}
	if (!m_bisOpenRightLeft)
		m_fAddPosX = 390.f;	
	else 
		m_fAddPosX = 1030.f;
	CEventCheck::GetInstance()->SetCurBackPack(CEventCheck::BACKPACKSTATE(m_iCurSelected));
	m_pEquipHud->SetAddPosition(m_fAddPosX);
	m_pMaterialHud->SetAddPosition(m_fAddPosX);
	m_pConsumHud->SetAddPosition(m_fAddPosX);
}

void CBackPackHud::LateUpdate(_double deltaTime)
{
}

void CBackPackHud::Render()
{
}

CBackPackHud * CBackPackHud::Create(CGameObject * pTarget)
{
	CBackPackHud*	obj = new CBackPackHud();
	if (FAILED(obj->Initailze(pTarget))) {
		SAFE_RELEASE(obj);
		return nullptr;
	}
	return obj;
}

void CBackPackHud::Free()
{
}

