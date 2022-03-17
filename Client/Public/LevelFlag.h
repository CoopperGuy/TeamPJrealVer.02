#pragma once

#include "Engine.h"
#include "VIBuffer_RectUI.h"
#include "Transform.h"
#include "IScriptObject.h"

BEGIN(Client)

class LevelFlag final :
	public IScriptObject
{
private:
	explicit LevelFlag();
	virtual ~LevelFlag() = default;
public:
	virtual HRESULT Initailze(CGameObject * pArg);
	virtual void Update(_double deltaTime) override;
	virtual void LateUpdate(_double deltaTime) override;
	virtual void Render();
private:
	CGameObject*		pTarget = nullptr;
	CStat*				m_pTargetStat = nullptr;
	CVIBuffer_RectUI*	m_pVIBuffer = nullptr;
	CVIBuffer_RectUI*	m_pVIBuffer20 = nullptr;
	CVIBuffer_RectUI*	m_pVIBuffer21 = nullptr;

	CGameObject*	levelFlag = nullptr;
	CGameObject*	levelFlag20 = nullptr;
	CGameObject*	levelFlag21 = nullptr;
private:
	_int	m_nextLevel = 1;
	_int	m_curLevel = -1;
	string levelFont;

public:
	static LevelFlag* Create(CGameObject*	pTarget = nullptr);
	virtual void Free() override;
};

END