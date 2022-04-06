#include "stdafx.h"
#include "..\Public\ItemDropAlret.h"
#include "RectTransform.h"
USING(Client)

CItemDropAlret::CItemDropAlret()
{
}

HRESULT CItemDropAlret::Initailze(string _itemName,CGameObject * pArg)
{
	CEngine*	pEngine = CEngine::GetInstance();

	m_pThisUI = static_cast<CEmptyUI*>(CEngine::GetInstance()->SpawnPrefab("U_ItemDropInfo"));
	list<CGameObject*> _child = m_pThisUI->GetChildren();
	m_pText = static_cast<CText*>(_child.front()->GetComponent("Com_Text"));
	string _tempName = _itemName + " x 1";
	m_pText->SetString(_tempName);
	m_pTransform = static_cast<CRectTransform*>(m_pThisUI->GetComponent("Com_Transform"));

	return S_OK;
}

void CItemDropAlret::Update(_double deltaTime)
{
	CRectTransform::RECTTRANSFORMDESC _desc =  m_pTransform->GetTransformDesc();
	_desc.posY += (_float)deltaTime * 30.f;
	m_pTransform->SetTransformMat(_desc);
}

void CItemDropAlret::LateUpdate(_double deltaTime)
{
	m_LifeDelta += deltaTime;
	if (m_LifeDelta > m_LifeTime)
	{
		this->SetDead();
		m_pThisUI->SetDead();
	}
}

void CItemDropAlret::Render()
{
}

CItemDropAlret * CItemDropAlret::Create(string _itemName , CGameObject * pTarget)
{
	CItemDropAlret*	obj = new CItemDropAlret();
	if (FAILED(obj->Initailze(_itemName,pTarget))) {
		SAFE_RELEASE(obj);
		return nullptr;
	}
	return obj;
}

void CItemDropAlret::Free()
{
}

