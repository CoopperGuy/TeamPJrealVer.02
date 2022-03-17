#include "EnginePCH.h"
#include "..\Public\Frustum.h"
#include "Engine.h"

USING(Engine)

IMPLEMENT_SINGLETON(CFrustum)


CFrustum::CFrustum()
{
}



HRESULT CFrustum::Ready_FrustumInProjSpace()
{
	m_vPoint[0] = _float3(-1.f, 1.f, 0.f);
	m_vPoint[1] = _float3(1.f, 1.f, 0.f);
	m_vPoint[2] = _float3(1.f, -1.f, 0.f);
	m_vPoint[3] = _float3(-1.f, -1.f, 0.f);

	m_vPoint[4] = _float3(-1.f, 1.f, 1.f);
	m_vPoint[5] = _float3(1.f, 1.f, 1.f);
	m_vPoint[6] = _float3(1.f, -1.f, 1.f);
	m_vPoint[7] = _float3(-1.f, -1.f, 1.f);
	//     4  5
	// (7)0 1 6
 	//    2 3
	//
	return S_OK;
}

HRESULT CFrustum::Transform_ToWorldSpace()
{
	_matrix proj =  CEngine::GetInstance()->GetTransform(CPipeline::D3DTS_PROJ);
	proj = XMMatrixInverse(nullptr, proj);
	_matrix view =  CEngine::GetInstance()->GetTransform(CPipeline::D3DTS_VIEW);
	view = XMMatrixInverse(nullptr, view);

	_vector		vPoint[8];

	for (_uint i = 0; i < 8; i++) {
		vPoint[i] = XMVector3TransformCoord(XMLoadFloat3(&m_vPoint[i]), proj);
		vPoint[i] = XMVector3TransformCoord(vPoint[i], view);
		vPoint[i] = XMVectorSetW(vPoint[i], 1);
	}

	Make_Plane(vPoint);

	return S_OK;
}

_bool CFrustum::isIn(_fvector vPosition, _float fRange)
{
	for (_uint i = 0; i < 6; ++i)
	{
		if (fRange < XMVectorGetX(XMPlaneDotCoord(XMLoadFloat4(&m_Plane[i]), vPosition)))
			return false;
	}
	return true;
}

void CFrustum::Make_Plane(_fvector * pPoints)
{
	XMStoreFloat4(&m_Plane[0], XMPlaneFromPoints(pPoints[1], pPoints[5], pPoints[6]));
	XMStoreFloat4(&m_Plane[1], XMPlaneFromPoints(pPoints[4], pPoints[0], pPoints[3]));

	XMStoreFloat4(&m_Plane[2], XMPlaneFromPoints(pPoints[4], pPoints[5], pPoints[1]));
	XMStoreFloat4(&m_Plane[3], XMPlaneFromPoints(pPoints[3], pPoints[2], pPoints[6]));

	XMStoreFloat4(&m_Plane[4], XMPlaneFromPoints(pPoints[5], pPoints[4], pPoints[7]));
	XMStoreFloat4(&m_Plane[5], XMPlaneFromPoints(pPoints[0], pPoints[1], pPoints[2]));
}

void CFrustum::Free()
{
}
