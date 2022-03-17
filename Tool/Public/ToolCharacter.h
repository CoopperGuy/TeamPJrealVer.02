#pragma once
#include "IScriptObject.h"
#include "Engine.h"
#include "Transform.h"
#include "Collider.h"
#include "Model.h" 
#include "Navigation.h"

BEGIN(Tool)
class CToolCharacter :	public IScriptObject
{
protected:
	explicit CToolCharacter();
	explicit CToolCharacter(CGameObject* pObj);
	virtual ~CToolCharacter() = default;
public:
	static CToolCharacter* Create(CGameObject* pObj);
	virtual void Free();
public:
	virtual void GetDamage(_vector sourceLocation) = 0;
	virtual void SetObjectTransform(CGameObject* pObj, BONEDESC* pBone);
	virtual _vector GetBonePosition(BONEDESC* pBone);

protected:
	_double					m_ShotTime = 0.0;

protected:
	CGameObject*	m_pGameObject = nullptr;
	CTransform*		m_pTransform = nullptr;
	CCollider*		m_pCollider = nullptr;
	CModel*			m_pModel = nullptr;
	CNavigation*	m_pNavigation = nullptr;
	PxController*	m_pController = nullptr;
};

END