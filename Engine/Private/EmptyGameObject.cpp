#include "EnginePCH.h"
#include "..\Public\EmptyGameObject.h"
#include "Engine.h"
#include "InstanceModel.h"
#include "BasicCollider.h"
#include "Collider.h"
#include "ModelManager.h"
USING(Engine)

CEmptyGameObject::CEmptyGameObject(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
	: CGameObject(pDevice, pDeviceContext)
{
}

CEmptyGameObject::CEmptyGameObject(const CEmptyGameObject & rhs)
	: CGameObject(rhs)
	, m_eRenderGroup(rhs.m_eRenderGroup)
	, m_iNumOBBs(rhs.m_iNumOBBs)
	, m_bRimLightEnable(rhs.m_bRimLightEnable)
	, m_vRimLightDest(rhs.m_vRimLightDest)
{
}

CEmptyGameObject * CEmptyGameObject::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
{
	CEmptyGameObject*		pInstance = new CEmptyGameObject(pDevice, pDeviceContext);

	if (FAILED(pInstance->InitializePrototype()))
	{
		MSG_BOX("Failed to Creating CEmptyGameObject");
		SafeRelease(pInstance);
	}

	return pInstance;
}

CGameObject * CEmptyGameObject::Clone(void * pArg)
{
	CEmptyGameObject*		pInstance = new CEmptyGameObject(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Creating CEmptyGameObject");
		SafeRelease(pInstance);
	}

	return pInstance;
}

CGameObject * CEmptyGameObject::PrefabClone(void * pArg)
{
	CEmptyGameObject*		pInstance = new CEmptyGameObject(*this);

	if (FAILED(pInstance->InitializePrefab(this, pArg)))
	{
		MSG_BOX("Failed to Creating CEmptyGameObject");
		SafeRelease(pInstance);
	}

	return pInstance;
}

CGameObject * CEmptyGameObject::ChildrenPrefabClone(CGameObject * Parent, void * pArg)
{
	CEmptyGameObject*	pInstance = new CEmptyGameObject(*this);
	CEmptyGameObject*	pParent = nullptr;

	if (Parent != nullptr)
		pParent = static_cast<CEmptyGameObject*>(Parent);

	if (FAILED(pInstance->InitializeChildrenPrefab(this, pParent, pArg)))
	{
		MSG_BOX("Failed to Creating ChildrenPrefabGameObject");
		SafeRelease(pInstance);
	}

	return pInstance;
}

void CEmptyGameObject::Free()
{
	__super::Free();

}

HRESULT CEmptyGameObject::InitializePrototype()
{
	if (FAILED(__super::InitializePrototype()))
		return E_FAIL;


	return S_OK;
}

HRESULT CEmptyGameObject::Initialize(void * pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(SetUpComponents()))
		return E_FAIL;
	memset(m_bRender, true, sizeof(m_bRender));

	return S_OK;
}

