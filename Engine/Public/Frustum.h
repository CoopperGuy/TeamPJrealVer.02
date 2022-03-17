#pragma once

#include "Base.h"

BEGIN(Engine)

class CFrustum :
	public CBase
{
	DECLARE_SINGLETON(CFrustum)
public:
	CFrustum();
	virtual ~CFrustum() = default;
public:
	HRESULT Ready_FrustumInProjSpace();
	HRESULT Transform_ToWorldSpace();

	_bool isIn(_fvector vPosition, _float fRange = 0.f);
private:
	_float3			m_vPoint[8];
	_float4			m_Plane[6];

private:
	void Make_Plane(_fvector* pPoints);
public:
	virtual void Free() override;
};

END