#include "EnginePCH.h"
#include "..\public\InstanceModel.h"
#include "InsMeshContainer.h"
#include "InsHierarchyNode.h"
#include "Texture.h"
#include "InsChannel.h"
#include "Animation.h"
#include "Engine.h"
#include "Pipeline.h"
#include "ModelManager.h"

CInstanceModel::CInstanceModel(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
	: CComponent(pDevice, pDeviceContext)
{

}

CInstanceModel::CInstanceModel(const CInstanceModel & rhs)
	: CComponent(rhs)
	, m_pScene(rhs.m_pScene)
	, m_iNumVertices(rhs.m_iNumVertices)
	, m_iNumFaces(rhs.m_iNumFaces)
	, m_pVertices(rhs.m_pVertices)
	, m_pPolygonIndices(rhs.m_pPolygonIndices)
	, m_ModelTextures(rhs.m_ModelTextures)
	, m_pVB(rhs.m_pVB)
	, m_pIB(rhs.m_pIB)
	, m_iStride(rhs.m_iStride)
	, m_EffectDescs(rhs.m_EffectDescs)
	, m_pEffect(rhs.m_pEffect)
	, m_PivotMatrix(rhs.m_PivotMatrix)
	, m_iInstanceCount(rhs.m_iInstanceCount)
	, m_VBInstanceDesc(rhs.m_VBInstanceDesc)
	, m_VBInstanceSubResourceData(rhs.m_VBInstanceSubResourceData)
	, m_InstanceMatrices(rhs.m_InstanceMatrices)
	, m_pVBInstance(rhs.m_pVBInstance)
	, Pos(rhs.Pos)

{
	/* 메시컨테이너의 뼈들 중, HierarchyNode*의 주소에는 아직 값을 채우지 않았다. */
	for (auto& pPrototypeMeshContainer : rhs.m_MeshContainers)
	{
		CInsMeshContainer*	pMeshContainer = pPrototypeMeshContainer->Clone();

		m_MeshContainers.push_back(pMeshContainer);
	}

	for (auto& pMeshTextues : m_ModelTextures)
	{
		for (_uint i = 0; i < AI_TEXTURE_TYPE_MAX; ++i)
			SafeAddRef(pMeshTextues->pModelTexture[i]);
	}

	SafeAddRef(m_pVB);
	SafeAddRef(m_pIB);

	for (auto& pEffectDesc : m_EffectDescs)
	{
		SafeAddRef(pEffectDesc.pLayout);
		SafeAddRef(pEffectDesc.pPass);
	}

	SafeAddRef(m_pEffect);
	SafeAddRef(m_pVBInstance);
}

HRESULT CInstanceModel::NativeConstruct_Prototype()
{

	return S_OK;
}

HRESULT CInstanceModel::NativeConstruct(void * pArg)
{
	//m_pInstanceDesc = new INSMODELDESC;
	//ZeroMemory(m_pInstanceDesc, sizeof(INSMODELDESC));

	//memcpy(&m_pInstanceDesc, pArg, sizeof(INSMODELDESC));
	INSMODELDESC* m_pInstanceDesc = (INSMODELDESC*)pArg;

	m_pMeshFilePath = m_pInstanceDesc->pMeshFilePath;
	m_pMeshFileName = m_pInstanceDesc->pMeshFileName;
	m_pShaderFilePath = "../../Assets/Shader/Shader_ModelInstance.fx";

	//CTransform* pos = (CTransform*)m_pInstanceDesc.pArg;
	//Pos = pos->GetState(CTransform::STATE_POSITION);

	char		szFullPath[MAX_PATH] = "";

	strcpy_s(szFullPath, m_pInstanceDesc->pMeshFilePath);
	strcat_s(szFullPath, m_pInstanceDesc->pMeshFileName);

	m_iInstanceCount = m_pInstanceDesc->_InstancingCount;

	/* 메시의 정보 읽는다. */
	m_pScene = m_Importer.ReadFile(szFullPath, aiProcess_ConvertToLeftHanded | aiProcess_Triangulate | aiProcess_CalcTangentSpace);
	if (nullptr == m_pScene)
	{
		MSG_BOX("CInstanceModel :: m_pScene is nullptr");
		return E_FAIL;
	};

	for (_uint i = 0; i < m_pScene->mNumMeshes; ++i)
	{
		m_iNumVertices += m_pScene->mMeshes[i]->mNumVertices;
		m_iNumFaces += m_pScene->mMeshes[i]->mNumFaces;
	}

	m_pVertices = new VTXMESH[m_iNumVertices];
	ZeroMemory(m_pVertices, sizeof(VTXMESH) * m_iNumVertices);

	m_pPolygonIndices = new POLYGONINDICES32[m_iNumFaces];
	ZeroMemory(m_pPolygonIndices, sizeof(POLYGONINDICES32) * m_iNumFaces);

	_uint		iStartVertexIndex = 0;
	_uint		iStartFaceIndex = 0;


	m_MeshContainers.reserve(m_pScene->mNumMeshes);

	for (_uint i = 0; i < m_pScene->mNumMeshes; ++i)
	{
		if (FAILED(Create_MeshContainer(m_pScene->mMeshes[i], &iStartVertexIndex, &iStartFaceIndex, XMMatrixIdentity())))
			return E_FAIL;
	}

	m_iStride = sizeof(VTXMESH);



	/* 현재 모델의 머테리얼들을 로드한다. */
	m_ModelTextures.reserve(m_pScene->mNumMaterials);

	for (_uint i = 0; i < m_pScene->mNumMaterials; ++i)
	{
		if (FAILED(Create_Materials(m_pScene->mMaterials[i], m_pInstanceDesc->pMeshFilePath)))
			return E_FAIL;
	}


	/* 메시컨테이너들을 머테리얼 기준으로 묶고 정렬한다. */
	if (FAILED(Sort_MeshesByMaterial()))
		return E_FAIL;

	if (false == m_pScene->HasAnimations())
	{
		/* 정점버퍼, 인덱스 버퍼를 생성하낟. */
		if (FAILED(Create_VertexIndexBuffer(m_pShaderFilePath, m_pInstanceDesc->pTransfroms)))
			return E_FAIL;

		return S_OK;
	}


	return S_OK;
}


HRESULT CInstanceModel::Bind_Buffers(_uint iPassIndex, void* pArg)
{
	if (pArg == nullptr)
		return E_FAIL;


	m_pTransform = (CTransform*)pArg;

	SetUp_ValueOnShader("g_WorldMatrix", &XMMatrixTranspose(m_pTransform->GetWorldMatrix()), sizeof(_matrix));
	SetUp_ValueOnShader("g_ViewMatrix", &XMMatrixTranspose(CEngine::GetInstance()->GetTransform(CPipeline::D3DTS_VIEW)), sizeof(_matrix));
	SetUp_ValueOnShader("g_ProjMatrix", &XMMatrixTranspose(CEngine::GetInstance()->GetTransform(CPipeline::D3DTS_PROJ)), sizeof(_matrix));
	SetUp_ValueOnShader("g_fDissolve", &m_fDissolve, sizeof(_float));
	SetUp_TextureOnShader("g_DissolveTexture", CModelManager::GetInstance()->GetDissoveTex());

	//if (FAILED(m_EffectDescs[0].pPass->Apply(0, m_pDeviceContext)))

	if (iPassIndex == 0)
	{
		_uint iNumLight = CLightManager::GetInstance()->GetNumRenderLights();
		for (int i = 0; i < (_int)iNumLight; ++i)
		{
			SetUp_ValueOnShader(("g_LightViewMatrix" + to_string(i)).c_str(), &XMMatrixTranspose(CLightManager::GetInstance()->GetViewMatrix(i)), sizeof(_matrix));
			SetUp_ValueOnShader(("g_LightProjMatrix" + to_string(i)).c_str(), &XMMatrixTranspose(CLightManager::GetInstance()->GetProjMatrix(i)), sizeof(_matrix));
			SetUp_ValueOnShader(("lightPosition" + to_string(i)).c_str(), &CLightManager::GetInstance()->GetPosition(i), sizeof(_float3));
			SetUp_ValueOnShader(("lightDir" + to_string(i)).c_str(), &CLightManager::GetInstance()->GetDirection(i), sizeof(_float3));

			_float fAngle = CLightManager::GetInstance()->GetAngle(i);
			SetUp_ValueOnShader(("lightAngle" + to_string(i)).c_str(), &fAngle, sizeof(_float));
		}
	}
	else
	{
		_uint lightIndex = CLightManager::GetInstance()->GetCurrentIndex();
		SetUp_ValueOnShader("g_LightViewMatrix0", &XMMatrixTranspose(CLightManager::GetInstance()->GetViewMatrix(lightIndex)), sizeof(_matrix));
		SetUp_ValueOnShader("g_LightProjMatrix0", &XMMatrixTranspose(CLightManager::GetInstance()->GetProjMatrix(lightIndex)), sizeof(_matrix));
		SetUp_ValueOnShader("lightPosition0", &CLightManager::GetInstance()->GetPosition(lightIndex), sizeof(_float3));
	}


	return S_OK;
}

HRESULT CInstanceModel::Render(_uint iMaterialIndex, _uint iPassIndex)
{
	if (nullptr == m_pDeviceContext)
	{
		MSG_BOX("(class CInstanceModel : m_pDeviceContext is nullptr");
		return E_FAIL;
	}

	ID3D11Buffer*	pVBBuffers[] = {
		m_pVB,
		m_pVBInstance
	};

	_uint	iStrides[] = {
		sizeof(VTXMESH),
		sizeof(VTXMATRIX)
	};

	_uint		iOffset[] = {
		0, 0
	};

	if (iPassIndex != 0)
		iPassIndex = 1;

	if (iPassIndex == 0)
	{
		_uint iNumLights = CLightManager::GetInstance()->GetNumRenderLights();
		for (int i = 0; i < (_int)iNumLights; ++i)
		{
			ID3D11ShaderResourceView*	pShadowSRV = CLightManager::GetInstance()->GetShaderResourceView(i);
			if (pShadowSRV)
				SetUp_TextureOnShader(("depthMapTexture" + to_string(i)).c_str(), pShadowSRV);
		}
	}

	CEngine::GetInstance()->GetDeviceContext()->IASetInputLayout(m_EffectDescs[iPassIndex].pLayout);

	m_pDeviceContext->IASetVertexBuffers(0, 2, pVBBuffers, iStrides, iOffset);
	m_pDeviceContext->IASetIndexBuffer(m_pIB, DXGI_FORMAT_R32_UINT, 0);
	m_pDeviceContext->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	m_pDeviceContext->IASetInputLayout(m_EffectDescs[iPassIndex].pLayout);

	if (FAILED(m_EffectDescs[iPassIndex].pPass->Apply(iPassIndex, m_pDeviceContext)))
		return E_FAIL;

	for (auto& pMeshContainer : m_SortByMaterialMesh[iMaterialIndex])
	{
		_uint i = pMeshContainer->Get_NumFaces();
		_uint g = pMeshContainer->Get_StartFaceIndex();
		_uint e = pMeshContainer->Get_StartVertexIndex();

		m_pDeviceContext->DrawIndexedInstanced(pMeshContainer->Get_NumFaces() * 3, m_iInstanceCount, pMeshContainer->Get_StartFaceIndex() * 3, pMeshContainer->Get_StartVertexIndex(), 0);
		//commandList->DrawIndexedInstanced(indexCount, instanceCount, startIndex, vertexOffset, startInstanceLocation);
		//m_pDeviceContext->DrawIndexedInstanced(pMeshContainer->Get_NumFaces(), m_iInstanceCount, pMeshContainer->Get_StartFaceIndex(), pMeshContainer->Get_StartVertexIndex(), 0);
	}


	return S_OK;
}

HRESULT CInstanceModel::SetUp_ValueOnShader(const char * pConstantName, void * pData, _uint iByteSize)
{
	if (nullptr == m_pEffect)
		return E_FAIL;

	ID3DX11EffectVariable*		pVariable = m_pEffect->GetVariableByName(pConstantName);
	if (nullptr == pVariable)
		return E_FAIL;

	if (FAILED(pVariable->SetRawValue(pData, 0, iByteSize)))
		return E_FAIL;

	return S_OK;
}

HRESULT CInstanceModel::SetUp_TextureOnShader(const char * pConstantName, _uint iMaterialIndex, aiTextureType eTextureType)
{
	if (nullptr == m_pEffect)
		return E_FAIL;

	ID3DX11EffectShaderResourceVariable*		pVariable = m_pEffect->GetVariableByName(pConstantName)->AsShaderResource();
	if (nullptr == pVariable)
		return E_FAIL;


	if (nullptr != m_ModelTextures[iMaterialIndex]->pModelTexture[eTextureType])
	{
		ID3D11ShaderResourceView*		pShaderResourceView =
			m_ModelTextures[iMaterialIndex]->pModelTexture[eTextureType]->Get_ShaderResourceView(0);
		if (nullptr == pShaderResourceView)
			return E_FAIL;
		if (FAILED(pVariable->SetResource(pShaderResourceView)))
			return E_FAIL;
	}




	return S_OK;
}

HRESULT CInstanceModel::SetUp_TextureOnShader(const char * pConstantName, ID3D11ShaderResourceView * pSRV)
{

	if (nullptr == m_pEffect)
		return E_FAIL;

	ID3DX11EffectShaderResourceVariable*		pVariable = m_pEffect->GetVariableByName(pConstantName)->AsShaderResource();
	if (nullptr == pVariable)
		return E_FAIL;

	if (FAILED(pVariable->SetResource(pSRV)))
		return E_FAIL;

	return S_OK;
}

HRESULT CInstanceModel::SetUp_TextureOnShader(const char * pConstantName, CTexture * pTextureComponent, _uint iTextureIndex)
{
	if (nullptr == m_pEffect)
		return E_FAIL;

	ID3DX11EffectShaderResourceVariable* pVariable = m_pEffect->GetVariableByName(pConstantName)->AsShaderResource();
	if (nullptr == pVariable)
		return E_FAIL;

	ID3D11ShaderResourceView*		pShaderResourceView = pTextureComponent->Get_ShaderResourceView(iTextureIndex);
	if (nullptr == pShaderResourceView)
		return E_FAIL;

	if (FAILED(pVariable->SetResource(pShaderResourceView)))
		return E_FAIL;
	return S_OK;
}


const std::string CInstanceModel::WCHARToString(const wchar_t * ptsz)
{
	int len = (int)wcslen(ptsz);

	char* psz = new char[2 * len + 1];

	wcstombs(psz, ptsz, 2 * len + 1);
	std::string s = psz;

	delete[] psz;

	return s;
}

HRESULT CInstanceModel::SetEffectMaterial()
{
	return E_NOTIMPL;
}

HRESULT CInstanceModel::Create_MeshContainer(aiMesh * pMesh, _uint* pStartVertexIndex, _uint* pStartFaceIndex, _fmatrix PivotMatrix)
{
	_uint		iStartVertexIndex = *pStartVertexIndex;

	for (_uint i = 0; i < pMesh->mNumVertices; ++i)
	{
		/* 정점의 위치를 가지고 온다. */
		memcpy(&m_pVertices[*pStartVertexIndex].vPosition, &pMesh->mVertices[i], sizeof(_float3));
		_vector		vPosition;
		vPosition = XMLoadFloat3(&m_pVertices[*pStartVertexIndex].vPosition);
		vPosition = XMVector3TransformCoord(vPosition, PivotMatrix);
		XMStoreFloat3(&m_pVertices[*pStartVertexIndex].vPosition, vPosition);

		/* 정점의 노멀를 가지고 온다. */
		memcpy(&m_pVertices[*pStartVertexIndex].vNormal, &pMesh->mNormals[i], sizeof(_float3));
		_vector		vNormal;
		vNormal = XMLoadFloat3(&m_pVertices[*pStartVertexIndex].vNormal);
		vNormal = XMVector3TransformCoord(vNormal, PivotMatrix);
		XMStoreFloat3(&m_pVertices[*pStartVertexIndex].vNormal, vNormal);

		/* 정점의 텍스쳐 유브이좌표를 가지고 온다. */
		memcpy(&m_pVertices[*pStartVertexIndex].vTexUV, &pMesh->mTextureCoords[0][i], sizeof(_float2));

		/* 정점의 탄젠트 정보를 가지고 온다. */
		memcpy(&m_pVertices[*pStartVertexIndex].vTangent, &pMesh->mTangents[i], sizeof(_float3));

		memcpy(&m_pVertices[*pStartVertexIndex].vBiNormal, &pMesh->mBitangents[i], sizeof(_float3));

		++(*pStartVertexIndex);
	}

	_uint		iStartFaceIndex = *pStartFaceIndex;

	for (_uint i = 0; i < pMesh->mNumFaces; ++i)
	{
		m_pPolygonIndices[*pStartFaceIndex]._0 = pMesh->mFaces[i].mIndices[0];
		m_pPolygonIndices[*pStartFaceIndex]._1 = pMesh->mFaces[i].mIndices[1];
		m_pPolygonIndices[*pStartFaceIndex]._2 = pMesh->mFaces[i].mIndices[2];
		++(*pStartFaceIndex);
	}

	CInsMeshContainer*			pMeshContainer = CInsMeshContainer::Create(pMesh->mName.data, pMesh->mNumFaces, iStartFaceIndex, iStartVertexIndex, pMesh->mMaterialIndex);
	if (nullptr == pMeshContainer)
		return E_FAIL;

	m_MeshContainers.push_back(pMeshContainer);

	return S_OK;
}

HRESULT CInstanceModel::Create_VertexIndexBuffer(string pShaderFilePath, vector<_float4x4> _ptransforms)
{


	/* For.VertexBuffer */
	D3D11_BUFFER_DESC			BufferDesc;
	ZeroMemory(&BufferDesc, sizeof(D3D11_BUFFER_DESC));

	BufferDesc.ByteWidth = sizeof(VTXMESH) * m_iNumVertices;
	BufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
	BufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	BufferDesc.CPUAccessFlags = 0;
	BufferDesc.MiscFlags = 0;
	BufferDesc.StructureByteStride = sizeof(VTXMESH);

	D3D11_SUBRESOURCE_DATA		SubResourceData;
	ZeroMemory(&SubResourceData, sizeof(D3D11_SUBRESOURCE_DATA));

	SubResourceData.pSysMem = m_pVertices;

	if (FAILED(m_pDevice->CreateBuffer(&BufferDesc, &SubResourceData, &m_pVB)))
		return E_FAIL;


	/* For.IndexBuffer */
	ZeroMemory(&BufferDesc, sizeof(D3D11_BUFFER_DESC));

	BufferDesc.ByteWidth = sizeof(POLYGONINDICES32) * m_iNumFaces;
	BufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
	BufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	BufferDesc.CPUAccessFlags = 0;
	BufferDesc.MiscFlags = 0;
	BufferDesc.StructureByteStride = 0;

	ZeroMemory(&SubResourceData, sizeof(D3D11_SUBRESOURCE_DATA));
	SubResourceData.pSysMem = m_pPolygonIndices;

	if (FAILED(m_pDevice->CreateBuffer(&BufferDesc, &SubResourceData, &m_pIB)))
		return E_FAIL;

#pragma region VERTEXINSTANCEBUFFER

	ZeroMemory(&m_VBInstanceDesc, sizeof(D3D11_BUFFER_DESC));

	m_VBInstanceDesc.ByteWidth = m_iInstanceCount * sizeof(VTXMATRIX);
	m_VBInstanceDesc.Usage = D3D11_USAGE_DYNAMIC;
	m_VBInstanceDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	m_VBInstanceDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	m_VBInstanceDesc.MiscFlags = 0;
	m_VBInstanceDesc.StructureByteStride = sizeof(VTXMATRIX);

	VTXMATRIX*			pInstanceVertices = new VTXMATRIX[m_iInstanceCount];


	for (_uint i = 0; i < (_uint)m_iInstanceCount; ++i)
	{
		pInstanceVertices[i].vRight = _float4(_ptransforms[i]._11, _ptransforms[i]._12, _ptransforms[i]._13, 0.f);
		pInstanceVertices[i].vUp = _float4(_ptransforms[i]._21, _ptransforms[i]._22, _ptransforms[i]._23, 0.f);
		pInstanceVertices[i].vLook = _float4(_ptransforms[i]._31, _ptransforms[i]._32, _ptransforms[i]._33, 0.f);
		pInstanceVertices[i].vPosition = _float4(_ptransforms[i]._41, _ptransforms[i]._42, _ptransforms[i]._43, 1.f);
		m_InstanceMatrices.push_back(pInstanceVertices[i]);
	}
	m_VBInstanceSubResourceData.pSysMem = pInstanceVertices;

	if (FAILED(m_pDevice->CreateBuffer(&m_VBInstanceDesc, &m_VBInstanceSubResourceData, &m_pVBInstance)))
		return E_FAIL;

	SafeDeleteArray(pInstanceVertices);

#pragma endregion 


	D3D11_INPUT_ELEMENT_DESC		ElementDesc[] = {
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 20, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TANGENT", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 32, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "BINORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 44, D3D11_INPUT_PER_VERTEX_DATA, 0 },

		{ "INSTANCE", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 0, D3D11_INPUT_PER_INSTANCE_DATA, 1 },
		{ "INSTANCE", 1, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 16, D3D11_INPUT_PER_INSTANCE_DATA, 1 },
		{ "INSTANCE", 2, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 32, D3D11_INPUT_PER_INSTANCE_DATA, 1 },
		{ "INSTANCE", 3, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 48, D3D11_INPUT_PER_INSTANCE_DATA, 1 }

	};

	if (FAILED(Compile_Shader(ElementDesc, 9, m_pShaderFilePath)))
		return E_FAIL;

	return S_OK;
}

