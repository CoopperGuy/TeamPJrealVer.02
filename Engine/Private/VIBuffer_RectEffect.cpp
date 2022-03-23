#include "EnginePCH.h"
#include "..\public\VIBuffer_RectEffect.h"

CVIBuffer_RectEffect::CVIBuffer_RectEffect(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext, string shaderPath)
	: CVIBuffer(pDevice, pDeviceContext)	
{
	m_shaderPath = shaderPath;
}

CVIBuffer_RectEffect::CVIBuffer_RectEffect(const CVIBuffer_RectEffect & rhs)
	: CVIBuffer(rhs)
	, m_pTexture(rhs.m_pTexture)
{
	if (m_pTexture != nullptr)
		SafeAddRef(m_pTexture);

	if (rhs.m_pShader != nullptr)
		m_pShader = rhs.m_pShader;
}

HRESULT CVIBuffer_RectEffect::InitializePrototype()
{
	if (FAILED(__super::InitializePrototype()))
		return E_FAIL;	

#pragma region VERTEXBUFFER

	m_iStride = sizeof(VTXTEX);
	m_iNumVertices = 4;
	m_iNumVertexBuffers = 1;
	m_iNumVerticesPerPrimitive = 3;

	/* For.D3D11_BUFFER_DESC */
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

	m_iNumPrimitive = 2;
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

	pIndices[0]._0 = 0;
	pIndices[0]._1 = 1;
	pIndices[0]._2 = 2;

	pIndices[1]._0 = 0;
	pIndices[1]._1 = 2;
	pIndices[1]._2 = 3;

	m_IBSubResourceData.pSysMem = pIndices;
	
#pragma endregion INDEXBUFFER

	if (FAILED(__super::Create_Buffers()))
		return E_FAIL;

	//D3D11_INPUT_ELEMENT_DESC		ElmentDesc[] = {
	//	,

	//};

	SafeDeleteArray(pIndices);

	return S_OK;
}

HRESULT CVIBuffer_RectEffect::Initialize(void * pArg)
{	
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (m_pShader == nullptr)
		m_pShader = make_shared<CShader>("../../Assets/Shader/Shader_Effect.fx");

	return S_OK;
}

HRESULT CVIBuffer_RectEffect::Render(_uint iPassIndex)
{
	if (nullptr == m_pDeviceContext)
		return E_FAIL;

	if (m_pTexture)
		m_pShader->SetUp_TextureOnShader("g_DiffuseTexture", m_pTexture);

	_uint		iOffset = 0;
		
	m_pDeviceContext->IASetVertexBuffers(0, m_iNumVertexBuffers, m_pVB.GetAddressOf(), &m_iStride, &iOffset);
	if (m_IBSubResourceData.pSysMem)
		m_pDeviceContext->IASetIndexBuffer(m_pIB.Get(), m_eIndexFormat, 0);
	m_pDeviceContext->IASetPrimitiveTopology(m_ePrimitive);

	m_pShader->Render(iPassIndex);

	if (m_IBSubResourceData.pSysMem)
		m_pDeviceContext->DrawIndexed(m_iNumPrimitive * m_iNumVerticesPerPrimitive, 0, 0);
	else
		m_pDeviceContext->Draw(m_iNumVertices, 0);

	//__super::Render(iPassIndex);
	return S_OK;
}

CVIBuffer_RectEffect * CVIBuffer_RectEffect::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext, string shaderPath)
{
	CVIBuffer_RectEffect*	pInstance = new CVIBuffer_RectEffect(pDevice, pDeviceContext, shaderPath);

	if (FAILED(pInstance->InitializePrototype()))
	{
		MSG_BOX("Failed To Creating CVIBuffer_RectEffect");
		SafeRelease(pInstance);
	}
	return pInstance;
}

CComponent * CVIBuffer_RectEffect::Clone(void * pArg)
{
	CVIBuffer_RectEffect*	pInstance = new CVIBuffer_RectEffect(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed To Creating CVIBuffer_RectEffect");
		SafeRelease(pInstance);
	}
	return pInstance;
}

void CVIBuffer_RectEffect::Free()
{
	__super::Free();

	if (m_pTexture)
		SafeRelease(m_pTexture);
}

string CVIBuffer_RectEffect::GetTextureFilePath()
{
	if (m_pTexture)
		return m_pTexture->GetFilePath();
	return "";
}

void CVIBuffer_RectEffect::SetTexture(string TextureFilePath)
{
	if (TextureFilePath == "")
		return;

	if (m_pTexture)
	{
		SafeRelease(m_pTexture);
		m_pTexture = nullptr;
	}

	char	szExt[MAX_PATH] = "";

	_splitpath(TextureFilePath.c_str(), nullptr, nullptr, nullptr, szExt);

	if (!strcmp(szExt, ".dds") || !strcmp(szExt, ".DDS"))
		m_pTexture = CTexture::Create(m_pDevice, m_pDeviceContext, CTexture::TYPE_DDS, TextureFilePath);
	else if (!strcmp(szExt, ".tga") || !strcmp(szExt, ".TGA") || !strcmp(szExt, ".Tga"))
		m_pTexture = CTexture::Create(m_pDevice, m_pDeviceContext, CTexture::TYPE_TGA, TextureFilePath);
	else
		m_pTexture = CTexture::Create(m_pDevice, m_pDeviceContext, CTexture::TYPE_WIC, TextureFilePath);
}
