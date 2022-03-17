#pragma once


#include "Base.h"

BEGIN(Engine)

/* Ư���ִϸ��̼ǿ� �ҼӵǾ��ִ� ��������. . */
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
	/* ���� ���� �������� Ű�����ӵ��۶����� ǥ���ؾ��� ���������� �����صд�. */
	/* �ϳ��� �ִϸ��̼� ���ۿ� ���Ǵ�. */
	vector<KEYFRAMEDESC*>		m_KeyFrames;
	_float4x4					m_TransformationMatrix;
	_uint						m_iCurrentKeyFrame = 0;

public:
	static CInsChannel* Create(const char* pChannelName);
	virtual void Free() override;
};

END