HRESULT CEmptyGameObject::InitializePrefab(CEmptyGameObject * rhs, void * pArg)
{
//#ifdef _DEBUG
//	_double start, end;
//
//	start = (_double)clock();
//#endif // _DEBUG

	__super::InitializePrefab(rhs, pArg);
	for (int i = 0; i < 10; i++) {
		m_bRender[i] = rhs->m_bRender[i];
	}
	/*Component_Transform*/
	if (FAILED(__super::SetUpComponents(0, "Prototype_Transform", "Com_Transform", (CComponent**)&m_pTransformCom)))
		return E_FAIL;
	if (pArg == nullptr) {
		CTransform* pTransform = dynamic_cast<CTransform*>(rhs->GetComponent("Com_Transform"));
		m_pTransformCom->SetMatrix(pTransform->GetMatrix());
	}
	else {
		_float4x4 mat;
		memcpy(&mat, (_float4x4*)pArg, sizeof(_float4x4));
	}
	/*Component_Renderer*/
	{
		if (FAILED(__super::SetUpComponents(0, "Prototype_Renderer", "Com_Renderer", (CComponent**)&m_pRendererCom)))
			return E_FAIL;
	}

	/*Component_RendererTransform*/
	{
		if (FAILED(__super::SetUpComponents(0, "Prototype_Transform", "Com_RenderTransform", (CComponent**)&m_pRenderTransformCom)))
			return E_FAIL;
	}

//#ifdef _DEBUG
//
//	end = (((_double)clock()));
//
//	cout << "Transform time : " << (end - start) / CLOCKS_PER_SEC << "\n";
//#endif // _DEBUG
//
//#ifdef _DEBUG
//	start = (_double)clock();
//#endif // _DEBUG
	/*Component_Coiilder*/
	{

		CCollider* pCollider = dynamic_cast<CCollider*>(rhs->GetComponent("Com_Collider"));
		if (pCollider != nullptr)
		{
			PhysxMutex.lock();

			CCollider::RIGIDBODYDESC desc = pCollider->GetRigidBodyDesc();

			if (dynamic_cast<CBoxCollider*>(pCollider)) {
				CBoxCollider* myBox = static_cast<CBoxCollider*>(static_cast<CBoxCollider*>(pCollider)->Clone(m_pTransformCom));
				AddComponent("Com_Collider", myBox);
				CBoxCollider* cloneBox = static_cast<CBoxCollider*>(pCollider);

				_float3 size = cloneBox->GetSize();
				myBox->SetUpRigidActor(&size, desc, this);

			}
			else if (dynamic_cast<CCapsuleCollider*>(pCollider)) {
				CCapsuleCollider* myCapsule = static_cast<CCapsuleCollider*>(static_cast<CCapsuleCollider*>(pCollider)->Clone(m_pTransformCom));
				AddComponent("Com_Collider", myCapsule);
				CCapsuleCollider* cloneCapsule = static_cast<CCapsuleCollider*>(pCollider);
				pair<_float, _float> size = cloneCapsule->GetSize();
				myCapsule->SetUpRigidActor(&size, desc, this);
			}
			else if (dynamic_cast<CSphereCollider*>(pCollider)) {
				CSphereCollider* mySphere = static_cast<CSphereCollider*>(static_cast<CSphereCollider*>(pCollider)->Clone(m_pTransformCom));
				AddComponent("Com_Collider", mySphere);
				CSphereCollider* cloneSphere = static_cast<CSphereCollider*>(pCollider);

				_float size = cloneSphere->GetSize();
				mySphere->SetUpRigidActor(&size, desc, this);
			}
			PhysxMutex.unlock();

		}

	}
//#ifdef _DEBUG
//
//	end = (((_double)clock()));
//
//	cout << "Collider time : " << (end - start) / CLOCKS_PER_SEC << "\n";
//#endif // _DEBUG
//#ifdef _DEBUG
//	start = (_double)clock();
//#endif // _DEBUG
	/*Component OBB*/
	{
		int i = 0;
		do {
			string com = "Com_OBB";
			if (i != 0)
				com += to_string(i);
			CBasicCollider* pOBB = dynamic_cast<CBasicCollider*>(rhs->GetComponent(com));
			if (pOBB != nullptr) {
				CBasicCollider* myOBB = static_cast<CBasicCollider*>(static_cast<CBasicCollider*>(pOBB)->Clone(m_pTransformCom));
				AddComponent(com, myOBB);
			}
			i++;
		} while (i < m_iNumOBBs);
	}
//#ifdef _DEBUG
//
//	end = (((_double)clock()));
//
//	cout << "OBB time : " << (end - start) / CLOCKS_PER_SEC << "\n";
//#endif // _DEBUG
//#ifdef _DEBUG
//	start = (_double)clock();
//#endif // _DEBUG
	/*Component_VIBuffer*/
	{
		CVIBuffer_Rect* vibuffer = dynamic_cast<CVIBuffer_Rect*>(rhs->GetComponent("Com_VIBuffer"));
		if (vibuffer != nullptr) {
			CVIBuffer_Rect* pCloneVIBuffer = static_cast<CVIBuffer_Rect*>(static_cast<CVIBuffer_Rect*>(vibuffer)->Clone(m_pTransformCom));
			AddComponent("Com_VIBuffer", pCloneVIBuffer);
		}
	}
//#ifdef _DEBUG
//
//	end = (((_double)clock()));
//
//	cout << "VIBuffer time : " << (end - start) / CLOCKS_PER_SEC << "\n";
//#endif // _DEBUG
//#ifdef _DEBUG
//	start = (_double)clock();
//#endif // _DEBUG
	/*Component_Model*/
	{
		CModel* pModel = static_cast<CModel*>(rhs->GetComponent("Com_Model"));

		if (pModel)
		{
			string MeshFullPath = pModel->GetMeshFilePath() + pModel->GetMeshFileName();
			CModel* pCloneModel = CModelManager::GetInstance()->GetCloneModel(MeshFullPath, m_pTransformCom);
			AddComponent("Com_Model", pCloneModel);
		}
	}
//#ifdef _DEBUG
//
//	end = (((_double)clock()));
//
//	cout << "Model time : " << (end - start) / CLOCKS_PER_SEC << "\n";
//#endif // _DEBUG
//#ifdef _DEBUG
//	start = (_double)clock();
//#endif // _DEBUG
	/*Component_Stat*/
	{
		CStat* pStat = static_cast<CStat*>(rhs->GetComponent("Com_Stat"));

		if (pStat)
		{
			CStat* pCloneStat = static_cast<CStat*>(static_cast<CStat*>(pStat)->Clone(m_pTransformCom));
			AddComponent("Com_Stat", pCloneStat);
		}
	}
	/*Component_NPCINFO*/
	{
		CNPCInfo* pNPC = static_cast<CNPCInfo*>(rhs->GetComponent("Com_NPCInfo"));

		if (pNPC)
		{
			CNPCInfo* pCloneNPC = static_cast<CNPCInfo*>(static_cast<CNPCInfo*>(pNPC)->Clone(m_pTransformCom));
			AddComponent("Com_NPCInfo", pCloneNPC);
		}
	}
//#ifdef _DEBUG
//
//	end = (((_double)clock()));
//
//	cout << "Stat time : " << (end - start) / CLOCKS_PER_SEC << "\n";
//#endif // _DEBUG
	return S_OK;
}

