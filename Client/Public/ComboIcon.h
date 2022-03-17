#pragma once

#include "Engine.h"
#include "EmptyUI.h"
#include "VIBuffer.h"
#include "VIBuffer_RectUI.h"
#include "RectTransform.h"
#include "IScriptObject.h"

BEGIN(Client)

class CComboIcon :
	public IScriptObject
{
public:
	enum COMBOHUD {
		COMBOHUD_ALRET, COMBOHUD_MOUSE, COMBOHUD_EFFECT, COMBOHUD_END
	};
private:
	explicit CComboIcon();
	virtual ~CComboIcon() = default;
public:
	virtual HRESULT Initailze(CRectTransform*	pSrc, CRectTransform*	pDst, _bool leftRight, _bool isEffect = false);
	virtual void Update(_double deltaTime) override;
	virtual void LateUpdate(_double deltaTime) override;
	virtual void Render();
public:
	void SetIsCreated() { isCreated = true; }
	void SetDead() { m_isDestroy = true; }
	void SetVIBuffer(CVIBuffer_RectUI* buf, _uint idx) { m_pVIBuffer[idx] = buf; }
	void SetUI(CEmptyUI* buf, _uint idx) { m_pEmptyUI[idx] = buf; }

private:
	CRectTransform*		m_pTransform = nullptr;
	CGameObject*		m_thisUI = nullptr;
	CVIBuffer_RectUI*	m_pVIBuffer[COMBOHUD_END];
	CEmptyUI*		m_pEmptyUI[COMBOHUD_END];

private:
	_float2	m_fDst{};
	_float2	m_fSrc{};
	_double m_lerpDelta = 0;
	_double m_lifeDelta = 0;
	_double m_lifeTime = 0.25;
	_bool	isCreated = false;
	_bool	m_isGoing = true;
	_bool	m_isDestroy = false;
	_bool	m_isEffect = false;
public:
	static CComboIcon* Create(CRectTransform*	pSrc, CRectTransform*	pDst, _bool leftRight, _bool isEffect = false);
	virtual void Free() override;
};

END