#include "stdafx.h"
#include "..\Public\ComboHud.h"
USING(Client)

CComboHud::CComboHud()
{
}

HRESULT CComboHud::Initailze(CGameObject * pArg)
{
	if (pArg != nullptr) {
		pTarget = pArg;
	}
	CGameObject*	hud = CEngine::GetInstance()->FindGameObjectWithName(0, "ComboHud");
	list<CGameObject*> child = hud->GetChildren();
	auto& iter = child.begin();
	for (_int i = 0; i < COMBOHUDCHILDMAX; i++) {
		m_pChildTransform[i] =dynamic_cast<CRectTransform*>((*iter)->GetComponent("Com_Transform"));
		iter++;
	}
	for (int i = 0; i < COMBOHUDPOS::COMBOPOS_END; i++) {
		m_pIconList[i] = nullptr;
	}
	return S_OK;
}

void CComboHud::Update(_double deltaTime)
{

}

void CComboHud::LateUpdate(_double deltaTime)
{
}

void CComboHud::Render()
{
}

void CComboHud::CreateComboIcon(COMBOHUDPOS pos,_bool isLeftRight, _bool isEffect)
{
	switch (pos)
	{
	case Client::CComboHud::COMBOPOS_TOP:
		if (m_pIconList[pos]) {
			m_pIconList[pos]->SetDead();
		}
		m_pIconList[pos] = CComboIcon::Create(m_pChildTransform[0], m_pChildTransform[2], isLeftRight, isEffect);
		break;
	case Client::CComboHud::COMBOPOS_BOT:
		if (m_pIconList[pos]) {
			m_pIconList[pos]->SetDead();
		}
		m_pIconList[pos] = CComboIcon::Create(m_pChildTransform[1], m_pChildTransform[3], isLeftRight, isEffect);
		break;
	}
}

CComboHud * CComboHud::Create(CGameObject * pTarget)
{
	CComboHud*	obj = new CComboHud();
	if (FAILED(obj->Initailze(pTarget))) {
		SAFE_RELEASE(obj);
		return nullptr;
	}
	return obj;
}

void CComboHud::Free()
{
}

