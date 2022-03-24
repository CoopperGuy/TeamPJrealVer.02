#include "EnginePCH.h"
#include "..\Public\VIBuffer_RectInstance.h"
#include "Pipeline.h"
#include "Engine.h"

CVIBuffer_RectInstance::CVIBuffer_RectInstance(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
	: CVIBuffer(pDevice, pDeviceContext)	
{

}

CVIBuffer_RectInstance::CVIBuffer_RectInstance(const CVIBuffer_RectInstance & rhs)
	: CVIBuffer(rhs)
	, m_VBInstanceDesc(rhs.m_VBInstanceDesc)
	//, m_VBInstanceSubResourceData(rhs.m_VBInstanceSubResourceData)
	//, m_pVBInstance(rhs.m_pVBInstance)
	, m_iNumInstance(rhs.m_iNumInstance)
	//, m_InstanceMatrices(rhs.m_InstanceMatrices)
	, m_shaderPath(rhs.m_shaderPath)
	, m_fLifeTime(rhs.m_fLifeTime)
	, m_iInstNum(rhs.m_iInstNum)
	, m_vColor(rhs.m_vColor)
	, m_fSpeed(rhs.m_fSpeed)
	, m_fSize(rhs.m_fSize)
	, m_fStartRadian(rhs.m_fStartRadian)
	, m_fRadiationAngle(rhs.m_fRadiationAngle)
	, m_eShape(rhs.m_eShape)
{
	//SafeAddRef(m_pVBInstance);

	if (rhs.m_pShader != nullptr)
		m_pShader = rhs.m_pShader;
}

HRESULT CVIBuffer_RectInstance::InitializePrototype(string pShaderFilePath, _uint iNumInstance)
{
	if (FAILED(__super::InitializePrototype()))
		return E_FAIL;	

	//m_iNumInstance = iNumInstance;
	m_iNumInstance = 100;
	m_iNumVertexBuffers = 2;
	m_iInstNum = m_iNumInstance;

#pragma region VERTEXBUFFER

	m_iStride = sizeof(VTXTEX);
	m_iNumVertices = 4;
	
	
	m_VBDesc.ByteWidth = m_iStride * m_iNumVertices;
	m_VBDesc.Usage = D3D11_USAGE_IMMUTABLE;
	m_VBDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	m_VBDesc.CPUAccessFlags = 0;
	m_VBDesc.MiscFlags = 0;
	m_VBDesc.StructureByteStride = m_iStride;

	m_pVertices = new VTXTEX[m_iNumVertices];
	ZeroMemory(m_pVertices, sizeof(VTXTEX) * m_iNumVertices);

	((VTXTEX*)m_pVertices)[0].vPosition = _float3(-0.5f, 0.5f, 0.f);
	((VTXTEX*)m_pVertices)[0].vTexUV = _float2(0.f, 0.f);

	((VTXTEX*)m_pVertices)[1].vPosition = _float3(0.5f, 0.5f, 0.f);
	((VTXTEX*)m_pVertices)[1].vTexUV = _float2(1.f, 0.f);

	((VTXTEX*)m_pVertices)[2].vPosition = _float3(0.5f, -0.5f, 0.f);
	((VTXTEX*)m_pVertices)[2].vTexUV = _float2(1.f, 1.f);

	((VTXTEX*)m_pVertices)[3].vPosition = _float3(-0.5f, -0.5f, 0.f);
	((VTXTEX*)m_pVertices)[3].vTexUV = _float2(0.f, 1.f);

	/* For.D3D11_SUBRESOURCE_DATA */
	m_VBSubResourceData.pSysMem = m_pVertices;

#pragma endregion VERTEXBUFFER

#pragma region INDEXBUFFER

	m_iNumPrimitive = 2 * m_iNumInstance;
	m_iNumVerticesPerPrimitive = 3;
	m_eIndexFormat = DXGI_FORMAT_R16_UINT;
	m_ePrimitive = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;

	/* For.D3D11_BUFFER_DESC */
	m_IBDesc.ByteWidth = sizeof(POLYGONINDICES16) * m_iNumPrimitive;
	m_IBDesc.Usage = D3D11_USAGE_IMMUTABLE;
	m_IBDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	m_IBDesc.CPUAccessFlags = 0;
	m_IBDesc.MiscFlags = 0;
	m_IBDesc.StructureByteStride = 0;

	POLYGONINDICES16*		pIndices = new POLYGONINDICES16[m_iNumPrimitive];
	ZeroMemory(pIndices, sizeof(POLYGONINDICES16) * m_iNumPrimitive);

	_uint		iNumPolygons = 0;

	for (_uint i = 0; i < m_iNumInstance; ++i)
	{
		pIndices[iNumPolygons]._0 = 0;
		pIndices[iNumPolygons]._1 = 1;
		pIndices[iNumPolygons]._2 = 2;
		++iNumPolygons;

		pIndices[iNumPolygons]._0 = 0;
		pIndices[iNumPolygons]._1 = 2;
		pIndices[iNumPolygons]._2 = 3;
		++iNumPolygons;
	}

	m_IBSubResourceData.pSysMem = pIndices;
	
#pragma endregion

	if (FAILED(__super::Create_Buffers()))
		return E_FAIL;	

#pragma region VERTEXINSTANCEBUFFER

	ZeroMemory(&m_VBInstanceDesc, sizeof(D3D11_BUFFER_DESC));

	m_VBInstanceDesc.ByteWidth = m_iNumInstance * sizeof(VTXRECTINST);
	m_VBInstanceDesc.Usage = D3D11_USAGE_DYNAMIC;
	m_VBInstanceDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	m_VBInstanceDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	m_VBInstanceDesc.MiscFlags = 0;
	m_VBInstanceDesc.StructureByteStride = sizeof(VTXRECTINST);

	VTXRECTINST*			pInstanceVertices = new VTXRECTINST[m_iNumInstance];
	m_VBInstanceSubResourceData.pSysMem = pInstanceVertices;	

	if (FAILED(m_pDevice->CreateBuffer(&m_VBInstanceDesc, &m_VBInstanceSubResourceData, &m_pVBInstance)))
		return E_FAIL;

	SafeDeleteArray(pInstanceVertices);
	SafeDeleteArray(pIndices);
	
#pragma endregion 

	m_shaderPath = pShaderFilePath;
	return S_OK;
}



HRESULT CVIBuffer_RectInstance::Initialize(void * pArg)
{
	if (m_pShader == nullptr)
		m_pShader = make_unique<CShader>(m_shaderPath);
	
	VTXRECTINST*			pInstanceVertices = new VTXRECTINST[m_iNumInstance];
	m_VBInstanceSubResourceData.pSysMem = pInstanceVertices;

	if (FAILED(m_pDevice->CreateBuffer(&m_VBInstanceDesc, &m_VBInstanceSubResourceData, &m_pVBInstance)))
		return E_FAIL;

	SafeDeleteArray(pInstanceVertices);

	if (pArg)
		m_pTargetTransform = (CTransform*)pArg;

	switch (m_eShape)
	{
	case Engine::CVIBuffer_RectInstance::RADIATION:
		Initialize_Radiation();
		break;
	case Engine::CVIBuffer_RectInstance::CONE:
		Initialize_Cone();
		break;
	}

	return S_OK;
}

HRESULT CVIBuffer_RectInstance::Update(_double TimeDelta)
{
	m_fLifeTimeAcc += (_float)TimeDelta;

	CEngine* pEngine = CEngine::GetInstance();
	if (pEngine == nullptr)
		return E_FAIL;

	D3D11_MAPPED_SUBRESOURCE		SubResource;

	/*CPipeline*		pPipeLine = GET_INSTANCE(CPipeline);
	
	sort(m_InstanceMatrices.begin(), m_InstanceMatrices.end(), [&](VTXRECTINST* SourMatrix, VTXRECTINST* DestMatrix) {
		_vector		SourPosition = XMVector4Transform(XMLoadFloat4(&SourMatrix->vPosition), pPipeLine->Get_Transform(CPipeline::D3DTS_VIEW));
		_vector		DestPosition = XMVector4Transform(XMLoadFloat4(&DestMatrix->vPosition), pPipeLine->Get_Transform(CPipeline::D3DTS_VIEW));

		if (XMVectorGetZ(SourPosition) > XMVectorGetZ(DestPosition))
			return true;

		return false;
	});

	RELEASE_INSTANCE(CPipeline);*/

	if (FAILED(m_pDeviceContext->Map(m_pVBInstance, 0, D3D11_MAP_WRITE_NO_OVERWRITE, 0, &SubResource)))
		return E_FAIL;	
	
	for (_uint i = 0; i < m_iNumInstance; ++i)
	{
		if (m_InstanceMatrices[i]->iRenderEnable == 1)
		{
			if (CEngine::GetInstance()->GetCurrentUsage() == CEngine::USAGE::USAGE_TOOL)
			{
				//billbord	
				_vector vLook = XMVector3Normalize(pEngine->GetCamPosition()) - XMLoadFloat4(&m_InstanceMatrices[i]->vPosition);
				_vector vRight = XMVectorSet(cosf(m_InstanceMatrices[i]->fRadian), sinf(m_InstanceMatrices[i]->fRadian), 0.f, 0.f);
				_vector vUp = XMVector3Normalize(XMVector3Cross(vLook, vRight));
				vRight = XMVector3Normalize(XMVector3Cross(vUp, vLook));

				XMStoreFloat4(&m_InstanceMatrices[i]->vDir, vRight);
				XMStoreFloat4(&m_InstanceMatrices[i]->vRight, vRight * m_InstanceMatrices[i]->fStartSize * m_fSize);
				XMStoreFloat4(&m_InstanceMatrices[i]->vUp, vUp * m_InstanceMatrices[i]->fStartSize * m_fSize);
				XMStoreFloat4(&m_InstanceMatrices[i]->vLook, vLook);
			}

			if (CEngine::GetInstance()->GetCurrentUsage() == CEngine::USAGE::USAGE_TOOL && m_fLifeTime < m_fLifeTimeAcc)
			{
				_float3 OffsetPosition = { 0.f, 0.f, 0.f };
				if (m_pTargetTransform != nullptr)
					XMStoreFloat3(&OffsetPosition, m_pTargetTransform->GetState(CTransform::STATE_POSITION));
				m_InstanceMatrices[i]->vPosition = _float4(OffsetPosition.x, OffsetPosition.y, OffsetPosition.z, 1.f);
			}
			else
				XMStoreFloat4(&m_InstanceMatrices[i]->vPosition, XMLoadFloat4(&m_InstanceMatrices[i]->vPosition) + XMLoadFloat4(&m_InstanceMatrices[i]->vDir) * TimeDelta * m_InstanceMatrices[i]->fStartSpeed * m_fSpeed);			
		}

		((VTXRECTINST*)SubResource.pData)[i] = *m_InstanceMatrices[i];
	}	

	m_pDeviceContext->Unmap(m_pVBInstance, 0);

	m_fAlpha = 1.f - (m_fLifeTimeAcc / m_fLifeTime);

	if (m_fLifeTime < m_fLifeTimeAcc)
		m_fLifeTimeAcc = 0.f;

	return S_OK;
}

HRESULT CVIBuffer_RectInstance::Render(_uint iPassIndex)
{
	if (nullptr == m_pDeviceContext)
		return E_FAIL;



	ID3D11Buffer*	pVBBuffers[] = {
		m_pVB.Get(), 		
		m_pVBInstance
	};

	_uint	iStrides[] = {
		sizeof(VTXTEX),
		sizeof(VTXRECTINST)
	};

	_uint		iOffset[] = {
		0, 0
	};

	m_pDeviceContext->IASetVertexBuffers(0, m_iNumVertexBuffers, pVBBuffers, iStrides, iOffset);
	m_pDeviceContext->IASetIndexBuffer(m_pIB.Get(), m_eIndexFormat, 0);
	m_pDeviceContext->IASetPrimitiveTopology(m_ePrimitive);
	
	m_pShader->SetUp_ValueOnShader("g_Color", &m_vColor, sizeof(_float4));
	m_pShader->SetUp_ValueOnShader("g_Alpha", &m_fAlpha, sizeof(_float));
	m_pShader->Render(iPassIndex);
	
	m_pDeviceContext->DrawIndexedInstanced(6, m_iNumInstance, 0, 0, 0);
	
	return S_OK;
}

HRESULT CVIBuffer_RectInstance::Initialize_Radiation()
{
	for (auto pIv : m_InstanceMatrices)
	{
		if (pIv)
			SafeDelete(pIv);
	}
	m_InstanceMatrices.clear();

	_float3 OffsetPosition = { 0.f, 0.f, 0.f };
	m_fLifeTimeAcc = 0.f;

	if (m_pTargetTransform != nullptr)
		XMStoreFloat3(&OffsetPosition, m_pTargetTransform->GetState(CTransform::STATE_POSITION));

	_uint iQuarterNum = m_iInstNum / 4;
	if (iQuarterNum == 0)
		iQuarterNum = 1;

	for (_uint i = 0; i < m_iNumInstance; ++i)
	{
		_uint IQuarter = i / iQuarterNum;
		
		VTXRECTINST*		pIV = new VTXRECTINST();		
		pIV->vPosition = _float4(OffsetPosition.x, OffsetPosition.y, OffsetPosition.z, 1.f);
		pIV->fStartSize = 0.1f + (rand() % 5 * 0.1f);
		pIV->fStartSpeed = 0.1f + (rand() % 201 * 0.001f);
		pIV->fRadian = XMConvertToRadians(_float((90 * IQuarter) + (rand() % 90)));
		//XMStoreFloat4(&pIV->vDir, XMVectorSet(cosf(pIV->fRadian), sinf(pIV->fRadian), 0.f, 0.f));

		_vector vLook = XMVector3Normalize(CEngine::GetInstance()->GetCamPosition()) - XMLoadFloat4(&pIV->vPosition);
		_vector vRight = XMVectorSet(cosf(pIV->fRadian), sinf(pIV->fRadian), 0.f, 0.f);
		_vector vUp = XMVector3Normalize(XMVector3Cross(vLook, vRight));
		vRight = XMVector3Normalize(XMVector3Cross(vUp, vLook));

		XMStoreFloat4(&pIV->vDir, vRight);

		if (CEngine::GetInstance()->GetCurrentUsage() == CEngine::USAGE::USAGE_TOOL)
		{
			XMStoreFloat4(&pIV->vRight, vRight);
			XMStoreFloat4(&pIV->vUp, vUp);
			XMStoreFloat4(&pIV->vLook, vLook);
		}
		else
		{
			XMStoreFloat4(&pIV->vRight, vRight * pIV->fStartSize * m_fSize);
			XMStoreFloat4(&pIV->vUp, vUp * pIV->fStartSize * m_fSize);
			XMStoreFloat4(&pIV->vLook, vLook);
		}

		if (i < m_iInstNum)
			pIV->iRenderEnable = 1;
		else
			pIV->iRenderEnable = 0;

		m_InstanceMatrices.push_back(pIV);
	}

	return S_OK;
}

HRESULT CVIBuffer_RectInstance::Initialize_Cone()
{
	for (auto pIv : m_InstanceMatrices)
	{
		if (pIv)
			SafeDelete(pIv);
	}
	m_InstanceMatrices.clear();

	_float3 OffsetPosition = { 0.f, 0.f, 0.f };
	m_fLifeTimeAcc = 0.f;

	if (m_pTargetTransform != nullptr)
		XMStoreFloat3(&OffsetPosition, m_pTargetTransform->GetState(CTransform::STATE_POSITION));
	
	for (_uint i = 0; i < m_iNumInstance; ++i)
	{
		VTXRECTINST*		pIV = new VTXRECTINST();		
		pIV->vPosition = _float4(OffsetPosition.x, OffsetPosition.y, OffsetPosition.z, 1.f);
		pIV->fStartSize = 0.1f + (rand() % 5 * 0.1f);
		pIV->fStartSpeed = 0.1f + (rand() % 201 * 0.001f);
		pIV->fRadian = XMConvertToRadians(float(m_fStartRadian + (rand() % ((_int)m_fRadiationAngle + 1) - ((_int)m_fRadiationAngle / 2))));
		XMStoreFloat4(&pIV->vDir, XMVectorSet(cosf(pIV->fRadian), sinf(pIV->fRadian), 0.f, 0.f));

		_vector vLook = XMVector3Normalize(CEngine::GetInstance()->GetCamPosition()) - XMLoadFloat4(&pIV->vPosition);
		_vector vRight = XMVectorSet(cosf(pIV->fRadian), sinf(pIV->fRadian), 0.f, 0.f);
		_vector vUp = XMVector3Normalize(XMVector3Cross(vLook, vRight));
		vRight = XMVector3Normalize(XMVector3Cross(vUp, vLook));

		if (CEngine::GetInstance()->GetCurrentUsage() == CEngine::USAGE::USAGE_TOOL)
		{
			XMStoreFloat4(&pIV->vRight, vRight);
			XMStoreFloat4(&pIV->vUp, vUp);
			XMStoreFloat4(&pIV->vLook, vLook);
		}
		else
		{
			XMStoreFloat4(&pIV->vRight, vRight * pIV->fStartSize * m_fSize);
			XMStoreFloat4(&pIV->vUp, vUp * pIV->fStartSize * m_fSize);
			XMStoreFloat4(&pIV->vLook, vLook);
		}

		if (i < m_iInstNum)
			pIV->iRenderEnable = 1;
		else
			pIV->iRenderEnable = 0;

		m_InstanceMatrices.push_back(pIV);
	}

	return S_OK;
}

CVIBuffer_RectInstance * CVIBuffer_RectInstance::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext, string pShaderFilePath, _uint iNumInstance)
{
	CVIBuffer_RectInstance*	pInstance = new CVIBuffer_RectInstance(pDevice, pDeviceContext);

	if (FAILED(pInstance->InitializePrototype(pShaderFilePath, iNumInstance)))
	{
		MSG_BOX("Failed To Creating CVIBuffer_Rect");
		SafeRelease(pInstance);
	}
	return pInstance;
}

CComponent * CVIBuffer_RectInstance::Clone(void * pArg)
{
	CVIBuffer_RectInstance*	pInstance = new CVIBuffer_RectInstance(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed To Creating CVIBuffer_Rect");
		SafeRelease(pInstance);
	}
	return pInstance;
}

void CVIBuffer_RectInstance::Free()
{
	__super::Free();
	for (auto& matrix : m_InstanceMatrices)
		SafeDelete(matrix);
	m_InstanceMatrices.clear();

	SafeRelease(m_pVBInstance);
}