HRESULT CInstanceModel::Create_Materials(aiMaterial* pMaterial, const char* pMeshFilePath)
{
	MODELTEXTURES*		pModelTexture = new MODELTEXTURES;
	ZeroMemory(pModelTexture, sizeof(MODELTEXTURES));

	for (_uint i = 0; i < AI_TEXTURE_TYPE_MAX; ++i)
	{
		aiString		strTexturePath;

		if (FAILED(pMaterial->GetTexture(aiTextureType(i), 0, &strTexturePath)))
			continue;

		char	szTextureFileName[MAX_PATH] = "";
		char	szExt[MAX_PATH] = "";

		_splitpath(strTexturePath.data, nullptr, nullptr, szTextureFileName, szExt);
		strcat_s(szTextureFileName, szExt);

		char		szFullPath[MAX_PATH] = "";
		strcpy_s(szFullPath, pMeshFilePath);
		strcat_s(szFullPath, szTextureFileName);

		_tchar		szWideFullPath[MAX_PATH] = TEXT("");

		MultiByteToWideChar(CP_ACP, 0, szFullPath, (int)strlen(szFullPath), szWideFullPath, MAX_PATH);

		string TexturFilePath = WCHARToString(szWideFullPath);



		if (!strcmp(szExt, ".dds"))
			pModelTexture->pModelTexture[i] = CTexture::Create(m_pDevice, m_pDeviceContext, CTexture::TYPE_DDS, TexturFilePath);
		else if (!strcmp(szExt, ".tga"))
			pModelTexture->pModelTexture[i] = CTexture::Create(m_pDevice, m_pDeviceContext, CTexture::TYPE_TGA, TexturFilePath);
		else
			pModelTexture->pModelTexture[i] = CTexture::Create(m_pDevice, m_pDeviceContext, CTexture::TYPE_WIC, TexturFilePath);
	}

	m_ModelTextures.push_back(pModelTexture);

	return S_OK;
}

