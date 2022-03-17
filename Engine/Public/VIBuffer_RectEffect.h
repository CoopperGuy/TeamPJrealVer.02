#pragma once

#include "VIBuffer.h"
#include "Texture.h"

BEGIN(Engine)

class ENGINE_DLL CVIBuffer_RectEffect final : public CVIBuffer
{
private:
	explicit CVIBuffer_RectEffect(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, string shaderPath);
	explicit CVIBuffer_RectEffect(const CVIBuffer_RectEffect& rhs);
	virtual ~CVIBuffer_RectEffect() = default;
public:
	virtual HRESULT InitializePrototype();
	virtual HRESULT Initialize(void* pArg);
	virtual HRESULT Render(_uint iPassIndex = 0) override;
public:
	static CVIBuffer_RectEffect* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, string shaderPath = "../../Assets/Shader/Shader_Effect.fx");
	virtual CComponent* Clone(void* pArg) override;
	virtual void Free() override;
	
public:
	string GetTextureFilePath();

	void SetTexture(string TextureFilePath);

private:
	CTexture* m_pTexture = nullptr;
};

END