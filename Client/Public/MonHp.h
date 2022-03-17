#pragma once

#include "Engine.h"
#include "VIBuffer.h"
#include "Transform.h"
#include "IScriptObject.h"
BEGIN(Client)

class CMonHp :
	public IScriptObject
{
	enum MONHP{MONHUD_HP,MONHUD_LEVEL,MONHUD_NAME,MONHUD_END};
private:
	explicit CMonHp();
	virtual ~CMonHp() = default;
public:
	virtual HRESULT Initailze(CGameObject * pArg);
	virtual void Update(_double deltaTime) override;
	virtual void LateUpdate(_double deltaTime) override;
	virtual void Render();
public:
	void pushHuds(CEmptyUI* hud) { m_pHuds.emplace_back(hud); }
	void pushBuffer(CVIBuffer_RectUI* buf) { m_pHudBuffer.emplace_back(buf); }
public:
	void SetThisUI(CEmptyUI* _ui) { m_pThisUI = _ui; }
	void SetIsEnd() { m_bisEnd = true; }
public:
	_bool GetIsEnd() { return m_bisEnd; }
public:
	void SetRelease();
private:
	CGameObject*		m_pTarget = nullptr;
	CVIBuffer_RectUI*	m_pVIBuffer = nullptr;
	CStat*				m_pTargetStat = nullptr;
	CEmptyUI*			m_pThisUI = nullptr;
private:
	vector<CEmptyUI*>		m_pHuds;
	vector<CVIBuffer_RectUI*>	m_pHudBuffer;
private:	
	_uint	m_iMonLevel = 1;
	_float	m_fPercetage = 1.f;
	_float	m_fBackPercentage = 1.f;
private:
	_bool	m_bisEnd = false;
public:
	static CMonHp* Create(CGameObject*	pTarget = nullptr);
	virtual void Free() override;
};

END