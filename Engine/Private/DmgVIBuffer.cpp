#include "EnginePCH.h"
#include "..\Public\DmgVIBuffer.h"
#include  "EmptyGameObject.h"
#include "Engine.h"
#include "VIBuffer_Rect.h"
#include "Transform.h"

BEGIN(Engine)

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

USING(Engine)

CDmgVIBuffer::CDmgVIBuffer()
{
}

HRESULT CDmgVIBuffer::Initailze(CGameObject * pArg, _float3 pos ,  _float dmg)
{
	m_fDmg = dmg;
	//std::thread loadDmgFont(ThreadDmgBuffer, this, pos,  m_fDmg);
	//loadDmgFont.detach();

	CGameObject* thisUI = CEngine::GetInstance()->AddGameObjectToPrefab(CEngine::GetInstance()->GetCurSceneNumber(), "Prototype_GameObject_DMGFont", "U_DamageVIBuffer");
	thisUI->SetisRender(false);
	static_cast<CEmptyGameObject*>(thisUI)->SetPosition(pos);
	this->SetThisUI(thisUI);
	CEngine::GetInstance()->AddScriptObject(this, CEngine::GetInstance()->GetCurSceneNumber());
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
		static_cast<CEmptyGameObject*>(iter)->SetLocalPosition(_float3{ (_float)i,0,0 });
	}
	this->SetTransform(dynamic_cast<CTransform*>(thisUI->GetComponent("Com_Transform")));
	this->SetisEnd();

	return S_OK;
}

void CDmgVIBuffer::Update(_double deltaTime)
{
	if (isEnd) {
		if (__super::IsDead() || m_pThisUI == nullptr || m_pThisUI->isDead() )
			return;
		_vector pos = m_pTransform->GetState(CTransform::STATE_POSITION);
		_float y = XMVectorGetY(pos) - 0.2f * deltaTime;
		m_pTransform->SetState(CTransform::STATE_POSITION, XMVectorSetY(pos, y));
	}
}

void CDmgVIBuffer::LateUpdate(_double deltaTime)
{
	if (isEnd) {
		m_LifeDelta += deltaTime;
		if (m_LifeTime < m_LifeDelta) {
			m_pThisUI->SetDead();
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

