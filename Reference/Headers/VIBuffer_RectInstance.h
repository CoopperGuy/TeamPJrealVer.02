#pragma once

#include "VIBuffer.h"

BEGIN(Engine)

class ENGINE_DLL CVIBuffer_RectInstance final : public CVIBuffer
{
	enum SHAPE { SPARK, CONE, SHAPE_END };

private:
	explicit CVIBuffer_RectInstance(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	explicit CVIBuffer_RectInstance(const CVIBuffer_RectInstance& rhs);
	virtual ~CVIBuffer_RectInstance() = default;

public:
	virtual HRESULT InitializePrototype(string pShaderFilePath, _uint iNumInstance);
	virtual HRESULT Initialize(void* pArg) override;

	HRESULT Update(_double TimeDelta);
	HRESULT Render(_uint iPassIndex);

private:
	HRESULT Initialize_Spark();
	HRESULT Initialize_Cone();

public:
	_float4& Get_Color() { return m_vColor; }

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

	SHAPE					m_eShape = SHAPE_END;

	_float4					m_vColor = { 0.5f, 0.5f, 0.5f, 0.f };
	_double					m_dLifeTime = 0.0;
	_double					m_dLifeTimeAcc = 0.0;
	
	_float					m_fSpeed = 5.f;
	_float					m_fSize = 1.f;
	_float					m_fAlpha = 1.f;

	_uint					m_iInstNum = 0;


public:
	static CVIBuffer_RectInstance* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, string pShaderFilePath, _uint iNumInstance = 1);
	virtual CComponent* Clone(void* pArg) override;
	virtual void Free() override;
};

END