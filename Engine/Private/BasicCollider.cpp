#include "EnginePCH.h"
#include "..\public\BasicCollider.h"
#include "PipeLine.h"
#include "Engine.h"
#include "GameObject.h"
#include "Stat.h"
#include "DmgVIBuffer.h"

USING(Engine)

CBasicCollider::CBasicCollider(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
	: CComponent(pDevice, pDeviceContext)
{
	XMStoreFloat3(&m_Offset, XMVectorSet(0.f, 0.f, 0.f, 1.f));
}

CBasicCollider::CBasicCollider(const CBasicCollider & rhs)
	: CComponent(rhs)
	, m_eType(rhs.m_eType)
	, m_pEffect(rhs.m_pEffect)
	, m_pBatch(rhs.m_pBatch)
	, m_pBB(rhs.m_pBB)
	, m_pSphere(rhs.m_pSphere)
	, m_pInputLayOut(rhs.m_pInputLayOut)
	, m_isCollision(rhs.m_isCollision)
	, ActiveColdt(rhs.ActiveColdt)
	, Setdamage(rhs.Setdamage)
	, m_ColliderDesc(rhs.m_ColliderDesc)
	, m_vMax(rhs.m_vMax)
	, m_vMin(rhs.m_vMin)
	, m_strBoneName(rhs.m_strBoneName)
	, m_isAttachBone(rhs.m_isAttachBone)
	, m_CollisionType(rhs.m_CollisionType)
{
	memcpy(&m_Offset, &rhs.m_Offset, sizeof(_float3));
	memcpy(m_vPoint, rhs.m_vPoint, sizeof(_float3) * 8);
	SafeAddRef(m_pInputLayOut);
}

HRESULT CBasicCollider::InitializePrototype(TYPE eType)
{
	if (FAILED(__super::InitializePrototype()))
		return E_FAIL;

	m_eType = eType;

	switch (m_eType)
	{
	case TYPE_AABB: case TYPE_OBB:
		m_pBB = new BoundingBox(_float3(0.0f, 0.f, 0.f), _float3(0.1f, 0.1f, 0.1f));
		break;
	case TYPE_SPHERE:
		m_pSphere = new BoundingSphere(_float3(0.0f, 0.f, 0.f), 0.5f);
		break;
	}

	m_pEffect = new BasicEffect(m_pDevice);
	if (nullptr == m_pEffect)
		return E_FAIL;

	m_pEffect->SetVertexColorEnabled(true);

	m_pBatch = new BATCH(m_pDeviceContext);
	if (nullptr == m_pBatch)
		return E_FAIL;

	const void*	pShaderByteCodes = nullptr;
	size_t		ShaderByteCodeLength = 0;

	m_pEffect->GetVertexShaderBytecode(&pShaderByteCodes, &ShaderByteCodeLength);

	if (FAILED(m_pDevice->CreateInputLayout(DirectX::VertexPositionColor::InputElements, DirectX::VertexPositionColor::InputElementCount, pShaderByteCodes, ShaderByteCodeLength, &m_pInputLayOut)))
		return E_FAIL;



	return S_OK;
}

HRESULT CBasicCollider::InitializePrototype_Sphere(_fvector vCenter)
{
	if (FAILED(__super::InitializePrototype()))
		return E_FAIL;

	m_eType = TYPE_SPHERE;


	_float3 fCenter;
	XMStoreFloat3(&fCenter, vCenter);
	m_pSphere = new BoundingSphere(fCenter, 1.f);

	m_pEffect = new BasicEffect(m_pDevice);
	if (nullptr == m_pEffect)
		return E_FAIL;

	m_pEffect->SetVertexColorEnabled(true);

	m_pBatch = new BATCH(m_pDeviceContext);
	if (nullptr == m_pBatch)
		return E_FAIL;

	const void*	pShaderByteCodes = nullptr;
	size_t		ShaderByteCodeLength = 0;

	m_pEffect->GetVertexShaderBytecode(&pShaderByteCodes, &ShaderByteCodeLength);

	if (FAILED(m_pDevice->CreateInputLayout(DirectX::VertexPositionColor::InputElements, DirectX::VertexPositionColor::InputElementCount, pShaderByteCodes, ShaderByteCodeLength, &m_pInputLayOut)))
		return E_FAIL;


	return S_OK;
}

HRESULT CBasicCollider::Initialize(void * pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;
	_float3		vExtents, vCenter;

	m_isCollision = false;

	_float		fRadius;

	switch (m_eType)
	{
	case CBasicCollider::TYPE_AABB:
		vExtents = _float3(m_ColliderDesc.vSize.x * 0.5f, m_ColliderDesc.vSize.y * 0.5f, m_ColliderDesc.vSize.z * 0.5f);
		vCenter = _float3(0.f, 0.f, 0.f);
		m_pBB = new BoundingBox(vCenter, vExtents);

		m_vPoint[0] = _float3(m_pBB->Center.x - m_pBB->Extents.x, m_pBB->Center.y + m_pBB->Extents.y, m_pBB->Center.z - m_pBB->Extents.z);
		m_vPoint[1] = _float3(m_pBB->Center.x + m_pBB->Extents.x, m_pBB->Center.y + m_pBB->Extents.y, m_pBB->Center.z - m_pBB->Extents.z);
		m_vPoint[2] = _float3(m_pBB->Center.x + m_pBB->Extents.x, m_pBB->Center.y - m_pBB->Extents.y, m_pBB->Center.z - m_pBB->Extents.z);
		m_vPoint[3] = _float3(m_pBB->Center.x - m_pBB->Extents.x, m_pBB->Center.y - m_pBB->Extents.y, m_pBB->Center.z - m_pBB->Extents.z);

		m_vPoint[4] = _float3(m_pBB->Center.x - m_pBB->Extents.x, m_pBB->Center.y + m_pBB->Extents.y, m_pBB->Center.z + m_pBB->Extents.z);
		m_vPoint[5] = _float3(m_pBB->Center.x + m_pBB->Extents.x, m_pBB->Center.y + m_pBB->Extents.y, m_pBB->Center.z + m_pBB->Extents.z);
		m_vPoint[6] = _float3(m_pBB->Center.x + m_pBB->Extents.x, m_pBB->Center.y - m_pBB->Extents.y, m_pBB->Center.z + m_pBB->Extents.z);
		m_vPoint[7] = _float3(m_pBB->Center.x - m_pBB->Extents.x, m_pBB->Center.y - m_pBB->Extents.y, m_pBB->Center.z + m_pBB->Extents.z);

		m_vOriginPoint[0] = _float3(m_pBB->Center.x - m_pBB->Extents.x, m_pBB->Center.y + m_pBB->Extents.y, m_pBB->Center.z - m_pBB->Extents.z);
		m_vOriginPoint[1] = _float3(m_pBB->Center.x + m_pBB->Extents.x, m_pBB->Center.y + m_pBB->Extents.y, m_pBB->Center.z - m_pBB->Extents.z);
		m_vOriginPoint[2] = _float3(m_pBB->Center.x + m_pBB->Extents.x, m_pBB->Center.y - m_pBB->Extents.y, m_pBB->Center.z - m_pBB->Extents.z);
		m_vOriginPoint[3] = _float3(m_pBB->Center.x - m_pBB->Extents.x, m_pBB->Center.y - m_pBB->Extents.y, m_pBB->Center.z - m_pBB->Extents.z);

		m_vOriginPoint[4] = _float3(m_pBB->Center.x - m_pBB->Extents.x, m_pBB->Center.y + m_pBB->Extents.y, m_pBB->Center.z + m_pBB->Extents.z);
		m_vOriginPoint[5] = _float3(m_pBB->Center.x + m_pBB->Extents.x, m_pBB->Center.y + m_pBB->Extents.y, m_pBB->Center.z + m_pBB->Extents.z);
		m_vOriginPoint[6] = _float3(m_pBB->Center.x + m_pBB->Extents.x, m_pBB->Center.y - m_pBB->Extents.y, m_pBB->Center.z + m_pBB->Extents.z);
		m_vOriginPoint[7] = _float3(m_pBB->Center.x - m_pBB->Extents.x, m_pBB->Center.y - m_pBB->Extents.y, m_pBB->Center.z + m_pBB->Extents.z);

		break;

	case CBasicCollider::TYPE_OBB:
		vExtents = _float3(m_ColliderDesc.vSize.x * 0.5f, m_ColliderDesc.vSize.y * 0.5f, m_ColliderDesc.vSize.z * 0.5f);
		vCenter = _float3(0.f, 0.f, 0.f);
		m_pBB = new BoundingBox(vCenter, vExtents);

		m_vPoint[0] = _float3(m_pBB->Center.x - m_pBB->Extents.x, m_pBB->Center.y + m_pBB->Extents.y, m_pBB->Center.z - m_pBB->Extents.z);
		m_vPoint[1] = _float3(m_pBB->Center.x + m_pBB->Extents.x, m_pBB->Center.y + m_pBB->Extents.y, m_pBB->Center.z - m_pBB->Extents.z);
		m_vPoint[2] = _float3(m_pBB->Center.x + m_pBB->Extents.x, m_pBB->Center.y - m_pBB->Extents.y, m_pBB->Center.z - m_pBB->Extents.z);
		m_vPoint[3] = _float3(m_pBB->Center.x - m_pBB->Extents.x, m_pBB->Center.y - m_pBB->Extents.y, m_pBB->Center.z - m_pBB->Extents.z);

		m_vPoint[4] = _float3(m_pBB->Center.x - m_pBB->Extents.x, m_pBB->Center.y + m_pBB->Extents.y, m_pBB->Center.z + m_pBB->Extents.z);
		m_vPoint[5] = _float3(m_pBB->Center.x + m_pBB->Extents.x, m_pBB->Center.y + m_pBB->Extents.y, m_pBB->Center.z + m_pBB->Extents.z);
		m_vPoint[6] = _float3(m_pBB->Center.x + m_pBB->Extents.x, m_pBB->Center.y - m_pBB->Extents.y, m_pBB->Center.z + m_pBB->Extents.z);
		m_vPoint[7] = _float3(m_pBB->Center.x - m_pBB->Extents.x, m_pBB->Center.y - m_pBB->Extents.y, m_pBB->Center.z + m_pBB->Extents.z);


		m_vOriginPoint[0] = _float3(m_pBB->Center.x - m_pBB->Extents.x, m_pBB->Center.y + m_pBB->Extents.y, m_pBB->Center.z - m_pBB->Extents.z);
		m_vOriginPoint[1] = _float3(m_pBB->Center.x + m_pBB->Extents.x, m_pBB->Center.y + m_pBB->Extents.y, m_pBB->Center.z - m_pBB->Extents.z);
		m_vOriginPoint[2] = _float3(m_pBB->Center.x + m_pBB->Extents.x, m_pBB->Center.y - m_pBB->Extents.y, m_pBB->Center.z - m_pBB->Extents.z);
		m_vOriginPoint[3] = _float3(m_pBB->Center.x - m_pBB->Extents.x, m_pBB->Center.y - m_pBB->Extents.y, m_pBB->Center.z - m_pBB->Extents.z);

		m_vOriginPoint[4] = _float3(m_pBB->Center.x - m_pBB->Extents.x, m_pBB->Center.y + m_pBB->Extents.y, m_pBB->Center.z + m_pBB->Extents.z);
		m_vOriginPoint[5] = _float3(m_pBB->Center.x + m_pBB->Extents.x, m_pBB->Center.y + m_pBB->Extents.y, m_pBB->Center.z + m_pBB->Extents.z);
		m_vOriginPoint[6] = _float3(m_pBB->Center.x + m_pBB->Extents.x, m_pBB->Center.y - m_pBB->Extents.y, m_pBB->Center.z + m_pBB->Extents.z);
		m_vOriginPoint[7] = _float3(m_pBB->Center.x - m_pBB->Extents.x, m_pBB->Center.y - m_pBB->Extents.y, m_pBB->Center.z + m_pBB->Extents.z);

		break;

	case CBasicCollider::TYPE_SPHERE: //초기값을 세팅
								 // Center(0,0,0), Radius( 1.f ) {}
		vCenter = _float3(0.f, 0.f, 0.f);
		fRadius = 1.f;
		m_pSphere = new BoundingSphere(vCenter, fRadius);

		break;
	}

	return S_OK;
}

_bool CBasicCollider::Update_State(_fmatrix TransformMatrix)
{
	_matrix transform = Remove_Scale(TransformMatrix);
	
	if (m_eType == CBasicCollider::TYPE_AABB)
		XMStoreFloat4x4(&m_TransformMatrix, Remove_ScaleRotation(TransformMatrix));

	else if (m_eType == CBasicCollider::TYPE_OBB) {
		XMStoreFloat4x4(&m_TransformMatrix, XMMatrixScalingFromVector(XMLoadFloat3(&m_pBB->Extents)) * XMMatrixTranslation(m_Offset.x, m_Offset.y, m_Offset.z) * transform);
	}
	if (m_eType == CBasicCollider::TYPE_SPHERE)
		XMStoreFloat4x4(&m_TransformMatrix, TransformMatrix);

	CGameObjectManager::GetInstance()->AddOBBCollsionList(m_CollisionType, m_pMaster,this);
	return _bool();
}

_bool CBasicCollider::Collision_AABB(CBasicCollider * pTargetCollider)
{
	if (TYPE_AABB != m_eType ||
		TYPE_AABB != pTargetCollider->m_eType)
		return false;

	_vector		vSourMin, vSourMax, vDestMin, vDestMax;

	vSourMin = XMVector3TransformCoord(XMLoadFloat3(&m_vMin), XMLoadFloat4x4(&m_TransformMatrix));
	vSourMax = XMVector3TransformCoord(XMLoadFloat3(&m_vMax), XMLoadFloat4x4(&m_TransformMatrix));

	vDestMin = XMVector3TransformCoord(XMLoadFloat3(&pTargetCollider->m_vMin), XMLoadFloat4x4(&pTargetCollider->m_TransformMatrix));
	vDestMax = XMVector3TransformCoord(XMLoadFloat3(&pTargetCollider->m_vMax), XMLoadFloat4x4(&pTargetCollider->m_TransformMatrix));


	/* 너비비교 */
	if (max(XMVectorGetX(vSourMin), XMVectorGetX(vDestMin)) >
		min(XMVectorGetX(vSourMax), XMVectorGetX(vDestMax)))
	{
		m_isCollision = false;
		pTargetCollider->m_isCollision = false;
		return false;
	}

	/* 높이비교 */
	if (max(XMVectorGetY(vSourMin), XMVectorGetY(vDestMin)) >
		min(XMVectorGetY(vSourMax), XMVectorGetY(vDestMax)))
	{
		m_isCollision = false;
		pTargetCollider->m_isCollision = false;
		return false;
	}

	/* 깊이비교 */
	if (max(XMVectorGetZ(vSourMin), XMVectorGetZ(vDestMin)) >
		min(XMVectorGetZ(vSourMax), XMVectorGetZ(vDestMax)))
	{
		m_isCollision = false;
		pTargetCollider->m_isCollision = false;
		return false;
	}

	m_isCollision = true;
	pTargetCollider->m_isCollision = true;

	return true;
}

_bool CBasicCollider::Collision_OBB(CBasicCollider * pTargetCollider)
{
	_vector			vSourPoint[8], vDestPoint[8];

	for (_uint i = 0; i < 8; ++i)
	{
		vSourPoint[i] = XMVector3TransformCoord(XMLoadFloat3(&m_vPoint[i]), XMLoadFloat4x4(&m_TransformMatrix));
		vDestPoint[i] = XMVector3TransformCoord(XMLoadFloat3(&pTargetCollider->m_vPoint[i]), XMLoadFloat4x4(&pTargetCollider->m_TransformMatrix));
	}

	OBBDESC		ObbDesc[2];
	ObbDesc[0] = Compute_OBB(vSourPoint);
	ObbDesc[1] = Compute_OBB(vDestPoint);


	_float		fDistance[3] = { 0.0f };

	for (_uint i = 0; i < 2; ++i)
	{
		for (_uint j = 0; j < 3; ++j)
		{
			fDistance[0] = fabs(XMVectorGetX(XMVector3Dot(XMLoadFloat3(&ObbDesc[0].vCenterAxis[0]), XMLoadFloat3(&ObbDesc[i].vAlignAxis[j])))) +
				fabs(XMVectorGetX(XMVector3Dot(XMLoadFloat3(&ObbDesc[0].vCenterAxis[1]), XMLoadFloat3(&ObbDesc[i].vAlignAxis[j])))) +
				fabs(XMVectorGetX(XMVector3Dot(XMLoadFloat3(&ObbDesc[0].vCenterAxis[2]), XMLoadFloat3(&ObbDesc[i].vAlignAxis[j]))));

			fDistance[1] = fabs(XMVectorGetX(XMVector3Dot(XMLoadFloat3(&ObbDesc[1].vCenterAxis[0]), XMLoadFloat3(&ObbDesc[i].vAlignAxis[j])))) +
				fabs(XMVectorGetX(XMVector3Dot(XMLoadFloat3(&ObbDesc[1].vCenterAxis[1]), XMLoadFloat3(&ObbDesc[i].vAlignAxis[j])))) +
				fabs(XMVectorGetX(XMVector3Dot(XMLoadFloat3(&ObbDesc[1].vCenterAxis[2]), XMLoadFloat3(&ObbDesc[i].vAlignAxis[j]))));

			fDistance[2] = fabs(XMVectorGetX(XMVector3Dot(XMLoadFloat3(&ObbDesc[1].vCenter) - XMLoadFloat3(&ObbDesc[0].vCenter), XMLoadFloat3(&ObbDesc[i].vAlignAxis[j]))));

			if (fDistance[0] + fDistance[1] < fDistance[2])
			{
				m_isCollision = false;
				pTargetCollider->m_isCollision = false;
				SetupCollisionFlag(false);
				return false;
			}
		}
	}

	m_isCollision = true;
	pTargetCollider->m_isCollision = true;
	SetupCollisionFlag(true);
	return true;
}

_bool CBasicCollider::Collision_Sphere(CBasicCollider * pTargetCollider)
{
	_vector Center = { m_pSphere->Center.x - pTargetCollider->m_pSphere->Center.x
		, m_pSphere->Center.y - pTargetCollider->m_pSphere->Center.y
		, m_pSphere->Center.z - pTargetCollider->m_pSphere->Center.z };

	_float center;
	XMStoreFloat(&center, Center);

	if ((m_pSphere->Radius + pTargetCollider->m_pSphere->Radius) < center)
		return false;

	else if ((m_pSphere->Radius + pTargetCollider->m_pSphere->Radius) >= center)
	{
		m_isCollision = true;
		pTargetCollider->m_isCollision = true;
	}

	return true;
}

_bool CBasicCollider::Collision_OBBToReset(CBasicCollider * pMyCollider, CBasicCollider * pTargetCollider)
{
	_vector			vSourPoint[8], vDestPoint[8];

	for (_uint i = 0; i < 8; ++i)
	{
		vSourPoint[i] = XMVector3TransformCoord(XMLoadFloat3(&pMyCollider->m_vPoint[i]), XMLoadFloat4x4(&pMyCollider->m_TransformMatrix));
		vDestPoint[i] = XMVector3TransformCoord(XMLoadFloat3(&pTargetCollider->m_vPoint[i]), XMLoadFloat4x4(&pTargetCollider->m_TransformMatrix));
	}

	OBBDESC		ObbDesc[2];
	ObbDesc[0] = Compute_OBB(vSourPoint);
	ObbDesc[1] = Compute_OBB(vDestPoint);

	_float		fDistance[3] = { 0.0f };

	for (_uint i = 0; i < 2; ++i)
	{
		for (_uint j = 0; j < 3; ++j)
		{
			fDistance[0] = fabs(XMVectorGetX(XMVector3Dot(XMLoadFloat3(&ObbDesc[0].vCenterAxis[0]), XMLoadFloat3(&ObbDesc[i].vAlignAxis[j])))) +
				fabs(XMVectorGetX(XMVector3Dot(XMLoadFloat3(&ObbDesc[0].vCenterAxis[1]), XMLoadFloat3(&ObbDesc[i].vAlignAxis[j])))) +
				fabs(XMVectorGetX(XMVector3Dot(XMLoadFloat3(&ObbDesc[0].vCenterAxis[2]), XMLoadFloat3(&ObbDesc[i].vAlignAxis[j]))));

			fDistance[1] = fabs(XMVectorGetX(XMVector3Dot(XMLoadFloat3(&ObbDesc[1].vCenterAxis[0]), XMLoadFloat3(&ObbDesc[i].vAlignAxis[j])))) +
				fabs(XMVectorGetX(XMVector3Dot(XMLoadFloat3(&ObbDesc[1].vCenterAxis[1]), XMLoadFloat3(&ObbDesc[i].vAlignAxis[j])))) +
				fabs(XMVectorGetX(XMVector3Dot(XMLoadFloat3(&ObbDesc[1].vCenterAxis[2]), XMLoadFloat3(&ObbDesc[i].vAlignAxis[j]))));

			fDistance[2] = fabs(XMVectorGetX(XMVector3Dot(XMLoadFloat3(&ObbDesc[1].vCenter) - XMLoadFloat3(&ObbDesc[0].vCenter), XMLoadFloat3(&ObbDesc[i].vAlignAxis[j]))));

			if (fDistance[0] + fDistance[1] < fDistance[2])
			{
				pMyCollider->m_isCollision = false;
				pTargetCollider->m_isCollision = false;
				pMyCollider->SetupCollisionFlag(false);
				pTargetCollider->SetupCollisionFlag(false);
				return false;
			}
		}
	}
	return true;
}

_bool CBasicCollider::Collision_OBB(CBasicCollider * pMyCollider, CBasicCollider * pTargetCollider)
{
	_vector			vSourPoint[8], vDestPoint[8];

	for (_uint i = 0; i < 8; ++i)
	{
		vSourPoint[i] = XMVector3TransformCoord(XMLoadFloat3(&pMyCollider->m_vPoint[i]), XMLoadFloat4x4(&pMyCollider->m_TransformMatrix));
		vDestPoint[i] = XMVector3TransformCoord(XMLoadFloat3(&pTargetCollider->m_vPoint[i]), XMLoadFloat4x4(&pTargetCollider->m_TransformMatrix));
	}

	OBBDESC		ObbDesc[2];
	ObbDesc[0] = Compute_OBB(vSourPoint);
	ObbDesc[1] = Compute_OBB(vDestPoint);

	_float		fDistance[3] = { 0.0f };

	for (_uint i = 0; i < 2; ++i)
	{
		for (_uint j = 0; j < 3; ++j)
		{
			fDistance[0] = fabs(XMVectorGetX(XMVector3Dot(XMLoadFloat3(&ObbDesc[0].vCenterAxis[0]), XMLoadFloat3(&ObbDesc[i].vAlignAxis[j])))) +
				fabs(XMVectorGetX(XMVector3Dot(XMLoadFloat3(&ObbDesc[0].vCenterAxis[1]), XMLoadFloat3(&ObbDesc[i].vAlignAxis[j])))) +
				fabs(XMVectorGetX(XMVector3Dot(XMLoadFloat3(&ObbDesc[0].vCenterAxis[2]), XMLoadFloat3(&ObbDesc[i].vAlignAxis[j]))));

			fDistance[1] = fabs(XMVectorGetX(XMVector3Dot(XMLoadFloat3(&ObbDesc[1].vCenterAxis[0]), XMLoadFloat3(&ObbDesc[i].vAlignAxis[j])))) +
				fabs(XMVectorGetX(XMVector3Dot(XMLoadFloat3(&ObbDesc[1].vCenterAxis[1]), XMLoadFloat3(&ObbDesc[i].vAlignAxis[j])))) +
				fabs(XMVectorGetX(XMVector3Dot(XMLoadFloat3(&ObbDesc[1].vCenterAxis[2]), XMLoadFloat3(&ObbDesc[i].vAlignAxis[j]))));

			fDistance[2] = fabs(XMVectorGetX(XMVector3Dot(XMLoadFloat3(&ObbDesc[1].vCenter) - XMLoadFloat3(&ObbDesc[0].vCenter), XMLoadFloat3(&ObbDesc[i].vAlignAxis[j]))));

			if (fDistance[0] + fDistance[1] < fDistance[2])
			{
				pMyCollider->m_isCollision = false;
				pTargetCollider->m_isCollision = false;
				pMyCollider->SetupCollisionFlag(false);
				pTargetCollider->SetupCollisionFlag(false);
				return false;
			}
		}
	}

	pMyCollider->m_isCollision = true;
	pTargetCollider->m_isCollision = true;
	pMyCollider->SetupCollisionFlag(true);
	pTargetCollider->SetupCollisionFlag(true);

	return true;
}

void CBasicCollider::Collision_PlayerToTarget(CBasicCollider * pMyCollider, list<OBJCOLLIDER>& _TargetObj)
{

	for (auto& TargetObj : _TargetObj)
	{

		CBasicCollider * pTargetCollider = TargetObj.second;
		if (pTargetCollider == nullptr)
		{
			MSG_BOX("CBaiscCollider :: CollisionCheckObj > pTargetCollider is nullptr");
			return;
		}


		if (Collision_OBB(pMyCollider, pTargetCollider))
		{

		}
	}

}

void CBasicCollider::CollisionWeaponeToTarget(list<OBJCOLLIDER>& pMyCollider, list<OBJCOLLIDER>& _TargetObj)
{

	for (auto& MyObj : pMyCollider)
	{

		for (auto& TargetObj : _TargetObj)
		{
			CBasicCollider * pWeaponeCollider = MyObj.second;
			if (pWeaponeCollider == nullptr)
			{
				MSG_BOX("CBaiscCollider :: CollisionCheckObj > pMyCollider is nullptr");
				return;
			}

			CBasicCollider * pTargetCollider = TargetObj.second;
			if (pTargetCollider == nullptr)
			{
				MSG_BOX("CBaiscCollider :: CollisionCheckObj > pTargetCollider is nullptr");
				return;
			}

			CStat* TargetpStat = static_cast<CStat*>(TargetObj.first->GetComponent("Com_Stat"));
			if (TargetpStat == nullptr)
				return;
			CGameObject * m_pGameObject = CEngine::GetInstance()->FindGameObjectWithName(0, "Player");
			CModel * pModel = static_cast<CModel*>(m_pGameObject->GetComponent("Com_Model"));
			CComponent* ComPlayer = m_pGameObject->GetComponent("Com_Player");
			CStat* PlayerStat = static_cast<CStat*>(m_pGameObject->GetComponent("Com_Stat"));
			if (PlayerStat == nullptr)
				return;

			if (static_cast<CStat*>(TargetpStat)->GetStatInfo().hp <= 0)
				return;
			else
			{
				if (PlayerStat->GetSTATES() == CStat::STATES_ATK)
				{
					if (pTargetCollider->Collision_OBB(pWeaponeCollider, pTargetCollider))
					{
						if (TargetpStat->GetStatInfo().isImmortal == true)
							return;
						if (pTargetCollider->GetCollisionFlag() == CBasicCollider::COLLISION_FOUND) {
							//_float Playeratk = static_cast<CStat*>(PlayerStat)->GetStatInfo().atk;
							pTargetCollider->SetHit(true);
							static_cast<CStat*>(TargetpStat)->Damaged(PlayerStat, true);
							//cout << "HP:" << static_cast<CStat*>(TargetpStat)->GetStatInfo().hp << endl;
							return;
						}
						else
							pTargetCollider->SetHit(false);
					}
					else
						pTargetCollider->SetHit(false);

				}
				else {
					pTargetCollider->Collision_OBBToReset(pWeaponeCollider, pTargetCollider);
				}
			}
		}
	}
}

void CBasicCollider::Collision_MonsterWeaponToPlayer(list<OBJCOLLIDER>& pMyCollider, list<OBJCOLLIDER>& _TargetObj)
{
	for (auto& MyObj : pMyCollider)
	{
		for (auto& TargetObj : _TargetObj)
		{
			CBasicCollider * pMyCollider = MyObj.second;
			if (pMyCollider == nullptr)
			{
				MSG_BOX("CBaiscCollider :: CollisionCheckObj > Mycollider is nullptr");
				return;
			}

			CBasicCollider * pTargetCollider = TargetObj.second;
			if (pTargetCollider == nullptr)
			{
				MSG_BOX("CBaiscCollider :: CollisionCheckObj > pTargetCollider is nullptr");
				return;
			}

			CStat* TargetpStat = static_cast<CStat*>(TargetObj.first->GetComponent("Com_Stat"));
			CStat* MyStat =static_cast<CStat*>(MyObj.first->GetComponent("Com_Stat"));

			if (TargetpStat == nullptr || MyStat == nullptr)
				return;

			if (static_cast<CStat*>(TargetpStat)->GetStatInfo().hp <= 0)
			{
				return;
			}

			if (MyStat->GetSTATES() == CStat::STATES_ATK) {
				if (pMyCollider->Collision_OBB(pMyCollider, pTargetCollider))
				{
					if (TargetpStat->GetStatInfo().isImmortal == true)
						return;
					if (pMyCollider->GetCollisionFlag() == COLLISIONTYPE::COLLISION_FOUND) {
						pTargetCollider->SetHit(true);
						if (static_cast<CStat*>(TargetpStat)->Damaged(static_cast<CStat*>(MyStat), false))
						{
							
							cout << static_cast<CStat*>(TargetpStat)->GetStatInfo().hp << endl;
							return;
						}
					}
					else
						pTargetCollider->SetHit(false);
				}
				else
					pTargetCollider->SetHit(false);
			}
		}
	}
}

void CBasicCollider::Collision_CheckObj(list<OBJCOLLIDER>& _MyObj, list<OBJCOLLIDER>& _TargetObj)
{
	for (auto& MyObj : _MyObj)
	{
		for (auto& TargetObj : _TargetObj)
		{
			CBasicCollider * pMyCollider = MyObj.second;
			if (pMyCollider == nullptr)
			{
				MSG_BOX("CBaiscCollider :: CollisionCheckObj > Mycollider is nullptr");
				return;
			}

			CBasicCollider * pTargetCollider = TargetObj.second;
			if (pTargetCollider == nullptr)
			{
				MSG_BOX("CBaiscCollider :: CollisionCheckObj > pTargetCollider is nullptr");
				return;
			}

			if (pMyCollider->Get_Collision() && pTargetCollider->Get_Collision())
				return;

			CComponent* TargetpStat = TargetObj.first->GetComponent("Com_Stat");
			CComponent* MyStat = MyObj.first->GetComponent("Com_Stat");

			if (TargetpStat == nullptr || MyStat == nullptr)
				return;

			if (static_cast<CStat*>(TargetpStat)->GetStatInfo().hp <= 0 || static_cast<CStat*>(MyStat)->GetStatInfo().hp <= 0)
			{
				return;
			}

			if (Collision_OBB(pMyCollider, pTargetCollider))
			{
				if (static_cast<CStat*>(TargetpStat)->Damaged(static_cast<CStat*>(MyStat)))
				{
					cout << static_cast<CStat*>(TargetpStat)->GetStatInfo().hp << endl;
					return;
				}
			}
		}
	}
}


HRESULT CBasicCollider::Render()
{
	if (nullptr == m_pEffect)
		return E_FAIL;

	m_pEffect->SetWorld(XMLoadFloat4x4(&m_TransformMatrix));


	_matrix view = CEngine::GetInstance()->GetTransform(CPipeline::D3DTS_VIEW);
	_matrix Proj = CEngine::GetInstance()->GetTransform(CPipeline::D3DTS_PROJ);

	m_pEffect->SetView(CEngine::GetInstance()->GetTransform(CPipeline::D3DTS_VIEW));
	m_pEffect->SetProjection(CEngine::GetInstance()->GetTransform(CPipeline::D3DTS_PROJ));

	m_pDeviceContext->IASetInputLayout(m_pInputLayOut);
	m_pEffect->Apply(m_pDeviceContext);

	_vector		vColor = m_isCollision == true ? DirectX::Colors::Red : DirectX::Colors::Green;

	m_pBatch->Begin();

	switch (m_eType)
	{
	case CBasicCollider::TYPE_AABB: case CBasicCollider::TYPE_OBB:
		DX::Draw(m_pBatch, *m_pBB, vColor);
		break;

	case CBasicCollider::TYPE_SPHERE:
		DX::Draw(m_pBatch, *m_pSphere, vColor);
		break;
	}

	m_pBatch->End();


	return S_OK;
}

void CBasicCollider::SetupCollisionFlag(_bool tf)
{
	switch (tf)
	{

	case true: 
	{
		switch (m_CollisionFlag)
		{
		case Engine::CBasicCollider::COLLSION_NONE:
			m_CollisionFlag = COLLISION_FOUND;
			break;
		case Engine::CBasicCollider::COLLISION_FOUND:
			m_CollisionFlag = COLLISION;
			break;
		case Engine::CBasicCollider::COLLISION:
			m_CollisionFlag = COLLISION;
			break;
		case Engine::CBasicCollider::COLLISION_LOST:
			m_CollisionFlag = COLLISION_FOUND;
			break;
		case Engine::CBasicCollider::COLLISION_END:
			break;
		default:
			break;
		}
		break;
	}
	case false: 
	{
		switch (m_CollisionFlag)
		{
		case Engine::CBasicCollider::COLLSION_NONE:
			m_CollisionFlag = COLLSION_NONE;
			break;
		case Engine::CBasicCollider::COLLISION_FOUND:
			m_CollisionFlag = COLLISION_LOST;
			break;
		case Engine::CBasicCollider::COLLISION:
			m_CollisionFlag = COLLISION_LOST;
			break;
		case Engine::CBasicCollider::COLLISION_LOST:
			m_CollisionFlag = COLLSION_NONE;
			break;
		case Engine::CBasicCollider::COLLISION_END:
			break;
		default:
			break;
		}
		break;
	}

	}
}

_fmatrix CBasicCollider::Remove_ScaleRotation(_fmatrix TransformMatrix)
{
	_matrix			NonRotateMatrix = XMMatrixIdentity();

	NonRotateMatrix.r[3] = TransformMatrix.r[3];

	return NonRotateMatrix;
}

_fmatrix CBasicCollider::Remove_Scale(_fmatrix TransformMatrix)
{
	_matrix			NonScaleMatrix = Remove_ScaleRotation(TransformMatrix);

	NonScaleMatrix.r[0] = XMVector3Normalize(TransformMatrix.r[0]);
	NonScaleMatrix.r[1] = XMVector3Normalize(TransformMatrix.r[1]);
	NonScaleMatrix.r[2] = XMVector3Normalize(TransformMatrix.r[2]);

	return NonScaleMatrix;
}


CBasicCollider::OBBDESC CBasicCollider::Compute_OBB(_fvector * pPoints)
{
	OBBDESC			OBBDesc;
	ZeroMemory(&OBBDesc, sizeof(OBBDESC));

	XMStoreFloat3(&OBBDesc.vCenter, (pPoints[0] + pPoints[6]) * 0.5f);
	
	XMStoreFloat3(&OBBDesc.vAlignAxis[0], XMVector3Normalize(pPoints[2] - pPoints[3]));
	XMStoreFloat3(&OBBDesc.vAlignAxis[1], XMVector3Normalize(pPoints[2] - pPoints[1]));
	XMStoreFloat3(&OBBDesc.vAlignAxis[2], XMVector3Normalize(pPoints[2] - pPoints[6]));

	XMStoreFloat3(&OBBDesc.vCenterAxis[0], ((pPoints[5] + pPoints[2]) * 0.5f) - XMLoadFloat3(&OBBDesc.vCenter));
	XMStoreFloat3(&OBBDesc.vCenterAxis[1], ((pPoints[5] + pPoints[0]) * 0.5f) - XMLoadFloat3(&OBBDesc.vCenter));
	XMStoreFloat3(&OBBDesc.vCenterAxis[2], ((pPoints[5] + pPoints[7]) * 0.5f) - XMLoadFloat3(&OBBDesc.vCenter));

	return OBBDesc;
}


void CBasicCollider::Set_Radius(_float vRadius)
{
	m_pSphere->Radius = vRadius;
}

void CBasicCollider::OBBOnEnter(CGameObject* pTarget)
{
	m_CollState = START;
	CComponent* pStat = pTarget->GetComponent("Com_Stat");
	static_cast<CStat*>(pStat)->Damaged(static_cast<CStat*>(pStat));
	cout << "---------------------" << endl;
	cout << "충돌" << endl;
	cout << static_cast<CStat*>(pStat)->GetStatInfo().hp << endl;
	cout << static_cast<CStat*>(pStat)->GetHpPercentage() << endl;
	cout << "---------------------" << endl;
	m_CollState = ING;
}

void CBasicCollider::OBBOnStay()
{
}

void CBasicCollider::OBBOnExit()
{
}

void CBasicCollider::ClearList()
{
}

_vector* CBasicCollider::GetObbBox()
{
	_vector vSourPoint[8];
	
	for (_uint i = 0; i < 8; ++i)
	{
		vSourPoint[i] = XMVector3TransformCoord(XMLoadFloat3(&m_vPoint[i]), XMLoadFloat4x4(&m_TransformMatrix));
	}

	return vSourPoint;
}

CBasicCollider * CBasicCollider::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext, TYPE eType)
{
	CBasicCollider*	pInstance = new CBasicCollider(pDevice, pDeviceContext);

	if (FAILED(pInstance->InitializePrototype(eType)))
	{
		MSG_BOX("Failed To Creating CBasicCollider");
		SafeRelease(pInstance);
	}
	return pInstance;
}

CBasicCollider * CBasicCollider::Create_Sphere(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext, _fvector vCenter)
{
	CBasicCollider*	pInstance = new CBasicCollider(pDevice, pDeviceContext);

	if (FAILED(pInstance->InitializePrototype_Sphere(vCenter)))
	{
		MSG_BOX("Failed To Creating CBasicCollider");
		SafeRelease(pInstance);
	}
	return pInstance;
}

CComponent * CBasicCollider::Clone(void * pArg)
{
	CBasicCollider*	pInstance = new CBasicCollider(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed To Creating CBasicCollider");
		SafeRelease(pInstance);
	}
	return pInstance;
}

void CBasicCollider::Free()
{
	__super::Free();

	if (false == m_isCloned)
	{
		SafeDelete(m_pEffect);
		SafeDelete(m_pBatch);
	}

	SafeDelete(m_pBB);
	SafeDelete(m_pSphere);


	SafeRelease(m_pInputLayOut);
}
