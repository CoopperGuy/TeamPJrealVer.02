#pragma once

#include "VIBuffer.h"
BEGIN(Engine)

class ENGINE_DLL CVIBuffer_Rect final : public CVIBuffer
{
private:
	explicit CVIBuffer_Rect(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, string shaderPath);
	explicit CVIBuffer_Rect(const CVIBuffer_Rect& rhs);
	virtual ~CVIBuffer_Rect() = default;
public:
	virtual HRESULT InitializePrototype();
	virtual HRESULT Initialize(void* pArg);
	virtual HRESULT Render(_uint iPassIndex = 0) override;
public:
	static CVIBuffer_Rect* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, string shaderPath = "../../Assets/Shader/Shader_Rect.fx");
	virtual CComponent* Clone(void* pArg) override;
	virtual void Free() override;

public:
	string GetTextureFilePath();
	void UpdateTexture(string texturePath);
	void SetColor(_float4 color) { m_Color = color; }
	void SetAlpha(_float _alpha) { m_fAlpha = _alpha; }
private:
	class CTexture*		m_pTexture = nullptr;
private:
	_float4			m_Color = { 1.f, 1.f, 1.f, 1.f };
private:
	_float	m_fAlpha = 1.f;
};

END