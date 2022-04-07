#pragma once
#include "IScriptObject.h"
#include "Engine.h"
BEGIN(Client)
class CDropRockSmall :
	public IScriptObject
{
public:
	CDropRockSmall();
	virtual ~CDropRockSmall() = default;

public:
	virtual HRESULT Initailze(CGameObject * pArg, _vector pos,_bool mini = false, _bool bone = false);
	virtual void Update(_double deltaTime) override;
	virtual void LateUpdate(_double deltaTime) override;

private:
	CGameObject*		m_pGameObject = nullptr;
	CCollider*			m_pCollider = nullptr;
	CTransform*			m_pTransform = nullptr;
	class CEffectDropRock* m_pDropRockEff = nullptr;

	_vector MyPos = {};


	_float PosX = 0.f;
	_float PosY = 0.f;
	_float PosZ = 0.f;
	_float StartPosY = 0.f;
	_float StartPosX = 0.f;

	_float Speed = 3.f;

	_float Time = 0.f;
	_float fAngleAcc;

	int randompos = 0;
	int randomX = 0;

	_bool m_bBone = false;
public:
	static CDropRockSmall*	Create(CGameObject*	pArg, _vector pos = {0.f,0.f,0.f}, _bool mini = false, _bool bone = false);
	virtual void Free() override;
};

END