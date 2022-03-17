#pragma once
#include "Enemy.h"

BEGIN(Client)

class CWaterEA :
	public CEnemy
{

	enum STATE {ATT, IDLE, STATEEND};
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

public:
	static CWaterEA* Create(CGameObject* pObj, _float3 position = _float3{ 0.f,0.f,0.f });
	virtual void Free() override;


private:
	STATE m_eState = STATEEND;

	_double m_dAttTime = 0;
	_double m_dIdleTime = 2;

	_bool	makeEffect = false;

	_uint indexd = 0;
};

END