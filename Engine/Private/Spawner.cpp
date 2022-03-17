#include "EnginePCH.h"
#include "..\Public\Spawner.h"
#include "Engine.h"

USING(Engine)

CSpawner::CSpawner(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
	:CComponent(pDevice,pDeviceContext)
{
}

CSpawner::CSpawner(const CSpawner & rhs)
	:CComponent(rhs)
{
}

HRESULT CSpawner::InitializePrototype()
{
	return S_OK;
}

HRESULT CSpawner::Initialize(void * pArg)
{
	return S_OK;
}

void CSpawner::Update(_double deltaTime)
{
	
}


CSpawner * CSpawner::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
{
	CSpawner*	pInstance = new CSpawner(pDevice, pDeviceContext);
	if (FAILED(pInstance->InitializePrototype()))
	{
		MSG_BOX("Failed To Create CSpawner");
		SafeRelease(pInstance);
	}
	return pInstance;
}

CComponent * CSpawner::Clone(void * pArg)
{
	CSpawner*	pInstance = new CSpawner(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed To Clone CText");
		SafeRelease(pInstance);
	}


	return pInstance;
}

void CSpawner::Free()
{
}
