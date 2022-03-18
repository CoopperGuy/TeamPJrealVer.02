#pragma once

#include "GameObject.h"
#include "Renderer.h"
#include "Transform.h"
#include "VIBuffer.h"
#include "Texture.h"

BEGIN(Engine)

class ENGINE_DLL CEmptyEffect final : public CGameObject
{
public:
	enum TEXTUREID { TEXTURE_MASK, TEXTURE_NOISE, TEXTURE_DIFFUSE, TEXTURE_END };
public:
	explicit CEmptyEffect(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	explicit CEmptyEffect(const CEmptyEffect& rhs);
	virtual ~CEmptyEffect() = default;
public:
	static CEmptyEffect* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual CGameObject* PrefabClone(void* pArg) override;
	virtual CGameObject* ChildrenPrefabClone(CGameObject* Parent, void* pArg) override;
	virtual void Free() override;

public:
	virtual HRESULT InitializePrototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual HRESULT InitializePrefab(CEmptyEffect* rhs, void* pArg) ;
	virtual HRESULT InitializeChildrenPrefab(CEmptyEffect* rhs, CEmptyEffect* Parent, void* pArg);
	virtual _uint Update(_double TimeDelta) override;
	virtual _uint LateUpdate(_double TimeDelta) override;
	virtual HRESULT Render(_uint iPassIndex = 0) override;
	
public:
	string GetTextureFilePath(TEXTUREID idx);
	_float3 GetScrollSpeedX() { return m_vScrollSpeedX; }
	_float3 GetScrollSpeedY() { return m_vScrollSpeedY; }
	_float2 GetDistortion(_uint iIdx) { return m_vDistortion[iIdx]; }

	_float GetPadding() { return m_fPadding; }
	_float GetDistortionScale() { return m_fDistortionScale; }
	_float GetDistortionBias() { return m_fDistortionBias; }
	_float GetEffectDuration() { return m_fEffectDuration; }
	_float GetFadeOutStartTime() { return m_fFadeOutStratTime; }
	_float GetFadeOutDuration() { return m_fFadeOutDuration; }
	_float GetFadeInStartTime() { return m_fFadeInStratTime; }
	_float GetFadeInDuration() { return m_fFadeInDuration; }
	_float GetFadeAlpha() { return m_fFadeAlpha; }
	_float GetProcessTime() { return m_fProcessTime; }
	_float GetMoveSpd() { return m_fMoveSpd; }

	_bool GetFadeOutEnable() { return m_bFadeOut; }
	_bool GetFadeInEnable() { return m_bFadeIn; }
	_bool GetBillBord() { return m_bBillBord; }
	_bool CheckLinkTarget() { return m_bLinkTarget; }
	_bool GetSpriteEnd() { return m_SpriteEnd;}

	string GetTargetObjectName() { return m_TargetObjName; }
	string GetTargetBoneName() { return m_TargetBoneName; }

	void SetTexture(string TextureFilePath, TEXTUREID eTextureId);
	void SetPassIndex(_uint iPassIndex) { m_iPassIndex = iPassIndex; }
	void SetScrollSpeedX(_float3 vScrollSpeedX) { m_vScrollSpeedX = vScrollSpeedX; }
	void SetScrollSpeedY(_float3 vScrollSpeedY) { m_vScrollSpeedY = vScrollSpeedY; }
	void setDistortion(_uint iIdx, _float2 vDistortion) { m_vDistortion[iIdx] = vDistortion; }
	void SetDistortionScale(_float fDistortionScale) { m_fDistortionScale = fDistortionScale; }
	void SetDistortionBias(_float fDistortionBias) { m_fDistortionBias = fDistortionBias; }

	void SetEffectDuration(_float fEffectDuration) { m_fEffectDuration = fEffectDuration; }
	void SetFadeOutEnable(_bool bFadeOut) { m_bFadeOut = bFadeOut; }
	void SetFadeOutStartTime(_float fFadeOutStratTime) { m_fFadeOutStratTime = fFadeOutStratTime; }
	void SetFadeOutDuration(_float fFadeOutDuration) { m_fFadeOutDuration = fFadeOutDuration; }
	void SetFadeInEnable(_bool bFadeIn) { m_bFadeIn = bFadeIn; }
	void SetFadeInStartTime(_float fFadeInStratTime) { m_fFadeInStratTime = fFadeInStratTime; }
	void SetFadeInDuration(_float fFadeInDuration) { m_fFadeInDuration = fFadeInDuration; }
	void SetAlpha(_float fAlpha) { m_fAlpha = fAlpha; }

	void SetBillBord(_bool billbord) { m_bBillBord = billbord; }
	void SetLinkTarget(_bool bLinkTarget) { m_bLinkTarget = bLinkTarget; }
	void SetTargetObjectName(string TargetObjName) { m_TargetObjName = TargetObjName; }
	void SetTargetBoneName(string TargetBoneName) { m_TargetBoneName = TargetBoneName; }

	void SetProcessTime(_float _processTime) { m_fProcessTime = _processTime; }
	void SetMoveSpd(_float _moveSpd) { m_fMoveSpd = _moveSpd; }
	void RemoveTargetObject() { m_pTargetObject = nullptr; }
	void LinkTransform();

	void SetSpriteSpeed(_float pSpeed) { m_fSpriteSpeed = pSpeed; }

private:
	HRESULT SetUpComponents();
	virtual void LinkTranformWithParent();
	void UpdateFadeEffect(_double TimeDelta);

private:
	void SetUp_ValueOnShader(string ComponentTag);
	void SetUp_ValueOnModelShader();

private:
	CTransform*		m_pTransformCom = nullptr;	
	CTransform*		m_pParentTransformCom = nullptr;
	CTransform*		m_pLocalTransformCom = nullptr;
	CRenderer*		m_pRendererCom = nullptr;
	CTexture*		m_pTexture[TEXTURE_END] = { nullptr, };
	CGameObject*	m_pTargetObject = nullptr;

/* UV Animation  */
	_float m_fFrameTime = 0.f;
	_float3 m_vScrollSpeedX = { 0.f, 0.f, 0.f };
	_float3 m_vScrollSpeedY = { 1.3f, 2.1f, 2.3f };
	_float3 m_vScale = { 1.f, 2.f, 3.f };
	_float m_fPadding = 0.f;

	_float2 m_vDistortion[3] = { { 0.1f, 0.2f }, { 0.1f, 0.3f }, { 0.1f, 0.1f } };
	_float	m_fDistortionScale = 0.8f;
	_float	m_fDistortionBias = 0.5f;

/* Effect Option */
	_float	m_fEffectFrameTime = 0.f;
	_float	m_fEffectDuration = 0.f;
	_float	m_fFadeAlpha = 1.f;
	_float  m_fAlpha = 1.f;
	_float3 m_vZoomScale = { 1.f, 1.f, 1.f };

	_bool	m_bFadeOut = false;
	_float	m_fFadeOutDuration = 0.f;
	_float	m_fFadeOutStratTime = 0.f;

	_bool	m_bFadeIn = false;
	_float	m_fFadeInDuration = 0.f;
	_float	m_fFadeInStratTime = 0.f;
	
	_bool	m_bZoomIn = false;	
	_bool	m_bZoomOut = false;

	_float	m_fSpriteNum = 0;

	_bool	m_bBillBord = false;

	_bool	m_bLinkTarget = false;
	string	m_TargetObjName = "";
	string	m_TargetBoneName = "";

	_bool	m_SpriteEnd = false;

private:
	_float m_fMoveSpd = 1.f;
	_float m_fProcessTime = 0.f;
	_float m_fSpriteSpeed = 15.f;

};

END