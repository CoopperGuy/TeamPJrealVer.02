#pragma once
#include "IScriptObject.h"
#include "Engine.h"
BEGIN(Client)

class CSoundHud :
	public IScriptObject
{
private:
	enum SOUNDS{SOUNDS_BGMSCROLL, SOUNDS_BGMBLOCK, SOUNDS_END};
private:
	explicit CSoundHud();
	virtual ~CSoundHud() = default;
public:
	virtual HRESULT Initailze(CGameObject * pArg);
	virtual void Update(_double deltaTime) override;
	virtual void LateUpdate(_double deltaTime) override;
	virtual void Render();
private:
	CEmptyUI*	m_pThisUI = nullptr;
private:
	vector<CEmptyUI*>	m_vecUIs;
private:
	pair<_float, _float>	m_Positions;
private:
	_bool	m_bIsDraged = false;
public:
	static CSoundHud* Create(CGameObject*	pTarget = nullptr);
	virtual void Free() override;
};

END