HRESULT CInstanceModel::Create_HierarchyNodes(aiNode* pNode, CInsHierarchyNode* pParent, _uint iDepth, _fmatrix PivotMatrix)
{
	_matrix		TransformationMatrix;
	/* 현재 이 노드가 대변하고있는 뼈의 mTransformation를 가지고 온다. */
	memcpy(&TransformationMatrix, &pNode->mTransformation, sizeof(_matrix));

	/* */
	CInsHierarchyNode*		pHierarchyNode = CInsHierarchyNode::Create(pNode->mName.data, TransformationMatrix * PivotMatrix, pParent, iDepth);
	if (nullptr == pHierarchyNode)
		return E_FAIL;

	m_HierarchyNodes.push_back(pHierarchyNode);

	for (_uint i = 0; i < pNode->mNumChildren; ++i)
		Create_HierarchyNodes(pNode->mChildren[i], pHierarchyNode, iDepth + 1);

	return S_OK;
}

HRESULT CInstanceModel::Sort_MeshesByMaterial()
{
	_uint		iNumMaterials = (_uint)m_ModelTextures.size();

	m_SortByMaterialMesh.resize(iNumMaterials);

	for (auto& pMeshContainer : m_MeshContainers)
	{
		_uint	iMeshMaterialIndex = pMeshContainer->Get_MeshMaterialIndex();
		if (iMeshMaterialIndex >= m_pScene->mNumMaterials)
			return E_FAIL;

		m_SortByMaterialMesh[iMeshMaterialIndex].push_back(pMeshContainer);
		SafeAddRef(pMeshContainer);
	}
	return S_OK;
}


