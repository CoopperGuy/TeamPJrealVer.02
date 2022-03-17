#pragma once
#include "IScriptObject.h"
#include "Engine.h"
BEGIN(Client)

class CMonHpVIBuffer :
	public IScriptObject
{
private:
	explicit CMonHpVIBuffer();
	virtual ~CMonHpVIBuffer() = default;
public:
	virtual HRESULT Initailze(CGameObject * pArg);
	virtual void Update(_double deltaTime) override;
	virtual void LateUpdate(_double deltaTime) override;
	virtual void Render();
public:
	void pushHuds(CEmptyGameObject* hp, CEmptyGameObject* hpBar) { m_pHp = hp; m_pHpBar = hpBar; }
	void pushBuffer(CVIBuffer_Rect* buf) { m_pHudBuffer.emplace_back(buf); }
	void SetHud_BarTransform(CTransform* Hudtrans, CTransform* BarTrans);
public:
	void SetIsEnd() { m_bisEnd = true; }
public:
	_bool GetIsEnd() { return m_bisEnd; }
private:
	CGameObject*	pTarget = nullptr;
	CVIBuffer_Rect*	m_pVIBuffer = nullptr;
	CStat*			m_pTargetStat = nullptr;
	CTransform*		m_pTargetTrans = nullptr;
private:
	CEmptyGameObject*		m_pHp;
	CEmptyGameObject*		m_pHpBar;
	vector<CVIBuffer_Rect*> m_pHudBuffer;
	CTransform*				m_pBarTrans = nullptr;
	CTransform*				m_pHudTrans = nullptr;

private:
	_float	m_fPercetage = 1.f;
	_float	m_fBackPercentage = 1.f;
	_uint	m_iMonLevel = 1;
	_bool	m_bisEnd = false;
public:
	static CMonHpVIBuffer* Create(CGameObject*	pTarget = nullptr);
	virtual void Free() override;
};

END