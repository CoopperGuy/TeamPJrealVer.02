#pragma once

#include "Base.h"

/* aiNode의 정보를가지고 있는 클래스이다. */
/* aiNode계층구조의 하나의 데이터의미. */
/* aiScene에 있는 RootNode최상위 부모로 하는클래스이다. */

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
	_float4x4			m_CombinedTransformationMatrix; /* 부모뼈들의 상태행렬을 누적시켜만든행렬. */
	CInsHierarchyNode*	m_pParent = nullptr;
	_uint				m_iDepth = 0;

private:
	/* 현재 노드가 가리키고 있는 뼈는 여러개의 애니메이션에서 채널로서 사용이 된다. */
	/* 각각의 애니메이션에서 사용되고 있는 채널들을 애니메이션 배열로 저장한다. */
	/* CInsChannel : 애니메이션을 돌리기위해 접근하는 뼈의 데이터. */
	/* 채널은 같은이름을가진 채널이 다수 존재할수도있다. */
	/* 채널의 이름은 뼈의이름. 하나의 뼈는 여러 애님에ㅣㅅ녀에서 사용ㄷ될 수 있기ㄸ매누에 .*/
	vector<class CInsChannel*>			m_Channels;


public:
	static CInsHierarchyNode* Create(const char* pNodeName, _fmatrix TransformationMatrix, CInsHierarchyNode* pParent, _uint iDepth);
	CInsHierarchyNode* Clone();
	virtual void Free() override;

};

END