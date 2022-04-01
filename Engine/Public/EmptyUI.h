#pragma once

#include "GameObject.h"
#include "Renderer.h"
#include "RectTransform.h"
#include "VIBuffer.h"

BEGIN(Engine)

class ENGINE_DLL CEmptyUI final : public CGameObject
{
private:
	explicit CEmptyUI(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	explicit CEmptyUI(const CEmptyUI& rhs);
	virtual ~CEmptyUI() = default;
public:
	static CEmptyUI* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual CGameObject* PrefabClone(void* pArg) override;
	virtual CGameObject* ChildrenPrefabClone(CGameObject* Parent, void* pArg) override;
	virtual void Free() override;

public:
	virtual HRESULT InitializePrototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual _uint Update(_double TimeDelta) override;
	virtual _uint LateUpdate(_double TimeDelta) override;
	virtual HRESULT Render(_uint iPassIndex = 0) override;

public:
	void SetRectTransform(CRectTransform::RECTTRANSFORMDESC _desc);
	void SetPosition(_float x, _float y);
	void SetClientPosition(_float x, _float y);
	virtual void LinkTranformWithParent();
	void SetSize(_float x, _float y);
public:
	void InteractMouse();
	bool IsHovered() { return m_bHover; }
	bool IsSelected() { return m_bSelect; }
public:
	void SetSortingOrder(_int order) { m_iSortingOrder = order; }
	void SetSpriteNum(_int _num) { m_iSpriteNum = _num; }
	void SetSpriteX(_int _x) { m_iSpriteX = _x; }
	void SetSpriteY(_int _y) { m_iSpriteY = _y; }
	void SetIsHovering(_bool tf) { m_isHovering = tf; }
	void SetIsShrink(_bool tf) { m_bisShrink = tf; }
	void SetIsBreath(_bool tf) { m_bisBreath = tf; }
	void SetIsSelected(_bool tf) { m_bSelect = tf; }
	void SetIsXShrink(_bool tf) { m_bisXShrink = tf; }
	void SetIsYShrink(_bool tf) { m_bisYShrink = tf; }
	void SetShrinkInfo(_float _max, _float _min, _float _spd) { m_fShrinkMax = _max; m_fShrinkMin = _min, m_fShrinkSpd = _spd; }
	void SetTransformOffst(_float x, _float y) { m_vTransformOffSet.x = x; m_vTransformOffSet.y = y; }
	void SetCorrectSize(_float x, _float y);
	void SetCorrectYSize(_float y);
	void SetPercentage(_float percentage) { m_fPercentage = percentage; }
	void SetBackPercetage(_float backPercentage) { m_fBackPercentage = backPercentage; }
	void SetDegree(_float degree) { m_fDegree = degree; }
	void SetSpriteTime(_float _time) { m_fSpriteTime = _time; }
	void SetAlpha(_float _alpha) { m_fAlpha = _alpha; }
	virtual void	SetisRender(_bool tf) override;
public:
	_bool&	GetIsShrink() { return m_bisShrink; }
	_bool&	GetIsHovering() { return m_isHovering; }
	_bool&	GetIsBreath() { return m_bisBreath; }
	_bool&	GetXShrink() { return m_bisXShrink; }
	_bool&	GetYShrink() { return m_bisYShrink; }
	_int	GetSpriteNum() { return m_iSpriteNum; }
	_int	GetSpriteX() { return m_iSpriteX; }
	_int	GetSpriteY() { return m_iSpriteY; }
	_int&	GetSortingOrder() { return m_iSortingOrder; }
	_float&	GetShrinkMax() { return m_fShrinkMax; }
	_float&	GetShrinkMin() { return m_fShrinkMin; }
	_float&	GetShrinkSpd() { return m_fShrinkSpd; }
	_float	GetPercentage() { return m_fPercentage; }
	_float	GetBackPercentage() { return m_fBackPercentage; }
	_float	GetDegree() { return m_fDegree; }
	_float	GetSpriteTime() { return m_fSpriteTime; }
	_float	GetAlpha() { return m_fAlpha; }
	_float2	GetTransformOffst() { return m_vTransformOffSet; }
	_float2 GetPosition();
	_float2	GetUISize();
public:
	PROPERTY(GetSpriteTime, SetSpriteTime)_float p_SpriteTime;
	PROPERTY(GetSpriteNum, SetSpriteNum)_int p_SpriteNum;
	PROPERTY(GetSpriteX, SetSpriteX)_int p_SpriteX;
	PROPERTY(GetSpriteY, SetSpriteY)_int p_SpriteY;
	PROPERTY(GetAlpha, SetAlpha)_float p_Alpha;

protected:
	_bool m_bHover = false;
	_bool m_bSelect = false;
	_bool m_isHovering = false;
	_bool m_bisShrink = false;
	_bool m_bisBreath = false;
	_bool m_bisXShrink = true;
	_bool m_bisYShrink = true;
protected:
	_int	m_iSortingOrder = 0;
	_int	m_iSprite = 0;
	_int	m_iSpriteNum = 0;
	_int	m_iSpriteX = 0;
	_int	m_iSpriteY = 0;
	_float2	m_vTransformOffSet = { 0.f, 0.f };
	_float	m_fShrinkSpd = 0.f;
	_float	m_fShrinkMax = 50.f;
	_float	m_fShrinkMin = 0.f;
	_float	m_fTime = 0.f;
	_float	m_fSelectHoverTime = 0.f;
	_float	m_fSpriteTime = 0.1f;
	_float	m_fSpriteDelta = 0.f;
	_float	m_fAlpha = 1.f;
private:
	_float	m_fPercentage = 1.f;
	_float	m_fBackPercentage = 1.f;
	_float	m_fDegree = 0.f;
private:
	HRESULT SetUpComponents();

private:
	CRectTransform*		m_pRectTransformCom = nullptr;
	CRenderer*			m_pRendererCom = nullptr;

};

END