#include "EnginePCH.h"
#include "..\Public\DmgVIBuffer.h"
#include  "EmptyGameObject.h"
#include "Engine.h"
#include "VIBuffer_Rect.h"
#include "Transform.h"

BEGIN(Engine)

void ThreadDmgBuffer(CDmgVIBuffer*	script, _float3 pos, _float dmg) {
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
		static_cast<CEmptyGameObject*>(iter)->SetLocalPosition(_float3{ (_float)i,0,0 });
	}
	script->SetTransform(dynamic_cast<CTransform*>(thisUI->GetComponent("Com_Transform")));
	script->SetisEnd();
}
END

USING(Engine)

CDmgVIBuffer::CDmgVIBuffer()
{
}

HRESULT CDmgVIBuffer::Initailze(CGameObject * pArg, _float3 pos, _float dmg, _bool isCrit, _bool effect, _bool isPlayer)
{
	m_fDmg = dmg;
	m_IsCritical = isCrit;
	//std::thread loadDmgFont(ThreadDmgBuffer, this, pos,  m_fDmg);
	//loadDmgFont.detach();

	CGameObject* thisUI = CEngine::GetInstance()->AddGameObjectToPrefab(CEngine::GetInstance()->GetCurSceneNumber(), "Prototype_GameObject_DMGFont", "U_DamageVIBuffer");
	thisUI->SetisRender(false);
	static_cast<CEmptyGameObject*>(thisUI)->SetPosition(pos);
	this->SetThisUI(thisUI);
	CEngine::GetInstance()->AddScriptObject(this, CEngine::GetInstance()->GetCurSceneNumber());
	list<CGameObject*> child = thisUI->GetChildren();
	int i = 0;
	_int idx = 0;
	auto iter = child.end();

	string strDmg = to_string((_int)dmg);

	for (auto& iter : child) {
		if (i < strDmg.length()) {
			string number;
			number.push_back(strDmg.at(i));
			if (isPlayer) {
				if (isCrit) {
					string dmgFontPath = "../../Assets/UITexture/Font/DmgF_CriAtk_" + number + ".dds";
					static_cast<CVIBuffer_Rect*>(iter->GetComponent("Com_VIBuffer"))->UpdateTexture(dmgFontPath);
				}
				else {
					string dmgFontPath = "../../Assets/UITexture/Font/DmgF_Atk_" + number + ".dds";
					static_cast<CVIBuffer_Rect*>(iter->GetComponent("Com_VIBuffer"))->UpdateTexture(dmgFontPath);
				}
			}
			else {
				string dmgFontPath = "../../Assets/UITexture/Font/DmgF_Dmg_" + number + ".dds";
				static_cast<CVIBuffer_Rect*>(iter->GetComponent("Com_VIBuffer"))->UpdateTexture(dmgFontPath);
			}
			i++;
		}
		else {
			if (idx != 4) {
				string dmgFontPath = "../../Assets/UITexture/Font/nullImage.dds";
				static_cast<CVIBuffer_Rect*>(iter->GetComponent("Com_VIBuffer"))->UpdateTexture(dmgFontPath);
			}

		}
		static_cast<CEmptyGameObject*>(iter)->SetLocalPosition(_float3{ (_float)i,0,0 });
		if (idx == 4) {
			static_cast<CEmptyGameObject*>(iter)->SetLocalPosition(_float3{ (_float)i / 2.f + 0.5f,0,-0.1f });
			iter->SetActive(false);
		}
		m_listChild.emplace_back(static_cast<CEmptyGameObject*>(iter));
		idx++;
	}
	this->SetTransform(dynamic_cast<CTransform*>(thisUI->GetComponent("Com_Transform")));
	this->SetisEnd();

	return S_OK;
}

void CDmgVIBuffer::Update(_double deltaTime)
{
	if (isEnd) {
		if (__super::IsDead() || m_pThisUI == nullptr || m_pThisUI->isDead())
			return;


		_vector pos = m_pTransform->GetState(CTransform::STATE_POSITION);
		_float y = XMVectorGetY(pos) - 0.2f * (_float)deltaTime;
		m_pTransform->SetState(CTransform::STATE_POSITION, XMVectorSetY(pos, y));


		if (isStartDisable) {
			m_fAlpha -= 2.f * (_float)deltaTime;
			for (auto& iter : m_listChild)
				static_cast<CVIBuffer_Rect*>(iter->GetComponent("Com_VIBuffer"))->SetAlpha(m_fAlpha);
		}
		if (isShrink) {
			if (m_IsCritical) {
				m_startSize += 0.825f * (_float)deltaTime;
			}
			else {
				m_startSize += 0.5f * (_float)deltaTime;
			}
			m_fEffectSize += 0.5f * (_float)deltaTime;
		}
		else {
			if (m_IsCritical) {
				if (m_startSize > 0.1f)
					m_startSize -= 0.5f*(_float)deltaTime;
			}
			else {
				if (m_startSize > 0.065f)
					m_startSize -= 0.5f*(_float)deltaTime;
			}
			if (m_fEffectSize > 0.65)
				m_fEffectSize -= 0.5f * (_float)deltaTime;

		}
		m_pTransform->SetScale(_float3{ m_startSize ,m_startSize ,m_startSize });
		CTransform* local = static_cast<CTransform*>(m_listChild[4]->GetComponent("Com_LocalTransform"));
		local->SetScale(_float3{ m_fEffectSize ,m_fEffectSize ,m_fEffectSize });
	}
}

void CDmgVIBuffer::LateUpdate(_double deltaTime)
{
	if (isEnd) {
		m_LifeDelta += deltaTime;
		m_durationDelta += deltaTime;
		if (m_LifeTime < m_LifeDelta) {
			m_pThisUI->SetDead();
			__super::SetDead();
			return;
		}
		if (m_fAlpha < 0.f) {
			m_pThisUI->SetDead();
			__super::SetDead();
			return;
		}

		if (m_startSize > 0.125f) {
			isShrink = false;
			isStartDisable = true;
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

CDmgVIBuffer * CDmgVIBuffer::Create(CGameObject * pTarget, _float3 pos, _float dmg, _bool isCrit, _bool effect, _bool isPlayer)
{
	CDmgVIBuffer*	obj = new CDmgVIBuffer();
	if (FAILED(obj->Initailze(pTarget, pos, dmg, isCrit, effect, isPlayer))) {
		SAFE_RELEASE(obj);
		return nullptr;
	}
	return obj;
}

void CDmgVIBuffer::Free()
{
}

