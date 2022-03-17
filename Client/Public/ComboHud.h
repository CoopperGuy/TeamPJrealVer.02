#pragma once

#include "Engine.h"
#include "VIBuffer.h"
#include "RectTransform.h"
#include "IScriptObject.h"
#include "ComboIcon.h"
BEGIN(Client)

#define COMBOHUDCHILDMAX 4

class CComboHud :
	public IScriptObject
{
public:
	enum COMBOHUDPOS{COMBOPOS_TOP,COMBOPOS_BOT,COMBOPOS_END};
private:
	explicit CComboHud();
	virtual ~CComboHud() = default;
public:
	virtual HRESULT Initailze(CGameObject * pArg);
	virtual void Update(_double deltaTime) override;
	virtual void LateUpdate(_double deltaTime) override;
	virtual void Render();
public:
	void CreateComboIcon(COMBOHUDPOS pos,_bool isLeftRight, _bool isEffect = false);
public:
	CRectTransform*	GetChildTransform(_uint idx) { if(idx < COMBOHUDCHILDMAX) return m_pChildTransform[idx]; }
private:
	CGameObject*	pTarget = nullptr;
	CRectTransform*	m_pChildTransform[COMBOHUDCHILDMAX] = {nullptr};
#ifdef _DEBUG
	_double m_testMakeIconDelta = 0.;
	_double m_testMakeIconTime = 1.5;
#endif
	CComboIcon*		m_pIconList[COMBOPOS_END] = { nullptr };
	_bool			m_bPosition = true;
public:
	static CComboHud* Create(CGameObject*	pTarget = nullptr);
	virtual void Free() override;
};

END