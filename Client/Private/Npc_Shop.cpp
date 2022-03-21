#include "stdafx.h"
#include "..\Public\Npc_Shop.h"
#include "ShopHud.h"
#include "Player.h"
#include "EventCheck.h"
BEGIN(Client)
void SetupShopList(CNpc_Shop* script) {
	script->SetupShop();
}
END

USING(Client)
static string itemImagePath = "../../Assets/UITexture/Item/";

CNpc_Shop::CNpc_Shop()
	:CNPC()
{
}

CNpc_Shop::CNpc_Shop(CGameObject * pObj)
	:CNPC(pObj)
{
}

HRESULT CNpc_Shop::Initailze(CGameObject * pArg)
{
	if (FAILED(__super::Initailze(pArg))) {
		return E_FAIL;
	}
	if (pArg) {
		m_pShopNPC = (CEmptyGameObject*)pArg;
		m_pNpcTransform = static_cast<CTransform*>(m_pShopNPC->GetComponent("Com_Transform"));
	/*	std::thread creatShop(SetupShopList, this);
		creatShop.detach();*/
		this->SetupShop();
	}
	CEngine::GetInstance()->AddScriptObject(this, CEngine::GetInstance()->GetCurSceneNumber());
	m_pShopHud = CShopHud::Create(nullptr);
	return S_OK;
}

void CNpc_Shop::Update(_double deltaTime)
{
	if (m_bisCreated) {
		_uint idx = 0;
		for (auto& iter : m_ShopList) {
			if (iter.first->IsHovered()) {
				if (CEngine::GetInstance()->IsMouseDown(0)) {
					m_iCurSelectedItem = idx;
				}
				else {
				}
			}
			idx++;
		}
		_uint checkIdx = 0;
		for (auto& iter : m_ShopList) {
			if (checkIdx == m_iCurSelectedItem)
				iter.first->SetIsSelected(true);
			else
				iter.first->SetIsSelected(false);
			checkIdx++;
		}
		if (CEngine::GetInstance()->Get_DIKDown(DIK_ESCAPE)) {
			g_AnotherMenu = false;
			m_pShopHud->SetActive(false);
			for (auto& iter : m_ShopList) {
				iter.first->SetActive(false);
			}
			CEventCheck::GetInstance()->isOpeningShop(false);
			m_iCurSelectedItem = -1;
		}

		_float distance;
		if (m_pNpcTransform) {
			distance = __super::CalculatePlayerDistance(m_pNpcTransform);
		}
		if (distance < m_fAlretMakeDistance) {
			if (CEngine::GetInstance()->Get_DIKDown(DIK_F)) {
				g_AnotherMenu = true;
				m_pShopHud->SetActive(true);
				for (auto& iter : m_ShopList) {
					iter.first->SetActive(true);
				}
				CEventCheck::GetInstance()->isOpeningShop(true);
				m_iCurSelectedItem = -1;
			}
		}
		


		if (m_pShopHud) {
			if (m_pShopHud->IsBuySelected()) {
				CEventCheck::GetInstance()->ContractShop(this);
			}
		}

	}

}

void CNpc_Shop::LateUpdate(_double deltaTime)
{
	if (m_bisCreated) {

	}
}

void CNpc_Shop::SetupShop()
{
	if (m_pShopNPC) {
		CShop*	shop =static_cast<CShop*>(m_pShopNPC->GetComponent("Com_Shop"));
		if (shop) {
			ITEMTYPE _type = shop->GetItemType();
			list<pair<string, ITEMINFO>> itemInfo = CEngine::GetInstance()->GetItemTypeList(_type);
			_float yPos = 120.f;
			for (auto& iter : itemInfo) {
				CEmptyUI*	shopItemUI = static_cast<CEmptyUI*>(CEngine::GetInstance()->SpawnPrefab("U_ShopItem"));
				shopItemUI->SetActive(false);
				m_ShopList.emplace_back(shopItemUI, iter.second);

				list<CGameObject*>	child = shopItemUI->GetChildren();
				list<CGameObject*>::iterator iter_Begin = child.begin();
				CText* itemName = static_cast<CText*>((*iter_Begin)->GetComponent("Com_Text"));
				iter_Begin++;
				CVIBuffer_RectUI* itemImage = static_cast<CVIBuffer_RectUI*>((*iter_Begin)->GetComponent("Com_VIBuffer"));
				iter_Begin++;
				CText* itemPrice = static_cast<CText*>((*iter_Begin)->GetComponent("Com_Text"));

				itemName->SetString(iter.first);
				string itemPath = itemImagePath + iter.second.imageName + ".dds";
				itemImage->UpdateTexture(itemPath, CVIBuffer_RectUI::TEXTURE_DIFFUSE);
				itemPrice->SetString(to_string(iter.second.price));

				_float2 pos = shopItemUI->GetPosition();
				shopItemUI->SetPosition(pos.x, yPos);
				yPos += 85.f;
			}
		}
	}

	m_bisCreated = true;
}

ITEMINFO CNpc_Shop::GetBuyingItem()
{
	if (m_iCurSelectedItem == -1) {
		return ITEMINFO();
	}
	return m_ShopList[m_iCurSelectedItem].second;
}

CNpc_Shop* CNpc_Shop::Create(CGameObject* pTartget)
{
	CNpc_Shop*	pObj = new CNpc_Shop();
	if (pObj->Initailze(pTartget)) {
		SafeRelease(pObj);
		return nullptr;
	}
	return pObj;
}

void CNpc_Shop::Free()
{

	m_ShopList.clear();
}

