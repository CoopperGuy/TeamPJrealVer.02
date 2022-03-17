#include "EnginePCH.h"
#include "..\public\InsChannel.h"


CInsChannel::CInsChannel()
{
}

HRESULT CInsChannel::NativeConstruct(const char * pChannelName)
{
	strcpy_s(m_szChannelName, pChannelName);

	return S_OK;
}

HRESULT CInsChannel::Add_KeyFrameDesc(KEYFRAMEDESC * pKeyFrameDesc)
{
	if (nullptr == pKeyFrameDesc)
		return E_FAIL;

	m_KeyFrames.push_back(pKeyFrameDesc);

	return S_OK;
}

CInsChannel * CInsChannel::Create(const char * pChannelName)
{
	CInsChannel*		pInstance = new CInsChannel();

	if (FAILED(pInstance->NativeConstruct(pChannelName)))
	{
		MSG_BOX("Failed to Creating CInsChannel");
		SafeRelease(pInstance);
	}

	return pInstance;
}

void CInsChannel::Free()
{
	for (auto& pKeyFrame : m_KeyFrames)
		SafeDelete(pKeyFrame);

	m_KeyFrames.clear();
}


