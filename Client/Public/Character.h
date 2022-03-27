#pragma once
#include "IScriptObject.h"
#include "Engine.h"
#include "Client_Defines.h"
#include "Transform.h"
#include "Collider.h"
#include "Model.h" 
#include "Navigation.h"

BEGIN(Client)
class CCharacter :	public IScriptObject
{
protected:
	explicit CCharacter();
	explicit CCharacter(CGameObject* pObj);
	virtual ~CCharacter() = default;
public:
	static CCharacter* Create(CGameObject* pObj);
	virtual void Free();
public:
	virtual void SetObjectTransform(CGameObject* pObj, BONEDESC* pBone);
	virtual _vector GetBonePosition(BONEDESC* pBone);

protected:
	_double					m_ShotTime = 0.0;

protected:
	CGameObject*	m_pGameObject = nullptr;
	CTransform*		m_pTransform = nullptr;
	CCollider*		m_pCollider = nullptr;
	CNavigation*	m_pNavigation = nullptr;
	PxController*	m_pController = nullptr;
public:
	CModel*			m_pModel = nullptr;

	// Inherited via IScriptObject
	virtual void Update(_double deltaTime) override;

	// Inherited via IScriptObject
	virtual void LateUpdate(_double deltaTime) override;
};

END