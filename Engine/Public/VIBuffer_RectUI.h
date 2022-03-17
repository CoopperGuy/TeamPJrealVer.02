#pragma once

#include "VIBuffer.h"
#include "RectTransform.h"
#include "Texture.h"
BEGIN(Engine)

class ENGINE_DLL CVIBuffer_RectUI final : public CVIBuffer
{
public:
	enum TEXTUREID{TEXTURE_DIFFUSE,TEXUTRE_MASK,TEXUTRE_NOISE,TEXUTRE_NORMAL,TEXTURE_END};
private:
	explicit CVIBuffer_RectUI(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	explicit CVIBuffer_RectUI(const CVIBuffer_RectUI& rhs);
	virtual ~CVIBuffer_RectUI() = default;
public:
	virtual HRESULT InitializePrototype();
	virtual HRESULT Initialize(void* pArg);
public:
	virtual HRESULT Render(_uint iPassIndex = 0) override;
public:
	static CVIBuffer_RectUI* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	virtual CComponent* Clone(void* pArg) override;
	virtual void Free() override;

public:
	string GetTextureFilePath(TEXTUREID idx);
	void UpdateTexture(string texturePath, TEXTUREID idx);
	_float4& GetColor() { return m_Color; }
	void SetColor(_float4 color) { m_Color = color; }

private:
	CRectTransform* m_pRectTransform = nullptr;
	CTexture*		m_pTexture[TEXTURE_END] = {nullptr,nullptr ,nullptr ,nullptr };
	_float4			m_Color = {0.f, 0.f, 0.f, 1.f};
	string			m_strCurPath{};
};

END