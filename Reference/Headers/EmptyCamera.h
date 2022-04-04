#pragma once
#include "Camera.h"

BEGIN(Engine)

class ENGINE_DLL CEmptyCamera :
	public CCamera
{
public:
	enum MOVIE
	{
		MOVIE_Y,
		MOVIE_NOY,
		MOVIE_NONE
	};
private:
	explicit CEmptyCamera(ID3D11Device* pDevice, ID3D11DeviceContext* pDevice_Context);
	explicit CEmptyCamera(const CCamera& rhs);
	virtual ~CEmptyCamera() = default;

public:
	virtual HRESULT InitializePrototype();
	virtual HRESULT Initialize(void* pArg);
	virtual _uint Update(_double TimeDelta);
	virtual _uint LateUpdate(_double TimeDelta);

public:
	PROPERTY(GetIsMove, SetIsMove) _bool p_isMove;
	PROPERTY(GetIsWait, SetIsWait) _bool p_isWait;
	PROPERTY(GetIsBack, SetIsBack) _bool p_isBack;
	PROPERTY(GetMovie, SetMovie) MOVIE p_Moive;
	PROPERTY(GetMoveTime, SetMoveTime) _float p_moveTime;
	PROPERTY(GetWaitTime, SetWaitTime) _float p_waitTime;
	PROPERTY(GetSrcPosition, SetSrcPosition) _float3 p_srcPosition;
	PROPERTY(GetSrcLookPosition, SetSrcLookPosition) _float3 p_srcLookPosition;
	PROPERTY(GetDestPosition, SetDestPosition) _float3 p_destPosition;
	PROPERTY(GetDestLookPosition, SetDestLookPosition) _float3 p_destLookPosition;
public:
	static CEmptyCamera* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	virtual CCamera* Clone(void* pArg) override;
	virtual void Free();
public:
	_bool GetIsMove() { return m_bIsMove; }
	_bool GetIsWait() { return m_bIsWait; }
	_bool GetIsBack() { return m_bIsBack; }
	MOVIE GetMovie() { return m_eMovie; }
	_float GetMoveTime() { return m_fMoveTime; }
	_float GetWaitTime() { return m_fWaitTime; }
	_float3 GetSrcPosition() { return m_vSrcPosition; }
	_float3 GetSrcLookPosition() { return m_vSrcLookPosition; }
	_float3 GetDestPosition() { return m_vDestPosition; }
	_float3 GetDestLookPosition() { return m_vDestLookPosition; }
public:
	void SetIsMove(_bool _move) { m_bIsMove = _move; }
	void SetIsWait(_bool _wait) { m_bIsWait = _wait; }
	void SetIsBack(_bool _back) { m_bIsBack = _back; }
	void SetMovie(MOVIE _movie) { m_eMovie = _movie; }
	void SetWaitTime(_float _time) { m_fWaitTime = _time; }
	void SetMoveTime(_float _time) { m_fMoveTime = _time; }
	void SetSrcPosition(_float3 _pos) { m_vSrcPosition = _pos; }
	void SetSrcLookPosition(_float3 _pos) { m_vSrcLookPosition = _pos; }
	void SetDestPosition(_float3 _pos) { m_vDestPosition = _pos; }
	void SetDestLookPosition(_float3 _pos) { m_vDestLookPosition = _pos; }

private:
	_bool	m_bIsMove = false;
	_bool	m_bIsWait = false;
	_bool	m_bIsBack = false;
	MOVIE	m_eMovie = MOVIE_Y;
private:
	_float	m_fMoveTime = 0.f;
	_float	m_fMoveDelta = 0.f;
	_float	m_fWaitTime = 0.f;
	_float	m_fWaitDelta = 0.f;
private:
	_float3	m_vSrcPosition = { 0.f,0.f ,0.f };
	_float3	m_vDestPosition = { 0.f,0.f ,0.f };
	_float3	m_vSrcLookPosition = { 0.f,0.f ,0.f };
	_float3	m_vDestLookPosition = { 0.f,0.f ,0.f };
};

END