#pragma once

#include "Engine.h"
#include "VIBuffer_RectUI.h"
#include "Transform.h"
#include "IScriptObject.h"

BEGIN(Client)

class CDmgFont :
	public IScriptObject
{
private:
	explicit CDmgFont();
	virtual ~CDmgFont() = default;
public:
	virtual HRESULT Initailze(CGameObject * pArg, _float dmg);
	virtual void Update(_double deltaTime) override;
	virtual void LateUpdate(_double deltaTime) override;
	virtual void Render();
public:
	void SetTarget(CGameObject* pTarget) { this->pTarget = pTarget;  }
	void SetisEnd() { this->isEnd = true; }
private:
	CGameObject*	pTarget = nullptr;
	CRectTransform*	pTransform = nullptr;
	_double m_LifeTime = 1.f;
	_double m_LifeDelta = 0.f;
	_bool isEnd = false;
	_float	m_fDmg = 0.f;
public:
	static CDmgFont* Create(CGameObject*	pTarget = nullptr, _float dmg = 0.f);
	virtual void Free() override;
};

END