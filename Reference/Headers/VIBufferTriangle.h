#pragma once
#include "VIBuffer.h"

BEGIN(Engine)
class ENGINE_DLL CVIBufferTriangle :public CVIBuffer
{
private:
	explicit CVIBufferTriangle(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, string shaderPath);
	explicit CVIBufferTriangle(const CVIBufferTriangle& rhs);
	virtual ~CVIBufferTriangle() = default;
public:
	virtual HRESULT InitializePrototype(_fvector* pPoints);
	virtual HRESULT Initialize(void* pArg);
	virtual HRESULT Render(_uint iPassIndex = 0) override;

public:
	static CVIBufferTriangle* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, _fvector* pPoints, string shaderPath = "../../Assets/Shader/Shader_TriangleDBG.fx");
	virtual CComponent* Clone(void* pArg) override;
	virtual void Free() override;

public:
	void SetColor(_float4 color) { m_Color = color; }
private:
	_float4			m_Color = { 1.f, 1.f, 0.f, 1.f };
};

END