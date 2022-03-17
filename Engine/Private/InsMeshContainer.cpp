#include "EnginePCH.h"
#include "..\public\InsMeshContainer.h"
#include "HierarchyNode.h"


CInsMeshContainer::CInsMeshContainer()
{

}

CInsMeshContainer::CInsMeshContainer(const CInsMeshContainer & rhs)
	: m_iNumFaces(rhs.m_iNumFaces)
	, m_iStartFaceIndex(rhs.m_iStartFaceIndex)
	, m_iStartVertexIndex(rhs.m_iStartVertexIndex)
	, m_iMaterialIndex(rhs.m_iMaterialIndex)
{
	strcpy(m_szName, rhs.m_szName);
	m_Bones.clear();
}

void CInsMeshContainer::Get_BoneMatrices(_matrix * pBoneMatrices)
{
	_uint		iNumBones = _uint(m_Bones.size());

	for (_uint i = 0; i < iNumBones; ++i)
	{
		pBoneMatrices[i] = XMMatrixTranspose(XMLoadFloat4x4(&m_Bones[i]->OffsetMatrix) * m_Bones[i]->pHierarchyNode->Get_CombinedTransformationMatrix());
	}
}

HRESULT CInsMeshContainer::NativeConstruct(const char* pName, _uint iNumFaces, _uint iStartFaceIndex, _uint iStartVertexIndex, _uint iMaterialIndex)
{
	strcpy(m_szName, pName);
	m_iNumFaces = iNumFaces;
	m_iStartFaceIndex = iStartFaceIndex;
	m_iStartVertexIndex = iStartVertexIndex;
	m_iMaterialIndex = iMaterialIndex;

	return S_OK;
}

HRESULT CInsMeshContainer::Add_Bones(BONEDESC * pBoneDesc)
{
	m_Bones.push_back(pBoneDesc);

	return S_OK;
}


CInsMeshContainer * CInsMeshContainer::Create(const char* pName, _uint iNumFaces, _uint iStartFaceIndex, _uint iStartVertexIndex, _uint iMaterialIndex)
{
	CInsMeshContainer*	pInstance = new CInsMeshContainer();

	if (FAILED(pInstance->NativeConstruct(pName, iNumFaces, iStartFaceIndex, iStartVertexIndex, iMaterialIndex)))
	{
		MSG_BOX("Failed To Creating CInsMeshContainer");
		SafeRelease(pInstance);
	}
	return pInstance;
}

CInsMeshContainer * CInsMeshContainer::Clone()
{
	CInsMeshContainer*	pInstance = new CInsMeshContainer(*this);

	/*if (FAILED(Clone_BoneDesc()))
	{
	Safe_Release(pInstance);
	}
	*/

	return pInstance;
}

void CInsMeshContainer::Free()
{
	for (auto& pBoneDesc : m_Bones)
		SafeDelete(pBoneDesc);
	m_Bones.clear();

}
