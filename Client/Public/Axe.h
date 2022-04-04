#pragma once
#include "Equipment.h"
#include "Engine.h"
#include "Player.h"
#include "BasicCollider.h"


BEGIN(Client)

class CAxe : public CEquipment
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

	_fmatrix Remove_Scale(_fmatrix _srcmatrix);
	_fmatrix Remove_ScaleRotation(_fmatrix _srcmatrix);
public:
	void Set_DrawTrail() { m_DrawTrail = true; }
	_bool Set_Component(string name);
public:
	void Set_TrailOnOff();


private:
	CGameObject*	pPlayer			    = nullptr;
	CTransform* m_pTargetRealTransform  = nullptr;
private:
	CVIBuffer_Trail* m_pTrailBuffer		= nullptr;

private:
	class CBasicCollider* m_pOBB = nullptr;

private:
	_bool			m_bcheck = true;
	_bool			m_bDamageCheck = false;
	_bool			m_DrawTrail = false;

private:
	_bool			m_effectCreate[(_int)Player_State::Player_End];

private:
	_float4x4 m_matRightBone = {};

};
END
