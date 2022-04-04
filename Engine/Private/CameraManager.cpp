#include "EnginePCH.h"
#include "..\Public\CameraManager.h"
#include "Camera.h"

USING(Engine)

IMPLEMENT_SINGLETON(CameraManager)

CameraManager::CameraManager()
{
}

_int CameraManager::Update(_double dDeltaTime)
{
	_int idx = 0;
	if (m_iCurActiveCamera >= m_vecCamera.size()) {
		m_iCurActiveCamera = 0;
	}
	for (auto& iter : m_vecCamera) {
		if (idx == m_iCurActiveCamera)
			iter->OnCamera();
		else
			iter->OffCamera();
		idx++;
	}
	return _int();
}

_int CameraManager::LateUpdate(_double dDeltaTime)
{
	return _int();
}

void CameraManager::SeceneChangeClear()
{
	
}

void CameraManager::Clear()
{
}

void CameraManager::Free()
{
	for (auto& iter : m_vecCamera) {
		SafeRelease(iter);
	}
	m_vecCamera.clear();
}
