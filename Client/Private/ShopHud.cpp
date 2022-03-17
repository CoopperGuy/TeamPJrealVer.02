#include "stdafx.h"
#include "..\Public\ShopHud.h"

USING(Client)

CShopHud::CShopHud()
{
}

HRESULT CShopHud::Initailze(CGameObject * pArg)
{
	m_pThisUI = static_cast<CEmptyUI*>(CEngine::GetInstance()->FindGameObjectWithName(SCENE::SCENE_STATIC, "ShopHud"));
	if (m_pThisUI) {
		m_pBuy = static_cast<CEmptyUI*>(m_pThisUI->GetChildren().front());
	}
	return S_OK;
}

void CShopHud::Update(_double deltaTime)
{

}

void CShopHud::LateUpdate(_double deltaTime)
{
}

void CShopHud::Render()
{
}

_bool CShopHud::IsBuySelected()
{
	if (m_pBuy) {
		if (m_pBuy->IsHovered()) {
			if (CEngine::GetInstance()->IsMouseDown(0)) {
				return true;
			}
		}
	}
	return false;
}

void CShopHud::SetActive(_bool isActive)
{
	m_pThisUI->SetActive(isActive);
}



CShopHud * CShopHud::Create(CGameObject * pTarget)
{
	CShopHud*	pObj = new CShopHud();
	if (pObj->Initailze(pTarget)) {
		SafeRelease(pObj);
		return nullptr;
	}
	return pObj;
}

void CShopHud::Free()
{
}

