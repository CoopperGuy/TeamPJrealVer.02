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
		m_pScrollBar = static_cast<CEmptyUI*>(CEngine::GetInstance()->FindGameObjectWithName(SCENE::SCENE_STATIC, "ShopScrollBar"));
		m_pScrollBG = static_cast<CEmptyUI*>(CEngine::GetInstance()->FindGameObjectWithName(SCENE::SCENE_STATIC, "ShopScrollBG"));
		m_pScrollBar->SetTransformOffst(0.f, 0.f);
		m_pShopList = static_cast<CEmptyUI*>(CEngine::GetInstance()->FindGameObjectWithName(SCENE::SCENE_STATIC, "ShopList"));
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
	if (m_pScrollBar) {
		_float2 bgSize = m_pScrollBG->GetUISize();
		_float ySize = m_fShopLength - bgSize.y;
		_float correctionYSize = bgSize.y * 0.5f;
		if (ySize >= 0.f) {
			correctionYSize = ySize * 0.5f;
			m_pScrollBar->SetSize(8.f, ySize);
		}
		else {
			m_pScrollBar->SetSize(8.f, 500.f);
		}
		if (m_pScrollBar->IsHovered()) {
			if (CEngine::GetInstance()->Get_MouseButtonState(CInput_Device::MOUSEBUTTONSTATE::MBS_LBUTTON)) {
				POINT		ptMouse;
				GetCursorPos(&ptMouse);
				ScreenToClient(g_hWnd, &ptMouse);
				ptMouse.y = ptMouse.y - (WINCY >> 1);
				_float2 scrollPos = m_pScrollBar->GetPosition();
				_float2 scrollSize = m_pScrollBar->GetUISize();
				_float2 scrollOffset = m_pScrollBar->GetTransformOffst();
				if (ptMouse.y > -256.f + correctionYSize && ptMouse.y < 256.f - correctionYSize) {
					m_pScrollBar->SetTransformOffst(0.f, (_float)ptMouse.y);
					_float2 position = m_pShopList->GetPosition();
					_float moveY = 0.f;
					moveY = scrollOffset.y - ptMouse.y;
					m_pShopList->SetPosition(position.x, position.y + moveY);

				}
			}
		}
	}
	return false;
}

void CShopHud::SetActive(_bool isActive)
{
	m_pThisUI->SetActive(isActive);
}

void CShopHud::SetShopLength(_float _length)
{
	m_fShopLength = _length;
}

void CShopHud::SetOffset(_float _length)
{
	_float2 bgSize = m_pScrollBG->GetUISize();
	_float ySize = _length - bgSize.y;
	_float correctionYSize = bgSize.y * 0.5f;
	if (ySize >= 0.f) {
		correctionYSize = ySize * 0.5f;
		m_pScrollBar->SetSize(8.f, ySize);
		m_pScrollBar->SetTransformOffst(0.f, -256.f + correctionYSize);
	}
	else {
		m_pScrollBar->SetTransformOffst(0.f, 0.f);
		m_pScrollBar->SetSize(8.f, 512.f);

	}
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

