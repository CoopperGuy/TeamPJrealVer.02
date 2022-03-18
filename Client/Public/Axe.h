#pragma once
#include "IScriptObject.h"
#include "Engine.h"
#include "Player.h"


BEGIN(Client)

class CAxe : public IScriptObject
{
public:
	explicit CAxe();
	explicit CAxe(CGameObject* pObj);
	virtual ~CAxe() = default;

public:
	HRESULT Initialize();
	virtual void Update(_double deltaTime) override;
	virtual void LateUpdate(_double deltaTime) override;
	virtual void Render();

public:
	static CAxe* Create(CGameObject* pObj);
	virtual void Free() override;

private:
	void Set_Attack();
	void State_Att();
	void State_Idle();

	void Create_Trail();

public:
	void Set_DrawTrail() { m_DrawTrail = true; }

public:
	void Set_TrailOnOff();

private:
	CGameObject*	pPlayer				= nullptr;
	CGameObject*	 pAxe				 = nullptr;
private:
	CTransform*		m_pTransform		= nullptr;
	CTransform*		m_pTargetTransform  = nullptr;
	CModel*			m_pTargetModel		= nullptr;
	CCollider*		m_pCollider			= nullptr;
	CModel*			m_pModel			= nullptr;
private:
	CVIBuffer_Trail* m_pTrailBuffer			= nullptr;
	CGameObject*	m_pTrail = nullptr;
private:
	class CBasicCollider* m_pOBB = nullptr;
private:
	_bool			m_bcheck = true;
	_bool			m_bDamageCheck = false;
	_bool			m_DrawTrail = false;
private:
	_float4x4 m_matRightBone = {};

};
END
