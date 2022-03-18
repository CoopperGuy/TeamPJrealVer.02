#include "EnginePCH.h"
#include "..\public\Transform.h"

CTransform::CTransform(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
	: CComponent(pDevice, pDeviceContext)
{
	XMStoreFloat4x4(&m_WorldMatrix, XMMatrixIdentity());
	m_TransformDesc.fRotatePerSec = 5.f;
	m_TransformDesc.fSpeedPerSec = 5.f;
}

CTransform::CTransform(const CTransform & rhs)
	: CComponent(rhs)
	, m_WorldMatrix(rhs.m_WorldMatrix)
	, m_TransformDesc(rhs.m_TransformDesc)
	, m_pivotMatrix(rhs.m_pivotMatrix)
{

}

_float CTransform::GetScale(STATE eState)
{
	return XMVectorGetX(XMVector3Length(GetState(eState)));	
}

void CTransform::SetLook(_fvector vDirection)
{
	_vector		vPosition = GetState(CTransform::STATE_POSITION);
	_vector		vUp = GetState(CTransform::STATE_UP);

	_vector		vRight = XMVector3Cross(vUp, vDirection);

	vRight = XMVector3Normalize(vRight) * GetScale(CTransform::STATE_RIGHT);
	_vector		vLook = XMVector3Cross(vRight, vUp);
	vLook = XMVector3Normalize(vLook) * GetScale(CTransform::STATE_LOOK);

	vUp = XMVector3Normalize(XMVector3Cross(vLook, vRight)) * GetScale(CTransform::STATE_UP);

	SetState(CTransform::STATE_RIGHT, vRight);
	SetState(CTransform::STATE_LOOK, vLook);
	SetState(CTransform::STATE_UP, vUp);

}

void CTransform::SetScale(_float3 vScale)
{
	_vector pScale = XMLoadFloat3(&vScale);

	_vector		vRight = XMVector3Normalize(GetState(CTransform::STATE_RIGHT)) * XMVectorGetX(pScale);
	_vector		vUp = XMVector3Normalize(GetState(CTransform::STATE_UP)) * XMVectorGetY(pScale);
	_vector		vLook = XMVector3Normalize(GetState(CTransform::STATE_LOOK)) * XMVectorGetZ(pScale);

	SetState(CTransform::STATE_RIGHT, vRight);
	SetState(CTransform::STATE_UP, vUp);
	SetState(CTransform::STATE_LOOK, vLook);
}

void CTransform::SetLookUpVector(_fvector vDirection)
{
	_vector		vPosition = GetState(CTransform::STATE_POSITION);
	_vector		vUp = XMVectorSet(0.f, 1.f, 0.f, 0.f);

	_vector		vRight = XMVector3Cross(vUp, vDirection);

	vRight = XMVector3Normalize(vRight) * GetScale(CTransform::STATE_RIGHT);
	_vector		vLook = XMVector3Normalize(vDirection) * GetScale(CTransform::STATE_LOOK);
	

	vUp = XMVector3Normalize(XMVector3Cross(vLook, vRight)) * GetScale(CTransform::STATE_UP);
	SetState(CTransform::STATE_RIGHT, vRight);
	SetState(CTransform::STATE_LOOK, vLook);
	SetState(CTransform::STATE_UP, vUp);
}

void CTransform::SetPxMatrix(PxMat44 mat)
{
	memcpy(&m_WorldMatrix, &mat, sizeof(_float4x4));
}

void CTransform::SetPxPosition(PxExtendedVec3 vec3)
{
	_float3 newPos = { (float)vec3.x, (float)vec3.y, (float)vec3.z };
	SetState(CTransform::STATE_POSITION, XMLoadFloat3(&newPos));
}

_float CTransform::Get_Degree()
{
	_vector scale, rotation, pos;
	XMMatrixDecompose(&scale, &rotation, &pos, XMLoadFloat4x4(&m_WorldMatrix));
	return XMConvertToDegrees(acos(XMVectorGetX(rotation) / sqrt(XMVectorGetX(rotation) / 2 + XMVectorGetY(rotation) / 2)));
}

_fmatrix CTransform::Remove_ScaleRotation(_fmatrix TransformMatrix)
{
	_matrix			NonRotateMatrix = XMMatrixIdentity();

	NonRotateMatrix.r[3] = TransformMatrix.r[3];

	return NonRotateMatrix;
}

_fmatrix CTransform::Remove_Scale(_fmatrix TransformMatrix)
{
	_matrix			NonScaleMatrix = Remove_ScaleRotation(TransformMatrix);

	NonScaleMatrix.r[0] = XMVector3Normalize(TransformMatrix.r[0]);
	NonScaleMatrix.r[1] = XMVector3Normalize(TransformMatrix.r[1]);
	NonScaleMatrix.r[2] = XMVector3Normalize(TransformMatrix.r[2]);

	return NonScaleMatrix;
}

void CTransform::QuaternionToEuler(_fvector quater, _float3& euler)
{
	_double x, y, z, w;
	x = XMVectorGetX(quater);
	y = XMVectorGetY(quater);
	z = XMVectorGetZ(quater);
	w = XMVectorGetW(quater);
	// roll (x-axis rotation)
	double sinr_cosp = 2 * (w * x + y * z);
	double cosr_cosp = 1 - 2 * (x * x + y * y);

	euler.y = std::atan2(sinr_cosp, cosr_cosp);

	// pitch (y-axis rotation)
	double sinp = 2 * (w * y - z * x);
	if (std::fabs(sinp) >= 1)
		euler.x = std::copysign(XM_PIDIV2, sinp); // use 90 degrees if out of range
	else
		euler.x = std::asin(sinp);

	// yaw (z-axis rotation)
	double siny_cosp = 2 * (w *z + x * y);
	double cosy_cosp = 1 - 2 * (y * y + z * z);
	euler.z = std::atan2(siny_cosp, cosy_cosp);

}

HRESULT CTransform::InitializePrototype()
{
	XMStoreFloat4x4(&m_WorldMatrix, XMMatrixIdentity());
	// XMStoreFloat4x4(&m_WorldMatrixInverse, XMMatrixInverse(nullptr, XMLoadFloat4x4(&m_WorldMatrix));

	return S_OK;
}

HRESULT CTransform::Initialize(void * pArg)
{
	XMStoreFloat4x4(&m_WorldMatrix, XMMatrixIdentity());

	if (nullptr != pArg)
		memcpy(&m_TransformDesc, pArg, sizeof(TRANSFORMDESC));

	return S_OK;
}

void CTransform::GoStraight(_double TimeDelta)
{
	_vector			vPosition = _vector();
	_vector			vLook = _vector();
	vLook = GetState(CTransform::STATE_LOOK);
	vPosition = GetState(CTransform::STATE_POSITION);

	vPosition += XMVector3Normalize(vLook) * m_TransformDesc.fSpeedPerSec * _float(TimeDelta);

	SetState(CTransform::STATE_POSITION, vPosition);
}

void CTransform::GoLeft(_double TimeDelta)
{
	_vector			vPosition = _vector();
	_vector			vRight = _vector();
	vRight = GetState(CTransform::STATE_RIGHT);
	vPosition = GetState(CTransform::STATE_POSITION);

	vPosition += XMVector3Normalize(vRight) * m_TransformDesc.fSpeedPerSec * (_float)TimeDelta * -1.0f;

	SetState(CTransform::STATE_POSITION, vPosition);
}

void CTransform::GoRight(_double TimeDelta)
{
	_vector		vRight = GetState(CTransform::STATE_RIGHT);
	_vector		vPosition = GetState(CTransform::STATE_POSITION);

	vPosition += XMVector3Normalize(vRight) * m_TransformDesc.fSpeedPerSec * (_float)TimeDelta;

	SetState(CTransform::STATE_POSITION, vPosition);
}

void CTransform::GoBackWard(_double TimeDelta)
{
	_vector		vLook = GetState(CTransform::STATE_LOOK);
	_vector		vPosition = GetState(CTransform::STATE_POSITION);

	vPosition += XMVector3Normalize(vLook) * m_TransformDesc.fSpeedPerSec * (_float)TimeDelta * -1.0f;

	SetState(CTransform::STATE_POSITION, vPosition);
}

void CTransform::GoToSetDir(_vector dir, _double deltaTime)
{
	_vector		vPosition = GetState(CTransform::STATE_POSITION);
	vPosition += dir * deltaTime;
	SetState(CTransform::STATE_POSITION, vPosition);

}

void CTransform::GoDown(_double deltaTime)
{
	_vector			vPosition = _vector();
	_vector			vUp = _vector();
	vUp = GetState(CTransform::STATE_UP);
	vPosition = GetState(CTransform::STATE_POSITION);

	vPosition += XMVector3Normalize(vUp) * m_TransformDesc.fSpeedPerSec * _float(deltaTime) * -1.0f;

	SetState(CTransform::STATE_POSITION, vPosition);
}

void CTransform::ChaseTarget(CTransform * pTarget)
{
}

void CTransform::ChaseTarget(_fvector vTargetPos, _double TimeDelta)
{
	_vector		vPosition = GetState(CTransform::STATE_POSITION);

	_vector		vDirection = vTargetPos - vPosition;

	vPosition += XMVector3Normalize(vDirection) * m_TransformDesc.fSpeedPerSec * (_float)TimeDelta;

	SetState(CTransform::STATE_POSITION, vPosition);

	LookAtForLandObject(vTargetPos);
}

void CTransform::LookAtForLandObject(_fvector vTargetPos)
{
	_vector		vPosition = GetState(CTransform::STATE_POSITION);
	_vector		vUp = GetState(CTransform::STATE_UP);

	_vector		vDirection = vTargetPos - vPosition;

	_vector		vRight = XMVector3Cross(vUp, vDirection);

	vRight = XMVector3Normalize(vRight) * GetScale(CTransform::STATE_RIGHT);
	_vector		vLook = XMVector3Cross(vRight, vUp);
	vLook = XMVector3Normalize(vLook) * GetScale(CTransform::STATE_LOOK);

	SetState(CTransform::STATE_RIGHT, vRight);
	SetState(CTransform::STATE_LOOK, vLook);
}

void CTransform::LookAt(_fvector vTargetPos)
{

}

void CTransform::SetUpRotation(_fvector vAxis, _float fDegree)
{
	_vector		vRight, vUp, vLook;

	vRight = XMVectorSet(1.f, 0.f, 0.f, 0.f) * GetScale(CTransform::STATE_RIGHT);
	vUp = XMVectorSet(0.f, 1.f, 0.f, 0.f) * GetScale(CTransform::STATE_UP);
	vLook = XMVectorSet(0.f, 0.f, 1.f, 0.f) * GetScale(CTransform::STATE_LOOK);

	_matrix		RotationMatrix = XMMatrixRotationAxis(vAxis, XMConvertToRadians(fDegree));

	SetState(CTransform::STATE_RIGHT, XMVector4Transform(vRight, RotationMatrix));
	SetState(CTransform::STATE_UP, XMVector4Transform(vUp, RotationMatrix));
	SetState(CTransform::STATE_LOOK, XMVector4Transform(vLook, RotationMatrix));
}

void CTransform::RotateAxis(_fvector vAxis, _double TimeDelta)
{
	_vector		vRight, vUp, vLook;

	vRight = GetState(CTransform::STATE_RIGHT);
	vUp = GetState(CTransform::STATE_UP);
	vLook = GetState(CTransform::STATE_LOOK);

	_matrix		RotationMatrix = XMMatrixRotationAxis(vAxis, XMConvertToRadians(m_TransformDesc.fRotatePerSec) * (float)TimeDelta);

	SetState(CTransform::STATE_RIGHT, XMVector4Transform(vRight, RotationMatrix));
	SetState(CTransform::STATE_UP, XMVector4Transform(vUp, RotationMatrix));
	SetState(CTransform::STATE_LOOK, XMVector4Transform(vLook, RotationMatrix));
}

CTransform * CTransform::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
{
	CTransform*	pInstance = new CTransform(pDevice, pDeviceContext);
	pInstance->InitializePrototype();
	//if (FAILED(pInstance->InitializePrototype()))
	//{
	//	MSG_BOX("Failed To Creating CTransform");
	//	SafeRelease(pInstance);
	//}
	return pInstance;
}

CComponent * CTransform::Clone(void * pArg)
{
	CTransform*	pInstance = new CTransform(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed To Creating CTransform");
		SafeRelease(pInstance);
	}
	return pInstance;
}

void CTransform::Free()
{
	__super::Free();
}
