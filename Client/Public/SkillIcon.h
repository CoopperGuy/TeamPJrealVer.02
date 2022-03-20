#pragma once

#include "Engine.h"
#include "VIBuffer_RectUI.h"
#include "Transform.h"
#include "IScriptObject.h"
BEGIN(Client)
class CSkillIcon :
	public IScriptObject
{
	enum SKILLIMAGE{SKILL_IMAGE,SKILL_BORDER,SKILL_END};
public:
	typedef struct tagSkillInfo {
		_int   level = 1.f;
		_float coolTime = 10.f;
		_float coolDelta = 0.f;
		string Name;
	}SKILLINFO;

private:
	explicit CSkillIcon();
	virtual ~CSkillIcon() = default;
public:
	virtual HRESULT Initailze(CGameObject * pArg);
	virtual void Update(_double deltaTime) override;
	virtual void LateUpdate(_double deltaTime) override;
	virtual void Render();
public:
	void	SetSkillInfo(_int idx, SKILLINFO _info);
public:
	void	UseSkill(_int idx) { m_tInfo[idx].coolDelta = 0.f; }
public:
	_bool	IsCoolDown(_int idx);
private:
	const static _int sizeOfUI = 4;
private:
	vector<CEmptyUI*>	m_child[sizeOfUI];
	vector<CVIBuffer_RectUI*>	m_pVIBufferList;
	vector<CEmptyUI*>		m_pUIList;
	CGameObject*	pTarget = nullptr;
private:
	_float	m_fTime = 0.f;
	_float	m_fDegree = 0.f;
private:
	SKILLINFO m_tInfo[sizeOfUI];
public:
	static CSkillIcon* Create(CGameObject*	pTarget = nullptr);
	virtual void Free() override;
};

END