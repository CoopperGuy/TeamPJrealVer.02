#include "EnginePCH.h"
#include "..\Public\EmptyCamera.h"
#include "CameraManager.h"
USING(Engine)
CEmptyCamera::CEmptyCamera(ID3D11Device * pDevice, ID3D11DeviceContext * pDevice_Context)
	:CCamera(pDevice, pDevice_Context)
{
}

CEmptyCamera::CEmptyCamera(const CCamera & rhs)
	: CCamera(rhs)
{
}

HRESULT CEmptyCamera::InitializePrototype()
{
	__super::InitializePrototype();

	return S_OK;
}

HRESULT CEmptyCamera::Initialize(void * pArg)
{
	__super::Initialize(pArg);
	m_bIsUse = false;
	return S_OK;
}

_uint CEmptyCamera::Update(_double TimeDelta)
{
	__super::Update(TimeDelta);
	if (m_bIsUse) {
		m_fMoveDelta += (_float)TimeDelta;
		if (m_fMoveTime < m_fMoveDelta) {
			m_fMoveDelta = 0.f;
			m_bIsUse = false;
			CameraManager::GetInstance()->SetDefaultCamera();
			return _uint();
		}
		
		_float t = m_fMoveDelta / m_fMoveTime;
		_vector pos = XMQuaternionSlerp(DirectX::XMLoadFloat3(&m_vSrcPosition), DirectX::XMLoadFloat3(&m_vDestPosition), t);
		_vector look =XMVector3Normalize(DirectX::XMLoadFloat3(&m_vDestLookPosition) - pos);
		m_pTransformCom->SetState(CTransform::STATE_POSITION, pos);
		if(m_eMovie == MOVIE::MOVIE_NOY)
			m_pTransformCom->LookAt(DirectX::XMLoadFloat3(&m_vDestLookPosition));
		else if(m_eMovie ==MOVIE::MOVIE_Y)
			m_pTransformCom->SetLookUpVector(look);
	}

	return _uint();
}

_uint CEmptyCamera::LateUpdate(_double TimeDelta)
{
	__super::LateUpdate(TimeDelta);

	return _uint();
}

CEmptyCamera * CEmptyCamera::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
{
	CEmptyCamera*		pInstance = new CEmptyCamera(pDevice, pDeviceContext);

	if (FAILED(pInstance->InitializePrototype()))
	{
		MSG_BOX("Failed to Creating CEmptyCamera");
		SafeRelease(pInstance);
	}

	return pInstance;
}

CCamera * CEmptyCamera::Clone(void * pArg)
{
	CEmptyCamera*		pInstance = new CEmptyCamera(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Clone CEmptyCamera");
		SafeRelease(pInstance);
	}

	return pInstance;
}

void CEmptyCamera::Free()
{
	__super::Free();
}
