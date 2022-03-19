#include "EnginePCH.h"
#include "..\public\VIBuffer_Rect.h"
#include "Texture.h"
#include "Transform.h"
#include "Engine.h"

CVIBuffer_Rect::CVIBuffer_Rect(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext, string shaderPath)
	: CVIBuffer(pDevice, pDeviceContext)	
{
	m_shaderPath = shaderPath;
}

CVIBuffer_Rect::CVIBuffer_Rect(const CVIBuffer_Rect & rhs)
	: CVIBuffer(rhs)	
	, m_pTexture(rhs.m_pTexture)
	
{
	SafeAddRef(m_pTexture);
	m_pShader = rhs.m_pShader;
}

HRESULT CVIBuffer_Rect::InitializePrototype()
{
	if (FAILED(__super::InitializePrototype()))
		return E_FAIL;	

	m_pShader = make_shared<CShader>("../../Assets/Shader/Shader_Rect.fx");

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

HRESULT CVIBuffer_Rect::Initialize(void * pArg)
{
	// m_pShader = make_unique<CShader>("../Shaders/Shader_Rect.fx");

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;
	return S_OK;
}

HRESULT CVIBuffer_Rect::Render(_uint iPassIndex)
{

	_uint		iOffset = 0;
	m_pDeviceContext->IASetVertexBuffers(0, m_iNumVertexBuffers, m_pVB.GetAddressOf(), &m_iStride, &iOffset);
	if (m_IBSubResourceData.pSysMem)
		m_pDeviceContext->IASetIndexBuffer(m_pIB.Get(), m_eIndexFormat, 0);
	m_pDeviceContext->IASetPrimitiveTopology(m_ePrimitive);

	_matrix viewInverse = XMMatrixInverse(nullptr, CEngine::GetInstance()->GetTransform(CPipeline::D3DTS_VIEW));
	_float4x4 newWorld;
	_float4x4 world = m_pObjTransform->GetMatrix();
	_vector scale, rotation, position;
	XMMatrixDecompose(&scale, &rotation, &position, m_pObjTransform->GetWorldMatrix());
	XMStoreFloat4x4(&newWorld, viewInverse);
	memcpy(newWorld.m[3], world.m[3], sizeof(_float3));
	m_pObjTransform->SetMatrix(XMMatrixScalingFromVector(scale) * XMLoadFloat4x4(&newWorld));

	m_pShader->SetUp_ValueOnShader("g_WorldMatrix", &XMMatrixTranspose(m_pObjTransform->GetWorldMatrix()), sizeof(_matrix));
	m_pShader->SetUp_ValueOnShader("g_ViewMatrix", &XMMatrixTranspose(CEngine::GetInstance()->GetTransform(CPipeline::D3DTS_VIEW)), sizeof(_matrix));
	m_pShader->SetUp_ValueOnShader("g_ProjMatrix", &XMMatrixTranspose(CEngine::GetInstance()->GetTransform(CPipeline::D3DTS_PROJ)), sizeof(_matrix));
	m_pShader->SetUp_ValueOnShader("vColor", &m_Color, sizeof(_float4));
	m_pShader->SetUp_ValueOnShader("g_Alpha", &m_fAlpha, sizeof(_float));

	if (m_pTexture)
		m_pShader->SetUp_TextureOnShader("Diffuse", m_pTexture);

	if (iPassIndex > 1) {
		iPassIndex = 1;
	}
	m_pShader->Render(iPassIndex);
	if (m_IBSubResourceData.pSysMem)
		m_pDeviceContext->DrawIndexed(m_iNumPrimitive * m_iNumVerticesPerPrimitive, 0, 0);
	else
		m_pDeviceContext->Draw(m_iNumVertices, 0);
	return S_OK;
}

CVIBuffer_Rect * CVIBuffer_Rect::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext, string shaderPath)
{
	CVIBuffer_Rect*	pInstance = new CVIBuffer_Rect(pDevice, pDeviceContext, shaderPath);

	if (FAILED(pInstance->InitializePrototype()))
	{
		MSG_BOX("Failed To Creating CVIBuffer_Rect");
		SafeRelease(pInstance);
	}
	return pInstance;
}

CComponent * CVIBuffer_Rect::Clone(void * pArg)
{
	CVIBuffer_Rect*	pInstance = new CVIBuffer_Rect(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed To Creating CVIBuffer_Rect");
		SafeRelease(pInstance);
	}
	return pInstance;
}

void CVIBuffer_Rect::Free()
{
	__super::Free();
	SafeRelease(m_pTexture);
}

string CVIBuffer_Rect::GetTextureFilePath()
{
	if (m_pTexture)
		return m_pTexture->GetFilePath();
	return "";
}

void CVIBuffer_Rect::UpdateTexture(string texturePath)
{
	if (m_pTexture) {
		SafeRelease(m_pTexture);
		m_pTexture = nullptr;
	}
	CTexture::TEXTURETYPE type = CTexture::TYPE_END;

	FILESYSTEM::path path = texturePath;
	string ext = path.extension().string();

	if (ext == ".dds" || ext == ".DDS")
		type = CTexture::TYPE_DDS;
	else if (ext == ".tga" || ext == ".TGA" || ext == ".Tga")
		type = CTexture::TYPE_TGA;
	else
		type = CTexture::TYPE_WIC;


	m_pTexture = CTexture::Create(m_pDevice, m_pDeviceContext, type, texturePath);
}
