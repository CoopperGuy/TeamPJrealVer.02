#pragma once
#include "Enemy.h"

BEGIN(Client)

class CWaterEA :
	public CEnemy
{

	enum STATE { ATT, IDLE, STATEEND };
private:
	explicit CWaterEA();
	explicit CWaterEA(CGameObject* rhs);
	virtual ~CWaterEA() = default;

public:
	HRESULT Initialize(_float3 position);
	virtual void Update(_double dDeltaTime);
	virtual void LateUpdate(_double dDeltaTime);
	virtual void Render();

private:
	class CEffectEAFire* m_pEffEAFire = nullptr;

private:
	void Set_FirePos();
	void Set_State(_double dDeltaTime);

public:
	static CWaterEA* Create(CGameObject* pObj, _float3 position = _float3{ 0.f,0.f,0.f });
	virtual void Free() override;

	_fmatrix Remove_ScaleRotation(_fmatrix TransformMatrix);


private:
	STATE m_eState = IDLE;

	_double m_dAttTime = 0;
	_double m_dIdleTime = 0;
	_double m_dAniIndex = 0.5;

	float start = 0.5f;
	float plus = 0.01f;

	_bool	makeEffect = false;

	_uint indexd = 0;
};

END