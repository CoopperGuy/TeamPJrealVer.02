#include "EnginePCH.h"
#include "..\Public\EmptyEffect.h"
#include "Engine.h"

#include "BasicCollider.h"
#include "ModelManager.h"

USING(Engine)

CEmptyEffect::CEmptyEffect(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
	: CGameObject(pDevice, pDeviceContext)
{
}

CEmptyEffect::CEmptyEffect(const CEmptyEffect & rhs)
	: CGameObject(rhs)
{
}

CEmptyEffect * CEmptyEffect::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
{
	CEmptyEffect*		pInstance = new CEmptyEffect(pDevice, pDeviceContext);

	if (FAILED(pInstance->InitializePrototype()))
	{
		MSG_BOX("Failed to Creating CEmptyEffect");
		SafeRelease(pInstance);
	}
	int i = 0;
	return pInstance;
}

CGameObject * CEmptyEffect::Clone(void * pArg)
{
	CEmptyEffect*		pInstance = new CEmptyEffect(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Creating CEmptyEffect");
		SafeRelease(pInstance);
	}

	return pInstance;
}

CGameObject * CEmptyEffect::PrefabClone(void * pArg)
{
	CEmptyEffect*	pInstance = new CEmptyEffect(*this);
		
	if (FAILED(pInstance->InitializePrefab(this, pArg)))
	{
		MSG_BOX("Failed to Creating PrefabEffect");
		SafeRelease(pInstance);
	}

	return pInstance;
}

CGameObject * CEmptyEffect::ChildrenPrefabClone(CGameObject * Parent, void * pArg)
{
	CEmptyEffect*	pInstance = new CEmptyEffect(*this);
	CEmptyEffect*	pParent = nullptr;

	if (Parent != nullptr)
		pParent = static_cast<CEmptyEffect*>(Parent);

	if (FAILED(pInstance->InitializeChildrenPrefab(this, pParent, pArg)))
	{
		MSG_BOX("Failed to Creating ChildrenPrefabEffect");
		SafeRelease(pInstance);
	}

	return pInstance;
}

void CEmptyEffect::Free()
{
	__super::Free();

	for (_uint i = 0; i < TEXTURE_END; i++) 
	{
		if (m_pTexture[i])
			SafeRelease(m_pTexture[i]);
	}		
}

HRESULT CEmptyEffect::InitializePrototype()
{
	if (FAILED(__super::InitializePrototype()))
		return E_FAIL;


	return S_OK;
}

HRESULT CEmptyEffect::Initialize(void * pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(SetUpComponents()))
		return E_FAIL;

	return S_OK;
}