HRESULT CInstanceModel::Compile_Shader(D3D11_INPUT_ELEMENT_DESC * pElementDescs, _uint iNumElement, string  pShaderFilePath, _uint iTechniqueIndex)
{
	_uint		iFlag = 0;

#ifdef _DEBUG
	iFlag = D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
#else
	iFlag = D3DCOMPILE_OPTIMIZATION_LEVEL1;
#endif

	ID3DBlob*		pCompileShader = nullptr;
	ID3DBlob*		pCompileShaderErrorMessage = nullptr;

	wstring ShaderPath;
	ShaderPath.assign(pShaderFilePath.begin(), pShaderFilePath.end());

	/* 외부에서 가져온 쉐이더 파일을 번역하여 바이너리화하였고 메모리영역안에 ㄷ마았다. */
	if (FAILED(D3DCompileFromFile(ShaderPath.c_str(), nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE, nullptr, "fx_5_0", iFlag, 0, &pCompileShader, &pCompileShaderErrorMessage)))
		return E_FAIL;

	if (FAILED(D3DX11CreateEffectFromMemory(pCompileShader->GetBufferPointer(), pCompileShader->GetBufferSize(), 0, m_pDevice, &m_pEffect)))
		return E_FAIL;

	ID3DX11EffectTechnique*		pTechnique = m_pEffect->GetTechniqueByIndex(iTechniqueIndex);
	if (nullptr == pTechnique)
		return E_FAIL;

	D3DX11_TECHNIQUE_DESC		TechniqueDesc;
	ZeroMemory(&TechniqueDesc, sizeof(D3DX11_TECHNIQUE_DESC));

	if (FAILED(pTechnique->GetDesc(&TechniqueDesc)))
		return E_FAIL;

	m_EffectDescs.reserve(TechniqueDesc.Passes);

	for (_uint i = 0; i < TechniqueDesc.Passes; ++i)
	{
		EFFECTDESC			EffectDesc;

		EffectDesc.pPass = pTechnique->GetPassByIndex(i);
		if (nullptr == EffectDesc.pPass)
			return E_FAIL;

		D3DX11_PASS_DESC			PassDesc;

		if (FAILED(EffectDesc.pPass->GetDesc(&PassDesc)))
			return E_FAIL;

		if (FAILED(m_pDevice->CreateInputLayout(pElementDescs, iNumElement, PassDesc.pIAInputSignature, PassDesc.IAInputSignatureSize, &EffectDesc.pLayout)))
			return E_FAIL;

		m_EffectDescs.push_back(EffectDesc);
	}

	return S_OK;
}


