#pragma once

#include "Component.h"

BEGIN(Engine)

class ENGINE_DLL CText final : public CComponent
{
public:
	explicit CText(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	explicit CText(const CText& rhs);
	virtual ~CText() = default;
public:
	static CText* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	virtual CComponent* Clone(void* pArg) override;
	virtual void Free() override;
public:
	virtual HRESULT InitializePrototype();
	virtual HRESULT Initialize(void* pArg);
	HRESULT		Render();
public:
	void SetTextInfo(string text, _float layerDepth = 0.f, _float4 color = { 1.f, 1.f, 1.f, 1.f }, _float2 scale = { 1.f, 1.f });
	void SetString(string text) { m_strText = text; }
	void SetColor(_float4 _color) { m_vColor = _color; }
	void SetIsCenter(_bool tf) { m_bisCenter = tf; }
	void SetShader(_bool _shaderCut) { m_bisShaderCut = _shaderCut; }
	void SetIsKorean(_bool _isKorean) { m_bIsKorean = _isKorean; }
public:
	string& GetText() { return m_strText; }
	_float4& GetColor() { return m_vColor; }
	_float	GetLayerDepth() { return m_fLayerDepth; }
	_float2& GetScale() { return m_vScale; }
	_bool&	GetIsCenter() { return m_bisCenter; }
	_bool	GetShaderCut() { return m_bisShaderCut; }
	_bool	GetIsKorean() { return m_bIsKorean; }
private:
	SpriteBatch*		m_pSpriteBatch;
	SpriteFont*			m_pSpriteFont;
	SpriteFont*			m_pSpriteKoreanFont;

private:
	class CRectTransform*			m_pTransform;
	string							m_strText = "";
	_float4							m_vColor = { 1.f, 1.f, 1.f, 1.f };
	_float							m_fLayerDepth = 0.f;
	_float2							m_vScale = { 1.f, 1.f };
	_bool							m_bisCenter = true;
	_bool							m_bisShaderCut = false;
	_bool							m_bIsKorean = false;
};

END