HRESULT CEmptyGameObject::InitializeChildrenPrefab(CEmptyGameObject * rhs, CEmptyGameObject * Parent, void * pArg)
{
	__super::InitializePrefab(rhs, pArg);
	for (int i = 0; i < 10; i++) {
		m_bRender[i] = rhs->m_bRender[i];
	}
	/*Component_Transform*/
	if (FAILED(__super::SetUpComponents(0, "Prototype_Transform", "Com_Transform", (CComponent**)&m_pTransformCom)))
		return E_FAIL;

	if (FAILED(__super::SetUpComponents(0, "Prototype_Transform", "Com_LocalTransform", (CComponent**)&m_pLocalTransformCom)))
		return E_FAIL;


	m_pParent = Parent;
	//m_pParent->AddChild(this);
	CTransform* pTransform = static_cast<CTransform*>(rhs->GetComponent("Com_Transform"));
	m_pLocalTransformCom->SetMatrix(pTransform->GetMatrix());

	SetParent(Parent);
	m_pParentTransformCom = static_cast<CTransform*>(Parent->GetComponent("Com_Transform"));
	AddComponent("Com_ParentTransform", m_pParentTransformCom);
	SafeAddRef(m_pParentTransformCom);

	m_pTransformCom->SetMatrix(m_pLocalTransformCom->GetWorldMatrix() * m_pParentTransformCom->GetWorldMatrix());

	/*Component_Renderer*/
	{
		if (FAILED(__super::SetUpComponents(0, "Prototype_Renderer", "Com_Renderer", (CComponent**)&m_pRendererCom)))
			return E_FAIL;
	}

	/*Component_RendererTransform*/
	{
		if (FAILED(__super::SetUpComponents(0, "Prototype_Transform", "Com_RenderTransform", (CComponent**)&m_pRenderTransformCom)))
			return E_FAIL;
	}


	/*Component_Coiilder*/
	{

		CCollider* pCollider = dynamic_cast<CCollider*>(rhs->GetComponent("Com_Collider"));
		if (pCollider != nullptr)
		{
			PhysxMutex.lock();

			CCollider::RIGIDBODYDESC desc = pCollider->GetRigidBodyDesc();

			if (dynamic_cast<CBoxCollider*>(pCollider)) {
				CBoxCollider* myBox = static_cast<CBoxCollider*>(static_cast<CBoxCollider*>(pCollider)->Clone(m_pTransformCom));
				AddComponent("Com_Collider", myBox);
				CBoxCollider* cloneBox = static_cast<CBoxCollider*>(pCollider);

				_float3 size = cloneBox->GetSize();
				myBox->SetUpRigidActor(&size, desc, this);

			}
			else if (dynamic_cast<CCapsuleCollider*>(pCollider)) {
				CCapsuleCollider* myCapsule = static_cast<CCapsuleCollider*>(static_cast<CCapsuleCollider*>(pCollider)->Clone(m_pTransformCom));
				AddComponent("Com_Collider", myCapsule);
				CCapsuleCollider* cloneCapsule = static_cast<CCapsuleCollider*>(pCollider);
				pair<_float, _float> size = cloneCapsule->GetSize();
				myCapsule->SetUpRigidActor(&size, desc, this);
			}
			else if (dynamic_cast<CSphereCollider*>(pCollider)) {
				CSphereCollider* mySphere = static_cast<CSphereCollider*>(static_cast<CSphereCollider*>(pCollider)->Clone(m_pTransformCom));
				AddComponent("Com_Collider", mySphere);
				CSphereCollider* cloneSphere = static_cast<CSphereCollider*>(pCollider);

				_float size = cloneSphere->GetSize();
				mySphere->SetUpRigidActor(&size, desc, this);
			}
			PhysxMutex.unlock();

		}

	}
	/*Component OBB*/
	{
		int i = 0;
		do {
			string com = "Com_OBB";
			if (i != 0)
				com += to_string(i);
			CBasicCollider* pOBB = dynamic_cast<CBasicCollider*>(rhs->GetComponent(com));
			if (pOBB != nullptr) {
				CBasicCollider* myOBB = static_cast<CBasicCollider*>(static_cast<CBasicCollider*>(pOBB)->Clone(m_pTransformCom));
				AddComponent(com, myOBB);
			}
			i++;
		} while (i < m_iNumOBBs);
	}

	/*Component_VIBuffer*/
	{
		CVIBuffer_Rect* vibuffer = dynamic_cast<CVIBuffer_Rect*>(rhs->GetComponent("Com_VIBuffer"));
		if (vibuffer != nullptr) {
			CVIBuffer_Rect* pCloneVIBuffer = static_cast<CVIBuffer_Rect*>(static_cast<CVIBuffer_Rect*>(vibuffer)->Clone(m_pTransformCom));
			AddComponent("Com_VIBuffer", pCloneVIBuffer);
		}
	}

	/*Component_Model*/
	{
		CModel* pModel = static_cast<CModel*>(rhs->GetComponent("Com_Model"));

		if (pModel)
		{
			string MeshFullPath = pModel->GetMeshFilePath() + pModel->GetMeshFileName();
			CModel* pCloneModel = CModelManager::GetInstance()->GetCloneModel(MeshFullPath, m_pTransformCom);
			AddComponent("Com_Model", pCloneModel);
		}
	}

	/*Component_Stat*/
	{
		CStat* pStat = static_cast<CStat*>(rhs->GetComponent("Com_Stat"));

		if (pStat)
		{
			CStat* pCloneStat = static_cast<CStat*>(static_cast<CStat*>(pStat)->Clone(m_pTransformCom));
			AddComponent("Com_Stat", pCloneStat);
		}
	}
	/*Component_NPCINFO*/
	{
		CNPCInfo* pNPC = static_cast<CNPCInfo*>(rhs->GetComponent("Com_NPCInfo"));

		if (pNPC)
		{
			CNPCInfo* pCloneNPC = static_cast<CNPCInfo*>(static_cast<CNPCInfo*>(pNPC)->Clone(m_pTransformCom));
			AddComponent("Com_NPCInfo", pCloneNPC);
		}
	}

	return S_OK;
}

