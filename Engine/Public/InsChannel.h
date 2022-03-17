#pragma once


#include "Base.h"

BEGIN(Engine)

/* 특정애니메이션에 소속되어있는 뼈의정보. . */
class CInsChannel final : public CBase
{
private:
	CInsChannel();
	virtual ~CInsChannel() = default;
public:
	vector<KEYFRAMEDESC*> Get_KeyFrames() {
		return m_KeyFrames;
	}

	const char* Get_Name() const {
		return m_szChannelName;
	}

	_matrix Get_TransformationMatrix() {
		return XMLoadFloat4x4(&m_TransformationMatrix);
	}

	_uint Get_CurrentKeyFrame() const {
		return m_iCurrentKeyFrame;
	}

	void Set_TransformationMatrix(_fmatrix TransformationMatrix) {
		XMStoreFloat4x4(&m_TransformationMatrix, TransformationMatrix);
	}

	void Set_CurrentKeyFrame(_uint iCurrentKeyFrame) {
		m_iCurrentKeyFrame = iCurrentKeyFrame;
	}

public:
	HRESULT NativeConstruct(const char* pChannelName);
	HRESULT Add_KeyFrameDesc(KEYFRAMEDESC* pKeyFrameDesc);
private:
	char					m_szChannelName[MAX_PATH] = "";
	/* 현재 뼈가 ㄱ락각의 키프레임동작때마다 표현해야할 상태정보를 보관해둔다. */
	/* 하나의 애니메이션 동작에 사용되는. */
	vector<KEYFRAMEDESC*>		m_KeyFrames;
	_float4x4					m_TransformationMatrix;
	_uint						m_iCurrentKeyFrame = 0;

public:
	static CInsChannel* Create(const char* pChannelName);
	virtual void Free() override;
};

END