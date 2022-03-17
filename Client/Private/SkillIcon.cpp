#include "stdafx.h"
#include "..\Public\SkillIcon.h"

USING(Client)

CSkillIcon::CSkillIcon()
{
}

HRESULT CSkillIcon::Initailze(CGameObject * pArg)
{
	if (pArg != nullptr) {
		pTarget = pArg;
	}
	else {
		m_pThisUI = static_cast<CEmptyUI*>(CEngine::GetInstance()->FindGameObjectWithName(CEngine::GetInstance()->GetCurSceneNumber(), "SkillIcon"));
	}
	m_pVIBuffer = dynamic_cast<CVIBuffer_RectUI*>(m_pThisUI->GetComponent("Com_VIBuffer"));
	list<CGameObject*> child = m_pThisUI->GetChildren();
	for (auto& iter : child) {
		m_childVIBuffer.emplace_back(dynamic_cast<CVIBuffer_RectUI*>(iter->GetComponent("Com_VIBuffer")));
	}
	CEngine::GetInstance()->AddScriptObject(this, SCENE_STATIC);
	return S_OK;
}

void CSkillIcon::Update(_double deltaTime)
{
}

void CSkillIcon::LateUpdate(_double deltaTime)
{
	m_fDegree += 0.01f;
	if (m_fDegree >= XMConvertToRadians(360.f)) {
		m_fDegree = 0.f;
	}
	m_pThisUI->SetDegree(m_fDegree);

}

void CSkillIcon::Render()
{
}

CSkillIcon * CSkillIcon::Create(CGameObject * pTarget)
{
	CSkillIcon*	obj = new CSkillIcon();
	if (FAILED(obj->Initailze(pTarget))) {
		SAFE_RELEASE(obj);
		return nullptr;
	}
	return obj;
}

void CSkillIcon::Free()
{
}
