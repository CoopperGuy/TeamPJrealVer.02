#include "EnginePCH.h"
#include "..\public\VIBuffer_Trail.h"
#include "Texture.h"
#include "Transform.h"
#include "Engine.h"

CVIBuffer_Trail::CVIBuffer_Trail(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext, string shaderPath)
	: CVIBuffer(pDevice, pDeviceContext)
{
	m_shaderPath = shaderPath;
}

CVIBuffer_Trail::CVIBuffer_Trail(const CVIBuffer_Trail & rhs)
	: CVIBuffer(rhs)
{
}

HRESULT CVIBuffer_Trail::InitializePrototype()
{
	if (FAILED(__super::InitializePrototype()))
		return E_FAIL;

#pragma region VERTEXBUFFER

	m_iStride = sizeof(VTXTEX);
	m_iNumVertices = 200;
	m_iNumVertexBuffers = 1;
	m_iNumVerticesPerPrimitive = 3;

	/* For.D3D11_BUFFER_DESC */
	m_VBDesc.ByteWidth = m_iStride * m_iNumVertices;
	m_VBDesc.Usage = D3D11_USAGE_DYNAMIC;
	m_VBDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	m_VBDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	m_VBDesc.MiscFlags = 0;
	m_VBDesc.StructureByteStride = m_iStride;

	m_pVertices = new VTXTEX[m_iNumVertices];
	ZeroMemory(m_pVertices, sizeof(VTXTEX) * m_iNumVertices);

	for (_uint i = 0; i < m_iNumVertices; i += 2)
	{
		((VTXTEX*)m_pVertices)[i].vPosition = _float3(0.f, 0.f, 0.f);
		((VTXTEX*)m_pVertices)[i].vTexUV = _float2(((_float)i / (_float)(m_iNumVertices - 2)), 1.f);

		((VTXTEX*)m_pVertices)[i + 1].vPosition = _float3(0.f, 0.f, 0.f);
		((VTXTEX*)m_pVertices)[i + 1].vTexUV = _float2(((_float)i / (_float)(m_iNumVertices - 2)), 0.f);
	}

	/* For.D3D11_SUBRESOURCE_DATA */
	m_VBSubResourceData.pSysMem = m_pVertices;

#pragma endregion VERTEXBUFFER

#pragma region INDEXBUFFER

	m_iNumPrimitive = m_iNumVertices - 2;
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

	for (_uint i = 0; i < m_iNumPrimitive; i += 2)
	{
		pIndices[i]._0 = i;
		pIndices[i]._1 = i + 3;
		pIndices[i]._2 = i + 2;

		pIndices[i + 1]._0 = i + 1;
		pIndices[i + 1]._1 = pIndices[i]._1;
		pIndices[i + 1]._2 = pIndices[i]._0;
	}

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

HRESULT CVIBuffer_Trail::Initialize(void * pArg)
{
	m_vecCatmullRom.reserve(1000);


	if (pArg)
	{
		_matrix WeaponTransform;
		memcpy(&WeaponTransform, pArg, sizeof(_matrix));

		D3D11_MAPPED_SUBRESOURCE		SubResource;

		if (FAILED(m_pDeviceContext->Map(m_pVB.Get(), 0, D3D11_MAP_WRITE_NO_OVERWRITE, 0, &SubResource)))
			return E_FAIL;

		_vector vLocalPosLow = XMVectorSet(0.f, -0.2f, 0.f, 0.f);
		_vector vLocalPosHigh = XMVectorSet(0.f, 0.5f, 0.f, 0.f);

		for (_uint i = 0; i < m_iNumVertices; i += 2)
		{
			XMStoreFloat3(&((VTXTEX*)SubResource.pData)[i].vPosition, XMVector3TransformCoord(vLocalPosLow, WeaponTransform));
			XMStoreFloat3(&((VTXTEX*)SubResource.pData)[i + 1].vPosition, XMVector3TransformCoord(vLocalPosHigh, WeaponTransform));
		}



		m_pDeviceContext->Unmap(m_pVB.Get(), 0);
	}

	m_pShader = make_unique<CShader>("../../Assets/Shader/Shader_Effect.fx");

	/*Diffuse*/
	char	szExt[MAX_PATH] = "";

	string TextureFilePath = "../../Assets/Textures/Effect/Diffuse/LV_ElRano_Object_SpermaPropB_E_LBR.dds";

	_splitpath(TextureFilePath.c_str(), nullptr, nullptr, nullptr, szExt);

	if (!strcmp(szExt, ".dds") || !strcmp(szExt, ".DDS"))
		m_pTexture = CTexture::Create(m_pDevice, m_pDeviceContext, CTexture::TYPE_DDS, TextureFilePath);
	else if (!strcmp(szExt, ".tga") || !strcmp(szExt, ".TGA") || !strcmp(szExt, ".Tga"))
		m_pTexture = CTexture::Create(m_pDevice, m_pDeviceContext, CTexture::TYPE_TGA, TextureFilePath);
	else
		m_pTexture = CTexture::Create(m_pDevice, m_pDeviceContext, CTexture::TYPE_WIC, TextureFilePath);

	/*Mask*/
	TextureFilePath = "../../Assets/Textures/Effect/Mask/Trun_FX_Trail02_Tex_HKJ.jpg";

	_splitpath(TextureFilePath.c_str(), nullptr, nullptr, nullptr, szExt);

	if (!strcmp(szExt, ".dds") || !strcmp(szExt, ".DDS"))
		m_pTexture1 = CTexture::Create(m_pDevice, m_pDeviceContext, CTexture::TYPE_DDS, TextureFilePath);
	else if (!strcmp(szExt, ".tga") || !strcmp(szExt, ".TGA") || !strcmp(szExt, ".Tga"))
		m_pTexture1 = CTexture::Create(m_pDevice, m_pDeviceContext, CTexture::TYPE_TGA, TextureFilePath);
	else
		m_pTexture1 = CTexture::Create(m_pDevice, m_pDeviceContext, CTexture::TYPE_WIC, TextureFilePath);


	/*Noise*/
	TextureFilePath = "../../Assets/Textures/Effect/Noise/FX_AEAura.jpg";

	_splitpath(TextureFilePath.c_str(), nullptr, nullptr, nullptr, szExt);

	if (!strcmp(szExt, ".dds") || !strcmp(szExt, ".DDS"))
		m_pTexture2 = CTexture::Create(m_pDevice, m_pDeviceContext, CTexture::TYPE_DDS, TextureFilePath);
	else if (!strcmp(szExt, ".tga") || !strcmp(szExt, ".TGA") || !strcmp(szExt, ".Tga"))
		m_pTexture2 = CTexture::Create(m_pDevice, m_pDeviceContext, CTexture::TYPE_TGA, TextureFilePath);
	else
		m_pTexture2 = CTexture::Create(m_pDevice, m_pDeviceContext, CTexture::TYPE_WIC, TextureFilePath);

	for (_int i = m_iNumVertices; i > 0; i--) {
		m_vecCatmullRom.push_back(((VTXTEX*)m_pVertices)[i]);
	}


	return S_OK;
}

HRESULT CVIBuffer_Trail::Update(_double TimeDelta, _fmatrix WeaponTransform)
{
	m_TimeAcc += TimeDelta;


	D3D11_MAPPED_SUBRESOURCE		SubResource;

	if (FAILED(m_pDeviceContext->Map(m_pVB.Get(), 0, D3D11_MAP_WRITE_NO_OVERWRITE, 0, &SubResource)))
		return E_FAIL;


	for (_uint i = 0; i < (m_iNumVertices - 2); i += 2)
	{
		((VTXTEX*)SubResource.pData)[(m_iNumVertices - 2) - i].vPosition = ((VTXTEX*)SubResource.pData)[(m_iNumVertices - 2) - (i + 2)].vPosition;
		((VTXTEX*)SubResource.pData)[(m_iNumVertices - 1) - i].vPosition = ((VTXTEX*)SubResource.pData)[(m_iNumVertices - 1) - (i + 2)].vPosition;
	}

	_vector vLocalPosLow = XMVectorSet(0.f, -0.2f, 0.f, 0.f);
	_vector vLocalPosHigh = XMVectorSet(0.f, 0.5f, 0.f, 0.f);

	DirectX::XMStoreFloat3(&((VTXTEX*)SubResource.pData)[0].vPosition, XMVector3TransformCoord(vLocalPosLow, WeaponTransform));
	DirectX::XMStoreFloat3(&((VTXTEX*)SubResource.pData)[1].vPosition, XMVector3TransformCoord(vLocalPosHigh, WeaponTransform));

	if (m_bisActive) {



		if (m_iVtxCnt < 4) {
			m_vecCatmullRom.emplace_back(((VTXTEX*)SubResource.pData)[0]);
			m_vecCatmullRom.emplace_back(((VTXTEX*)SubResource.pData)[1]);

			m_iCatmullRomIdx[0] = 0;
			m_iCatmullRomIdx[1] = 2;

			m_iVtxCnt += 2;
		}
		else {
			m_iEndIdx = m_iCatmullRomCnt * 2 + m_iVtxCnt;

			m_iCatmullRomIdx[2] = m_iEndIdx - 2;
			m_iCatmullRomIdx[3] = m_iEndIdx;

			m_vecCatmullRom.resize(m_iEndIdx + 2);

			m_vecCatmullRom[m_iEndIdx - 2].vPosition = m_vecCatmullRom[m_iVtxCnt - 2].vPosition;
			m_vecCatmullRom[m_iEndIdx - 1].vPosition = m_vecCatmullRom[m_iVtxCnt - 1].vPosition;

			m_vecCatmullRom[m_iEndIdx].vPosition = ((VTXTEX*)SubResource.pData)[0].vPosition;
			m_vecCatmullRom[m_iEndIdx + 1].vPosition = ((VTXTEX*)SubResource.pData)[1].vPosition;

			for (_uint i = 0; i < m_iCatmullRomCnt; i++) {
				_int idx = i * 2 + m_iVtxCnt - 2;
				_float fWeight = _float(i + 1) / (m_iCatmullRomCnt + 1);

				_vector pos0 = XMVectorCatmullRom(
					XMLoadFloat3(&m_vecCatmullRom[m_iCatmullRomIdx[0]].vPosition),
					XMLoadFloat3(&m_vecCatmullRom[m_iCatmullRomIdx[1]].vPosition),
					XMLoadFloat3(&m_vecCatmullRom[m_iCatmullRomIdx[2]].vPosition),
					XMLoadFloat3(&m_vecCatmullRom[m_iCatmullRomIdx[3]].vPosition),
					fWeight);

				_vector pos1 = XMVectorCatmullRom(
					XMLoadFloat3(&m_vecCatmullRom[m_iCatmullRomIdx[0] + 1].vPosition),
					XMLoadFloat3(&m_vecCatmullRom[m_iCatmullRomIdx[1] + 1].vPosition),
					XMLoadFloat3(&m_vecCatmullRom[m_iCatmullRomIdx[2] + 1].vPosition),
					XMLoadFloat3(&m_vecCatmullRom[m_iCatmullRomIdx[3] + 1].vPosition),
					fWeight);

				//m_vecCatmullRom[m_iVtxCnt - 2 + i].vPosition;
				XMStoreFloat3(&m_vecCatmullRom[idx].vPosition, pos0);
				XMStoreFloat3(&m_vecCatmullRom[idx+1].vPosition, pos1);
			}

			if (m_vecCatmullRom.size() > m_iNumVertices) {
				for (size_t i = m_vecCatmullRom.size() - 1; i > (m_vecCatmullRom.size() - m_iNumVertices); i--) {
					((VTXTEX*)SubResource.pData)[(m_vecCatmullRom.size() - 1) - i].vPosition = m_vecCatmullRom[i].vPosition;
				}
			}
			else {
				for (size_t i = m_vecCatmullRom.size() - 1; i > 0; i--) {
					((VTXTEX*)SubResource.pData)[(m_vecCatmullRom.size() - 1) - i].vPosition = m_vecCatmullRom[i].vPosition;
				}
				for (_uint i = m_vecCatmullRom.size() - 1; i < m_iNumVertices; i++) {
					((VTXTEX*)SubResource.pData)[i].vPosition = m_vecCatmullRom[m_vecCatmullRom.size() - 1].vPosition;
				}
			}

			for (_uint i = 0; i < m_iNumVertices; i += 2)
			{
				((VTXTEX*)SubResource.pData)[i].vTexUV = _float2(((_float)i / (_float)(m_iNumVertices - 2)), 1.f);
				((VTXTEX*)SubResource.pData)[i + 1].vTexUV = _float2(((_float)i / (_float)(m_iNumVertices - 2)), 0.f);
			}


			m_iVtxCnt = m_iEndIdx + 2;

			m_iCatmullRomIdx[0] = m_iCatmullRomIdx[1];
			m_iCatmullRomIdx[1] = m_iCatmullRomIdx[2];

		}
	}
	m_pDeviceContext->Unmap(m_pVB.Get(), 0);

	if (m_bisActive) {
		m_fAlpha += 2.f * (_float)TimeDelta;
		if (m_fAlpha >= 0.f)
			m_fAlpha = 0.f;
	}
	else {
		m_fAlpha -= 4.f * (_float)TimeDelta;
		if (m_fAlpha <= -1.f) {
			m_fAlpha = -1.f;
			m_vecCatmullRom.clear();
			m_iVtxCnt = 0;
		}
	}
	return S_OK;
}

HRESULT CVIBuffer_Trail::Render(_uint iPassIndex)
{
	if (m_fAlpha <= -0.99f)
		return S_OK;
	_uint		iOffset = 0;

	_float3 m_vScrollSpeedX = { 0.5f, 0.5f, 0.f };
	_float3 m_vScrollSpeedY = { 0.f, 0.f, 0.f };
	_float3 m_vScale = { 1.f, 2.f, 3.f };
	_float m_fPadding = 0.f;

	_float2 m_vDistortion[3] = { { 0.1f, 0.2f },{ 0.1f, 0.3f },{ 0.1f, 0.1f } };
	_float	m_fDistortionScale = 4.f;
	_float	m_fDistortionBias = 1.f;
	_float	m_fFadeAlpha = 1.f;

	m_pShader->SetUp_ValueOnShader("g_fFrameTime", &m_TimeAcc, sizeof(_float));
	m_pShader->SetUp_ValueOnShader("g_vScrollSpeedX", &m_vScrollSpeedX, sizeof(_float3));
	m_pShader->SetUp_ValueOnShader("g_vScrollSpeedY", &m_vScrollSpeedY, sizeof(_float3));
	m_pShader->SetUp_ValueOnShader("g_vScale", &m_vScale, sizeof(_float3));

	m_pShader->SetUp_ValueOnShader("g_vDistortion", &m_vDistortion, sizeof(_float2) * 3);
	m_pShader->SetUp_ValueOnShader("g_fDistortionScale", &m_fDistortionScale, sizeof(_float));
	m_pShader->SetUp_ValueOnShader("g_fDistortionBias", &m_fDistortionBias, sizeof(_float));

	m_pShader->SetUp_ValueOnShader("g_fFadeAlpha", &m_fFadeAlpha, sizeof(_float));

	m_pShader->SetUp_ValueOnShader("g_AlphaSet", &m_fAlpha, sizeof(_float));


	m_pShader->SetUp_ValueOnShader("g_ViewMatrix", &XMMatrixTranspose(CEngine::GetInstance()->GetTransform(CPipeline::D3DTS_VIEW)), sizeof(_matrix));
	m_pShader->SetUp_ValueOnShader("g_ProjMatrix", &XMMatrixTranspose(CEngine::GetInstance()->GetTransform(CPipeline::D3DTS_PROJ)), sizeof(_matrix));
	//m_pShader->SetUp_ValueOnShader("vColor", &m_Color, sizeof(_float4));

	if (m_pTexture)
		m_pShader->SetUp_TextureOnShader("g_DiffuseTexture", m_pTexture);
	if (m_pTexture)
		m_pShader->SetUp_TextureOnShader("g_MaskTexture", m_pTexture1);
	if (m_pTexture)
		m_pShader->SetUp_TextureOnShader("g_NoiseTexture", m_pTexture2);

	m_pDeviceContext->IASetVertexBuffers(0, m_iNumVertexBuffers, m_pVB.GetAddressOf(), &m_iStride, &iOffset);
	m_pDeviceContext->IASetIndexBuffer(m_pIB.Get(), m_eIndexFormat, 0);
	m_pDeviceContext->IASetPrimitiveTopology(m_ePrimitive);
	m_pShader->Render(3);

	m_pDeviceContext->DrawIndexed(m_iNumPrimitive * m_iNumVerticesPerPrimitive, 0, 0);

	return S_OK;
}

CVIBuffer_Trail * CVIBuffer_Trail::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext, string shaderPath)
{
	CVIBuffer_Trail*	pInstance = new CVIBuffer_Trail(pDevice, pDeviceContext, shaderPath);

	if (FAILED(pInstance->InitializePrototype()))
	{
		MSG_BOX("Failed To Creating CVIBuffer_Trail");
		SafeRelease(pInstance);
	}
	return pInstance;
}

CComponent * CVIBuffer_Trail::Clone(void * pArg)
{
	CVIBuffer_Trail*	pInstance = new CVIBuffer_Trail(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed To Creating CVIBuffer_Trail");
		SafeRelease(pInstance);
	}
	return pInstance;
}

void CVIBuffer_Trail::Free()
{
	__super::Free();
	SafeRelease(m_pTexture);
	SafeRelease(m_pTexture1);
	SafeRelease(m_pTexture2);
}

string CVIBuffer_Trail::GetTextureFilePath()
{
	if (m_pTexture)
		return m_pTexture->GetFilePath();
	return "";
}

void CVIBuffer_Trail::UpdateTexture(string texturePath)
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
