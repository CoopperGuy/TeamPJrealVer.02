#pragma once
#include "IScriptObject.h"
#include "Engine.h"

BEGIN(Client)

class CGoldAxe : public IScriptObject
{
public:
	explicit CGoldAxe();
	explicit CGoldAxe(CGameObject* pObj);
	virtual ~CGoldAxe() = default;

public:
	HRESULT Initialize();
	virtual void Update(_double deltaTime) override;
	virtual void LateUpdate(_double deltaTime) override;
	virtual void Render();

public:
	static CGoldAxe* Create(CGameObject* pObj);
	virtual void Free() override;

private:
	CTransform*		m_pTransform = nullptr;
	CTransform*		m_pTargetTransform = nullptr;
	CModel*			m_pTargetModel = nullptr;
	CCollider*		m_pCollider = nullptr;
	CModel*			m_pModel = nullptr;

private:
	_float4x4 m_matRightBone = {};
};
END
