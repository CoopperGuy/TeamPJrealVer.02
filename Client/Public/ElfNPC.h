#pragma once
#include "NPC.h"
#include "Engine.h"

BEGIN(Client)

class CElfNPC :
	public CNPC
{
private:
	explicit CElfNPC();
	explicit CElfNPC(CGameObject* pObj);
	virtual ~CElfNPC() = default;
public:
	virtual HRESULT Initailze(CGameObject * pArg);
	virtual void Update(_double deltaTime) override;
	virtual void LateUpdate(_double deltaTime) override;
public:
	void SetTalkDesk(CEmptyUI*	m_pUI);
	void SetTalkBox(CEmptyUI*	m_pUI);
public:
	void CheckAlret();
	void MakeTalkDesk(_double deltaTime);
	void MakeTalkBox(_double deltaTime);
	void BoxCheck();
	void AcceptQuest();
public:
	void NormalTalk(_int id);
	void QuestTalk(_int id);
	void BoxTalk(_int id);
private:
	CEmptyGameObject*	m_pTarget = nullptr;
	CTransform*			m_pThisTransform = nullptr;
	CEmptyGameObject*	m_pAlretUI = nullptr;
private:
	CNPCInfo::NPCTAG	m_tNPCTag;
	_int				m_iCurTalkProgress = 0;
private:
	mutable _float		m_fAlretMakeDistance = 0.5f;
public:
	static CElfNPC* Create(CGameObject* pTartget = nullptr);
	virtual void Free() override;
};

END