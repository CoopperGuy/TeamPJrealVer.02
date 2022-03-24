#pragma once
#include "IScriptObject.h"
#include "Engine.h"

BEGIN(Client)

class CTownPortal :
	public IScriptObject
{
public:
	CTownPortal();
	virtual ~CTownPortal() = default;

public:
	virtual HRESULT Initailze(CGameObject * pArg);
	virtual void Update(_double deltaTime) override;
	virtual void LateUpdate(_double deltaTime) override;
public:
	SCENE	GetCurScene() { return m_eSceneNum; }
	_bool	GetChangeScene() { return m_bChangeScene; }
private:
	_float DistanceWithDirection();
private:
	CEmptyGameObject*	m_pPortal = nullptr;
	CEmptyGameObject*	m_pPlayer = nullptr;
	SCENE				m_eSceneNum = SCENE_END;
	_bool				m_bChangeScene = false;
public:
	static CTownPortal*	Create(CGameObject*	pTarget = nullptr);
	virtual void Free() override;
};

END