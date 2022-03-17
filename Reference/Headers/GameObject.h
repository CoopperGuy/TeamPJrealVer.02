#pragma once

#include "Base.h"
#include "UUID.h"



BEGIN(Engine)
class CTransform;

class ENGINE_DLL CGameObject abstract : public CBase
{
protected:
	explicit CGameObject(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	explicit CGameObject(const CGameObject& rhs);
	virtual ~CGameObject() = default;
public:
	virtual HRESULT InitializePrototype();
	virtual HRESULT Initialize(void* pArg);
	virtual HRESULT InitializePrefab(CGameObject* rhs, void* pArg);
	virtual _uint	Update(_double dDeltaTime);
	virtual _uint	LateUpdate(_double dDeltaTime);
	virtual HRESULT Render(_uint iPassIndex = 0);

public:
	void SetDead();
	void DeleteRealObject();
	_bool isDead() { return m_bDead; }
public:
	_int& GetPassIndex() { return m_iPassIndex; }
public:
	void SetPassIndex(_int idx) { m_iPassIndex = idx; }
public:
	/* Getter Setter Name for Tool Inspector */
	string GetName() { return m_Name; }
	void SetName(string name) { m_Name = name; }
public:
	string GetLayer() { return m_Layer; }
	void SetLayer(string layer) { m_Layer = layer; }
public:
	_bool GetInstancing() { return m_bInstancing; }
	void SetInstacing(_bool Ins) { m_bInstancing = Ins; }

public:
	_bool GetLinkEquip() { return m_bLinkEquip; }
	void SetLinkEquip(_bool boolean) { m_bLinkEquip = boolean; }
public:
	uint64_t GetUUID() { return m_UUID(); }
public:
	void SetInfo(string name, string layer, uint64_t uuid, _bool active,_uint curScene);
public:
	virtual void LinkTranformWithParent();
public:
	void SetParent(CGameObject* pParent);
	void AddChild(CGameObject* pChild);
	void AddChild2(CGameObject* pChild);
	void AddChildPrototypeTag(string pPrototypeTag);
	void RemoveChild(CGameObject* pChild);
	void RemoveParent();

	void ClearChildren();
	CGameObject* GetParent() { return m_pParent; }
	list<CGameObject*> GetChildren() { return m_listChildren; }
	list<string> GetChildrenPrototypeTag() { return m_listChildrenPrototypeTag; }
	CGameObject* FindChildWithName(string name);

public:
	_bool&	IsActive() { return m_bIsActive; }
	const _bool	GetActive() const { return m_bIsActive; }
	void	SetActive(_bool value);
	void	SetFrustum(_float value) { m_fFrustumRange = value; }
	virtual void	SetisRender(_bool tf) { m_bisRender = tf; }
	void SetCollisionSize(_float x, _float y , _float z) { m_CollisionSize = _float3(x, y, z); }
public:
	_float& GetFrustum() { return m_fFrustumRange; }
	_bool  GetIsRender() { return m_bisRender; }
	_float GetViewZ();
	_float3 GetPosition();
	_float3	GetCollsionSize() { return m_CollisionSize; }
protected:
	class CEngine*				m_pEngine = nullptr;
	ID3D11Device*			m_pDevice = nullptr;
	ID3D11DeviceContext*	m_pDeviceContext = nullptr;

protected:
	string					m_Name = "Empty Object";
	string					m_Layer = "Default";
	CUUID					m_UUID;
	_float3					m_CollisionSize;
	_float					m_fFrustumRange = 1.f;
	_int					m_iPassIndex = 0;
	_bool					m_bInstancing = false;
protected:
	_bool					m_bDead = false;
	_bool					m_bIsActive = true;
	_bool					m_bisChild = false;
	_bool					m_bisRender = true;
	_bool					m_bLinkEquip = false;
	_bool					m_bAttack = false;
	_bool					m_bCollsionIng = false;
	_bool					m_bComboAtt = false;
protected:
	unordered_map<string, class CComponent*>			m_Components;
	typedef unordered_map<string, class CComponent*>	COMPONENTS;


	list<CGameObject*>	m_listChildren;
	list<string>		m_listChildrenPrototypeTag;
	CGameObject*		m_pParent = nullptr;

	CTransform*			m_pTransform = nullptr;


public:
	_bool	Get_AttackState() { return m_bAttack; }
	void	Set_AttackState(_bool bstate) { m_bAttack = bstate; }

	_bool	Get_CollsionIng() { return m_bCollsionIng; }
	void	Set_CollsionIng(_bool bCollsionIng) { m_bCollsionIng = bCollsionIng; }
//
//public:
//	unordered_map<string, CGameObject*> m_pColliderList;
//	void AddColliderList(string _Type, CGameObject* _pGameObj);

public:
	HRESULT SetUpComponents(_uint iSceneIndex, string pPrototypeTag, string pComponentTag, CComponent** pOut = nullptr, void* pArg = nullptr);
	HRESULT AddComponent(_uint iSceneIndex, string pPrototypeTag, string pComponentTag, void* pArg = nullptr);
	HRESULT AddComponent(string pComponentTag, CComponent* pComponent);
	HRESULT AddModelComponent(_uint iSceneIndex, CComponent* pModel);
	HRESULT AddModelInsComponent(_uint iSceneIndex, CComponent* pModel);
	CComponent* GetComponent(string pComponentTag);
	HRESULT RemoveComponent(string pComponentTag);
	virtual unordered_map<string, class CComponent*>	GetComponentList();
	//???? 
	
public:
	virtual CGameObject* Clone(void* pArg) = 0;
	virtual CGameObject* PrefabClone(void* pArg);
	virtual CGameObject* ChildrenPrefabClone(CGameObject* Parent, void* pArg);
	virtual void Free() override;

};

END