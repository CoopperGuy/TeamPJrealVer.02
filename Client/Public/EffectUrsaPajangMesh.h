#pragma once

#include "Engine.h"
#include "BasicEffect.h"

BEGIN(Client)
class CObb;
class CEffectUrsaPajangMesh final :
	public CBasicEffect
{
private:
	explicit CEffectUrsaPajangMesh();
	virtual ~CEffectUrsaPajangMesh() = default;

public:
	virtual HRESULT Initialize(void* pArg, _vector pos);
	virtual void Update(_double deltaTime);
	virtual void LateUpdate(_double deltaTime);
	virtual void Render();
public:
	static CEffectUrsaPajangMesh* Create(void * pArg, _vector pos);
	virtual void Free() override; 

private:
	_double m_dDeadTime = 0;
	_float3 StartScail = {};

	list<CGameObject*> childList = {};

};

END