_uint CEmptyGameObject::Update(_double TimeDelta)
{
	if (__super::Update(TimeDelta))
		return E_FAIL;
	if (CEngine::GetInstance()->GetCurSceneNumber() < 5 && CEngine::GetInstance()->GetCurrentUsage() == CEngine::USAGE::USAGE_CLIENT)
		return 0;
	if (!m_bIsActive)
		return 0;

	if (m_bRimLightEnable) {
		m_RimLightTimeAcc += TimeDelta;
		_float t = m_RimLightTimeAcc / m_RimLightTime;
		XMStoreFloat4(&m_vRimgLightColor,  XMVectorLerp(XMLoadFloat4(&m_vRimLightSrc), XMLoadFloat4(&m_vRimLightDest), t));
		if (m_RimLightTime < m_RimLightTimeAcc) {
			m_bRimLightEnable = false;
			m_RimLightTimeAcc = 0;
			m_vRimLightSrc = _float4(0.f, 0.f, 0.f, 0.f);
		}
	}
	if (CEngine::GetInstance()->GetCurrentUsage() == CEngine::USAGE::USAGE_TOOL)
	{
		CComponent* pModel = GetComponent("Com_Model");
		if (pModel)
		{
			dynamic_cast<CModel*>(pModel)->SetUp_AnimationIndex(0);
			dynamic_cast<CModel*>(pModel)->Play_Animation(TimeDelta);
		}
	}

	CStat* Stat = static_cast<CStat*>(GetComponent("Com_Stat"));
	if (Stat) {
		Stat->Update(TimeDelta);
	}

	CComponent* collider = GetComponent("Com_Collider");
	if (collider)
		dynamic_cast<CCollider*>(collider)->LateUpdate(TimeDelta);

	int i = 0;
	do {
		string com = "Com_OBB";
		if (i != 0)
			com += to_string(i);
		CComponent* OBB = GetComponent(com);
		if (OBB)
		{
			CBasicCollider* nowOBb = static_cast<CBasicCollider*>(OBB);
			if (!nowOBb->GetIsAttachBone())
				nowOBb->Update_State(m_pTransformCom->GetWorldMatrix());
			else {
				string boneName = nowOBb->GetBoneName();
				CComponent* pModel = GetComponent("Com_Model");
				if (pModel)
				{	
					if (CEngine::GetInstance()->GetCurrentUsage() == CEngine::USAGE::USAGE_CLIENT) {
						_matrix boneMat = static_cast<CModel*>(pModel)->Get_BoneWithoutOffset(boneName.c_str());
						nowOBb->Update_State(boneMat * m_pRenderTransformCom->GetWorldMatrix());
					}
					else {
						_matrix boneMat = static_cast<CModel*>(pModel)->Get_BoneWithoutOffset(boneName.c_str());
						nowOBb->Update_State(boneMat * m_pTransformCom->GetWorldMatrix());
					}
				}
			}

		}
		i++;
	} while (i < m_iNumOBBs);


	CComponent* AABB = GetComponent("Com_AABB");
	if (AABB)
		dynamic_cast<CBasicCollider*>(AABB)->Update_State(m_pTransformCom->GetWorldMatrix());

	return S_OK;
}

