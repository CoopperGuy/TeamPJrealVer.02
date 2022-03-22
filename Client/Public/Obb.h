#pragma once
#include "IScriptObject.h"
#include "Engine.h"

BEGIN(Engine)
class CBasicCollider;
END

BEGIN(Client)

class CObb :
	public IScriptObject
{
private:
	explicit CObb();
	virtual ~CObb() = default;
public:
	virtual HRESULT Initailze(_float3 position, _float3 size, _float dmg, ID _collisionType, _float _duration, CTransform * pArg);
	virtual HRESULT Initailze(_fvector position, _fvector size, _float dmg, ID _collisionType, _float _duration, CTransform * pArg);

	virtual void Update(_double deltaTime) override;
	virtual void LateUpdate(_double deltaTime) override;
	virtual void Render();
public:
	void SetSize(_fvector _size);
	void SetSize(_float3 _size);
	void SetupDead();
	void SetPosision(_float3 _pos);
private:
	CEmptyGameObject*	m_pObj = nullptr;
	CBasicCollider*		m_pOBB = nullptr;
	CStat*				m_pStat = nullptr;
	CTransform*			m_pTargetTransform = nullptr;
	CTransform*			m_pTransform = nullptr;

private:
	_float				m_fDuration = 1.f;
	_float				m_fDelta = 0.f;
public:
	static CObb* Create(_float3 position, _float3 size, _float dmg, ID _collisionType, _float _duration, CTransform*	pTarget = nullptr);
	static CObb* Create(_fvector position, _fvector size, _float dmg, ID _collisionType, _float _duration, CTransform*	pTarget = nullptr);

	virtual void Free() override;
};

END