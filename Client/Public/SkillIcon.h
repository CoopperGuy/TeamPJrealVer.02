#pragma once

#include "Engine.h"
#include "VIBuffer_RectUI.h"
#include "Transform.h"
#include "IScriptObject.h"
#include "Skills.h"
BEGIN(Client)
class CSkillIcon :
	public IScriptObject
{
private:
	explicit CSkillIcon();
	virtual ~CSkillIcon() = default;
public:
	virtual HRESULT Initailze(CGameObject * pArg);
	virtual void Update(_double deltaTime) override;
	virtual void LateUpdate(_double deltaTime) override;
	virtual void Render();
private:
	vector<CVIBuffer_RectUI*>	m_childVIBuffer;
	CVIBuffer_RectUI*	m_pVIBuffer = nullptr;
	CGameObject*	pTarget = nullptr;
	CEmptyUI*		m_pThisUI = nullptr;
private:
	_float	m_fTime = 0.f;
	_float	m_fDegree = 0.f;
private:
	CSkills*	m_cSkill = nullptr;
public:
	static CSkillIcon* Create(CGameObject*	pTarget = nullptr);
	virtual void Free() override;
};

END