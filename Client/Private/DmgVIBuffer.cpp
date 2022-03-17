#include "stdafx.h"
#include "..\Public\DmgVIBuffer.h"

BEGIN(Client)
void ThreadDmgBuffer(CDmgVIBuffer*	script, _float3 pos , _float dmg ) {
	CGameObject*	thisUI = CEngine::GetInstance()->SpawnPrefab("U_DamageVIBuffer");
	thisUI->SetisRender(false);
	static_cast<CEmptyGameObject*>(thisUI)->SetPosition(pos);
	script->SetThisUI(thisUI);
	CEngine::GetInstance()->AddScriptObject(script, CEngine::GetInstance()->GetCurSceneNumber());
	list<CGameObject*> child = thisUI->GetChildren();
	int i = 0;
	auto iter = child.end();

	string strDmg = to_string((_int)dmg);

	for (auto& iter : child) {
		if (i < strDmg.length()) {
			string number;
			number.push_back(strDmg.at(i));
			string dmgFontPath = "../../Assets/UITexture/Font/DmgF_Atk_" + number + ".dds";
			static_cast<CVIBuffer_Rect*>(iter->GetComponent("Com_VIBuffer"))->UpdateTexture(dmgFontPath);
			i++;
		}
		else {
			string dmgFontPath = "../../Assets/UITexture/Font/nullImage.dds";
			static_cast<CVIBuffer_Rect*>(iter->GetComponent("Com_VIBuffer"))->UpdateTexture(dmgFontPath);

		}
		static_cast<CEmptyGameObject*>(iter)->SetLocalPosition(_float3{(_float)i,0,0});
	}
	script->SetTransform(dynamic_cast<CTransform*>(thisUI->GetComponent("Com_Transform")));
	script->SetisEnd();
}
END

USING(Client)

CDmgVIBuffer::CDmgVIBuffer()
{
}

HRESULT CDmgVIBuffer::Initailze(CGameObject * pArg, _float3 pos ,  _float dmg)
{
	m_fDmg = dmg;
	std::thread loadDmgFont(ThreadDmgBuffer, this, pos,  m_fDmg);
	loadDmgFont.detach();

	return S_OK;
}

void CDmgVIBuffer::Update(_double deltaTime)
{
	if (isEnd) {
		if (__super::IsDead() || m_pThisUI == nullptr || m_pThisUI->isDead() )
			return;
	}
}

void CDmgVIBuffer::LateUpdate(_double deltaTime)
{
	if (isEnd) {
		m_LifeDelta += deltaTime;
		if (m_LifeTime < m_LifeDelta) {
			m_pThisUI->DeleteRealObject();
			__super::SetDead();
			m_LifeDelta = 0;
			return;
		}
		m_pThisUI->SetisRender(true);
	}
	else {
		m_pThisUI->SetisRender(false);
	}
}

void CDmgVIBuffer::Render()
{
}

CDmgVIBuffer * CDmgVIBuffer::Create(CGameObject * pTarget, _float3 pos , _float dmg)
{
	CDmgVIBuffer*	obj = new CDmgVIBuffer();
	if (FAILED(obj->Initailze(pTarget,pos, dmg))) {
		SAFE_RELEASE(obj);
		return nullptr;
	}
	return obj;
}

void CDmgVIBuffer::Free()
{
}

