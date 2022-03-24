#pragma once

#include "GameObject.h"
#include "Renderer.h"
#include "Transform.h"
#include "VIBuffer.h"
#include "Navigation.h"
#include "InstanceModel.h"


BEGIN(Engine)
class CBasicCollider;

class ENGINE_DLL CEmptyGameObject final : public CGameObject
{
public:
	explicit CEmptyGameObject(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	explicit CEmptyGameObject(const CEmptyGameObject& rhs);
	virtual ~CEmptyGameObject() = default;
public:
	static CEmptyGameObject* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual CGameObject* PrefabClone(void* pArg) override;
	virtual CGameObject* ChildrenPrefabClone(CGameObject* Parent, void* pArg) override;
	virtual void Free() override;

public:
	virtual HRESULT InitializePrototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual HRESULT InitializePrefab(CEmptyGameObject* rhs, void* pArg);
	virtual HRESULT InitializeChildrenPrefab(CEmptyGameObject* rhs, CEmptyGameObject* Parent, void* pArg);
	virtual _uint Update(_double TimeDelta) override;
	virtual _uint LateUpdate(_double TimeDelta) override;
	virtual HRESULT Render(_uint iPassIndex = 0) override;

public:
	void SetRenderGroup(CRenderer::RENDER type) { m_eRenderGroup = type; }
	void SetLocalPosition(_float3 pos);
	void SetPosition(_float3 pos);
	void SetTargetTransform(CTransform* pTarget) { m_pTargetTransform = pTarget; }
	virtual void	SetisRender(_bool tf) override;
	void Set_Render(_uint index, _bool Render) { m_bRender[index] = Render; }
	void SetNumOBB(_int obb) { m_iNumOBBs = obb; }
	void SetRimLight(_bool bRimLight, _fvector _lightColor, _double _lifeTime);
	void SetPercentage(_float _percentage) { m_fPercentage = _percentage; }
	void SetBackPercentage(_float _backpercentage) { m_fBackPercentage = _backpercentage; }
	//DirectX::Colors::Red;
public:
	_int&	GetRenderGroup() { return	(_int&)m_eRenderGroup; }
	_bool	GetHaveParent();
	CTransform* GetTargetTransform() { return m_pTargetTransform; }
	_int	GetNumOBB() { return m_iNumOBBs; }
private:
	HRESULT SetUpComponents();
	virtual void LinkTranformWithParent();


private:
	CBasicCollider* m_pOBB					= nullptr;
	CBasicCollider* m_pAABB					= nullptr;
	CTransform*		m_pTransformCom			= nullptr;
	CTransform*		m_pRenderTransformCom	= nullptr;
	CRenderer*		m_pRendererCom			= nullptr;
	CTransform*		m_pParentTransformCom	= nullptr;
	CTransform*		m_pLocalTransformCom	= nullptr;
	CTransform*		m_pTargetTransform		= nullptr;
	CNavigation*	m_pNavigation			= nullptr;
	_bool			m_bRender[10];
private:
	_int			m_iNumOBBs = 0;
	_int			m_iAnimationIndex = 0;

private:
	_bool			m_bRimLightEnable = false;
	_float4			m_vRimLightColor = { 1.f, 0.5f, 0.5f, 1.f };
	_double			m_RimLightTimeAcc = 0.f;
	_double			m_RimLightTime = 0.15f;
private:
	_float	m_fPercentage = 1.f;
	_float	m_fBackPercentage = 1.f;

private:
	CRenderer::RENDER m_eRenderGroup		= CRenderer::RENDER_NONALPHA;

};

END