#include "stdafx.h"
#include "..\Public\SkillIcon.h"

USING(Client)

static string iconPath = "../../Assets/UITexture/skill/";

CSkillIcon::CSkillIcon()
{
}

HRESULT CSkillIcon::Initailze(CGameObject * pArg)
{
	if (pArg != nullptr)
		m_pPlayer = static_cast<CEmptyGameObject*>(pArg);
	CEngine* engine = CEngine::GetInstance();
	for (_int i = 0; i < sizeOfUI; i++) {
		string strName = "SkillIcon0" + to_string(i);
		CEmptyUI* UI = static_cast<CEmptyUI*>(engine->FindGameObjectWithName(SCENE_STATIC, strName));
		m_pUIList.emplace_back(UI);
		CVIBuffer_RectUI* viBuffer = static_cast<CVIBuffer_RectUI*>(UI->GetComponent("Com_VIBuffer"));
		m_pVIBufferList.emplace_back(viBuffer);
		list<CGameObject*> child = UI->GetChildren();
		for(auto& iter : child)
			m_child[i].emplace_back(static_cast<CEmptyUI*>(iter));

	}
	CEngine::GetInstance()->AddScriptObject(this, SCENE_STATIC);
	return S_OK;
}

void CSkillIcon::Update(_double deltaTime)
{
	for (_int i = 0; i < sizeOfUI; i++) {
		m_tInfo[i].coolDelta += (_float)deltaTime;
		if (m_tInfo[i].coolDelta > m_tInfo[i].coolTime)
			m_tInfo[i].coolDelta = m_tInfo[i].coolTime;

	}
	if (m_pPlayer) {
		if (m_tInfo[2].coolDelta < m_tInfo[2].skillDuration)
			static_cast<CStat*>(m_pPlayer->GetComponent("Com_Stat"))->SetStatusEffect(CStat::STATUSEFFECT::BLOOD);
		else 
			static_cast<CStat*>(m_pPlayer->GetComponent("Com_Stat"))->ShutStatusEffect(CStat::STATUSEFFECT::BLOOD);
	}
}

void CSkillIcon::LateUpdate(_double deltaTime)
{
	//m_fDegree += 0.01f;
	//if (m_fDegree >= XMConvertToRadians(360.f)) {
	//	m_fDegree = 0.f;
	//}
	//m_pThisUI->SetDegree(m_fDegree);
	for (_int i = 0; i < sizeOfUI; i++) {
		_float percentage = m_tInfo[i].coolDelta / m_tInfo[i].coolTime * 360.f;
		m_child[i][SKILL_BORDER]->SetDegree(XMConvertToRadians(percentage));
	}
}

void CSkillIcon::Render()
{
}

void CSkillIcon::SetSkillInfo(_int idx, SKILLINFO _info)
{
	m_tInfo[idx] = _info;
	string path = iconPath + m_tInfo[idx].Name + ".dds";
	static_cast<CVIBuffer_RectUI*>(m_child[idx][SKILL_IMAGE]->GetComponent("Com_VIBuffer"))->UpdateTexture(path,CVIBuffer_RectUI::TEXTURE_DIFFUSE);
}

_bool CSkillIcon::IsCoolDown(_int idx)
{
	if (m_tInfo[idx].coolTime > m_tInfo[idx].coolDelta)
		return false;
	return true;
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
