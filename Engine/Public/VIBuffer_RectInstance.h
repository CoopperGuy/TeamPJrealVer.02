#pragma once

#include "VIBuffer.h"

BEGIN(Engine)

class ENGINE_DLL CVIBuffer_RectInstance final : public CVIBuffer
{
public:
	enum SHAPE { RADIATION, CONE, SHAPE_END };
private:
	explicit CVIBuffer_RectInstance(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	explicit CVIBuffer_RectInstance(const CVIBuffer_RectInstance& rhs);
	virtual ~CVIBuffer_RectInstance() = default;

public:
	virtual HRESULT InitializePrototype(string pShaderFilePath, _uint iNumInstance);
	virtual HRESULT Initialize(void* pArg) override;

	HRESULT Update(_double TimeDelta);
	HRESULT Render(_uint iPassIndex);

public:
	HRESULT Initialize_Radiation();
	HRESULT Initialize_Cone();

public:
	_uint 	 Get_CurShape() { return (_uint)m_eShape; }
	_float4&  Get_Color() { return m_vColor; }
	_float&	 Get_Speed() { return m_fSpeed; }
	_float&   Get_LifeTime() { return m_fLifeTime; }
	_float&  Get_Size() { return m_fSize; }
	_float&	 Get_StartRadian() { return m_fStartRadian; }
	_float&	 Get_RadiationAngle() { return m_fRadiationAngle; }
	_int&	 Get_InstanceNum() { return m_iInstNum; }
	_float4&  Get_SrcColor() { return m_vSrcColor; }
	_float4&  Get_DestColor() { return m_vDestColor; }
	_bool	Get_LerpColor() { return m_blerpColor; }

	void	 Set_Shape(SHAPE eShape) { m_eShape = eShape; }
	void	 Set_Color(_float4 vColor) { m_vColor = vColor; }
	void	 Set_Speed(_float fSpeed) { m_fSpeed = fSpeed; }
	void	 Set_LifeTime(_float fLifeTime) { m_fLifeTime = fLifeTime; }
	void	 Set_Size(_float fSize) { m_fSize = fSize; }
	void	 Set_StartRadian(_float fStartRadian) { m_fStartRadian = fStartRadian; }
	void	 Set_RadiationAngle(_float fRadiationAngle) { m_fRadiationAngle = fRadiationAngle; }
	void	 Set_InstanceNum(_int fInstNum) { m_iInstNum = fInstNum; }
	void	 Set_SrcColor(_float4 _vColor) { m_vSrcColor = _vColor; }
	void	 Set_DestColor(_float4 _vColor) { m_vDestColor = _vColor; }
	void	 Set_lerpColor(_bool _isLerp) { m_blerpColor = _isLerp; }

public:
	vector<VTXRECTINST*>&		GetInstanceMatrices() {
		return m_InstanceMatrices;
	};

private:
	ID3D11Buffer*			m_pVBInstance = nullptr;
	D3D11_BUFFER_DESC		m_VBInstanceDesc;
	D3D11_SUBRESOURCE_DATA	m_VBInstanceSubResourceData;

	_uint					m_iNumInstance = 0;
	vector<VTXRECTINST*>		m_InstanceMatrices;
	string					m_shaderPath = "";

	CTransform*				m_pTargetTransform = nullptr;

	SHAPE					m_eShape = CONE;

	_float4					m_vColor = { 1.f, 1.f, 1.f, 0.f };
	_float					m_fLifeTime = 5.0;
	_float					m_fLifeTimeAcc = 0.0;
	
	_float					m_fSpeed = 5.f;
	_float					m_fSize = 1.f;
	_float					m_fAlpha = 1.f;

	_float					m_fStartRadian = 0.f;
	_float					m_fRadiationAngle = 30.f;

	_int					m_iInstNum = 100;

private:
	_float4					m_vSrcColor = { 0.f, 0.f, 0.f, 0.f };
	_float4					m_vDestColor = { 1.f, 1.f, 1.f, 0.f };
	_bool					m_blerpColor = false;

public:
	static CVIBuffer_RectInstance* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, string pShaderFilePath, _uint iNumInstance = 1);
	virtual CComponent* Clone(void* pArg) override;
	virtual void Free() override;
};

END