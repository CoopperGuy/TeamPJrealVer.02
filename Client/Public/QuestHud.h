#pragma once
#include "IScriptObject.h"
#include "Engine.h"
#include "QuestData.h"
#include "SideQuestHud.h"
BEGIN(Client)

class CQuestHud :
	public IScriptObject
{
private:
	explicit CQuestHud();
	virtual ~CQuestHud() = default;
public:
	virtual HRESULT Initailze(CGameObject * pArg);
	virtual void Update(_double deltaTime) override;
	virtual void LateUpdate(_double deltaTime) override;
	virtual void Render();
public:
	void	RemoveQuest(_int questID);
	void	AddQuestAtList();
	void	CheckQuestProgress();
private:
	vector<pair<CEmptyUI*, CQuestData*>>	m_QuestList;
private:
	CGameObject*		m_pTarget = nullptr;
	CEmptyUI*			m_pThisUI = nullptr;
	CEmptyUI*			m_pQuestPos = nullptr;
	CSideQuestHud*		m_pSideQuestHud = nullptr;
private:
	_float				m_fQuestYPos = 0.f;
	mutable _float		m_fYPosPadding = 64.f;
private:
	std::mutex			m_Mutex;
public:
	static CQuestHud* Create(CGameObject*	pTarget = nullptr);
	virtual void Free() override;
};

END