_uint CEmptyGameObject::LateUpdate(_double TimeDelta)
{
	if (!m_bIsActive)
		return 0;
	if (nullptr == m_pRendererCom &&CEngine::GetInstance()->GetCurrentUsage() == CEngine::USAGE::USAGE_TOOL)
		return -1;
	if (CEngine::GetInstance()->GetCurSceneNumber() < 5 && CEngine::GetInstance()->GetCurrentUsage() == CEngine::USAGE::USAGE_CLIENT)
		return 0;

	if (m_pParent || dynamic_cast<CEmptyGameObject*>(m_pParent))
	{
		m_pTransformCom->SetMatrix(m_pLocalTransformCom->GetWorldMatrix() * m_pParentTransformCom->GetWorldMatrix());
	}

	if (CEngine::GetInstance()->isInFrustum(m_pTransformCom->GetState(CTransform::STATE_POSITION), m_fFrustumRange)) {
		return m_pRendererCom->AddRenderGroup(m_eRenderGroup, this);
	}


	return 0;
}

HRESULT CEmptyGameObject::Render(_uint iPassIndex)
{
	if (!m_bIsActive)
		return S_OK;

	if (m_bisRender == false)
		return S_OK;

	CComponent* buffer = GetComponent("Com_VIBuffer");
	if (buffer) {
		static_cast<CVIBuffer*>(buffer)->GetShader()->SetUp_ValueOnShader("g_Percentage", &m_fPercentage, sizeof(_float));
		static_cast<CVIBuffer*>(buffer)->GetShader()->SetUp_ValueOnShader("g_Back", &m_fBackPercentage, sizeof(_float));
		static_cast<CVIBuffer*>(buffer)->Render(m_iPassIndex);
	}
	CComponent* modelCom = GetComponent("Com_Model");
	if (modelCom)
	{
		CModel* pModel = dynamic_cast<CModel*>(modelCom);
		if (pModel->GetLinkEquip())
		{
			if (m_pTargetTransform)
				pModel->Bind_Buffers(iPassIndex, m_pTargetTransform);
			else
				pModel->Bind_Buffers(iPassIndex);
		}
		else
			pModel->Bind_Buffers(iPassIndex);

		_uint		iNumMaterials = pModel->Get_NumMaterials();

		//for (_uint i = 0; i < iNumMaterials; ++i)
		//{
		//	pModel->SetUp_TextureOnShader("g_DiffuseTexture", i, aiTextureType_DIFFUSE);
		//	// TODO: Handle passIndex
		//	pModel->Render(i, 3);
		//}

		for (_uint i = 0; i < iNumMaterials; ++i)
		{
			if (m_bRender[i])
			{
				pModel->SetUp_TextureOnShader("g_DiffuseTexture", i, aiTextureType_DIFFUSE);
				pModel->SetUp_TextureOnShader("g_NormalTexture", i, aiTextureType_NORMALS);
				pModel->SetUp_TextureOnShader("g_EmissiveTexture", i, aiTextureType_EMISSIVE);
				pModel->SetUp_TextureOnShader("g_SpecularTexture", i, aiTextureType_SPECULAR);
								
				pModel->GetShader()->SetUp_ValueOnShader("g_RimLightEnable", &m_bRimLightEnable, sizeof(_bool));
				pModel->GetShader()->SetUp_ValueOnShader("g_CamPosition", &m_pEngine->GetCamPosition(), sizeof(_float3));
				pModel->GetShader()->SetUp_ValueOnShader("g_RimLitghColor", &m_vRimgLightColor, sizeof(_float4));
				// TODO: Handle passIndex
				pModel->Render(i, iPassIndex);
			}
		}

	}


	if (buffer && modelCom)
		int i = 0;

	CComponent* collider = GetComponent("Com_Collider");
	if (collider)
		dynamic_cast<CCollider*>(collider)->Render();

	/* Trail_Test */
	CComponent* pTrail = GetComponent("Com_Trail");
	if (pTrail)
	{
		static_cast<CVIBuffer_Trail*>(pTrail)->Render();
	}

	int i = 0;
	do {
		string com = "Com_OBB";
		if (i != 0)
			com += to_string(i);
		CComponent* OBB = GetComponent(com);
		if (OBB)
		{
			dynamic_cast<CBasicCollider*>(OBB)->Render();
		}
		i++;
	} while (i < m_iNumOBBs);

#ifdef _DEBUG
	CComponent* Navi = GetComponent("Com_Navigation");
	if (Navi)
		m_pNavigation->Render();


	CComponent* AABB = GetComponent("Com_AABB");
	if (AABB)
		dynamic_cast<CBasicCollider*>(AABB)->Render();
#endif // !DEBUG


	return S_OK;
}

