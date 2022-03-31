#pragma once

#include "VIBuffer.h"
BEGIN(Engine)

class ENGINE_DLL CVIBuffer_Trail final : public CVIBuffer
{
private:
	explicit CVIBuffer_Trail(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, string shaderPath);
	explicit CVIBuffer_Trail(const CVIBuffer_Trail& rhs);
	virtual ~CVIBuffer_Trail() = default;
public:
	virtual HRESULT InitializePrototype();
	virtual HRESULT Initialize(void* pArg);
	HRESULT Update(_double TimeDelta, _fmatrix WeaponTransform);
	virtual HRESULT Render(_uint iPassIndex = 0) override;
public:
	static CVIBuffer_Trail* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, string shaderPath = "../../Assets/Shader/Shader_Rect.fx");
	virtual CComponent* Clone(void* pArg) override;
	virtual void Free() override;

private:
	_float3	 m_vLowOffset = { 0.f, 0.f, 0.f };
	_float3  m_vHighOffset = { 0.f, 0.f, 0.f };

	_double m_TimeAcc = 0.0;

	_float	 m_fAlpha = -1.f;

private:
	_uint	m_iCatmullRomCnt = 10;
	_int	m_iVtxCnt = 0;
	_int	m_iEndIdx = 0;
	_int	m_iCatmullRomIdx[4];
	vector<VTXTEX> m_vecCatmullRom;
};

END