#pragma once
#include "IScriptObject.h"
#include "Engine.h"
BEGIN(Client)

class CItemInfo :
	public IScriptObject
{
private:
	enum IINFO { IINFO_NAME, IINFO_IMAGE, IINFO_HP, IINFO_ATK, IINFO_DEF, IINFO_END };

private:
	explicit CItemInfo();
	virtual ~CItemInfo() = default;
public:
	virtual HRESULT Initailze(CGameObject * pArg);
	virtual void Update(_double deltaTime) override;
	virtual void LateUpdate(_double deltaTime) override;
	virtual void Render();
public:
	void SetActive(_bool _active);
	void SetItemInfo(ITEMINFO _info);
	void SetYPosition(_float _y);
private:
	CEmptyUI*	m_pThisUI = nullptr;
	string		m_strName;
	string		m_strImagePath;
	string		m_strHP;
	string		m_strAtk;
	string		m_strDef;
private:
	vector<CEmptyUI*>	m_vecUIs;
private:
	ITEMINFO	m_tItemInfo;
public:
	static CItemInfo* Create(CGameObject*	pTarget = nullptr);
	virtual void Free() override;
};

END