void CInstanceModel::Add_ChannelToHierarchyNode(_uint iAnimationindex, CInsChannel * pChannel)
{
	auto	iter = find_if(m_HierarchyNodes.begin(), m_HierarchyNodes.end(), [&](CInsHierarchyNode* pNode)->bool
	{
		return !strcmp(pNode->Get_Name(), pChannel->Get_Name());
	});

	/* 여기서 추가되는 채널은 노드(뼈)의 정보와 같은 뼈정보플 표ㅗ현하는 채널이ㅏㄷ. */
	(*iter)->Add_Channel(iAnimationindex, pChannel);

}

CInsHierarchyNode * CInstanceModel::Find_HierarchyNode(const char * pBoneName)
{
	auto	iter = find_if(m_HierarchyNodes.begin(), m_HierarchyNodes.end(), [&](CInsHierarchyNode* pNode)
	{
		if (0 == strcmp(pNode->Get_Name(), pBoneName))
			return true;

		return false;
	});

	return *iter;
}

CInstanceModel * CInstanceModel::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
{
	CInstanceModel*	pInstance = new CInstanceModel(pDevice, pDeviceContext);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSG_BOX("Failed To Creating CInstanceModel");
		SafeRelease(pInstance);
	}
	return pInstance;
}

CComponent * CInstanceModel::Clone(void * pArg)
{
	CInstanceModel*	pInstance = new CInstanceModel(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSG_BOX("Failed To Creating CInstanceModel");
		SafeRelease(pInstance);
	}
	return pInstance;
}

