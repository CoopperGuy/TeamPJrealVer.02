#pragma once

#include "GameObject.h"
#include "Renderer.h"
#include "Transform.h"
#include "VIBuffer.h"
#include "Navigation.h"
#include "VIBuffer_RectInstance.h"

BEGIN(Engine)

class ENGINE_DLL CEmptyMapObject final : public CGameObject
{
public:
	explicit CEmptyMapObject(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	explicit CEmptyMapObject(const CEmptyMapObject& rhs);
	virtual ~CEmptyMapObject() = default;
public:
	static CEmptyMapObject* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	virtual CGameObject* Clone(void* pArg) ;
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
	void SetRenderGroup(CRenderer::RENDER type) { m_eRenderGroup = type; }
public:
	_int&	GetRenderGroup() { return	(_int&)m_eRenderGroup; }
	_bool	GetHaveParent();
private:
	HRESULT SetUpComponents();
	virtual void LinkTranformWithParent();

public:

	_bool& Get_SelectVillage() { return m_bVillage; }
	_bool& Get_SelectDungeon() { return m_bDungeon; }
	
	_bool& Get_SelectBuilding() { return m_bBuilding; }
	_bool& Get_SelectBackGround() { return m_bBackGround; }
	_bool& Get_SelectGround() { return m_bGround; }

	_int& Get_InstancingCount() { return m_iInstancingCount; }

public:
	void Set_SelectVillage(_bool Stage) { m_bVillage = Stage; }
	void Set_SelectDungeon(_bool Stage) { m_bDungeon = Stage; }

	void Set_SelectBuilding(_bool SelectObj) { m_bBuilding = SelectObj; }
	void Set_SelectBackGround(_bool SelectObj) { m_bBackGround = SelectObj; }
	void Set_SelectGround(_bool SelectObj) { m_bGround = SelectObj; }

	void Set_InstancingCount(_int InstancingCount) { m_iInstancingCount = InstancingCount; }

private:
	CTransform*		m_pTransformCom = nullptr;
	CRenderer*		m_pRendererCom = nullptr;
	CTransform*		m_pParentTransformCom = nullptr;
	CTransform*		m_pLocalTransformCom = nullptr;
	CNavigation*	m_pNavigation = nullptr;
	class CInstanceModel*	m_pModelCom = nullptr;

private:
	CRenderer::RENDER m_eRenderGroup = CRenderer::RENDER_NONALPHA;


private:
	//스테이지 나누기
	_bool m_bVillage = false;
	_bool m_bDungeon = false;

	//오브젝트 나누기
	_bool m_bBuilding = false; // 건물
	_bool m_bBackGround = false; // 건물, 바닥 제외한 etc 느낌
	_bool m_bGround = false; //바닥 (풀,바위 , 나무 등등)

	_int  m_iInstancingCount = 0;
};

END