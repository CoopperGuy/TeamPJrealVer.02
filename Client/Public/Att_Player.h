#pragma once
#include "StateMachine.h"

BEGIN(Client)

class CAtt_Player final : public CStateMachine
{
public:
	CAtt_Player();
	virtual ~CAtt_Player() = default;

public:
	virtual void Enter(CPlayer& pPlayer);
	virtual CStateMachine* Input(CPlayer& pPlayer) override;
	virtual void Update(_double dDeltaTime, CPlayer& pPlayer) override;
	virtual void End(CPlayer& pPlayer) override;

public:
	virtual void Free() override;

public:
	_bool m_bDuring = false;
	_bool m_bRB = false;
	_int m_iRBComboIndex = -1;
	_uint m_iCurIndex = 0;
private:
};

END
