#pragma once
#include "IScriptObject.h"
#include "Engine.h"
BEGIN(Client)

class CPortalUI :
	public IScriptObject
{
public:
	enum SCENES {STAGE01,STAGE02,SCENES_END};
private:
	explicit CPortalUI();
	virtual ~CPortalUI() = default;
public:
	virtual HRESULT Initailze(CGameObject * pArg);
	virtual void Update(_double deltaTime) override;
	virtual void LateUpdate(_double deltaTime) override;
	virtual void Render();
public:
	void	SetIsActive(_bool _active) { m_bIsActive = _active; }
	void	SetSCENES(SCENES _scene) { m_eScenes = _scene; }
public:
	void	ClosePortalUI();
public:
	_bool	GetIsActive() { return m_bIsActive; }
	SCENES	GetSCENES() { return m_eScenes; }
private:
	CEmptyUI*		m_pProtalUI = nullptr;
	CEmptyUI*		m_pLeftArrow = nullptr;
	CEmptyUI*		m_pRightArrow = nullptr;
	CEmptyUI*		m_pEscape = nullptr;

private:
	CEmptyUI*		m_pPosition = nullptr;
	CEmptyUI*		m_pScenes = nullptr;
private:
	SCENES			m_eScenes = SCENES_END;
	_bool			m_bIsActive = false;
public:
	static CPortalUI* Create(CGameObject*	pTarget = nullptr);
	virtual void Free() override;
};

END