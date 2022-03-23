#pragma once

#include "Client_Defines.h"
#include "Camera.h"


BEGIN(Client)
class CCamera_Fly final : public CCamera
{
public:
	enum SHAKE { SHAKE_RIGHT, SHAKE_LEFT, SHAKE_ING, SHAKE_END };
public:
	explicit CCamera_Fly(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	explicit CCamera_Fly(const CCamera_Fly& rhs);
	virtual ~CCamera_Fly() = default;
public:
	virtual HRESULT InitializePrototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual _uint Update(_double TimeDelta) override;
	virtual _uint LateUpdate(_double TimeDelta) override;
public:
	void	ShakeCamera(SHAKE _shake, _int _cnt,_float _duration, _float _spd);
	void	ShakeLookCamera(SHAKE _shake, _fvector _look, _float _duration);
	void	ShakeLook(_fvector _look, _float _duration);
	void	ShakePosition(_fvector _pos, _float _duration);
	void	ShakeUpDown(_int _cnt, _float _duration, _float _spd);
	void	ZoomFov(_float _duration, _float _destFov, _float _spd);

private:
	void	PhysxCameraCollision(_double deltaTime);
private:
	HRESULT SetUp_Components();
	void Set_Pos(_double DeltaTime);
	void ShakeX(_double DeltaTime);
	void ShakeY(_double DeltaTime);
	void ShakeFov(_double DeltaTime);
private:
	CTransform* m_pTargetTransform = nullptr;
	CCollider*	m_pTargetCollider = nullptr;
private:
	_vector		m_pTargetLook = { 0.f, 0.f, 1.f};
	_float3		m_vLook{};
	_float3		m_vShakeLook{};
	_float3		m_vShakePos = { 0.f, 0.f, 0.f };
	_float3		m_vShakeYPos{};
	_float		m_vDistance = 5.f;

private:
	_float	m_fCameraAngleLerpSpd = 10.f;
	_float	m_fCameraWheelSpd = 0.03f;
	_float	m_fCameraMoveLerpSpd = 0.3f;
	_float	m_fCameraColliedSpd = 10.f;
private:
	_float	m_fCameraCollisionDelta = 0.f;
	_float	m_fCameraCollisionTime = 0.1f;
	_float	m_fDuration = 0.f;
	_float	m_fDurationDelta = 0.f;
	_float	m_fDirection = 0.f;
private:
	_bool	m_bShake = false;
	SHAKE	m_eShaking = SHAKE::SHAKE_END;
	_int	m_iCnt = 0;
	_float	m_fSpd = 0.f;
private:
	_bool	m_bYShake = false;
	_int	m_iYCnt = 0;
	_float	m_fYDirection = 0.f;
	_float	m_fYSpd = 0.f;
	_float	m_fYDuration = 0.f;
	_float	m_fYDurationDelta = 0.f;
private:
	_float	m_FovShakeDuration = 0.f;
	_float	m_FovShakeDurationDelta = 0.f;
	_float	m_fDestFov = 0.f;
	_float	m_fFovSpd = 5.f;
	_bool	m_bFovShake = false;
public:
	static CCamera_Fly* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END