#include "stdafx.h"
#include "..\Public\ReinforceHud.h"
#include "Item.h"
USING(Client)
static string itemPath = "../../Assets/UITexture/Item/";
CReinforceHud::CReinforceHud()
{
}

HRESULT CReinforceHud::Initailze(CGameObject * pArg)
{
	CEngine*	pEngine = CEngine::GetInstance();
	m_vecReinforceUIs.reserve(REINFORCE_END);
	m_pThisUI = static_cast<CEmptyUI*>(pEngine->FindGameObjectWithName(0, "ReinforceUI"));
	list<CGameObject*> child = m_pThisUI->GetChildren();
	for (auto& iter : child) {
		m_vecReinforceUIs.emplace_back(static_cast<CEmptyUI*>(iter));
	}
	return S_OK;
}

void CReinforceHud::Update(_double deltaTime)
{
	/*SetUpReinforceItem();
	SetUpREinforceMaterial();*/
	m_pThisUI->SetActive(m_bIsOnOff);
	if (m_pThisUI->IsActive()) {
		if (m_vecReinforceUIs[REINFORCE_BUTTON]->IsHovered())
		{
			if (CEngine::GetInstance()->Get_MouseButtonStateDown(CInput_Device::MOUSEBUTTONSTATE::MBS_LBUTTON))
			{
				if (!m_bIsReinforceWhile)
				{
					m_bIsReinforceWhile = true;
				}
			}
		}
		if (m_bIsReinforceWhile)
			StartReinforce((_float)deltaTime);
		UpdateImage();
	
	}
	else {
		string _itemName = itemPath + "nullImage.dds";
		if (m_pCurReinforceItem) {
			CVIBuffer_RectUI* _itemImg = static_cast<CVIBuffer_RectUI*>(m_vecReinforceUIs[REINFORCE_ITEMIMG]->GetComponent("Com_VIBuffer"));
			_itemImg->UpdateTexture(_itemName, CVIBuffer_RectUI::TEXTURE_DIFFUSE);
			m_pCurReinforceItem = nullptr;
		}
		if (m_pCurMaterial) {
			CVIBuffer_RectUI* _itemImg = static_cast<CVIBuffer_RectUI*>(m_vecReinforceUIs[REINFORCE_MATEIMG]->GetComponent("Com_VIBuffer"));
			_itemImg->UpdateTexture(_itemName, CVIBuffer_RectUI::TEXTURE_DIFFUSE);
			m_pCurMaterial = nullptr;
		}
		m_bIsReinforceWhile = false;
	}
}

void CReinforceHud::LateUpdate(_double deltaTime)
{
	_float percentage = m_fReinforceDelta / m_fReinforceTime * 360.f;
	m_vecReinforceUIs[REINFORCE_EFFECT]->SetDegree(XMConvertToRadians(percentage));
	if (m_pThisUI->IsActive()) {
		if (m_vecReinforceUIs[REINFORCE_CLOSE]->IsHovered())
		{
			if (CEngine::GetInstance()->Get_MouseButtonStateDown(CInput_Device::MOUSEBUTTONSTATE::MBS_LBUTTON))
			{
				m_bIsOnOff = false;
			}
		}
	}
}

void CReinforceHud::Render()
{
}

void CReinforceHud::UpdateImage()
{
	if (m_pCurReinforceItem) {
		CVIBuffer_RectUI* _itemImg = static_cast<CVIBuffer_RectUI*>(m_vecReinforceUIs[REINFORCE_ITEMIMG]->GetComponent("Com_VIBuffer"));
		string _itemName = itemPath + m_pCurReinforceItem->GetItempInfo().imageName + ".dds";
		_itemImg->UpdateTexture(_itemName, CVIBuffer_RectUI::TEXTURE_DIFFUSE);
	}
	else {
		CVIBuffer_RectUI* _itemImg = static_cast<CVIBuffer_RectUI*>(m_vecReinforceUIs[REINFORCE_ITEMIMG]->GetComponent("Com_VIBuffer"));
		string _itemName = itemPath  + "nullImage.dds";
		_itemImg->UpdateTexture(_itemName, CVIBuffer_RectUI::TEXTURE_DIFFUSE);

	}
	if (m_pCurMaterial) {
		CVIBuffer_RectUI* _itemImg = static_cast<CVIBuffer_RectUI*>(m_vecReinforceUIs[REINFORCE_MATEIMG]->GetComponent("Com_VIBuffer"));
		string _itemName = itemPath + m_pCurMaterial->GetItempInfo().imageName + ".dds";
		_itemImg->UpdateTexture(_itemName, CVIBuffer_RectUI::TEXTURE_DIFFUSE);
	}
	else {
		CVIBuffer_RectUI* _itemImg = static_cast<CVIBuffer_RectUI*>(m_vecReinforceUIs[REINFORCE_MATEIMG]->GetComponent("Com_VIBuffer"));
		string _itemName = itemPath + "nullImage.dds";
		_itemImg->UpdateTexture(_itemName, CVIBuffer_RectUI::TEXTURE_DIFFUSE);

	}
}

void CReinforceHud::SetUpReinforceItem(CItem*	_item)
{
	if (_item)
		m_pCurReinforceItem = _item;
	if (m_pCurReinforceItem) {
		CVIBuffer_RectUI* _itemImg = static_cast<CVIBuffer_RectUI*>(m_vecReinforceUIs[REINFORCE_ITEMIMG]->GetComponent("Com_VIBuffer"));
		string _itemName = itemPath + m_pCurReinforceItem->GetItempInfo().imageName +".dds";
		_itemImg->UpdateTexture(_itemName, CVIBuffer_RectUI::TEXTURE_DIFFUSE);
	}
}

void CReinforceHud::SetUpReinforceMaterial(CItem*	_item)
{
	if (_item)
		m_pCurMaterial = _item;
	if (m_pCurMaterial) {
		CVIBuffer_RectUI* _itemImg = static_cast<CVIBuffer_RectUI*>(m_vecReinforceUIs[REINFORCE_MATEIMG]->GetComponent("Com_VIBuffer"));
		string _itemName = itemPath + m_pCurMaterial->GetItempInfo().imageName + ".dds";
		_itemImg->UpdateTexture(_itemName, CVIBuffer_RectUI::TEXTURE_DIFFUSE);
	}
}

void CReinforceHud::StartReinforce(_float _deltaTime)
{
	if (m_pCurReinforceItem && m_pCurMaterial) {
		if (m_pCurMaterial->GetItempInfo().numOfItem > 0) {
			m_fReinforceDelta += _deltaTime;
			if (m_fReinforceTime < m_fReinforceDelta) {
				m_pCurMaterial->UseItem();
				m_pCurReinforceItem->ItemLevelUp();
				m_bIsReinforceWhile = false;
				m_fReinforceDelta = 0.f;
			}
		}
		else 
			m_bIsReinforceWhile = false;
	}else
		m_bIsReinforceWhile = false;
}

CReinforceHud * CReinforceHud::Create(CGameObject * pTarget)
{
	CReinforceHud*	obj = new CReinforceHud();
	if (FAILED(obj->Initailze(pTarget))) {
		SAFE_RELEASE(obj);
		return nullptr;
	}
	return obj;
}

void CReinforceHud::Free()
{
}

