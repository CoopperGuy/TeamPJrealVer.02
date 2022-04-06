#pragma once
#include "Enemy.h"
#include "Engine.h"

BEGIN(Client)
enum class eTrapSpear {ATK, PRICK, GO_INTO, IDLE, TRAPSPEAR_END};

class CTrapSpear : public CEnemy
{
public:
	explicit CTrapSpear();
	explicit CTrapSpear(CGameObject* pObj);
	virtual ~CTrapSpear() = default;

public:
	virtual HRESULT Initialize(void* pArg, _double dEndTime);
	virtual void Update(_double deltaTime);
	virtual void LateUpdate(_double deltaTime);
	virtual void Render();

public:
	static CTrapSpear* Create(void* pArg = nullptr, _double dEndTime = 0.0);
	virtual void Free() override;

private:
	void CheckingFinished();
	_bool State_IDLE();
private:
	class CStat*	m_pStat			= nullptr;
	CBasicCollider* m_pOBB			= nullptr;
	CBasicCollider* m_pOBB1			= nullptr;
private:
	_double m_dStartTime = 0.0;
	_double m_dEndTime = 0.0;
	_double m_dStart	 = 10.0;
	_double m_dEnd		 = 0.0;
	_double m_dAnimSpeed  = 0.0;

	_bool m_bStart = false;
	_bool m_bEnd = false;
	eTrapSpear m_eState  = eTrapSpear::TRAPSPEAR_END;
};

END