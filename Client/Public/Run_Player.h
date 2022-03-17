#pragma once
#include "StateMachine.h"

BEGIN(Client)

class CRun_Player final : public CStateMachine
{
public:
	CRun_Player();
	virtual ~CRun_Player() = default;

public:
	virtual void Enter(CPlayer& pPlayer);
	virtual CStateMachine* Input(CPlayer& pPlayer) override;
	virtual void Update(_double dDeltaTime, CPlayer& pPlayer) override;
	virtual void End(CPlayer& pPlayer) override;

public:
	virtual void Free() override;
};

END