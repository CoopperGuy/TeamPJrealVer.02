#pragma once

#include "Component.h"

BEGIN(Engine)

class ENGINE_DLL CLight final : public CComponent
{
private:
	explicit CLight(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	explicit CLight(const CLight& rhs);
	virtual ~CLight() = default;
public:
	const LIGHTDESC* GetLightDesc() const {
		return &m_LightDesc;
	}

	LIGHTDESC& GetDesc() { return m_LightDesc; }

public:
	HRESULT Initialize(const LIGHTDESC& LightDesc, class CTransform* pTransform, _bool _tempLight = false);
	HRESULT Render_Light();
	HRESULT Render_DebugBuffer();
	_uint	Update(_double deltaTime);
public:
	void SetIsTempLight(_bool temp) { m_bTempLight = temp; }
	void SetLifeTime(_double _time) { m_LifeTime = _time; }
	void AddDeltaTime(_double deltaTime) { m_DeltaTime += deltaTime; }
public:
	_matrix GetViewMatrix();
	_matrix GetProjMatrix();
	_vector GetPosition();
	_vector GetDirection();
	_float	GetAngle();
	ID3D11ShaderResourceView* GetShaderResourceView();
	string GetTargetName() { return m_targetName; };

	_bool GetIsTempLight() { return m_bTempLight; }
	_double GetDeltaTime() { return m_DeltaTime; }
	_double GetLifeTime() { return m_LifeTime; }
	_bool GetIsTimeOver() { return m_DeltaTime >= m_LifeTime; }
	class CVIBuffer_Rect_Viewport* GetVIBuffer() { return m_pVIBuffer; }
public:
	void	SetRange(_float range) { m_LightDesc.fLightRange = range; }


private:
	//ID3D11Device*					m_pDevice = nullptr;
	//ID3D11DeviceContext*			m_pDeviceContext = nullptr;
	LIGHTDESC						m_LightDesc;
	class CVIBuffer_Rect_Viewport*	m_pVIBuffer = nullptr;
	class CRenderTarget*			m_pRenderTarget = nullptr;
	class CTransform*				m_pTransform = nullptr;
	string							m_targetName = "";
private:
	_bool							m_bTempLight = false;
	_double							m_DeltaTime = 0;
	_double							m_LifeTime = 0.05;
	_float4							m_fDestColor{};
private:
	_bool							m_bIsClone = false;
public:
	static CLight* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, const LIGHTDESC& LightDesc, CTransform* pTransform, _bool _tempLight = false);
	virtual CComponent* Clone(void* pArg);
	virtual void Free() override;
};

END