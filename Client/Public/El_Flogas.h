#pragma once
#include "Enemy.h"
#include "Engine.h"

BEGIN(Client)

class CEl_Flogas : public CEnemy
{
public:
	explicit CEl_Flogas();
	explicit CEl_Flogas(CGameObject* pObj);
	virtual ~CEl_Flogas() = default;

public:
	virtual HRESULT Initialize(string name, class CFlogas* pObj);
	virtual void Update(_double deltaTime);
	virtual void LateUpdate(_double deltaTime);
	virtual void Render();

public:
	static CEl_Flogas* Create(string name, class CFlogas* pObj = nullptr);
	virtual void Free() override;

private:
	CTransform* m_pTargetTransform	= nullptr;
	string		m_ScriptName		= {};
	_float3     m_vTargetPos;
	_float3		m_vOriginPos;

	CFlogas* m_pFlogas = nullptr;
};

END