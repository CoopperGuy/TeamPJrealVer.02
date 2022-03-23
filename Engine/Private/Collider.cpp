#include "EnginePCH.h"
#include "..\Public\Collider.h"
#include "Engine.h"
#include "DmgVIBuffer.h"
USING(Engine)

CCollider::CCollider(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
	: CComponent(pDevice, pDeviceContext)
	, m_pEngine(CEngine::GetInstance())
	, m_quaternion(0.f,0.f,0.f,0.f)
	, m_euler(0.f,0.f,0.f)
	, m_Size(1.f, 1.f, 1.f)

{
}

CCollider::CCollider(const CCollider & rhs)
	: CComponent(rhs)
	, m_pEngine(CEngine::GetInstance())
	, m_RigidBodyDesc(rhs.m_RigidBodyDesc)
	, m_quaternion(rhs.m_quaternion)
	, m_euler(rhs.m_euler)
	, m_Size(rhs.m_Size)
{
}

void CCollider::Free()
{
	__super::Free();
	if (m_isCloned)
		SafeRelease(m_pDebugLine);
	if (m_pRigidActor)
	{
		PhysxMutex.lock();
		m_pRigidActor->release();
		m_pRigidActor = nullptr;
		PhysxMutex.unlock();
	}
}

HRESULT CCollider::InitializePrototype()
{
	ZeroMemory(&m_RigidBodyDesc, sizeof(m_RigidBodyDesc));
	return S_OK;
}

HRESULT CCollider::Initialize(void * pArg)
{
	if (pArg)
		m_pObjTransform = (CTransform*)pArg;

	return S_OK;
}

_uint CCollider::LateUpdate(double deltaTime)
{
	CModel* pModel = nullptr;
	CTransform* pRender = static_cast<CTransform*>(m_pMaster->GetComponent("Com_RenderTransform"));

	if (CEngine::GetInstance()->GetCurrentUsage() == CEngine::USAGE::USAGE_CLIENT)
	{
		if (m_pController)
		{
			/* Adjusting mesh position */


			pModel = static_cast<CModel*>(m_pMaster->GetComponent("Com_Model"));
			if (pModel)
			{
				PxExtendedVec3 physxPos = m_pController->getFootPosition();


				//if (pModel->Get_isChangedAnimation()) {
				//	//cout << m_pMaster->GetName() << "change\n";
				//	m_pObjTransform->SetPxPosition(physxPos);
				//	pRender->SetMatrix(m_pObjTransform->GetWorldMatrix());
				//}

				if (pModel->Get_Root(true))
				{
					//caluate RootAnimation Position
					_vector rootScale, rootRotation, rootPosition;
					if(m_pMaster->GetName() != "Ursa")
						XMMatrixDecompose(&rootScale, &rootRotation, &rootPosition, pModel->Get_BoneMatrix("Root"));
					else
						XMMatrixDecompose(&rootScale, &rootRotation, &rootPosition, pModel->Get_BoneWithoutOffset("Root"));
					rootPosition = XMVectorSetY(rootPosition, 0.f);

					// root animation correction when detected collision
					_vector vecPhysxPos = XMVectorSet((_float)physxPos.x, (_float)physxPos.y, (_float)physxPos.z, 1.f);
					_vector realPos =  XMLoadFloat3(&m_preFramePosition);

					_vector physxLength = XMVector3Length(XMVector3TransformCoord(rootPosition, m_pObjTransform->GetWorldMatrix()) - vecPhysxPos);
					_vector realLength = XMVector3Length(XMVector3TransformCoord(rootPosition, m_pObjTransform->GetWorldMatrix())  - realPos);

					if (XMVectorGetX(realLength) < XMVectorGetX(physxLength) && fabs(XMVectorGetX(realLength) - XMVectorGetX(physxLength)) > 0.001f) {
						_vector physx_ThisGap = (XMVectorSetY(realPos, 0.f) - XMVectorSetY(vecPhysxPos, 0.f));
						pRender->SetState(CTransform::STATE_POSITION, pRender->GetState(CTransform::STATE_POSITION) - physx_ThisGap);
						//m_pObjTransform->SetState(CTransform::STATE_POSITION, vecPhysxPos);
						XMStoreFloat3(&m_preFramePosition, vecPhysxPos);
						_vector correctPos = XMVector3TransformCoord(rootPosition, pRender->GetWorldMatrix());
						m_pObjTransform->SetState(CTransform::STATE_POSITION, correctPos);

						XMStoreFloat4x4(&m_pxMat, m_pObjTransform->GetWorldMatrix());
						PxExtendedVec3 centerPos = m_pController->getPosition();
						_float3 cenPos = _float3{ (_float)centerPos.x,(_float)centerPos.y,(_float)centerPos.z };
						memcpy(&m_pxMat.m[3], &cenPos, sizeof(_float3));
					}
					else {
						//root anim none collision
						_vector correctPos = XMVector3TransformCoord(rootPosition, pRender->GetWorldMatrix());
						m_pObjTransform->SetState(CTransform::STATE_POSITION, correctPos);


						_vector frameGap = correctPos - XMLoadFloat3(&m_preFramePosition);
						PxVec3 moveRoot = PxVec3(XMVectorGetX(frameGap), XMVectorGetY(frameGap), XMVectorGetZ(frameGap));
						m_pController->move(moveRoot, 0.00001f, (_float)deltaTime, nullptr);

						XMStoreFloat4x4(&m_pxMat, m_pObjTransform->GetWorldMatrix());
						PxExtendedVec3 centerPos = m_pController->getPosition();
						_float3 cenPos = _float3{ (_float)centerPos.x,(_float)centerPos.y,(_float)centerPos.z };
						memcpy(&m_pxMat.m[3], &cenPos, sizeof(_float3));
					}
					

					//_vector physx_ThisGap = (XMVectorSetY(realPos,0.f) - XMVectorSetY(vecPhysxPos, 0.f));
					//if (XMVectorGetX(XMVector3Length(physx_ThisGap)) != 0) {
					//	pRender->SetState(CTransform::STATE_POSITION, pRender->GetState(CTransform::STATE_POSITION) - physx_ThisGap);
					//	//m_pObjTransform->SetState(CTransform::STATE_POSITION, vecPhysxPos);
					//	XMStoreFloat3(&m_preFramePosition, vecPhysxPos);
					//	_vector correctPos = XMVector3TransformCoord(rootPosition, pRender->GetWorldMatrix());
					//	m_pObjTransform->SetState(CTransform::STATE_POSITION, correctPos);

					//	XMStoreFloat4x4(&m_pxMat, m_pObjTransform->GetWorldMatrix());
					//	PxExtendedVec3 centerPos = m_pController->getPosition();
					//	_float3 cenPos = _float3{ (_float)centerPos.x,(_float)centerPos.y,(_float)centerPos.z };
					//	memcpy(&m_pxMat.m[3], &cenPos, sizeof(_float3));
					//}
					//else {
					//	//root anim none collision
					//	_vector correctPos = XMVector3TransformCoord(rootPosition, pRender->GetWorldMatrix());
					//	m_pObjTransform->SetState(CTransform::STATE_POSITION, correctPos);


					//	_vector frameGap = correctPos - XMLoadFloat3(&m_preFramePosition);
					//	PxVec3 moveRoot = PxVec3(XMVectorGetX(frameGap), XMVectorGetY(frameGap), XMVectorGetZ(frameGap));
					//	m_pController->move(moveRoot, 0.001f, (_float)deltaTime, nullptr);

					//	XMStoreFloat4x4(&m_pxMat, m_pObjTransform->GetWorldMatrix());
					//	PxExtendedVec3 centerPos = m_pController->getPosition();
					//	_float3 cenPos = _float3{ (_float)centerPos.x,(_float)centerPos.y,(_float)centerPos.z };
					//	memcpy(&m_pxMat.m[3], &cenPos, sizeof(_float3));
					//}
				}
				else
				{
					m_pObjTransform->SetPxPosition(physxPos);
					pRender->SetMatrix(m_pObjTransform->GetWorldMatrix());
					PxExtendedVec3 centerPos = m_pController->getPosition();
					_float3 cenPos = _float3{ (_float)centerPos.x,(_float)centerPos.y,(_float)centerPos.z };
					XMStoreFloat4x4(&m_pxMat, m_pObjTransform->GetWorldMatrix());
					EulerToQuaternion(m_euler, m_quaternion);
				}
				XMStoreFloat3(&m_preFramePosition, m_pObjTransform->GetState(CTransform::STATE_POSITION));
			}
		}
	

		/* Back to px transform */
		
		// Check if its dynamic or static
		else if (m_pRigidActor)
		{
			/* for static actor */
			PxTransform actorTransform = m_pRigidActor->getGlobalPose();
			actorTransform.p.x += m_vRelativePos.x;
			actorTransform.p.y += m_vRelativePos.y;
			actorTransform.p.z += m_vRelativePos.z;
			actorTransform.q = PxQuat(m_quaternion.x, m_quaternion.y, m_quaternion.z, m_quaternion.w);
			m_pRigidActor->setGlobalPose(actorTransform);
			pRender->SetMatrix(m_pObjTransform->GetWorldMatrix());
			EulerToQuaternion(m_euler, m_quaternion);
		}

	}
	else {
		if (m_pRigidActor)
		{
			PxTransform actorTransform = m_pRigidActor->getGlobalPose();
			actorTransform.p.x += m_vRelativePos.x;
			actorTransform.p.y += m_vRelativePos.y;
			actorTransform.p.z += m_vRelativePos.z;
			actorTransform.q = PxQuat(m_quaternion.x, m_quaternion.y, m_quaternion.z, m_quaternion.w);
			m_pRigidActor->setGlobalPose(actorTransform);
			pRender->SetMatrix(m_pObjTransform->GetWorldMatrix());
			EulerToQuaternion(m_euler, m_quaternion);
		}
		else {
			EulerToQuaternion(m_euler, m_quaternion);
		}
	}
	
	return _uint();
}

HRESULT CCollider::Render()
{
	if (m_pDebugLine)
	{
		if (CEngine::GetInstance()->GetCurrentUsage() == CEngine::USAGE::USAGE_CLIENT) {
			if (!m_pRigidActor) {
				dynamic_cast<CVIBuffer*>(m_pDebugLine)->RenderDebug(m_pxMat);
			}
			else {
				_vector setSize;
				_matrix scaleMat;
				if (m_pController) {
					PxExtendedVec3 pxPos = m_pController->getPosition();
					_vector pos = XMVectorSet((_float)pxPos.x, (_float)pxPos.y, (_float)pxPos.z, 1.f);
					_matrix mat = XMMatrixRotationQuaternion(XMLoadFloat4(&m_quaternion)) * XMMatrixTranslationFromVector(pos);
					if (dynamic_cast<CBoxCollider*>(this)) {
						setSize = XMVectorSet(m_Size.x, m_Size.y, m_Size.z, 1.f);
						scaleMat = XMMatrixScalingFromVector(setSize);
						mat = scaleMat * mat;
					}
					XMStoreFloat4x4(&m_pxMat, mat);
					dynamic_cast<CVIBuffer*>(m_pDebugLine)->RenderDebug(m_pxMat);
				}
			}
		}
		else {
			if (m_pRigidActor) {
				PxTransform actorTransform = m_pRigidActor->getGlobalPose();
				_vector pos = XMVectorSet(actorTransform.p.x, actorTransform.p.y, actorTransform.p.z, 1.f);
				_matrix mat = XMMatrixRotationQuaternion(XMLoadFloat4(&m_quaternion)) * XMMatrixTranslationFromVector(pos);
				
				XMStoreFloat4x4(&m_pxMat, mat);
				dynamic_cast<CVIBuffer*>(m_pDebugLine)->RenderDebug(m_pxMat);
			}
			else {
				_vector setSize;
				_matrix scaleMat;
				_vector pos = m_pObjTransform->GetState(CTransform::STATE_POSITION);
				_matrix mat = XMMatrixRotationQuaternion(XMLoadFloat4(&m_quaternion)) * XMMatrixTranslationFromVector(pos);
				if (dynamic_cast<CBoxCollider*>(this)) {
					setSize = XMVectorSet(m_Size.x, m_Size.y, m_Size.z, 1.f);
					scaleMat = XMMatrixScalingFromVector(setSize);
					mat = scaleMat * mat;
				}
				XMStoreFloat4x4(&m_pxMat, mat);
				dynamic_cast<CVIBuffer*>(m_pDebugLine)->RenderDebug(m_pxMat);

			}
		}
	}
	return S_OK;
}

_float3 CCollider::GetEuler()
{
	return _float3(XMConvertToDegrees(m_euler.x), XMConvertToDegrees(m_euler.y), XMConvertToDegrees(m_euler.z));
}

void CCollider::SetPosition(_float3 pos)
{
	if (m_pController) {
		PxExtendedVec3 movePos = PxExtendedVec3(pos.x, pos.y, pos.z);
		m_pController->setPosition(movePos);
	}
}

void CCollider::ReleaseController() {
	if (m_pController) {
		m_pController->release();
		m_pController = nullptr;
	}
}

void CCollider::SetEuler(_float3 euler)
{
	m_euler = _float3(XMConvertToRadians(euler.x), XMConvertToRadians(euler.y), XMConvertToRadians(euler.z));
}


void CCollider::QuaternionToEuler(_float4 quaternion, _float3 & euler)
{
	_double x, y, z, w;
	x = quaternion.x;
	y = quaternion.y;
	z = quaternion.z;
	w = quaternion.w;
	// roll (x-axis rotation)
	double sinr_cosp = 2 * (w * x + y * z);
	double cosr_cosp = 1 - 2 * (x * x + y * y);

	euler.y = (_float)std::atan2(sinr_cosp, cosr_cosp);

	// pitch (y-axis rotation)
	double sinp = 2 * (w * y - z * x);
	if (std::fabs(sinp) >= 1)
		euler.x = (_float)std::copysign(XM_PIDIV2, sinp); // use 90 degrees if out of range
	else
		euler.x = (_float)std::asin(sinp);

	// yaw (z-axis rotation)
	double siny_cosp = 2 * (w *z + x * y);
	double cosy_cosp = 1 - 2 * (y * y + z * z);
	euler.z = (_float)std::atan2(siny_cosp, cosy_cosp);
}

void CCollider::EulerToQuaternion(_float3 euler, _float4 & quaternion)
{
	XMStoreFloat4(&quaternion, XMQuaternionRotationRollPitchYawFromVector(XMLoadFloat3(&euler)));
}
