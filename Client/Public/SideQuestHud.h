#pragma once
#include "IScriptObject.h"
#include "Engine.h"
BEGIN(Client)

class CSideQuestHud :
	public IScriptObject
{
private:
	explicit CSideQuestHud();
	virtual ~CSideQuestHud() = default;
public:
	virtual HRESULT Initailze(CGameObject * pArg);
	virtual void Update(_double deltaTime) override;
	virtual void LateUpdate(_double deltaTime) override;
	virtual void Render();
public:
	void	UpdateList();
	void	LinkQuestList(vector<pair<CEmptyUI*, CQuestData*>>& _list);
private:
	vector<pair<CEmptyUI*, CQuestData*>>	m_QuestList;
private:
	CGameObject*		m_pTarget = nullptr;
	CEmptyUI*			m_pThisUI = nullptr;
	vector<CEmptyUI*>	m_pQuestSubList;
private:
	std::mutex			mutexUpdate;
public:
	static CSideQuestHud* Create(CGameObject*	pTarget = nullptr);
	virtual void Free() override;
};

END