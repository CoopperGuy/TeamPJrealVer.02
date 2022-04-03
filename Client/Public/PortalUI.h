#pragma once
#include "IScriptObject.h"
#include "Engine.h"
BEGIN(Client)

class CPortalUI :
	public IScriptObject
{
public:
	enum SCENES {STAGE01,STAGE02, STAGE03,SCENES_END};
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
	CEmptyUI*				m_pPosition = nullptr;
	vector<CEmptyUI*>		m_pScenes;
private:
	SCENES			m_eScenes = SCENES_END;
	_bool			m_bIsActive = false;
private:
	_float			m_fCurScnensPosX = 640.f;
	_float			m_fDestScnensPosX = 640.f;
	_float			m_fPreScnensPosX = 640.f;
	_int			m_iCurIndex = 0;
private:
	_double			m_moveDelta = 0;
	_double			m_moveTime = 0.3;
private:
	mutable	_int	m_iMaxIdx = 1;
	mutable	_int	m_iMinIdx = 0;

public:
	static CPortalUI* Create(CGameObject*	pTarget = nullptr);
	virtual void Free() override;
};

END