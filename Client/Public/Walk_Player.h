#pragma once
#include "StateMachine.h"

BEGIN(Client)

class CWalk_Player final : public CStateMachine
{
public:
	CWalk_Player();
	virtual ~CWalk_Player() = default;

public:
	virtual void Enter(CPlayer& pPlayer);
	virtual CStateMachine* Input(CPlayer& pPlayer) override;
	virtual void Update(_double dDeltaTime, CPlayer& pPlayer) override;
	virtual void End(CPlayer& pPlayer) override;
public:
	virtual void Free() override;
};

END