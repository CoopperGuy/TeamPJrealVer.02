#include "EnginePCH.h"
#include "..\Public\NPCInfo.h"

USING(Engine)

CNPCInfo::CNPCInfo(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
	:CComponent(pDevice,pDeviceContext)
{
}

CNPCInfo::CNPCInfo(const CNPCInfo & rhs)
	:CComponent(rhs)
	, m_tNPCInfo(rhs.m_tNPCInfo)
{
}

HRESULT CNPCInfo::InitializePrototype()
{
	return S_OK;
}

HRESULT CNPCInfo::Initialize(void * pArg)
{
	return S_OK;
}

CNPCInfo * CNPCInfo::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
{
	CNPCInfo*	pInstance = new CNPCInfo(pDevice, pDeviceContext);
	if (FAILED(pInstance->InitializePrototype()))
	{
		MSG_BOX("Failed To Create CNPCInfo");
		SafeRelease(pInstance);
	}
	return pInstance;
}

CComponent * CNPCInfo::Clone(void * pArg)
{
	CNPCInfo*	pInstance = new CNPCInfo(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed To Clone CText");
		SafeRelease(pInstance);
	}


	return pInstance;
}

void CNPCInfo::Free()
{
}
