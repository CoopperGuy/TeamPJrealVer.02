#pragma once

#include "VIBuffer.h"

BEGIN(Engine)

class ENGINE_DLL CInstanceModel final : public CComponent
{
private:
	explicit CInstanceModel(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	explicit CInstanceModel(const CInstanceModel& rhs);
	virtual ~CInstanceModel() = default;
public:
	_uint Get_NumMaterials() const {
		return (_uint)m_ModelTextures.size();
	}

public:
	virtual HRESULT NativeConstruct_Prototype();
	virtual HRESULT NativeConstruct(void* pArg);
public:
	HRESULT Bind_Buffers(_uint iPassIndex, void* pArg);
	HRESULT Render(_uint iMaterialIndex, _uint iPassIndex);


	HRESULT SetUp_ValueOnShader(const char* pConstantName, void* pData, _uint iByteSize);
	HRESULT SetUp_TextureOnShader(const char* pConstantName, _uint iMaterialIndex, aiTextureType eTextureType);
	HRESULT SetUp_TextureOnShader(const char* pConstantName, ID3D11ShaderResourceView* pSRV);
	HRESULT SetUp_TextureOnShader(const char* pConstantName, class CTexture* pTextureComponent, _uint iTextureIndex = 0);
private:
	const std::string WCHARToString(const wchar_t* ptsz);

public:
	string GetMeshFilePath() { return m_pMeshFilePath; }
	string GetMeshFileName() { return m_pMeshFileName; }
	string GetShaderFilePath() { return m_pShaderFilePath; }
	string GetEffectFilePath() { return m_pEffectFilePath; }
	void	SetDissolve(_float value) { m_fDissolve = value; }
	void	SetEffectFilePath(string FilePath) { m_pEffectFilePath = FilePath; }
	HRESULT	SetEffectMaterial();


protected:
	string			m_pMeshFilePath;
	string			m_pMeshFileName;
	string			m_pShaderFilePath = "";
	string			m_pEffectFilePath = "";
	_float			m_fDissolve = 0.f;


private:
	/* 로드한 모델의 모든 메시들을 포함한다. */
	/* 모든 머테리얼 정보를 포함한다. */
	const aiScene*		m_pScene = nullptr;
	Assimp::Importer	m_Importer;

private:
	_uint				m_iNumVertices = 0;
	_uint						m_iNumVertexBuffers = 0;
	_uint				m_iNumFaces = 0;
	VTXMESH*			m_pVertices = nullptr;
	POLYGONINDICES32*	m_pPolygonIndices = nullptr;
	INSMODELDESC		m_pInstanceDesc;

	_int	m_iInstanceCount = 0;
	ID3D11Buffer*			m_pVBInstance = nullptr;
	D3D11_BUFFER_DESC		m_VBInstanceDesc;
	D3D11_SUBRESOURCE_DATA	m_VBInstanceSubResourceData;
	vector<VTXMATRIX>		m_InstanceMatrices;
private:
	vector<class CInsMeshContainer*>			m_MeshContainers;

	/* 재질기준으로 같은 재질을 가진 메시컨테이너끼리 묶어둔다. */
	vector<vector<class CInsMeshContainer*>>	m_SortByMaterialMesh;
	vector<MODELTEXTURES*>					m_ModelTextures;
	vector<class CInsHierarchyNode*>			m_HierarchyNodes;

private:
	ID3D11Buffer*				m_pVB = nullptr;
	ID3D11Buffer*				m_pIB = nullptr;
	_uint						m_iStride = 0;

protected:
	vector<EFFECTDESC>			m_EffectDescs;

	ID3DX11Effect*				m_pEffect = nullptr;
	class CTransform*				m_pTransform = nullptr;
protected:
	_matrix						m_PivotMatrix;
	_vector						Pos;

private:
	HRESULT Create_MeshContainer(aiMesh* pMesh, _uint* pStartVertexIndex, _uint* pStartFaceIndex, _fmatrix PivotMatrix);
	HRESULT Create_VertexIndexBuffer(string pShaderFilePath, vector<_float4x4> _ptransforms);
	HRESULT Create_Materials(aiMaterial*	pMaterial, const char* pMeshFilePath);
	HRESULT Create_HierarchyNodes(aiNode* pNode, class CInsHierarchyNode* pParent = nullptr, _uint iDepth = 0, _fmatrix PivotMatrix = XMMatrixIdentity());
	HRESULT Sort_MeshesByMaterial();
	HRESULT Compile_Shader(D3D11_INPUT_ELEMENT_DESC* pElementDescs, _uint iNumElement, string pShaderFilePath, _uint iTechniqueIndex = 0);

	void Add_ChannelToHierarchyNode(_uint iAnimationindex, class CInsChannel* pChannel);

	CInsHierarchyNode* Find_HierarchyNode(const char* pBoneName);



public:
	static CInstanceModel* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	virtual CComponent* Clone(void* pArg) override;
	virtual void Free() override;
};

END