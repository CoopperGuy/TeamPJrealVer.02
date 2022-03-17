#include "EnginePCH.h"
#include "..\Public\Shop.h"
#include "EmptyUI.h"
CShop::CShop(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
	:CComponent(pDevice,pDeviceContext)
{
}

CShop::CShop(const CShop & rhs)
	:CComponent(rhs)
	,m_ItemType(rhs.m_ItemType)
{
	m_ItemRange = rhs.m_ItemRange;
}

HRESULT CShop::InitializePrototype()
{
	return S_OK;
}

HRESULT CShop::Initialize(void * pArg)
{
	return S_OK;
}

void CShop::SetupItemList(ITEMINFO _item)
{
	//create prefab
	//add
	//therad
	//would be called in script

}

ITEMINFO CShop::BuyItem()
{
	for (auto& iter : m_vecItemList) {
		if (iter.first->IsSelected()) {
			return iter.second;
		}
	}
	return ITEMINFO();
}

ITEMINFO CShop::SellItem()
{
	return ITEMINFO();
}

CShop * CShop::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
{
	CShop*	pInstance = new CShop(pDevice, pDeviceContext);

	if (FAILED(pInstance->InitializePrototype()))
	{
		MSG_BOX("Failed To Create CShop");
		SafeRelease(pInstance);
	}
	return pInstance;
}

CComponent * CShop::Clone(void * pArg)
{
	CComponent*	pInstance = new CShop(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed To Clone CShop");
		SafeRelease(pInstance);
	}
	return pInstance;
}

void CShop::Free()
{
	for (auto& iter : m_vecItemList) {
		SafeRelease(iter.first);
	}
	m_vecItemList.clear();
}