void CInstanceModel::Free()
{
	__super::Free();

	//SafeDeleteArray(m_pVertices);
	//SafeDeleteArray(m_pPolygonIndices);

	//for (auto& pModelTextures : m_ModelTextures)
	//{
	//	for (auto& pTexture : pModelTextures->pModelTexture)
	//		SafeRelease(pTexture);

	//	SafeDelete(pModelTextures);
	//}
	//m_ModelTextures.clear();

	//m_Importer.FreeScene();


	if (false == m_isCloned)
	{
		SafeDeleteArray(m_pVertices);
		SafeDeleteArray(m_pPolygonIndices);

		for (auto& pModelTextures : m_ModelTextures)
		{
			for (auto& pTexture : pModelTextures->pModelTexture)
				SafeRelease(pTexture);

			SafeDelete(pModelTextures);
		}
		m_ModelTextures.clear();

		m_Importer.FreeScene();
	}


	for (auto& pModelTextures : m_ModelTextures)
	{
		for (auto& pTexture : pModelTextures->pModelTexture)
			SafeRelease(pTexture);
	}
	m_ModelTextures.clear();

	for (auto& pMeshContainers : m_SortByMaterialMesh)
	{
		for (auto& pMeshContainer : pMeshContainers)
			SafeRelease(pMeshContainer);
		pMeshContainers.clear();
	}
	m_SortByMaterialMesh.clear();

	for (auto& pMeshContainer : m_MeshContainers)
		SafeRelease(pMeshContainer);
	m_MeshContainers.clear();

	for (auto& pHierarchyNode : m_HierarchyNodes)
		SafeRelease(pHierarchyNode);
	m_HierarchyNodes.clear();


	SafeRelease(m_pVB);
	SafeRelease(m_pIB);

	for (auto& EffectDesc : m_EffectDescs)
	{
		SafeRelease(EffectDesc.pLayout);
		SafeRelease(EffectDesc.pPass);
	}
	m_EffectDescs.clear();


	SafeRelease(m_pEffect);


	SafeRelease(m_pVBInstance);
}