void CEmptyGameObject::SetLocalPosition(_float3 pos)
{
	if (m_pLocalTransformCom != nullptr) {
		m_pLocalTransformCom->SetState(CTransform::STATE_POSITION, XMLoadFloat3(&pos));
	}
}

void CEmptyGameObject::SetPosition(_float3 pos)
{
	m_pTransformCom->SetState(CTransform::STATE_POSITION, XMLoadFloat3(&pos));
}

_bool CEmptyGameObject::GetHaveParent()
{
	if (m_pParent)
		return true;
	return false;
}

void CEmptyGameObject::SetisRender(_bool tf)
{
	m_bisRender = tf;
	for (auto& iter : m_listChildren) {
		iter->SetisRender(m_bisRender);
	}
}

void CEmptyGameObject::SetRimLight(_bool bRimLight, _fvector _lightColor, _double _lifeTime)
{
	m_bRimLightEnable = bRimLight;
	XMStoreFloat4(&m_vRimLightDest, _lightColor);
	m_RimLightTime = _lifeTime;
}

HRESULT CEmptyGameObject::SetUpComponents()
{
	if (FAILED(__super::SetUpComponents(0, "Prototype_Transform", "Com_Transform", (CComponent**)&m_pTransformCom)))
		return E_FAIL;

	if (FAILED(__super::SetUpComponents(0, "Prototype_Transform", "Com_ParentTransform", (CComponent**)&m_pParentTransformCom)))
		return E_FAIL;

	if (FAILED(__super::SetUpComponents(0, "Prototype_Transform", "Com_LocalTransform", (CComponent**)&m_pLocalTransformCom)))
		return E_FAIL;

	if (FAILED(__super::SetUpComponents(0, "Prototype_Renderer", "Com_Renderer", (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	if (FAILED(__super::SetUpComponents(0, "Prototype_Transform", "Com_RenderTransform", (CComponent**)&m_pRenderTransformCom)))
		return E_FAIL;

	/*if (FAILED(__super::SetUpComponents(0, "Prototype_Navigation", "Com_Navigation", (CComponent**)&m_pNavigation)))
		return E_FAIL;*/

	return S_OK;
}

void CEmptyGameObject::LinkTranformWithParent()
{
	if (m_pParent || dynamic_cast<CEmptyGameObject*>(m_pParent))
	{
		m_pParentTransformCom = dynamic_cast<CTransform*>(m_pParent->GetComponent("Com_Transform"));
		m_bisChild = true;
	}



}
