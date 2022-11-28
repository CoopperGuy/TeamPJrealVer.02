#include "stdafx.h"
#include "..\Public\ItemInfo.h"

USING(Client)

static string _itemImgPath = "../../Assets/UITexture/InvenItem/";
CItemInfo::CItemInfo()
{
}

HRESULT CItemInfo::Initailze(CGameObject * pArg)
{
	CEngine*	pEngine = CEngine::GetInstance();
	m_vecUIs.reserve(IINFO_END);
	m_pThisUI = static_cast<CEmptyUI*>(pEngine->FindGameObjectWithName(0, "ItemInfoHud"));
	list<CGameObject*>	child = m_pThisUI->GetChildren();
	for (auto& iter : child)
		m_vecUIs.emplace_back(static_cast<CEmptyUI*>(iter));
	m_pThisUI->SetActive(false);
	return S_OK;
}

void CItemInfo::Update(_double deltaTime)
{
}

void CItemInfo::LateUpdate(_double deltaTime)
{
}

void CItemInfo::Render()
{
}

void CItemInfo::SetActive(_bool _active)
{
	m_pThisUI->SetActive(_active);
}

void CItemInfo::SetItemInfo(ITEMINFO _info)
{
	_float reinforceRatio = 0.1f;
	m_tItemInfo = _info;
	_int _level = m_tItemInfo.level;
	_int _hp = m_tItemInfo.hp + (m_tItemInfo.hp * reinforceRatio) *_level;
	_int _atk = m_tItemInfo.atk + (m_tItemInfo.atk * reinforceRatio) *_level;
	_int _def = m_tItemInfo.def + (m_tItemInfo.def * reinforceRatio) *_level;

	m_strHP = "HP : " + to_string(_hp);
	m_strAtk = "ATK : " + to_string(_atk);
	m_strDef = "DEF : " + to_string(_def);
	m_strImagePath = _itemImgPath + m_tItemInfo.imageName + ".dds";
	static_cast<CText*>(m_vecUIs[IINFO_NAME]->GetComponent("Com_Text"))->SetString(m_tItemInfo.itemName);
	static_cast<CText*>(m_vecUIs[IINFO_HP]->GetComponent("Com_Text"))->SetString(m_strHP);
	static_cast<CText*>(m_vecUIs[IINFO_ATK]->GetComponent("Com_Text"))->SetString(m_strAtk);
	static_cast<CText*>(m_vecUIs[IINFO_DEF]->GetComponent("Com_Text"))->SetString(m_strDef);
	static_cast<CVIBuffer_RectUI*>(m_vecUIs[IINFO_IMAGE]->GetComponent("Com_VIBuffer"))->UpdateTexture(m_strImagePath, CVIBuffer_RectUI::TEXTURE_DIFFUSE);

}

void CItemInfo::SetYPosition(_float _y)
{
	_float2 _pos = m_pThisUI->GetPosition();
	m_pThisUI->SetPosition(_pos.x, _y);
}

CItemInfo * CItemInfo::Create(CGameObject * pTarget)
{
	CItemInfo*	obj = new CItemInfo();
	if (FAILED(obj->Initailze(pTarget))) 
	{
		SAFE_RELEASE(obj);
		return nullptr;
	}
	return obj;
}

void CItemInfo::Free()
{
}
