#pragma once

#include "StateMachine.h"

BEGIN(Client)

class CIdle_Player final : public CStateMachine
{
public:
	CIdle_Player();
	virtual ~CIdle_Player() = default;

public:
	virtual void Enter(CPlayer& pPlayer);
	virtual CStateMachine* Input(CPlayer& pPlayer) override;
	virtual void Update(_double dDeltaTime, CPlayer& pPlayer) override;
	virtual void End(CPlayer& pPlayer) override;

public:
	virtual void Free() override;
public:
	_double m_dDelta = 0.f;
};

END