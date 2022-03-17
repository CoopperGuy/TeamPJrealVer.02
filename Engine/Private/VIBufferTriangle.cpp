#include "EnginePCH.h"
#include "VIBufferTriangle.h"
#include "Engine.h"

CVIBufferTriangle::CVIBufferTriangle(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext, string shaderPath)
	: CVIBuffer(pDevice, pDeviceContext)
{
	m_shaderPath = shaderPath;
}

CVIBufferTriangle::CVIBufferTriangle(const CVIBufferTriangle & rhs)
	: CVIBuffer(rhs)
{
}

HRESULT CVIBufferTriangle::InitializePrototype(_fvector * pPoints)
{
	if (FAILED(__super::InitializePrototype()))
		return E_FAIL;

#pragma region VERTEXBUFFER

	m_iStride = sizeof(VTXCOLOR);
	m_iNumVertices = 3;
	m_iNumVertexBuffers = 1;

	/* For.D3D11_BUFFER_DESC */
	m_VBDesc.ByteWidth = m_iStride * m_iNumVertices;
	m_VBDesc.Usage = D3D11_USAGE_IMMUTABLE;
	m_VBDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	m_VBDesc.CPUAccessFlags = 0;
	m_VBDesc.MiscFlags = 0;
	m_VBDesc.StructureByteStride = m_iStride;

	m_pVertices = new VTXCOLOR[m_iNumVertices];
	ZeroMemory(m_pVertices, sizeof(VTXCOLOR) * m_iNumVertices);

	XMStoreFloat3(&((VTXCOLOR*)m_pVertices)[0].vPos, pPoints[0]);
	((VTXCOLOR*)m_pVertices)[0].vColor = _float4(1.f, 1.f, 1.f, 1.f);

	XMStoreFloat3(&((VTXCOLOR*)m_pVertices)[1].vPos, pPoints[1]);
	((VTXCOLOR*)m_pVertices)[1].vColor = _float4(1.f, 1.f, 1.f, 1.f);

	XMStoreFloat3(&((VTXCOLOR*)m_pVertices)[2].vPos, pPoints[2]);
	((VTXCOLOR*)m_pVertices)[2].vColor = _float4(1.f, 1.f, 1.f, 1.f);

	/* For.D3D11_SUBRESOURCE_DATA */
	m_VBSubResourceData.pSysMem = m_pVertices;

#pragma endregion VERTEXBUFFER

#pragma region INDEXBUFFER

	m_iNumPrimitive = 3;
	m_iNumVerticesPerPrimitive = 2;
	m_eIndexFormat = DXGI_FORMAT_R16_UINT;
	m_ePrimitive = D3D_PRIMITIVE_TOPOLOGY_LINELIST;

	/* For.D3D11_BUFFER_DESC */
	m_IBDesc.ByteWidth = sizeof(LINEINDICES16) * m_iNumPrimitive;
	m_IBDesc.Usage = D3D11_USAGE_IMMUTABLE;
	m_IBDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	m_IBDesc.CPUAccessFlags = 0;
	m_IBDesc.MiscFlags = 0;
	m_IBDesc.StructureByteStride = 0;

	LINEINDICES16*		pIndices = new LINEINDICES16[m_iNumPrimitive];
	ZeroMemory(pIndices, sizeof(LINEINDICES16) * m_iNumPrimitive);

	pIndices[0]._0 = 0;
	pIndices[0]._1 = 1;

	pIndices[1]._0 = 1;
	pIndices[1]._1 = 2;

	pIndices[2]._0 = 2;
	pIndices[2]._1 = 0;

	m_IBSubResourceData.pSysMem = pIndices;

#pragma endregion INDEXBUFFER

	if (FAILED(__super::Create_Buffers()))
		return E_FAIL;


	///* 현재 정점이 어떤 멤버들을 가지고 있어! */
	//D3D11_INPUT_ELEMENT_DESC		ElmentDesc[] = {
	//	{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	//	{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	//};

	//if (FAILED(__super::Compile_Shader(ElmentDesc, 2, pShaderFilePath)))
	//	return E_FAIL;
	m_pShader = make_unique<CShader>("../../Assets/Shader/Shader_TriangleDBG.fx");
	SafeDeleteArray(pIndices);

	return S_OK;
}

HRESULT CVIBufferTriangle::Initialize(void * pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	//m_pShader = make_unique<CShader>("../../Assets/Shader/Shader_TriangleDBG.fx");

	return S_OK;
}

HRESULT CVIBufferTriangle::Render(_uint iPassIndex)
{
	if (nullptr == m_pDeviceContext)
		return E_FAIL;

	_uint		iOffset = 0;
	m_pDeviceContext->IASetVertexBuffers(0, m_iNumVertexBuffers, m_pVB.GetAddressOf(), &m_iStride, &iOffset);
	if (m_IBSubResourceData.pSysMem)
		m_pDeviceContext->IASetIndexBuffer(m_pIB.Get(), m_eIndexFormat, 0);
	m_pDeviceContext->IASetPrimitiveTopology(m_ePrimitive);

	m_pShader->SetUp_ValueOnShader("g_WorldMatrix", &XMMatrixIdentity(), sizeof(_matrix));
	m_pShader->SetUp_ValueOnShader("g_ViewMatrix", &XMMatrixTranspose(CEngine::GetInstance()->GetTransform(CPipeline::D3DTS_VIEW)), sizeof(_matrix));
	m_pShader->SetUp_ValueOnShader("g_ProjMatrix", &XMMatrixTranspose(CEngine::GetInstance()->GetTransform(CPipeline::D3DTS_PROJ)), sizeof(_matrix));
	m_pShader->SetUp_ValueOnShader("vColor", &m_Color, sizeof(_float4));

	if (iPassIndex > 1) {
		iPassIndex = 1;
	}
	m_pShader->Render(iPassIndex);
	if (m_IBSubResourceData.pSysMem)
		m_pDeviceContext->DrawIndexed(m_iNumPrimitive * m_iNumVerticesPerPrimitive, 0, 0);
	else
		m_pDeviceContext->Draw(m_iNumVertices, 0);

	return S_OK;


	//__super::Render(0);
	return S_OK;
}

CVIBufferTriangle * CVIBufferTriangle::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext, _fvector * pPoints, string shaderPath)
{
	CVIBufferTriangle*	pInstance = new CVIBufferTriangle(pDevice, pDeviceContext, shaderPath);

	if (FAILED(pInstance->InitializePrototype(pPoints)))
	{
		MSG_BOX("Failed To Creating CVIBufferTriangle");
		SafeRelease(pInstance);
	}
	return pInstance;
}

CComponent * CVIBufferTriangle::Clone(void * pArg)
{
	CVIBufferTriangle*	pInstance = new CVIBufferTriangle(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed To Creating CVIBufferTriangle Clone");
		SafeRelease(pInstance);
	}
	return pInstance;
}

void CVIBufferTriangle::Free()
{
	__super::Free();
}
