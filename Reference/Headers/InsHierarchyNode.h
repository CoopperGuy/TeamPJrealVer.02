#pragma once

#include "Base.h"

/* aiNode�� ������������ �ִ� Ŭ�����̴�. */
/* aiNode���������� �ϳ��� �������ǹ�. */
/* aiScene�� �ִ� RootNode�ֻ��� �θ�� �ϴ�Ŭ�����̴�. */

BEGIN(Engine)

class CInsHierarchyNode final : public CBase
{
public:
	CInsHierarchyNode();
	virtual ~CInsHierarchyNode() = default;
public:
	_uint Get_Depth() const {
		return m_iDepth;
	}

	const char* Get_Name() const {
		return m_szNodeName;
	}

	_matrix Get_CombinedTransformationMatrix() {
		return XMLoadFloat4x4(&m_CombinedTransformationMatrix);
	}

	_matrix Get_OffsetMatrix() {
		return XMLoadFloat4x4(&m_OffSetMatrix);
	}

	void Set_OffSetMatrix(_fmatrix OffSetMatrix) {
		XMStoreFloat4x4(&m_OffSetMatrix, OffSetMatrix);
	}

public:
	HRESULT NativeConstruct(const char* pNodeName, _fmatrix TransformationMatrix, CInsHierarchyNode* pParent, _uint iDepth);
	HRESULT Add_Channel(_uint iAnimationIndex, class CInsChannel* pChannel);
	void Update_CombinedTransformationMatrix(_uint iAnimationIndex);
	void Reserve_Channels(_uint iNumAnimation);

private:
	char				m_szNodeName[MAX_PATH] = "";
	_float4x4			m_OffSetMatrix;
	_float4x4			m_TransformationMatrix;
	_float4x4			m_CombinedTransformationMatrix; /* �θ������ ��������� �������Ѹ������. */
	CInsHierarchyNode*	m_pParent = nullptr;
	_uint				m_iDepth = 0;

private:
	/* ���� ��尡 ����Ű�� �ִ� ���� �������� �ִϸ��̼ǿ��� ä�ημ� ����� �ȴ�. */
	/* ������ �ִϸ��̼ǿ��� ���ǰ� �ִ� ä�ε��� �ִϸ��̼� �迭�� �����Ѵ�. */
	/* CInsChannel : �ִϸ��̼��� ���������� �����ϴ� ���� ������. */
	/* ä���� �����̸������� ä���� �ټ� �����Ҽ����ִ�. */
	/* ä���� �̸��� �����̸�. �ϳ��� ���� ���� �ִԿ��Ӥ��࿡�� ��뤧�� �� �ֱ⤨�Ŵ��� .*/
	vector<class CInsChannel*>			m_Channels;


public:
	static CInsHierarchyNode* Create(const char* pNodeName, _fmatrix TransformationMatrix, CInsHierarchyNode* pParent, _uint iDepth);
	CInsHierarchyNode* Clone();
	virtual void Free() override;

};

END