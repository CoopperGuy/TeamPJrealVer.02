#pragma once
#include "IScriptObject.h"
#include "Engine.h"
BEGIN(Client)

class CLoadingGauge :
	public IScriptObject
{
public:
private:
	explicit CLoadingGauge();
	virtual ~CLoadingGauge() = default;
public:
	virtual HRESULT Initailze(CGameObject * pArg);
	virtual void Update(_double deltaTime) override;
	virtual void LateUpdate(_double deltaTime) override;
	virtual void Render();
public:
	void	AddPercentage(_float num);
	void	SetPercentage(_float num) { m_fDegree = num * XMConvertToRadians(3.6f); }
public:
	_float	GetPercentage() { return (m_fDegree / XMConvertToRadians(3.6f)); }
private:
	vector<CEmptyUI*>	m_child;
private:
	CVIBuffer_RectUI*	m_pVIBuffer = nullptr;
	CGameObject*	pTarget = nullptr;
	CEmptyUI*		m_pThisUI = nullptr;
	CText*			m_pText = nullptr;
private:
	_float	m_fTime = 0.f;
	_float	m_fDegree = 0.f;
public:
	static CLoadingGauge* Create(CGameObject*	pTarget = nullptr);
	virtual void Free() override;
};

END