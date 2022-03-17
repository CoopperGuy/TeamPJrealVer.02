#include "EnginePCH.h"
#include "..\Public\EmptyMapObject.h"
#include "Engine.h"
#include "VIBuffer_RectInstance.h"
#include "InstanceModel.h"

USING(Engine)

CEmptyMapObject::CEmptyMapObject(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
	: CGameObject(pDevice, pDeviceContext)
{
}

CEmptyMapObject::CEmptyMapObject(const CEmptyMapObject & rhs)
	: CGameObject(rhs)
{
}

CEmptyMapObject * CEmptyMapObject::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
{
	CEmptyMapObject*		pInstance = new CEmptyMapObject(pDevice, pDeviceContext);

	if (FAILED(pInstance->InitializePrototype()))
	{
		MSG_BOX("Failed to Creating CEmptyMapObject");
		SafeRelease(pInstance);
	}

	return pInstance;
}

CGameObject * CEmptyMapObject::Clone(void * pArg)
{
	CEmptyMapObject*		pInstance = new CEmptyMapObject(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Creating CEmptyMapObject");
		SafeRelease(pInstance);
	}

	return pInstance;
}

CGameObject * CEmptyMapObject::PrefabClone(void * pArg)
{
	return nullptr;
}

CGameObject * CEmptyMapObject::ChildrenPrefabClone(CGameObject * Parent, void * pArg)
{
	return nullptr;
}

void CEmptyMapObject::Free()
{
	__super::Free();
}

HRESULT CEmptyMapObject::InitializePrototype()
{
	if (FAILED(__super::InitializePrototype()))
		return E_FAIL;


	return S_OK;
}

HRESULT CEmptyMapObject::Initialize(void * pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(SetUpComponents()))
		return E_FAIL;

	return S_OK;
}

_uint CEmptyMapObject::Update(_double TimeDelta)
{
	if (__super::Update(TimeDelta))
		return E_FAIL;


	return S_OK;
}

_uint CEmptyMapObject::LateUpdate(_double TimeDelta)
{
	if (nullptr == m_pRendererCom)
		return -1;


	if (m_pParent || dynamic_cast<CEmptyMapObject*>(m_pParent))
	{
		m_pTransformCom->SetMatrix(m_pLocalTransformCom->GetWorldMatrix() * m_pParentTransformCom->GetWorldMatrix());
	}

	CComponent* pModel = GetComponent("Com_InsModel");
	m_pModelCom = dynamic_cast<CInstanceModel*>(pModel);




	return m_pRendererCom->AddRenderGroup(m_eRenderGroup, this);
}

HRESULT CEmptyMapObject::Render(_uint iPassIndex)
{
	if (!m_bIsActive)
		return S_OK;

	CComponent* buffer = GetComponent("Com_VIBuffer");
	if (buffer)
		dynamic_cast<CVIBuffer*>(buffer)->Render(iPassIndex);

	CComponent* pModel = GetComponent("Com_InsModel");
	m_pModelCom = dynamic_cast<CInstanceModel*>(pModel);

	if (m_pModelCom)
	{
		_uint		iNumMaterials = m_pModelCom->Get_NumMaterials();
		m_pModelCom->Bind_Buffers(iPassIndex, m_pTransformCom);

		for (_uint i = 0; i < iNumMaterials; ++i)
		{
			m_pModelCom->SetUp_TextureOnShader("g_DiffuseTexture", i, aiTextureType_DIFFUSE);
			m_pModelCom->Render(i, iPassIndex);
		}

		CComponent* collider = GetComponent("Com_Collider");
		if (collider)
			dynamic_cast<CCollider*>(collider)->Render();
	}


	return S_OK;
}

_bool CEmptyMapObject::GetHaveParent()
{
	if (m_pParent)
		return true;
	return false;
}

HRESULT CEmptyMapObject::SetUpComponents()
{
	if (FAILED(__super::SetUpComponents(0, "Prototype_Transform", "Com_Transform", (CComponent**)&m_pTransformCom)))
		return E_FAIL;

	if (FAILED(__super::SetUpComponents(0, "Prototype_Transform", "Com_ParentTransform", (CComponent**)&m_pParentTransformCom)))
		return E_FAIL;

	if (FAILED(__super::SetUpComponents(0, "Prototype_Transform", "Com_LocalTransform", (CComponent**)&m_pLocalTransformCom)))
		return E_FAIL;

	if (FAILED(__super::SetUpComponents(0, "Prototype_Renderer", "Com_Renderer", (CComponent**)&m_pRendererCom)))
		return E_FAIL;


	return S_OK;
}

void CEmptyMapObject::LinkTranformWithParent()
{
	if (m_pParent || dynamic_cast<CEmptyMapObject*>(m_pParent))
	{
		m_pParentTransformCom = dynamic_cast<CTransform*>(m_pParent->GetComponent("Com_Transform"));
		m_bisChild = true;
	}

}
