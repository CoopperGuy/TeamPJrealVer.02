#include "stdafx.h"
#include "..\public\Camera_Fly.h"
#include "Engine.h"
#include "EventCheck.h"

USING(Client)
CCamera_Fly::CCamera_Fly(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
	: CCamera(pDevice, pDeviceContext)
{
}

CCamera_Fly::CCamera_Fly(const CCamera_Fly & rhs)
	: CCamera(rhs)
{
}

HRESULT CCamera_Fly::InitializePrototype()
{
	if (FAILED(__super::InitializePrototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CCamera_Fly::Initialize(void * pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;
	CGameObject* pGameObject = CEngine::GetInstance()->FindGameObjectWithName(SCENE_STATIC, "Player");
	if (pGameObject)
	{
		m_pTargetCollider = dynamic_cast<CCollider*>(pGameObject->GetComponent("Com_Collider"));
		m_pTargetTransform = dynamic_cast<CTransform*>(pGameObject->GetComponent("Com_Transform"));
		XMStoreFloat3(&m_vLook, m_pTargetTransform->GetState(CTransform::STATE_LOOK));
	}
	CEventCheck::GetInstance()->SetCameraFly(this);
	return S_OK;
}

_uint CCamera_Fly::Update(_double TimeDelta)
{

	if (nullptr == m_pTransformCom)
		return -1;

	PhysxCameraCollision(TimeDelta);

	// 마우스 중앙 고정
	if (g_Menu == false && g_AnotherMenu == false)
	{
		POINT		ptMouse = { g_iWinCX >> 1, g_iWinCY >> 1 };
		ClientToScreen(g_hWnd, &ptMouse);
		SetCursorPos(ptMouse.x, ptMouse.y);
	}

	//// Q || E 누르면 m_pTargetLook을 회전시긴다
	if (!m_pTargetTransform)
	{
		if (GetActiveWindow() == g_hWnd && m_bRolling)
		{
			if (m_pEngine->IsKeyPressed(VK_UP))
			{
				m_pTransformCom->GoStraight(TimeDelta * 2.f);
			}

			if (m_pEngine->IsKeyPressed(VK_DOWN))
			{
				m_pTransformCom->GoBackWard(TimeDelta * 2.f);
			}

			if (m_pEngine->IsKeyPressed(VK_LEFT))
			{
				m_pTransformCom->GoLeft(TimeDelta * 2.f);
			}

			if (m_pEngine->IsKeyPressed(VK_RIGHT))
			{
				m_pTransformCom->GoRight(TimeDelta * 2.f);
			}

			if (m_pEngine->GetMouseMoveValue().z > 0)
				m_pTransformCom->GoStraight(TimeDelta * 3.f);
			if (m_pEngine->GetMouseMoveValue().z < 0)
				m_pTransformCom->GoBackWard(TimeDelta * 3.f);

			_long			MouseMove = 0;

			if (m_pEngine->IsMousePressed(2))
			{
				if (MouseMove = (long)m_pEngine->GetMouseMoveValue().x)
					if (MouseMove = m_pEngine->Get_MouseMoveState(CInput_Device::MMS_X))
						m_pTransformCom->RotateAxis(XMVectorSet(0.f, 1.f, 0.f, 0.f), MouseMove * TimeDelta * 3.f);

				if (MouseMove = (long)m_pEngine->GetMouseMoveValue().y)
					if (MouseMove = m_pEngine->Get_MouseMoveState(CInput_Device::MMS_Y))
						m_pTransformCom->RotateAxis(m_pTransformCom->GetState(CTransform::STATE_RIGHT), MouseMove * TimeDelta * 3.f);
			}
		}
	}
	else {
		Set_Pos(TimeDelta);
	}
	__super::Update(TimeDelta);
	return _uint();
}

_uint CCamera_Fly::LateUpdate(_double TimeDelta)
{
	if (0 > __super::LateUpdate(TimeDelta))
		return -1;
	return _uint();
}

void CCamera_Fly::ShakeCamera(SHAKE _shake, _int _cnt, _float _duration, _float _spd)
{
	m_eShaking = _shake;
	m_fDuration = _duration;
	m_iCnt = _cnt;
	m_fSpd = _spd;
	switch (m_eShaking)
	{
	case Client::CCamera_Fly::SHAKE_RIGHT:
	{
		m_fDirection = 1.f;
	}
	break;
	case Client::CCamera_Fly::SHAKE_LEFT:
	{
		m_fDirection = -1.f;
	}
	break;
	case Client::CCamera_Fly::SHAKE_ING:
	{
		m_fDirection = -1.f;
	}
	break;
	}
}

void CCamera_Fly::ShakeLookCamera(SHAKE _shake, _fvector _look, _float _duration)
{
	switch (_shake)
	{
	case Client::CCamera_Fly::SHAKE_RIGHT:
	{

	}
	break;
	case Client::CCamera_Fly::SHAKE_LEFT:
	{

	}
	break;
	case Client::CCamera_Fly::SHAKE_ING:
	{

	}
	break;
	}
}

void CCamera_Fly::ShakeLook(_fvector _look, _float _duration)
{
	XMStoreFloat3(&m_vShakeLook, _look);
	m_bShake = true;
}

void CCamera_Fly::ShakePosition(_fvector _pos, _float _duration)
{
	_vector cameraPos = m_pTransformCom->GetState(CTransform::STATE_POSITION);
	_vector look = m_pTransformCom->GetState(CTransform::STATE_LOOK);

	XMStoreFloat3(&m_vShakeLook, XMVector3Normalize(_pos - cameraPos));
	//XMStoreFloat3(&m_vShakeLook, DirectX::XMVectorLerp(XMLoadFloat3(&m_vShakeLook), look,  0.5f));

	m_bShake = true;
}

void CCamera_Fly::ShakeUpDown(_int _cnt, _float _duration, _float _spd)
{
	m_iYCnt = _cnt;
	m_fYSpd = _spd;
	m_fYDuration = _duration;
	m_bYShake = true;
	m_fYDirection = 1.f;
}

void CCamera_Fly::PhysxCameraCollision(_double deltaTime)
{
	if (CEngine::GetInstance()->GetCurSceneNumber() < SCENE_GAMEPLAY)
		return;
	PxExtendedVec3 physxPos = m_pTargetCollider->GetPosition();
	_vector vTargetCenterPos = XMVectorSet((_float)physxPos.x, (_float)physxPos.y, (_float)physxPos.z, 1.f);
	m_fCameraCollisionDelta += (_float)deltaTime;
	/*if (m_fCameraCollisionTime < m_fCameraCollisionDelta)
	{*/
	_vector vCamPos = m_pTransformCom->GetState(CTransform::STATE_POSITION);
	_vector dir = vTargetCenterPos - vCamPos;
	_vector vRayDir = XMVector3Normalize(dir);
	PxRaycastBuffer buf;
	PxQueryFilterData filterData;
	filterData.data.word1 = CPxManager::GROUP1;
	filterData.data.word1 = CPxManager::GROUP4;
	filterData.flags |= PxQueryFlag::eSTATIC;
	_vector length = XMVector3Length(dir);
	if (CEngine::GetInstance()->Raycast(vCamPos, vRayDir, 100.f, buf, filterData))
	{
		if ((XMVectorGetX(length) - buf.getAnyHit(0).distance) > 0.1f)
		{
			//cout << "camera Collied \n";
			m_vDistance -= (_float)deltaTime * m_fCameraColliedSpd;
			if (m_vDistance <= 0.2f)
				m_vDistance = 0.2f;
		}
		/*	else
				cout << "camera No Collied \n";*/
	}
	/*	m_fCameraCollisionDelta = 0;
	}*/
}

HRESULT CCamera_Fly::SetUp_Components()
{
	return S_OK;
}

void CCamera_Fly::Set_Pos(_double DeltaTime)
{
	PxExtendedVec3 tempPos = m_pTargetCollider->GetPosition();
	_float3	physxPos = _float3((_float)tempPos.x, (_float)tempPos.y, (_float)tempPos.z);
	_float3 lockOnPos = CEventCheck::GetInstance()->GetLockOnPos();
	_vector lerpTargetCenterPos = XMVectorLerp(XMLoadFloat3(&physxPos), XMLoadFloat3(&lockOnPos), DeltaTime * 10.f);
	_vector vTargetCenterPos = lerpTargetCenterPos + XMLoadFloat3(&m_vShakePos);
	_matrix mat = XMMatrixIdentity();
	_long			MouseMove = 0;
	_long			MouseWheel = 0;
	//_vector scale, rotation, position;
	//DirectX::XMMatrixDecompose(&scale, &rotation, &position, m_pTransformCom->GetWorldMatrix());
	//_float3	preEuler;
	//m_pTransformCom->QuaternionToEuler(rotation, preEuler);

	_vector preLook = m_pTransformCom->GetState(CTransform::STATE_LOOK);
	//preLook = XMVectorSetY(preLook, 0.f);

	_vector preameraPos = m_pTransformCom->GetState(CTransform::STATE_POSITION);


	if (g_Menu == false && g_AnotherMenu == false)
	{
		if (MouseMove = m_pEngine->Get_MouseMoveState(CInput_Device::MMS_Y))
		{
			m_pTransformCom->RotateAxis(m_pTransformCom->GetState(CTransform::STATE_RIGHT), DeltaTime * MouseMove * 2.0f);
		}

		if (MouseMove = m_pEngine->Get_MouseMoveState(CInput_Device::MMS_X))
		{
			m_pTransformCom->RotateAxis(DirectX::XMVectorSet(0.f, 1.f, 0.f, 0.f), DeltaTime * MouseMove * 2.f);
		}
		if (MouseWheel = m_pEngine->Get_MouseMoveState(CInput_Device::MMS_WHEEL))
		{
			m_vDistance -= (_float)MouseWheel * (_float)DeltaTime * m_fCameraWheelSpd;
			if (m_vDistance <= 0.2f)
			{
				m_vDistance = 0.2f;
			}
			else if (m_vDistance >= 3.f)
			{
				m_vDistance = 3.f;
			}
		}
	}

	ShakeX(DeltaTime);
	ShakeY(DeltaTime);
	ShakeFov(DeltaTime);
	_vector afterLook = m_pTransformCom->GetState(CTransform::STATE_LOOK);
	_float3 euler;
	m_pTransformCom->QuaternionToEuler(afterLook, euler);
	_vector look = DirectX::XMQuaternionSlerp(preLook, afterLook, m_fCameraAngleLerpSpd * (_float)DeltaTime);


	if (euler.x >= XMConvertToRadians(85.f)) {
		m_pTransformCom->SetLookUpVector(preLook);
	}
	else if (euler.x <= XMConvertToRadians(-85.f)) {
		m_pTransformCom->SetLookUpVector(preLook);
	}
	else {
		m_pTransformCom->SetLookUpVector(XMVector3Normalize(look));
	}


	_vector cameraPos = m_pTransformCom->GetState(CTransform::STATE_POSITION) + XMLoadFloat3(&m_vShakePos) + XMLoadFloat3(&m_vShakeYPos);
	_vector lerpPos = DirectX::XMVectorLerp(cameraPos, vTargetCenterPos, m_fCameraMoveLerpSpd);
	m_pTransformCom->SetState(CTransform::STATE_POSITION, lerpPos);


	mat = XMMatrixTranslation(0.f, 0.f, -m_vDistance) * XMLoadFloat4x4(&m_pTransformCom->GetMatrix());
	m_pTransformCom->SetMatrix(mat);
}

void CCamera_Fly::ShakeX(_double DeltaTime)
{
	if (m_eShaking == SHAKE::SHAKE_ING)
	{
		m_fDurationDelta += DeltaTime;
		if (m_fDuration <= m_fDurationDelta) {
			if (m_iCnt > 0) {
				m_fDurationDelta = 0.f;
				m_fDirection *= -1.f;
				m_iCnt--;
			}
			else {
				XMStoreFloat3(&m_vShakePos, DirectX::XMVectorLerp(XMLoadFloat3(&m_vShakePos), XMVectorZero(), (_float)DeltaTime*3.f));
				m_eShaking = SHAKE::SHAKE_END;
				m_fDurationDelta = 0.f;
				m_fDirection = 0.f;
				m_fSpd = 0.f;
			}
		}
		else {
			_vector cameraRight = m_pTransformCom->GetState(CTransform::STATE_RIGHT);
			XMStoreFloat3(&m_vShakePos, DirectX::XMVectorLerp(XMLoadFloat3(&m_vShakePos), cameraRight, (_float)m_fDurationDelta) * m_fDirection);
		}
	}
	else if (m_eShaking != SHAKE::SHAKE_END) {
		m_fDurationDelta += DeltaTime;
		if (m_fDuration <= m_fDurationDelta) {
			m_fDurationDelta = m_fDuration;
			XMStoreFloat3(&m_vShakePos, DirectX::XMVectorLerp(XMLoadFloat3(&m_vShakePos), XMVectorZero(), (_float)DeltaTime*3.f * m_fSpd));
			m_eShaking = SHAKE::SHAKE_END;
		}
		else {
			_vector cameraRight = m_pTransformCom->GetState(CTransform::STATE_RIGHT);
			XMStoreFloat3(&m_vShakePos, DirectX::XMVectorLerp(XMLoadFloat3(&m_vShakePos), cameraRight, (_float)m_fDurationDelta) * m_fDirection * m_fSpd);
		}
	}
	else {
		if (m_fDurationDelta > 0.f)
			m_fDurationDelta -= DeltaTime;
		else {
			m_fDurationDelta = 0.f;
		}
		XMStoreFloat3(&m_vShakePos, DirectX::XMVectorLerp(XMLoadFloat3(&m_vShakePos), XMVectorZero(), (_float)DeltaTime*3.f));
		m_fDuration = 0.f;
		m_fDirection = 0.f;
		m_fSpd = 0.f;
		m_eShaking = SHAKE::SHAKE_END;
	}


	m_bShake = false;


}

void CCamera_Fly::ShakeY(_double DeltaTime)
{
	m_fYDurationDelta += DeltaTime;
	if (m_fYDuration <= m_fYDurationDelta) {
		if (m_iYCnt > 0) {
			m_fYDurationDelta = 0.f;
			m_fYDirection *= -1.f;
			m_iYCnt--;
		}
		else {
			XMStoreFloat3(&m_vShakeYPos, DirectX::XMVectorLerp(XMLoadFloat3(&m_vShakeYPos), XMVectorZero(), (_float)DeltaTime*3.f));
			m_fYDurationDelta = 0.f;
			m_fYDirection = 0.f;
			m_fYSpd = 0.f;
		}
	}
	else {
		_vector up = m_pTransformCom->GetState(CTransform::STATE_UP);
		XMStoreFloat3(&m_vShakeYPos, DirectX::XMVectorLerp(XMLoadFloat3(&m_vShakeYPos), up, (_float)m_fYDurationDelta) * m_fYDirection);
	}
	m_bYShake = false;
}

void CCamera_Fly::ShakeFov(_double DeltaTime)
{
	m_fov = m_fov * (1 - DeltaTime * 5.f) + m_fDestFov * DeltaTime * 5.f;
	if (m_FovShakeDurationDelta >= m_FovShakeDuration) {
		m_fDestFov = m_constFov;
		m_FovShakeDurationDelta = 0.f;
		m_FovShakeDuration = 0.f;
	}
	else 
		m_FovShakeDurationDelta += DeltaTime;
	
}

void CCamera_Fly::ZoomFov(_float _duration, _float _destFov, _float _spd)
{
	m_fDestFov = _destFov;
	m_FovShakeDuration = _duration;
	m_fFovSpd = _spd;
}

CCamera_Fly * CCamera_Fly::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
{
	CCamera_Fly*		pInstance = new CCamera_Fly(pDevice, pDeviceContext);

	if (FAILED(pInstance->InitializePrototype()))
	{
		MSG_BOX("Failed to Creating CCamera_Fly");
		SafeRelease(pInstance);
	}

	return pInstance;
}

CGameObject * CCamera_Fly::Clone(void * pArg)
{
	CCamera_Fly*		pInstance = new CCamera_Fly(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Creating CCamera_Fly");
		SafeRelease(pInstance);
	}

	return pInstance;
}

void CCamera_Fly::Free()
{
	__super::Free();
	//SafeRelease(m_pTargetTransform);
}
