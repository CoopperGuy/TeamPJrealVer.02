#pragma once
#include "Base.h"
#include "Camera.h"
BEGIN(Engine)
class CCamera;
class CameraManager :
	public CBase
{
	DECLARE_SINGLETON(CameraManager)
private:
	CameraManager();
	virtual ~CameraManager() = default;
public:
	_int	Update(_double dDeltaTime);
	_int	LateUpdate(_double dDeltaTime);
public:
	void	ActiveCameraByIndex(_int _idx) { m_iCurActiveCamera = _idx; }
	HRESULT	AddCamera(CCamera* _camera) { SafeAddRef(_camera);  m_vecCamera.emplace_back(_camera); return S_OK; }
	HRESULT	SetDefaultCamera() { m_iCurActiveCamera = 0; return S_OK;}
public:
	_int	GetCurActiveCamera() { return m_iCurActiveCamera; }
private:
	vector<CCamera*>	m_vecCamera;
private:
	_int	m_iCurActiveCamera = 0;
public:
	void	SeceneChangeClear();
	void	Clear();
	virtual void Free() override;
};

END