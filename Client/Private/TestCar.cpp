#include "stdafx.h"
#include "..\Public\TestCar.h"
#include "Engine.h"
USING(Client)

CTestCar::CTestCar(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
	:CGameObject(pDevice,pDeviceContext)
{
}

CTestCar::CTestCar(const CTestCar & rhs)
	:CGameObject(rhs)
{
}

HRESULT CTestCar::InitializePrototype()
{
	if (FAILED(__super::InitializePrototype())) {
		return E_FAIL;
	}
	return S_OK;
}

HRESULT CTestCar::Initialize(void * pArg)
{
	if (FAILED(__super::Initialize(pArg))) {
		return E_FAIL;
	}
	return S_OK;
}

_uint CTestCar::Update(_double dDeltaTime)
{
	if (__super::Update(dDeltaTime) < 0) {
		return -1;
	}
	return _uint();
}

_uint CTestCar::LateUpdate(_double dDeltaTime)
{
	if (__super::LateUpdate(dDeltaTime) < 0) {
		return -1;
	}
	return _uint();
}

HRESULT CTestCar::Render(_uint iPassIndex)
{
	if (FAILED(__super::Render(iPassIndex))) {
		return E_FAIL;
	}
	return S_OK;
}

CTestCar * CTestCar::Create(CTestCar* obj)
{
	CTestCar*	pObject = new CTestCar(*obj);
	if (FAILED(pObject->InitializePrototype())) {
		SafeDelete(pObject);
		return nullptr;
	}
	return pObject;
}

CGameObject * CTestCar::Clone(void * pArg)
{
	return nullptr;
}
