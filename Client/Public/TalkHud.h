#pragma once

#include "IScriptObject.h"
#include "Engine.h"
BEGIN(Client)

class CTalkHud :
	public IScriptObject
{
private:
	explicit CTalkHud();
	virtual ~CTalkHud() = default;
public:
	virtual HRESULT Initailze(CGameObject * pArg);
	virtual void Update(_double deltaTime) override;
	virtual void LateUpdate(_double deltaTime) override;
	virtual void Render();
public:
	void		StartConversation(_uint npcID, _uint questID, _uint questProgress = 0);
private:
	CGameObject*	m_pTarget = nullptr;
	CEmptyUI*		m_pThisUI = nullptr;
	CText*			m_pTextDesk = nullptr;
public:
	static CTalkHud* Create(CGameObject*	pTarget = nullptr);
	virtual void Free() override;
};

END