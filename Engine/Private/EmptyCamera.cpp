#include "EnginePCH.h"
#include "..\Public\EmptyCamera.h"

USING(Engine)
CEmptyCamera::CEmptyCamera(ID3D11Device * pDevice, ID3D11DeviceContext * pDevice_Context)
	:CCamera(pDevice, pDevice_Context)
{
}

CEmptyCamera::CEmptyCamera(const CCamera & rhs)
	:CCamera(rhs)
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
	return S_OK;
}

_uint CEmptyCamera::Update(_double TimeDelta)
{
	__super::Update(TimeDelta);

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
