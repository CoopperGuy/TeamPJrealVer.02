#pragma once
#include "IScriptObject.h"
#include "Engine.h"
BEGIN(Client)

class CGold :
	public IScriptObject
{
private:
	explicit CGold();
	virtual ~CGold() = default;
public:
	virtual HRESULT Initailze(CGameObject * pArg);
	virtual void Update(_double deltaTime) override;
	virtual void LateUpdate(_double deltaTime) override;
	virtual void Render();
public:
	void UpdateGoldString(string str);
private:
	CText*	m_pGold = nullptr;
public:
	static CGold* Create(CGameObject*	pTarget = nullptr);
	virtual void Free() override;
};

END