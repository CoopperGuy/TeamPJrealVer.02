#include "stdafx.h"
#include "..\Public\MonHp.h"

static string numFontPath = "../../Assets/UITexture/Font/DmgF_Atk_";

BEGIN(Client)
void ThreadMonHpBar(CMonHp* script) {
	CGameObject*	monHud = CEngine::GetInstance()->SpawnPrefab("U_MonsterHud");
	CEngine::GetInstance()->AddScriptObject(script,CEngine::GetInstance()->GetCurSceneNumber());
	script->SetThisUI(static_cast<CEmptyUI*>(monHud));

	list<CGameObject*> child = monHud->GetChildren();
	auto& iter = child.begin();
	CEmptyUI*	hpBar = static_cast<CEmptyUI*>(*iter);
	(script)->pushHuds(hpBar);
	script->pushBuffer(static_cast<CVIBuffer_RectUI*>(hpBar->GetComponent("Com_VIBuffer")));
	iter++;

	CEmptyUI*	levelFlag = static_cast<CEmptyUI*>(*iter);
	(script)->pushHuds(levelFlag);
	script->pushBuffer(static_cast<CVIBuffer_RectUI*>(levelFlag->GetComponent("Com_VIBuffer")));
	iter++;

	CEmptyUI*	monName = static_cast<CEmptyUI*>(*iter);
	(script)->pushHuds(monName);

	script->SetIsEnd();
	//get childern
}
END

USING(Client)

CMonHp::CMonHp()
{
}

HRESULT CMonHp::Initailze(CGameObject * pArg)
{
	if (pArg != nullptr) {
		m_pTarget = pArg;
		m_pTargetStat = static_cast<CStat*>(m_pTarget->GetComponent("Com_Stat"));

	}
	
	std::thread loadHpBar(ThreadMonHpBar, this);
	loadHpBar.detach();

	return S_OK;
}

void CMonHp::Update(_double deltaTime)
{
	////use pTarget hp
	if (m_bisEnd) {
		if (m_pTargetStat) {
			m_fPercetage = m_pTargetStat->GetHpPercentage();
			if (m_fPercetage < m_fBackPercentage) {
				m_fBackPercentage -= _float(0.05f*deltaTime);
			}
			m_pHuds[MONHUD_HP]->SetPercentage(m_fPercetage);
			m_pHuds[MONHUD_HP]->SetBackPercetage(m_fBackPercentage);
			_int level = m_pTargetStat->GetStatInfo().level;
		/*	if (level > 9) {

			}
			else {*/
				string levelPath = numFontPath + to_string(1) + ".dds";
				m_pHudBuffer[MONHUD_LEVEL]->UpdateTexture(levelPath, CVIBuffer_RectUI::TEXTURE_DIFFUSE);
			//}
			static_cast<CText*>(m_pHuds[MONHUD_NAME]->GetComponent("Com_Text"))->SetString(m_pTarget->GetName());
		}
	}
	////have to change color
}

void CMonHp::LateUpdate(_double deltaTime)
{
}

void CMonHp::Render()
{
}

void CMonHp::SetRelease()
{
	this->SetDead();
	/*if (m_pTarget)
		m_pTarget->SetDead();*/
	if(m_pThisUI)
		m_pThisUI->SetDead();
}

CMonHp * CMonHp::Create(CGameObject * pTarget)
{
	CMonHp*	obj = new CMonHp();
	if (FAILED(obj->Initailze(pTarget))) {
		SAFE_RELEASE(obj);
		return nullptr;
	}
	return obj;
}

void CMonHp::Free()
{
}

