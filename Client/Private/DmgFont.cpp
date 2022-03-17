#include "stdafx.h"
#include "..\Public\DmgFont.h"

BEGIN(Client)
void ThreadInit(CDmgFont*	script, _float dmg)
{
	CGameObject*	target = CEngine::GetInstance()->SpawnPrefab("DamageFont");
	script->SetTarget(target);
	CEngine::GetInstance()->AddScriptObject(script, CEngine::GetInstance()->GetCurSceneNumber());
	list<CGameObject*> child = target->GetChildren();
	int i = 0;
	auto iter = child.end();
	
	string strDmg = to_string((_int)dmg);

	for (auto& iter : child) {
		if (i < strDmg.length()) {
			string number;
			number.push_back(strDmg.at(i));	
			string dmgFontPath = "../../Assets/UITexture/Font/DmgF_Atk_" + number + ".dds";
			dynamic_cast<CVIBuffer_RectUI*>(iter->GetComponent("Com_VIBuffer"))->UpdateTexture(dmgFontPath, CVIBuffer_RectUI::TEXTURE_DIFFUSE);
			i++;
		}
	}
	script->SetisEnd();
}
END

USING(Client)

CDmgFont::CDmgFont()
{
}

HRESULT CDmgFont::Initailze(CGameObject * pArg, _float dmg)
{
	m_fDmg = dmg;
	std::thread loadDmgFont(ThreadInit, this, m_fDmg);
	loadDmgFont.detach();
	
	return S_OK;
}

void CDmgFont::Update(_double deltaTime)
{

	if (isEnd) {
		if (__super::IsDead() || pTarget->isDead() || pTarget == nullptr)
			return;
		pTransform = dynamic_cast<CRectTransform*>(pTarget->GetComponent("Com_Transform"));
		_float2 pos = pTransform->GetPosition();
		pos.y += _float(10.f * deltaTime);
		pTransform->SetPosition(pos.x, pos.y);
	}
	
}

void CDmgFont::LateUpdate(_double deltaTime)
{
	if (isEnd) {
		m_LifeDelta += deltaTime;
		if (m_LifeTime < m_LifeDelta) {
			pTarget->SetDead();
			__super::SetDead();
			m_LifeDelta = 0;
			return;
		}
	}
}

void CDmgFont::Render()
{
}

CDmgFont * CDmgFont::Create(CGameObject * pTarget, _float dmg)
{
	CDmgFont*	obj = new CDmgFont();
	if (FAILED(obj->Initailze(pTarget,dmg))) {
		SAFE_RELEASE(obj);
		return nullptr;
	}
	return obj;
}

void CDmgFont::Free()
{
}


