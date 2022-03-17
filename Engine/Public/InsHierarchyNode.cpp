#include "EnginePCH.h"
#include "InsHierarchyNode.h"
#include "InsChannel.h"

CInsHierarchyNode::CInsHierarchyNode()
{
}

HRESULT CInsHierarchyNode::NativeConstruct(const char * pNodeName, _fmatrix TransformationMatrix, CInsHierarchyNode * pParent, _uint iDepth)
{
	strcpy_s(m_szNodeName, pNodeName);
	XMStoreFloat4x4(&m_TransformationMatrix, XMMatrixTranspose(TransformationMatrix));
	m_pParent = pParent;
	m_iDepth = iDepth;

	return S_OK;
}

HRESULT CInsHierarchyNode::Add_Channel(_uint iAnimationIndex, CInsChannel * pChannel)
{
	m_Channels[iAnimationIndex] = pChannel;

	SafeAddRef(pChannel);

	return S_OK;
}

void CInsHierarchyNode::Update_CombinedTransformationMatrix(_uint iAnimationIndex)
{
	_matrix		TransformationMatrix;

	if (nullptr == m_Channels[iAnimationIndex])
		TransformationMatrix = XMLoadFloat4x4(&m_TransformationMatrix);

	else
		TransformationMatrix = m_Channels[iAnimationIndex]->Get_TransformationMatrix();

	if (nullptr != m_pParent)
		/* 나의 상태행렬과 부모의 컴바인드 행렬을 곱해서 나의 컴바인드를 셋팅한다. */
		XMStoreFloat4x4(&m_CombinedTransformationMatrix, TransformationMatrix * XMLoadFloat4x4(&m_pParent->m_CombinedTransformationMatrix));
	else
		XMStoreFloat4x4(&m_CombinedTransformationMatrix, TransformationMatrix);
}

void CInsHierarchyNode::Reserve_Channels(_uint iNumAnimation)
{
	m_Channels.resize(iNumAnimation);
}

CInsHierarchyNode * CInsHierarchyNode::Create(const char * pNodeName, _fmatrix TransformationMatrix, CInsHierarchyNode * pParent, _uint iDepth)
{
	CInsHierarchyNode*		pInstance = new CInsHierarchyNode();

	if (FAILED(pInstance->NativeConstruct(pNodeName, TransformationMatrix, pParent, iDepth)))
	{
		MSG_BOX("Failed to Creating CHierarchyNode");
		SafeRelease(pInstance);
	}

	return pInstance;
}

CInsHierarchyNode * CInsHierarchyNode::Clone()
{
	CInsHierarchyNode*	pInstance = new CInsHierarchyNode(*this);

	return nullptr;
}

void CInsHierarchyNode::Free()
{
	for (auto& pChannel : m_Channels)
		SafeRelease(pChannel);

	m_Channels.clear();
}
