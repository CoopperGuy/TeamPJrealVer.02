#pragma once

#include "Engine.h"
#include "Transform.h"
#include "IScriptObject.h"

BEGIN(Client)

class CBasicEffect  :	public IScriptObject
{
protected:
	explicit CBasicEffect();
	explicit CBasicEffect(CGameObject* pObj);
	virtual ~CBasicEffect() = default;
protected:
	virtual HRESULT Initialize(CGameObject * pArg);
	virtual void Update(_double deltaTime) override;
	virtual void LateUpdate(_double deltaTime) override;
protected:
	CGameObject*	m_pTargetObj = nullptr;
public:
	static CBasicEffect* Create(CGameObject* pTarget = nullptr);
	virtual void Free() ;

public:
	void SetAnimationEnd() { m_bAnimationEnd = true; }


protected:
	CGameObject*		m_pAttachTarget = nullptr;
	CGameObject*		m_pGameObject = nullptr;
	CTransform*			m_pTransform = nullptr;


protected:
	string				TargetName;
	string				TargetBoneName;

	_bool				m_bAnimationEnd = false;

};

END