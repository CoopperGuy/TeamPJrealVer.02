#pragma once
#include "IScriptObject.h"
#include "Engine.h"
BEGIN(Client)

class CStatusUI :
	public IScriptObject
{
public:
	enum {STATUS_LEVEL, STATUS_HP, STATUS_ATK, STATUS_DEF, STATUS_CRIT, STATUS_STA, STATUS_EXP, 
		STATUS_BOHP, STATUS_HOATK, STATUS_BODEF, STATUS_END};
private:
	explicit CStatusUI();
	virtual ~CStatusUI() = default;
public:
	virtual HRESULT Initailze(CGameObject * pArg);
	virtual void Update(_double deltaTime) override;
	virtual void LateUpdate(_double deltaTime) override;
	virtual void Render();
private:
	CStat*	m_pPlayerStatus = nullptr;
private:
	std::vector<CText*>	m_vecTexts;
public:
	static CStatusUI* Create(CGameObject*	pTarget = nullptr);
	virtual void Free() override;
};

END