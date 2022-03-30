#pragma once
#include "Enemy.h"
#include "Engine.h"

BEGIN(Client)

class CEl_Flogas : public CEnemy
{
	enum ELEMENT_STATE { RUN, IDLE, DIE, DEADBODY, EL_END };
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

public:
	_bool Get_DeadMotion() { return m_bDeadMotion; }
	_vector Get_EffectDist() { return XMLoadFloat3(&m_vEffectDist); }
	_uint Get_Count() { return m_iEffectCount; }
private:
	CTransform* m_pTargetTransform	= nullptr;
	CFlogas*		m_pFlogas		= nullptr;
	class CStat*	m_pStat			= nullptr;
	class CMonHpVIBuffer*	m_pMonHp		= nullptr;
	CBasicCollider* m_pOBB			= nullptr;
	string			m_ScriptName	= {};

	_uint		m_iEffectCount = 0;
	_float3     m_vTargetPos;
	_float3     m_vEffectDist;
	_float3		m_vOriginPos;
	_float4x4	m_OriginWorld;
	_float		m_fScale = 1.5f;
	_double		m_dExplosionTime = 0.0;
	_double		m_dCreatTime = 0.0;

	_bool m_bMove = false;
	_bool m_bDeadMotion = false;
	_bool m_bHpzero = false;
	_bool m_bCreateEffect = false;
};

END