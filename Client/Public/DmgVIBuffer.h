#pragma once
#include "IScriptObject.h"
#include "Engine.h"

BEGIN(Client)

class CDmgVIBuffer :
	public IScriptObject
{
private:
	explicit CDmgVIBuffer();
	virtual ~CDmgVIBuffer() = default;
public:
	virtual HRESULT Initailze(CGameObject * pArg, _float3 pos, _float dmg);
	virtual void Update(_double deltaTime) override;
	virtual void LateUpdate(_double deltaTime) override;
	virtual void Render();
public:
	void SetThisUI(CGameObject* pThis) { this->m_pThisUI = pThis; }
	void SetisEnd() { this->isEnd = true; }
	void SetTransform(CTransform* trans) { m_pTransform = trans; }
private:
	CGameObject*	pTarget = nullptr;
	CGameObject*	m_pThisUI = nullptr;
	CTransform*		m_pTransform = nullptr;
	_double			m_LifeTime = 1.f;
	_double			m_LifeDelta = 0.f;
	_float			m_fDmg = 0.f;
private:
	_bool isEnd = false;
public:
	static CDmgVIBuffer* Create(CGameObject*	pTarget = nullptr, _float3 pos = {0.f,0.f,0.f}, _float dmg = 0.f);
	virtual void Free() override;
};

END