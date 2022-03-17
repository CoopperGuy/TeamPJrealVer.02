#include "stdafx.h"
#include "..\public\Player.h"

#include "GameInstance.h"

CPlayerTool::CPlayerTool(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
	: CGameObject(pDevice, pDeviceContext)
{
}

CPlayerTool::CPlayerTool(const CPlayerTool & rhs)
	: CGameObject(rhs)
{
}

HRESULT CPlayerTool::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CPlayerTool::NativeConstruct(void * pArg)
{
	if (FAILED(__super::NativeConstruct(pArg)))
		return E_FAIL;
		
	if (FAILED(SetUp_Components()))
		return E_FAIL;	

	//m_pTransformCom->Set_Scale(XMVectorSet(0.01f, 0.01f, 0.01f, 1.f));

	m_pModelCom->SetUp_AnimationIndex(0);		

	return S_OK;
}

_int CPlayerTool::Tick(_double TimeDelta)
{
	if (0 > __super::Tick(TimeDelta))
		return -1;

	if (GetKeyState(VK_UP) < 0)
	{
		m_pTransformCom->Go_Straight(TimeDelta, m_pNaviCom);
	}

	if (GetKeyState(VK_DOWN) < 0)
	{
		m_pTransformCom->Go_BackWard(TimeDelta);
	}

	if (GetKeyState(VK_LEFT) < 0)
	{
		m_pTransformCom->RotateAxis(XMVectorSet(0.f, 1.f, 0.f, 0.f), TimeDelta * -1.f);
	}

	if (GetKeyState(VK_RIGHT) < 0)
	{
		m_pTransformCom->RotateAxis(XMVectorSet(0.f, 1.f, 0.f, 0.f), TimeDelta);
	}


	m_pAABBCom->Update_State(m_pTransformCom->Get_WorldMatrix());

	return _uint();
}

_int CPlayerTool::Late_Tick(_double TimeDelta)
{
	if (nullptr == m_pRendererCom)
		return -1;

	if (0 > __super::Late_Tick(TimeDelta))
		return -1;

	m_pModelCom->Play_Animation(TimeDelta);

	_matrix		ColliderMatrix;
	_matrix		BoneMatrix = m_pModelCom->Get_BoneMatrix("Bone_ArmHand_R");

	_matrix		TransMatrix;
	TransMatrix = XMMatrixTranslation(-60.0f, 0.f, -120.f);

	m_pOBBCom->Update_State(TransMatrix * BoneMatrix * m_pTransformCom->Get_WorldMatrix());

	return m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONALPHA, this);
}

HRESULT CPlayerTool::Render()
{
 	if (nullptr == m_pModelCom)
		return E_FAIL;	
	
	if (FAILED(__super::Render()))
		return E_FAIL;

	if (FAILED(SetUp_ConstantTable()))
		return E_FAIL;

	_uint		iNumMaterials = m_pModelCom->Get_NumMaterials();

	m_pModelCom->Bind_Buffers();

	for (_uint i = 0; i < iNumMaterials; ++i)
	{
		m_pModelCom->SetUp_TextureOnShader("g_DiffuseTexture", i, aiTextureType_DIFFUSE);
		m_pModelCom->Render(i, 1);
	}	

#ifdef _DEBUG
	m_pAABBCom->Render();
	m_pOBBCom->Render();

	m_pNaviCom->Render();
#endif

	return S_OK;
}

HRESULT CPlayerTool::SetUp_Components()
{
	/* For.Com_Renderer */
	if (FAILED(__super::SetUp_Components(LEVEL_STATIC, TEXT("Component_Renderer"), TEXT("Com_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	/* For.Com_Model */
	if (FAILED(__super::SetUp_Components(LEVEL_GAMEPLAY, TEXT("Component_Model_Player"), TEXT("Com_Model"), (CComponent**)&m_pModelCom)))
		return E_FAIL;

	/* For.Com_Transform */
	CTransform::TRANSFORMDESC		TransformDesc;
	TransformDesc.fSpeedPerSec = 5.f;
	TransformDesc.fRotatePerSec = 90.0f;

	if (FAILED(__super::SetUp_Components(LEVEL_STATIC, TEXT("Component_Transform"), TEXT("Com_Transform"), (CComponent**)&m_pTransformCom, &TransformDesc)))
		return E_FAIL;

	/* For.Com_Navigation*/
	if (FAILED(__super::SetUp_Components(LEVEL_GAMEPLAY, TEXT("Component_Navigation"), TEXT("Com_Navigation"), (CComponent**)&m_pNaviCom)))
		return E_FAIL;
	

	/* For.Com_AABB */
	CCollider::COLLIDERDESCS		ColliderDesc;
	ZeroMemory(&ColliderDesc, sizeof(CCollider::COLLIDERDESCS));
	ColliderDesc.vSize = _float3(2.f, 4.f, 2.f);

	if (FAILED(__super::SetUp_Components(LEVEL_GAMEPLAY, TEXT("Component_Collider_AABB"), TEXT("Com_AABB"), (CComponent**)&m_pAABBCom, &ColliderDesc)))
		return E_FAIL;	

	/* For.Com_OBB */
	ZeroMemory(&ColliderDesc, sizeof(CCollider::COLLIDERDESCS));
	ColliderDesc.vSize = _float3(0.5f, 0.5f, 0.5f);

	if (FAILED(__super::SetUp_Components(LEVEL_GAMEPLAY, TEXT("Component_Collider_OBB"), TEXT("Com_OBB"), (CComponent**)&m_pOBBCom, &ColliderDesc)))
		return E_FAIL;



	return S_OK;
}

HRESULT CPlayerTool::SetUp_ConstantTable()
{
	if (nullptr == m_pModelCom)
		return E_FAIL;

	CGameInstance*	pGameInstance = GET_INSTANCE(CGameInstance);

	m_pModelCom->SetUp_ValueOnShader("g_WorldMatrix", &XMMatrixTranspose(m_pTransformCom->Get_WorldMatrix()), sizeof(_matrix));
	m_pModelCom->SetUp_ValueOnShader("g_ViewMatrix", &XMMatrixTranspose(pGameInstance->Get_Transform(CPipeLine::D3DTS_VIEW)), sizeof(_matrix));
	m_pModelCom->SetUp_ValueOnShader("g_ProjMatrix", &XMMatrixTranspose(pGameInstance->Get_Transform(CPipeLine::D3DTS_PROJ)), sizeof(_matrix));	

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

CPlayerTool * CPlayerTool::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
{
	CPlayerTool*		pInstance = new CPlayerTool(pDevice, pDeviceContext);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSG_BOX("Failed to Creating CPlayerTool");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CPlayerTool::Clone(void * pArg)
{
	CPlayerTool*		pInstance = new CPlayerTool(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSG_BOX("Failed to Creating CPlayerTool");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CPlayerTool::Free()
{
	__super::Free();

	Safe_Release(m_pNaviCom);
	Safe_Release(m_pOBBCom);
	Safe_Release(m_pAABBCom);
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pModelCom);
	Safe_Release(m_pRendererCom);
}
