#pragma once
#include "StateMachine.h"

class CSkill_Player : public CStateMachine
{
public:
	CSkill_Player();
	virtual ~CSkill_Player() = default;

public:
	virtual void Enter(CPlayer& pPlayer);
	virtual CStateMachine* Input(CPlayer& pPlayer) override;
	virtual void Update(_double dDeltaTime, CPlayer& pPlayer) override;
	virtual void End(CPlayer& pPlayer) override;

public:
	virtual void Free() override;

public:
	_bool m_bDuring		= false;
	_bool m_bFinished	= false;
	_bool m_bLoop		= false;
	_uint m_iStartIndex = 0;
	_uint m_iCurIndex	= 0;
	_uint m_iCurrentAniIndex = 0;
	_double m_SkillTime = 0.f;
	vector<_uint> m_vecSkill;
};

