#pragma once

#include "Character.h"
#include "Engine.h"

BEGIN(Client)

class CNPC :
	public CCharacter
{
protected:
	explicit CNPC();
	explicit CNPC(CGameObject* pObj);
	virtual ~CNPC() = default;
public:
	virtual HRESULT Initailze(CGameObject * pArg);
	virtual void Update(_double deltaTime) override;
	virtual void LateUpdate(_double deltaTime) override;
public:
	virtual _float CalculatePlayerDistance(CTransform* npcTransform);
protected:
	CEmptyGameObject*	m_pPlayerTarget = nullptr;
	CTransform*			m_pPlayerTransform = nullptr;

protected:
	CEmptyUI*			m_pTalkDesk = nullptr;
	CEmptyUI*			m_pTalkBox = nullptr;

	CEmptyUI*			m_pTalkText = nullptr;
	CText*				m_pText = nullptr;
	CText*				m_pBoxText = nullptr;
protected:
	_bool				m_bisTalkCreate = false;
	_bool				m_bisTalkBoxCreate = false;
	_bool				m_bisEndTalk = false;
public:
	virtual void Free() override;
};

END