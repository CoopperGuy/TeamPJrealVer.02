#pragma once

#include "Component.h"
BEGIN(Engine)

class ENGINE_DLL CRenderer final : public CComponent
{
public:
	enum RENDER { RENDER_PRIORITY, RENDER_NONALPHA, RENDER_EXTRA, RENDER_ALPHA, RENDER_TRAIL, RENDER_UI, RENDER_TEXT, RENDER_END };
private:
	explicit CRenderer(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	explicit CRenderer(const CRenderer& rhs);
	virtual ~CRenderer() = default;
public:
	static CRenderer* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	virtual CComponent* Clone(void* pArg) override;
	virtual void Free() override;

public:
	virtual HRESULT InitializePrototype();
	virtual HRESULT Initialize(void* pArg);
	
public:
	HRESULT AddRenderGroup(RENDER eRenderID, class CGameObject* pRenderObject);
	HRESULT DrawRenderGroup();

public:
	ID3D11ShaderResourceView*  GetShaderResourceView(string pTargetTag);

private:
	class CTargetManager*					m_pTargetManager = nullptr;
	class CVIBuffer_Rect_Viewport*			m_pVIBuffer = nullptr;
	class CVIBuffer_Rect_Viewport*			m_pVIBuffer_HDR = nullptr;
	class CVIBuffer_Rect_Viewport*			m_pVIBuffer_Bloom = nullptr;
	class CVIBuffer_Rect_Viewport*			m_pVIBuffer_SSAO = nullptr;

private:
	list<class CGameObject*>			m_RenderGroups[RENDER_END];
	typedef list<class CGameObject*>	RENDERGROUPS;
private:
	HRESULT RenderPriority();
	HRESULT RenderLightDepth();
	HRESULT RenderNonAlpha();
	HRESULT Render_LightAcc();
	HRESULT Render_Blend();
	HRESULT Render_Extra();
	HRESULT RenderAlpha();
	HRESULT RenderTrail();
	HRESULT RenderUI();
	HRESULT RenderText();

	HRESULT Render_Shader();
	HRESULT Render_Bloom();
	HRESULT Render_SSAO();
	HRESULT Render_Main();

private:
	_uint	m_iValue = 0;
	_bool	m_bDebuger = false;
};

END