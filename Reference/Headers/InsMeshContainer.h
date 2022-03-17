#pragma once

#include "Base.h"

/* ���ϳ��� �����ϴ� ������ �޽��������� ���� ����ϱ� ������ �����Ѵ�. */

BEGIN(Engine)

class CInsMeshContainer final : public CBase
{
public:
	CInsMeshContainer();
	CInsMeshContainer(const CInsMeshContainer& rhs);
	virtual ~CInsMeshContainer() = default;
public:
	_uint Get_MeshMaterialIndex() const {
		return m_iMaterialIndex;
	}

	_uint Get_NumFaces() const {
		return m_iNumFaces;
	}

	_uint Get_StartFaceIndex() const {
		return m_iStartFaceIndex;
	}

	_uint Get_StartVertexIndex() const {
		return m_iStartVertexIndex;
	}

	vector<BONEDESC*> Get_BoneDesc() {
		return m_Bones;
	}

	void Get_BoneMatrices(_matrix* pBoneMatrices);

public:
	virtual HRESULT NativeConstruct(const char* pName, _uint iNumFaces, _uint iStartFaceIndex, _uint iStartVertexIndex, _uint iMaterialIndex);
public:
	HRESULT Add_Bones(BONEDESC* pBoneDesc);
	//HRESULT Clone_BoneDesc();

private:
	char		m_szName[MAX_PATH] = "";
	_uint		m_iNumFaces = 0;
	_uint		m_iStartFaceIndex = 0;
	_uint		m_iStartVertexIndex = 0;
	_uint		m_iMaterialIndex = 0;

	/* ���� �޽������̳�(������)�� � �����κ��� ����� ����޾ƾ��ϴ���. */
	vector<BONEDESC*>		m_Bones;

public:
	static CInsMeshContainer* Create(const char* pName, _uint iNumFaces, _uint iStartFaceIndex, _uint iStartVertexIndex, _uint iMaterialIndex);
	CInsMeshContainer* Clone();
	virtual void Free() override;
};

END