HRESULT CEmptyEffect::InitializePrefab(CEmptyEffect* rhs, void * pArg)
{
	m_iPassIndex = rhs->m_iPassIndex;
	m_fFrameTime = rhs->m_fFrameTime;
	m_vScrollSpeedX = rhs->m_vScrollSpeedX;
	m_vScrollSpeedY = rhs->m_vScrollSpeedY;
	m_vScale = rhs->m_vScale;
	m_fPadding = rhs->m_fPadding;
	m_fDistortionScale = rhs->m_fDistortionScale;
	m_fDistortionBias = rhs->m_fDistortionBias;
	m_fEffectFrameTime = rhs->m_fEffectFrameTime;
	m_fFadeAlpha = rhs->m_fFadeAlpha;
	m_vZoomScale = rhs->m_vZoomScale;
	m_bZoomIn = rhs->m_bZoomIn;
	m_bZoomOut = rhs->m_bZoomOut;
	m_bFadeOut = rhs->m_bFadeOut;
	m_fFadeOutDuration = rhs->m_fFadeOutDuration;
	m_fFadeOutStratTime = rhs->m_fFadeOutStratTime;
	m_bFadeIn = rhs->m_bFadeIn;
	m_fFadeInDuration = rhs->m_fFadeInDuration;
	m_fFadeInStratTime = rhs->m_fFadeInStratTime;
	m_fEffectDuration = rhs->m_fEffectDuration;
	m_SpriteEnd = rhs->m_SpriteEnd;
	m_fSpriteSpeed = rhs->m_fSpriteSpeed;
	m_iSpriteNumX = rhs->m_iSpriteNumX;
	m_iSpriteNumY = rhs->m_iSpriteNumY;
	m_iSpriteNumTotal = rhs->m_iSpriteNumTotal;
	m_bBillBord = rhs->m_bBillBord;


	for (_uint i = 0; i < 3; ++i)
	{
		m_vDistortion[i] = rhs->m_vDistortion[i];
	}
		
#pragma region setup_component
	if (FAILED(__super::SetUpComponents(0, "Prototype_Transform", "Com_Transform", (CComponent**)&m_pTransformCom)))
		return E_FAIL;

	if (pArg)
	{
		m_pTransformCom->SetMatrix(*(_matrix*)pArg);
	}
	else
	{
		CTransform* pTransform = static_cast<CTransform*>(rhs->GetComponent("Com_Transform"));
		m_pTransformCom->SetMatrix(pTransform->GetMatrix());
	}

	/*for.Texture Component*/
	for (_uint i = 0; i < (_uint)TEXTURE_END; ++i)
	{
		if (rhs->m_pTexture[i])
		{
			m_pTexture[i] = rhs->m_pTexture[i];
			SafeAddRef(m_pTexture[i]);
		}
	}

	/*for.Renderer Component*/
	if (FAILED(__super::SetUpComponents(0, "Prototype_Renderer", "Com_Renderer", (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	/*for.Model Component*/
	CModel* pModel = static_cast<CModel*>(rhs->GetComponent("Com_Model"));
	if (pModel)
	{
		string MeshFullPath = pModel->GetMeshFilePath() + pModel->GetMeshFileName();
		CModel* pCloneModel = CModelManager::GetInstance()->GetCloneModel(MeshFullPath, m_pTransformCom);
		AddComponent("Com_Model", pCloneModel);
	}
	/*for.VIBuffer Component*/
	CVIBuffer_RectEffect* pVIBuffer = static_cast<CVIBuffer_RectEffect*>(rhs->GetComponent("Com_VIBuffer"));
	if (pVIBuffer)
	{
		CVIBuffer_RectEffect* pCloneVIBuffer = static_cast<CVIBuffer_RectEffect*>(pVIBuffer->Clone(m_pTransformCom));
		AddComponent("Com_VIBuffer", pCloneVIBuffer);
	}

	/*for.PointInstance Component*/
	CVIBuffer_PointInstance* pPointInstance = static_cast<CVIBuffer_PointInstance*>(rhs->GetComponent("Com_PointInstance"));
	if (pPointInstance)
	{
		CVIBuffer_PointInstance* pCloneInstance = static_cast<CVIBuffer_PointInstance*>(pPointInstance->Clone());
		AddComponent("Com_PointInstance", pCloneInstance);

		_float3 vPosition;
		XMStoreFloat3(&vPosition, m_pTransformCom->GetState(CTransform::STATE_POSITION));
		pCloneInstance->Set_Position(vPosition);
	}

	/*for.DecalBuffer Component*/

#pragma endregion

	return S_OK;
}

HRESULT CEmptyEffect::InitializeChildrenPrefab(CEmptyEffect* rhs, CEmptyEffect * Parent, void * pArg)
{
	m_iPassIndex = rhs->m_iPassIndex;
	m_fFrameTime = rhs->m_fFrameTime;
	m_vScrollSpeedX = rhs->m_vScrollSpeedX;
	m_vScrollSpeedY = rhs->m_vScrollSpeedY;
	m_vScale = rhs->m_vScale;
	m_fPadding = rhs->m_fPadding;
	m_fDistortionScale = rhs->m_fDistortionScale;
	m_fDistortionBias = rhs->m_fDistortionBias;
	m_fEffectFrameTime = rhs->m_fEffectFrameTime;
	m_fFadeAlpha = rhs->m_fFadeAlpha;
	m_vZoomScale = rhs->m_vZoomScale;
	m_bZoomIn = rhs->m_bZoomIn;
	m_bZoomOut = rhs->m_bZoomOut;
	m_bFadeOut = rhs->m_bFadeOut;
	m_fFadeOutDuration = rhs->m_fFadeOutDuration;
	m_fFadeOutStratTime = rhs->m_fFadeOutStratTime;
	m_bFadeIn = rhs->m_bFadeIn;
	m_fFadeInDuration = rhs->m_fFadeInDuration;
	m_fFadeInStratTime = rhs->m_fFadeInStratTime;
	m_fEffectDuration = rhs->m_fEffectDuration;
	m_SpriteEnd = rhs->m_SpriteEnd;
	m_fSpriteSpeed = rhs->m_fSpriteSpeed;
	m_iSpriteNumX = rhs->m_iSpriteNumX;
	m_iSpriteNumY = rhs->m_iSpriteNumY;
	m_iSpriteNumTotal = rhs->m_iSpriteNumTotal;
	m_bBillBord = rhs->m_bBillBord;

	for (_uint i = 0; i < 3; ++i)
	{
		m_vDistortion[i] = rhs->m_vDistortion[i];
	}

#pragma region setup_component
	if (FAILED(__super::SetUpComponents(0, "Prototype_Transform", "Com_Transform", (CComponent**)&m_pTransformCom)))
		return E_FAIL;
		
	if (FAILED(__super::SetUpComponents(0, "Prototype_Transform", "Com_LocalTransform", (CComponent**)&m_pLocalTransformCom)))
		return E_FAIL;
	
	m_pParent = Parent;

	CTransform* pTransform = static_cast<CTransform*>(rhs->GetComponent("Com_Transform"));
	m_pLocalTransformCom->SetMatrix(pTransform->GetMatrix());
	
	m_pParentTransformCom = static_cast<CTransform*>(Parent->GetComponent("Com_Transform"));
	AddComponent("Com_ParentTransform", m_pParentTransformCom);
	SafeAddRef(m_pParentTransformCom);

	m_pTransformCom->SetMatrix(m_pLocalTransformCom->GetWorldMatrix() * m_pParentTransformCom->GetWorldMatrix());
	
	/*for.Texture Component*/
	for (_uint i = 0; i < (_uint)TEXTURE_END; ++i)
	{
		if (rhs->m_pTexture[i])
		{
			m_pTexture[i] = rhs->m_pTexture[i];
			SafeAddRef(m_pTexture[i]);
		}
	}

	/*for.Renderer Component*/
	if (FAILED(__super::SetUpComponents(0, "Prototype_Renderer", "Com_Renderer", (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	/*for.Model Component*/
	CModel* pModel = static_cast<CModel*>(rhs->GetComponent("Com_Model"));

	if (pModel)
	{
		string MeshFullPath = pModel->GetMeshFilePath() + pModel->GetMeshFileName();
		CModel* pCloneModel = CModelManager::GetInstance()->GetCloneModel(MeshFullPath, m_pTransformCom);
		AddComponent("Com_Model", pCloneModel);
	}
	/*for.VIBuffer Component*/
	CVIBuffer_RectEffect* pVIBuffer = static_cast<CVIBuffer_RectEffect*>(rhs->GetComponent("Com_VIBuffer"));
	if (pVIBuffer)
	{
		CVIBuffer_RectEffect* pCloneVIBuffer = static_cast<CVIBuffer_RectEffect*>(pVIBuffer->Clone(m_pTransformCom));
		AddComponent("Com_VIBuffer", pCloneVIBuffer);
	}

	/*Component OBB*/
	/*{
		CBasicCollider* pOBB = static_cast<CBasicCollider*>(rhs->GetComponent("Com_OBB"));
		if (pOBB != nullptr) {
			CBasicCollider* myOBB = static_cast<CBasicCollider*>(static_cast<CBasicCollider*>(pOBB)->Clone(m_pTransformCom));
			AddComponent("Com_OBB", myOBB);
		}
	}*/


	/*for.DecalBuffer Component*/

	//Parent->AddChild(this);
#pragma endregion

	return S_OK;
}

_uint CEmptyEffect::Update(_double TimeDelta)
{
	if (__super::Update(TimeDelta))
		return E_FAIL;
	
	m_fFrameTime += (_float)TimeDelta;
	
	m_fSpriteNum += (_float)TimeDelta * m_fSpriteSpeed;


	if (m_fSpriteNum >= (_float)m_iSpriteNumTotal)
	{
		m_SpriteEnd = true;
		m_fSpriteNum = 0;
	}

	CComponent* OBB = GetComponent("Com_OBB");
	if (OBB)
	{
		dynamic_cast<CBasicCollider*>(OBB)->Update_State(m_pTransformCom->GetWorldMatrix());
	}
	

	UpdateFadeEffect(TimeDelta);


	return S_OK;
}

_uint CEmptyEffect::LateUpdate(_double TimeDelta)
{
	if (nullptr == m_pRendererCom)
		return -1;
		

	if (m_pParent)
	{
		_matrix matParent;
		_vector scale, rotation, position;
		XMMatrixDecompose(&scale, &rotation, &position, m_pParentTransformCom->GetWorldMatrix());
		scale = XMVectorSet(1.f, 1.f, 1.f, 0.f);		
		matParent = XMMatrixAffineTransformation(scale, XMVectorSet(0.f, 0.f, 0.f, 0.f), rotation, position);
		
		m_pTransformCom->SetMatrix(m_pLocalTransformCom->GetWorldMatrix() * matParent);
	}
	else if (m_pTargetObject)
	{		
		CModel* pTargetModel = static_cast<CModel*>(m_pTargetObject->GetComponent("Com_Model"));
		CTransform* pTargetTransform = static_cast<CTransform*>(m_pTargetObject->GetComponent("Com_Transform"));
		_matrix TargetMatrix;
		_vector scale, rotation, position;
		XMMatrixDecompose(&scale, &rotation, &position, pTargetTransform->GetWorldMatrix());
		scale = XMVectorSet(1.f, 1.f, 1.f, 0.f);
		TargetMatrix = XMMatrixAffineTransformation(scale, XMVectorSet(0.f, 0.f, 0.f, 0.f), rotation, position);

		m_pTransformCom->SetMatrix(m_pLocalTransformCom->GetWorldMatrix() * pTargetModel->Get_BoneMatrix(m_TargetBoneName.c_str()) * TargetMatrix);
	}

	CComponent* RectInstCom = GetComponent("Com_RectInstance");
	if (RectInstCom)
		static_cast<CVIBuffer_RectInstance*>(RectInstCom)->Update(TimeDelta);

	CComponent* TrailCom = GetComponent("Com_Trail");
	if (TrailCom != nullptr)
		m_pRendererCom->AddRenderGroup(CRenderer::RENDER_TRAIL, this);
	else
	m_pRendererCom->AddRenderGroup(CRenderer::RENDER_ALPHA, this);

	return 0;
}

HRESULT CEmptyEffect::Render(_uint iPassIndex)
{
	if (!m_bIsActive)
		return S_OK;

	//iPassIndex = m_iPassIndex;


#ifdef _DEBUG
	CComponent* OBB = GetComponent("Com_OBB");
	if (OBB)
		dynamic_cast<CBasicCollider*>(OBB)->Render();
#endif
	if (m_bBillBord) {
		_matrix viewInverse = XMMatrixInverse(nullptr, CEngine::GetInstance()->GetTransform(CPipeline::D3DTS_VIEW));
		_float4x4 newWorld;
		_float4x4 world = m_pTransformCom->GetMatrix();
		_vector scale, rotation, position;
		XMMatrixDecompose(&scale, &rotation, &position, m_pTransformCom->GetWorldMatrix());
		XMStoreFloat4x4(&newWorld, viewInverse);
		memcpy(newWorld.m[3], world.m[3], sizeof(_float3));
		m_pTransformCom->SetMatrix(XMMatrixScalingFromVector(scale) * XMLoadFloat4x4(&newWorld));
	}

	/* Render RectTexture */
	CComponent* buffer = GetComponent("Com_VIBuffer");
	if (buffer)
	{
		SetUp_ValueOnShader("Com_VIBuffer");

		if (m_pTexture[TEXTURE_DIFFUSE])
			static_cast<CVIBuffer*>(buffer)->GetShader()->SetUp_TextureOnShader("g_DiffuseTexture", m_pTexture[TEXTURE_DIFFUSE]);

		if (m_pTexture[TEXTURE_MASK])
			static_cast<CVIBuffer*>(buffer)->GetShader()->SetUp_TextureOnShader("g_MaskTexture", m_pTexture[TEXTURE_MASK]);

		if (m_pTexture[TEXTURE_NOISE])
			static_cast<CVIBuffer*>(buffer)->GetShader()->SetUp_TextureOnShader("g_NoiseTexture", m_pTexture[TEXTURE_NOISE]);

		if (m_pTexture[TEXTURE_NORMAL])
			static_cast<CVIBuffer*>(buffer)->GetShader()->SetUp_TextureOnShader("g_NoiseTexture", m_pTexture[TEXTURE_NORMAL]);

		static_cast<CVIBuffer*>(buffer)->Render(m_iPassIndex);
	}

	/* Render Mesh */
	CComponent* modelCom = GetComponent("Com_Model");
	if (modelCom)
	{
		CModel* pModel = static_cast<CModel*>(modelCom);

		SetUp_ValueOnModelShader();
		pModel->Bind_Buffers(m_iPassIndex);
		
		_uint		iNumMaterials = pModel->Get_NumMaterials();
				
		for (_uint i = 0; i < iNumMaterials; ++i)
		{
			pModel->SetUp_TextureOnShader("g_DiffuseTexture", i, aiTextureType_DIFFUSE);

			if (m_pTexture[TEXTURE_MASK])
				pModel->GetShader()->SetUp_TextureOnShader("g_MaskTexture", m_pTexture[TEXTURE_MASK]);

			if (m_pTexture[TEXTURE_NOISE])
				pModel->GetShader()->SetUp_TextureOnShader("g_NoiseTexture", m_pTexture[TEXTURE_NOISE]);

			pModel->Render(i, m_iPassIndex);
		}

	}

	/* Render Decal */
	CComponent* DecalCom = GetComponent("Com_Decal");
	if (DecalCom)
	{
		CVIBuffer_Cube* pDecal = static_cast<CVIBuffer_Cube*>(DecalCom);
		ID3D11ShaderResourceView* pDepthSRV = m_pRendererCom->GetShaderResourceView("Target_Depth");
		if (pDepthSRV == nullptr)
			return E_FAIL;

		pDecal->GetShader()->SetUp_TextureOnShader("g_DepthTexture", pDepthSRV);

		_matrix WordlMatrixInv = m_pTransformCom->GetWorldMatrixInverse();

		pDecal->GetShader()->SetUp_ValueOnShader("g_WorldMatrixInv", &XMMatrixTranspose(WordlMatrixInv), sizeof(_float4x4));

		_matrix viewMatrix = CEngine::GetInstance()->GetTransform(CPipeline::D3DTS_VIEW);
		viewMatrix = XMMatrixInverse(nullptr, viewMatrix);
		pDecal->GetShader()->SetUp_ValueOnShader("g_ViewMatrixInv", &XMMatrixTranspose(viewMatrix), sizeof(_float4x4));

		_matrix projMatrix = CEngine::GetInstance()->GetTransform(CPipeline::D3DTS_PROJ);
		projMatrix = XMMatrixInverse(nullptr, projMatrix);
		pDecal->GetShader()->SetUp_ValueOnShader("g_ProjMatrixInv", &XMMatrixTranspose(projMatrix), sizeof(_float4x4));
		
		pDecal->Render();
	}

	/* Render PointInstance */
	CComponent* PointInstanceCom = GetComponent("Com_PointInstance");
	if (PointInstanceCom)
	{
		SetUp_ValueOnShader("Com_PointInstance");
		static_cast<CVIBuffer*>(PointInstanceCom)->GetShader()->SetUp_ValueOnShader("g_vCamPosition", &m_pEngine->GetCamPosition(), sizeof(_vector));

		if (m_pTexture[TEXTURE_DIFFUSE])
			static_cast<CVIBuffer*>(PointInstanceCom)->GetShader()->SetUp_TextureOnShader("g_DiffuseTexture", m_pTexture[TEXTURE_DIFFUSE]);

		if (m_pTexture[TEXTURE_MASK])
			static_cast<CVIBuffer*>(PointInstanceCom)->GetShader()->SetUp_TextureOnShader("g_MaskTexture", m_pTexture[TEXTURE_MASK]);

		if (m_pTexture[TEXTURE_NOISE])
			static_cast<CVIBuffer*>(PointInstanceCom)->GetShader()->SetUp_TextureOnShader("g_NoiseTexture", m_pTexture[TEXTURE_NOISE]);

		if (m_pTexture[TEXTURE_NORMAL])
			static_cast<CVIBuffer*>(buffer)->GetShader()->SetUp_TextureOnShader("g_NoiseTexture", m_pTexture[TEXTURE_NORMAL]);

		static_cast<CVIBuffer_PointInstance*>(PointInstanceCom)->Render(m_iPassIndex);
	}

	/* Render RectInstance */
	CComponent* RectInstanceCom = GetComponent("Com_RectInstance");
	if (RectInstanceCom)
	{		
		static_cast<CVIBuffer*>(RectInstanceCom)->GetShader()->SetUp_ValueOnShader("g_ViewMatrix", &XMMatrixTranspose(CEngine::GetInstance()->GetTransform(CPipeline::D3DTS_VIEW)), sizeof(_matrix));
		static_cast<CVIBuffer*>(RectInstanceCom)->GetShader()->SetUp_ValueOnShader("g_ProjMatrix", &XMMatrixTranspose(CEngine::GetInstance()->GetTransform(CPipeline::D3DTS_PROJ)), sizeof(_matrix));
		
		if (m_pTexture[TEXTURE_MASK])
			static_cast<CVIBuffer*>(RectInstanceCom)->GetShader()->SetUp_TextureOnShader("g_MaskTexture", m_pTexture[TEXTURE_MASK]);
		
		static_cast<CVIBuffer_RectInstance*>(RectInstanceCom)->Render(m_iPassIndex);
	}
	
	/* Render Trail */
	CComponent* TrailCom = GetComponent("Com_Trail");
	if (TrailCom)
	{
		SetUp_ValueOnShader("Com_Trail");
		if (m_pTexture[TEXTURE_DIFFUSE])
			static_cast<CVIBuffer*>(TrailCom)->GetShader()->SetUp_TextureOnShader("g_DiffuseTexture", m_pTexture[TEXTURE_DIFFUSE]);

		if (m_pTexture[TEXTURE_MASK])
			static_cast<CVIBuffer*>(TrailCom)->GetShader()->SetUp_TextureOnShader("g_MaskTexture", m_pTexture[TEXTURE_MASK]);

		if (m_pTexture[TEXTURE_NOISE])
			static_cast<CVIBuffer*>(TrailCom)->GetShader()->SetUp_TextureOnShader("g_NoiseTexture", m_pTexture[TEXTURE_NOISE]);

		static_cast<CVIBuffer_Trail*>(TrailCom)->Render(m_iPassIndex);
	}


	return S_OK;
}

void CEmptyEffect::SetUp_ValueOnShader(string ComponentTag)
{	
	CComponent* buffer = GetComponent(ComponentTag);
	if (buffer == nullptr)
	{
		MSG_BOX("Failed To SetUp_ValueOnShader - Effect");
		return;
	}
	
	static_cast<CVIBuffer*>(buffer)->GetShader()->SetUp_ValueOnShader("g_WorldMatrix", &XMMatrixTranspose(m_pTransformCom->GetWorldMatrix()), sizeof(_matrix));
	static_cast<CVIBuffer*>(buffer)->GetShader()->SetUp_ValueOnShader("g_ViewMatrix", &XMMatrixTranspose(CEngine::GetInstance()->GetTransform(CPipeline::D3DTS_VIEW)), sizeof(_matrix));
	static_cast<CVIBuffer*>(buffer)->GetShader()->SetUp_ValueOnShader("g_ProjMatrix", &XMMatrixTranspose(CEngine::GetInstance()->GetTransform(CPipeline::D3DTS_PROJ)), sizeof(_matrix));

	static_cast<CVIBuffer*>(buffer)->GetShader()->SetUp_ValueOnShader("g_fFrameTime", &m_fFrameTime, sizeof(_float));
	static_cast<CVIBuffer*>(buffer)->GetShader()->SetUp_ValueOnShader("g_vScrollSpeedX", &m_vScrollSpeedX, sizeof(_float3));
	static_cast<CVIBuffer*>(buffer)->GetShader()->SetUp_ValueOnShader("g_vScrollSpeedY", &m_vScrollSpeedY, sizeof(_float3));
	static_cast<CVIBuffer*>(buffer)->GetShader()->SetUp_ValueOnShader("g_vScale", &m_vScale, sizeof(_float3));
	//static_cast<CVIBuffer*>(buffer)->GetShader()->SetUp_ValueOnShader("g_fPadding", &m_fPadding, sizeof(_float));

	static_cast<CVIBuffer*>(buffer)->GetShader()->SetUp_ValueOnShader("g_vDistortion", &m_vDistortion, sizeof(_float2) * 3);
	static_cast<CVIBuffer*>(buffer)->GetShader()->SetUp_ValueOnShader("g_fDistortionScale", &m_fDistortionScale, sizeof(_float));
	static_cast<CVIBuffer*>(buffer)->GetShader()->SetUp_ValueOnShader("g_fDistortionBias", &m_fDistortionBias, sizeof(_float));

	static_cast<CVIBuffer*>(buffer)->GetShader()->SetUp_ValueOnShader("g_fFadeAlpha", &m_fFadeAlpha, sizeof(_float));
	static_cast<CVIBuffer*>(buffer)->GetShader()->SetUp_ValueOnShader("g_fAlpha", &m_fAlpha, sizeof(_float));
	
	_uint iSpriteNum = (_uint)m_fSpriteNum;
	static_cast<CVIBuffer*>(buffer)->GetShader()->SetUp_ValueOnShader("g_iSpriteNum", &iSpriteNum, sizeof(_uint));
	static_cast<CVIBuffer*>(buffer)->GetShader()->SetUp_ValueOnShader("g_iSpriteNumX", &m_iSpriteNumX, sizeof(_uint));
	static_cast<CVIBuffer*>(buffer)->GetShader()->SetUp_ValueOnShader("g_iSpriteNumY", &m_iSpriteNumY, sizeof(_uint));

}

void CEmptyEffect::SetUp_ValueOnModelShader()
{
	CComponent* pModel = GetComponent("Com_Model");

	static_cast<CModel*>(pModel)->GetShader()->SetUp_ValueOnShader("g_fFrameTime", &m_fFrameTime, sizeof(_float));
	static_cast<CModel*>(pModel)->GetShader()->SetUp_ValueOnShader("g_vScrollSpeedX", &m_vScrollSpeedX, sizeof(_float3));
	static_cast<CModel*>(pModel)->GetShader()->SetUp_ValueOnShader("g_vScrollSpeedY", &m_vScrollSpeedY, sizeof(_float3));
	static_cast<CModel*>(pModel)->GetShader()->SetUp_ValueOnShader("g_vScale", &m_vScale, sizeof(_float3));

	static_cast<CModel*>(pModel)->GetShader()->SetUp_ValueOnShader("g_vDistortion", &m_vDistortion, sizeof(_float2) * 3);
	static_cast<CModel*>(pModel)->GetShader()->SetUp_ValueOnShader("g_fDistortionScale", &m_fDistortionScale, sizeof(_float));
	static_cast<CModel*>(pModel)->GetShader()->SetUp_ValueOnShader("g_fDistortionBias", &m_fDistortionBias, sizeof(_float));

	static_cast<CModel*>(pModel)->GetShader()->SetUp_ValueOnShader("g_fFadeAlpha", &m_fFadeAlpha, sizeof(_float));
	static_cast<CModel*>(pModel)->GetShader()->SetUp_ValueOnShader("g_fAlpha", &m_fAlpha, sizeof(_float));

	if(CEngine::GetInstance()->GetCurrentUsage() == CEngine::USAGE::USAGE_TOOL)
		static_cast<CModel*>(pModel)->GetShader()->SetUp_ValueOnShader("g_ProcessTime", &m_fFrameTime, sizeof(_float));
	else
		static_cast<CModel*>(pModel)->GetShader()->SetUp_ValueOnShader("g_ProcessTime", &m_fProcessTime, sizeof(_float));
	static_cast<CModel*>(pModel)->GetShader()->SetUp_ValueOnShader("g_UVSpd", &m_fMoveSpd, sizeof(_float));
}

string CEmptyEffect::GetTextureFilePath(TEXTUREID idx)
{
	if (m_pTexture[idx] != nullptr)
		return m_pTexture[idx]->GetFilePath();

	return "";
}

void CEmptyEffect::SetTexture(string TextureFilePath, TEXTUREID eTextureId)
{
	if (TextureFilePath == "")
		return;

	if (m_pTexture[eTextureId])
	{
		SafeRelease(m_pTexture[eTextureId]);
		m_pTexture[eTextureId] = nullptr;
	}

	char	szExt[MAX_PATH] = "";

	_splitpath(TextureFilePath.c_str(), nullptr, nullptr, nullptr, szExt);

	if (!strcmp(szExt, ".dds") || !strcmp(szExt, ".DDS"))
		m_pTexture[eTextureId] = CTexture::Create(m_pDevice, m_pDeviceContext, CTexture::TYPE_DDS, TextureFilePath);
	else if (!strcmp(szExt, ".tga") || !strcmp(szExt, ".TGA") || !strcmp(szExt, ".Tga"))
		m_pTexture[eTextureId] = CTexture::Create(m_pDevice, m_pDeviceContext, CTexture::TYPE_TGA, TextureFilePath);
	else
		m_pTexture[eTextureId] = CTexture::Create(m_pDevice, m_pDeviceContext, CTexture::TYPE_WIC, TextureFilePath);
}

HRESULT CEmptyEffect::SetUpComponents()
{
	if (FAILED(__super::SetUpComponents(0, "Prototype_Transform", "Com_Transform", (CComponent**)&m_pTransformCom)))
		return E_FAIL;

	if (FAILED(__super::SetUpComponents(0, "Prototype_Transform", "Com_ParentTransform", (CComponent**)&m_pParentTransformCom)))
		return E_FAIL;

	if (FAILED(__super::SetUpComponents(0, "Prototype_Transform", "Com_LocalTransform", (CComponent**)&m_pLocalTransformCom)))
		return E_FAIL;

	if (FAILED(__super::SetUpComponents(0, "Prototype_Renderer", "Com_Renderer", (CComponent**)&m_pRendererCom)))
		return E_FAIL;	

	//AddComponent(0, "Prototype_VIBuffer_Cube", "Com_Decal", GetComponent("Com_Transform"));

	return S_OK;
}

void CEmptyEffect::LinkTransform()
{	
	m_pTargetObject = m_pEngine->FindGameObjectWithName(m_pEngine->GetCurSceneNumber(), m_TargetObjName);
	if (m_pTargetObject == nullptr)
		return;

	m_bLinkTarget = true;
	//m_pLocalTransformCom->SetMatrix(m_pTransformCom->GetWorldMatrix());
}

void CEmptyEffect::LinkTranformWithParent()
{
	if (m_pParent)
	{
		m_pParentTransformCom = static_cast<CTransform*>(m_pParent->GetComponent("Com_Transform"));
		m_bisChild = true;
	}
}

void CEmptyEffect::UpdateFadeEffect(_double TimeDelta)
{
	if (m_bFadeIn || m_bFadeOut)
	{
		m_fEffectFrameTime += (_float)TimeDelta;

		if (m_fEffectFrameTime > m_fEffectDuration)
		{
			m_fEffectFrameTime = 0.f;
			m_fFadeAlpha = 1.f;
			m_vZoomScale = { 1.f, 1.f, 1.f };
		}
	}
	else
	{
		m_fFadeAlpha = 1.f;
		m_vZoomScale = { 1.f, 1.f, 1.f };
		return;
	}

	if (m_bFadeIn)
	{
		if (m_fEffectFrameTime >= m_fFadeInStratTime && m_fEffectFrameTime <= m_fFadeInStratTime + m_fFadeInDuration)
		{
			_float t = (m_fEffectFrameTime - m_fFadeInStratTime) / m_fFadeInDuration;
			m_fFadeAlpha = XMVectorGetX(XMVectorLerp(XMVectorSet(0.f, 0.f, 0.f, 0.f), XMVectorSet(1.f, 1.f, 1.f, 1.f), t));
		}
	}

	if (m_bFadeOut)
	{
		if (m_fEffectFrameTime >= m_fFadeOutStratTime && m_fEffectFrameTime <= m_fFadeOutStratTime + m_fFadeOutDuration)
		{
			_float t = (m_fEffectFrameTime - m_fFadeOutStratTime) / m_fFadeOutDuration;
			m_fFadeAlpha = XMVectorGetX(XMVectorLerp(XMVectorSet(1.f, 1.f, 1.f, 1.f), XMVectorSet(0.f, 0.f, 0.f, 0.f), t));
		}
	}	
}
