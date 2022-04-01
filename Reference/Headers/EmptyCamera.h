#pragma once
#include "Camera.h"

BEGIN(Engine)

class CEmptyCamera :
	public CCamera
{
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
	PROPERTY(GetMoveTime, SetMoveTime) _float p_moveTime;
	PROPERTY(GetSrcPosition, SetSrcPosition) _float3 p_srcPosition;
	PROPERTY(GetSrcLookPosition, SetSrcLookPosition) _float3 p_srcLookPosition;
	PROPERTY(GetDestPosition, SetDestPosition) _float3 p_destPosition;
	PROPERTY(GetDestLookPosition, SetDestLookPosition) _float3 p_destLookPosition;


public:
	static CEmptyCamera* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	virtual CCamera* Clone(void* pArg) override;
	virtual void Free();
public:
	void SetIsMove(_bool _move) { m_bIsMove = _move; }
	void SetMoveTime(_float _time) { m_fMoveTime = _time; }
	void SetSrcPosition(_float3 _pos) { m_vSrcPosition = _pos; }
	void SetSrcLookPosition(_float3 _pos) { m_vSrcLookPosition = _pos; }
	void SetDestPosition(_float3 _pos) { m_vDestPosition = _pos; }
	void SetDestLookPosition(_float3 _pos) { m_vDestLookPosition = _pos; }
public:
	_bool GetIsMove() { return m_bIsMove; }
	_float GetMoveTime() { return m_fMoveTime; }
	_float3 GetSrcPosition() { return m_vSrcPosition; }
	_float3 GetSrcLookPosition() { return m_vSrcLookPosition; }
	_float3 GetDestPosition() { return m_vDestPosition; }
	_float3 GetDestLookPosition() { return m_vDestLookPosition; }
private:
	_bool	m_bIsMove = false;
private:
	_float	m_fMoveTime = 0.f;
	_float	m_fMoveDelta = 0.f;
private:
	_float3	m_vSrcPosition{};
	_float3	m_vDestPosition{};
	_float3	m_vSrcLookPosition{};
	_float3	m_vDestLookPosition{};
};

END