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
	if (m_pThisUI->IsActive())
	{
		if (m_pBuy) {
			if (m_pBuy->isFristEnter()) {
				CEngine::GetInstance()->StopSound(CHANNELID::UI07);
				CEngine::GetInstance()->PlaySoundW("ItemHover.mp3", CHANNELID::UI07);
			}
			if (m_pBuy->IsHovered()) {
				if (CEngine::GetInstance()->Get_MouseButtonStateDown(CInput_Device::MOUSEBUTTONSTATE::MBS_LBUTTON)) {
					return true;
				}
			}
		}
		if (m_pScrollBar) {

			if (m_pScrollBar->IsHovered()) {
				if (CEngine::GetInstance()->Get_MouseButtonState(CInput_Device::MOUSEBUTTONSTATE::MBS_LBUTTON)) {
					m_bIsScrolled = true;
				}
			}
			if (CEngine::GetInstance()->Get_MouseButtonStateUp(CInput_Device::MOUSEBUTTONSTATE::MBS_LBUTTON)) {
				m_bIsScrolled = false;
			}
			if (m_bIsScrolled) {
				POINT		ptMouse;
				GetCursorPos(&ptMouse);
				ScreenToClient(g_hWnd, &ptMouse);
				ptMouse.y = ptMouse.y - (1080 >> 1);
				_float2 scrollPos = m_pScrollBar->GetPosition();
				_float2 scrollSize = m_pScrollBar->GetUISize();
				_float2 scrollOffset = m_pScrollBar->GetTransformOffst();
				_float2	bgSize = m_pScrollBG->GetUISize();
				_float offset = bgSize.y * 0.5f;
				if (ptMouse.y > -offset + m_fcorrectionYSize && ptMouse.y < offset - m_fcorrectionYSize) {
					m_pScrollBar->SetTransformOffst(0.f, (_float)ptMouse.y);
					_float2 position = m_pShopList->GetPosition();
					_float moveY = 0.f;
					moveY = (scrollOffset.y - ptMouse.y) / m_yRatio;
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
	m_yRatio = (bgSize.y / _length);
	_float ySize = bgSize.y * m_yRatio;
	m_fShopLength = _length;
	if (m_yRatio <= 1.f) {
		m_fcorrectionYSize = ySize * 0.5f;
		m_pScrollBar->SetSize(8.f, ySize);
		m_pScrollBar->SetTransformOffst(0.f, -bgSize.y*0.5f + m_fcorrectionYSize);
	}
	else {
		m_pScrollBar->SetTransformOffst(0.f, 0.f);
		m_pScrollBar->SetSize(8.f, bgSize.y);

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

