#pragma once
#include "IScriptObject.h"

BEGIN(Engine)
class CGameObject;
class CTransform;
class CEmptyGameObject;
class CDmgVIBuffer :
	public IScriptObject
{
private:
	explicit CDmgVIBuffer();
	virtual ~CDmgVIBuffer() = default;
public:
	virtual HRESULT Initailze(CGameObject * pArg, _float3 pos, _float dmg, _bool isCrit = false, _bool effect = false, _bool isPlayer = true);
	virtual void Update(_double deltaTime) override;
	virtual void LateUpdate(_double deltaTime) override;
	virtual void Render();
public:
	void SetThisUI(CGameObject* pThis) { this->m_pThisUI = pThis; }
	void SetisEnd() { this->isEnd = true; }
	void SetTransform(CTransform* trans) { m_pTransform = trans; }
private:
	CGameObject*	pTarget = nullptr;
	CGameObject*	m_pThisUI = nullptr;
	CTransform*		m_pTransform = nullptr;
private:
	vector<CEmptyGameObject*> m_listChild;
private:
	_double			m_LifeTime = 0.5f;
	_double			m_LifeDelta = 0.f;
	_double			m_duration = 0.1f;
	_double			m_durationDelta = 0.f;
private:
	_float			m_fDmg = 0.f;
	_float			m_startSize = 0.1f;
	_float			m_fAlpha = 1.f;
	_float			m_fEffectSize = 1.f;
private:
	_bool isEnd = false;
	_bool isShrink = true;
	_bool isStartDisable = false;
	_bool m_IsCritical = false;
public:
	static CDmgVIBuffer* Create(CGameObject*	pTarget = nullptr, _float3 pos = { 0.f,0.f,0.f }, _float dmg = 0.f, _bool isCrit = false, _bool effect = false, _bool isPlayer = true);
	virtual void Free() override;
};

END