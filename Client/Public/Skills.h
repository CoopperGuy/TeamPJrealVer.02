#pragma once

#include "Base.h"

BEGIN(Client)

class CSkills : public CBase
{
public:
	typedef struct tagSkillInfo {
		_float coolTime;
		_float coolDelta;
		string Name;
	}SKILLINFO;
private:
	explicit CSkills();
	virtual ~CSkills() = default;
public:
	void	SetInfo(SKILLINFO _info) { m_tSkillInfo = _info; }
public:
	SKILLINFO GetInfo() { return m_tSkillInfo; }
private:
	SKILLINFO	m_tSkillInfo;
public:
	static CSkills* Create(CSkills*	pTarget = nullptr);
	virtual void Free() override;
};

END