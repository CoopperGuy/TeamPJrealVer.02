#include "EnginePCH.h"
#include "..\public\GameObject.h"
#include "Engine.h"
#include "Component.h"
CGameObject::CGameObject(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
	: m_pDevice(pDevice)
	, m_pDeviceContext(pDeviceContext)
	, m_pEngine(CEngine::GetInstance())
{
}

CGameObject::CGameObject(const CGameObject & rhs)
	: m_pDevice(rhs.m_pDevice)
	, m_pDeviceContext(rhs.m_pDeviceContext)
	, m_pEngine(CEngine::GetInstance())
	, m_UUID(CUUID())
{
}

void CGameObject::SetDead()
{
	m_bDead = true;
	m_pEngine->AddDeadObject(m_Name);
	m_bIsActive = false;
	_float3 pos = _float3(-100.f, -100.f, -100.f);
	CCollider* col = static_cast<CCollider*>(GetComponent("Com_Collider"));
	if (col) {
		PxExtendedVec3 pxPos = col->GetPosition();
		pos = _float3((_float)pxPos.x, -100.f, (_float)pxPos.z);
		col->SetPosition(pos);
	}

	if(m_pTransform)
		m_pTransform->SetState(CTransform::STATE_POSITION, XMLoadFloat3(&pos));
	
	if (m_pParent)
		m_pParent->RemoveChild(this);


	if (0 < m_listChildren.size())
	{
		for (auto& iter = m_listChildren.begin(); iter != m_listChildren.end();)
		{
			(*iter)->SetDead();
			// Child에서 deparent하기때문에 iter를 다시 begin으로 잡아준다
			iter = m_listChildren.begin();
		}
	}
	if(m_listChildren.size() > 0)
		m_listChildren.clear();

}

void CGameObject::DeleteRealObject()
{
	if (dynamic_cast<CCollider*>(GetComponent("Com_Collider")))
		return;
	m_bDead = true;

	if (m_pParent)
		m_pParent->RemoveChild(this);


	if (0 < m_listChildren.size())
	{
		for (auto& iter = m_listChildren.begin(); iter != m_listChildren.end();)
		{
			(*iter)->SetDead();
			// Child에서 deparent하기때문에 iter를 다시 begin으로 잡아준다
			iter = m_listChildren.begin();
		}
	}
	if (m_listChildren.size() > 0)
		m_listChildren.clear();

}



HRESULT CGameObject::InitializePrototype()
{
	for (auto& iter : m_Components) {
		iter.second->SetMaster(this);
	}
	return S_OK;
}

HRESULT CGameObject::Initialize(void * pArg)
{
	for (auto& iter : m_Components) {
		iter.second->SetMaster(this);
	}
	return S_OK;
}

HRESULT CGameObject::InitializePrefab(CGameObject * rhs, void * pArg)
{
	m_Name = rhs->m_Name;
	m_Layer = rhs->m_Layer;
	m_CollisionSize = rhs->m_CollisionSize;
	m_fFrustumRange = rhs->m_fFrustumRange;
	m_iPassIndex = rhs->m_iPassIndex;
	m_bInstancing = rhs->m_bInstancing;
	m_bDead = rhs->m_bDead;
	m_bIsActive = rhs->m_bIsActive;
	m_bisChild = rhs->m_bisChild;
	m_bisRender = rhs->m_bisRender;
	m_bLinkEquip = rhs->m_bLinkEquip;
	m_bAttack = rhs->m_bAttack;
	m_bCollsionIng = rhs->m_bCollsionIng;
	m_bComboAtt = rhs->m_bComboAtt;

	return S_OK;
}

_uint CGameObject::Update(_double TimeDelta)
{
	for (auto& iter : m_Components) {
		iter.second->SetMaster(this);
	}
	return _uint();
}

_uint CGameObject::LateUpdate(_double dDeltaTime)
{
	return _uint();
}

HRESULT CGameObject::Render(_uint iPassIndex)
{

	return S_OK;
}

void CGameObject::SetInfo(string name, string layer, uint64_t uuid, _bool active, _uint curScene)
{
	// if UUID is not 0, it means it has uuid to inherit
	if (uuid)
		m_UUID = uuid;

	m_Name = name;
	m_Layer = layer;
	m_bIsActive = active;
	m_pEngine->AddGameObjectWithName(curScene, name, this);
	m_pEngine->AddGameObjectWithUUID(curScene, uuid, this);
}

void CGameObject::SetPrefabInfo(string name, string layer, uint64_t uuid, _bool active, _uint curScene)
{
	if (uuid)
		m_UUID = uuid;

	m_Name = name;
	m_Layer = layer;
	m_bIsActive = active;
}

void CGameObject::LinkTranformWithParent()
{
}


void CGameObject::SetActive(_bool value)
{
	m_bIsActive = value;
	for (auto& child : m_listChildren)
		child->SetActive(value);
}

_float CGameObject::GetViewZ()
{

	if(!m_pTransform){
		m_pTransform = static_cast<CTransform*>(GetComponent("Com_Transform"));
	}
	if (m_pTransform) {
		_vector viewPos = XMVector4Transform(m_pTransform->GetState(CTransform::STATE_POSITION), m_pEngine->GetTransform(CPipeline::D3DTS_VIEW));
		return XMVectorGetZ(viewPos);
	}
	return 0.f;
}

_float3 CGameObject::GetPosition()
{
	if (!m_pTransform) {
		m_pTransform = static_cast<CTransform*>(GetComponent("Com_Transform"));
	}
	if (m_pTransform) {
		_float3 position;
	 	XMStoreFloat3(&position, m_pTransform->GetState(CTransform::STATE_POSITION));
		return position;
	}
	return _float3();
}

_float3 CGameObject::GetCollisionPosition()
{
	CCollider* col = static_cast<CCollider*>(GetComponent("Com_Collider"));
	if (col) {
		PxExtendedVec3 pos = col->GetPosition();
		return _float3((_float)pos.x, (_float)pos.y, (_float)pos.z);
	}
	return _float3();
}

void CGameObject::SetParent(CGameObject* pParent)
{
	m_pParent = pParent;
	LinkTranformWithParent();
}

void CGameObject::AddChild(CGameObject* pChild)
{
	if (pChild->GetParent())
		pChild->GetParent()->RemoveChild(pChild);
	m_listChildren.push_back(pChild);
	pChild->SetParent(this);
}

void CGameObject::AddChild2(CGameObject* pChild)
{
	if (pChild->GetParent())
		pChild->GetParent()->RemoveChild(pChild);
	m_listChildren.push_back(pChild);
}

void CGameObject::AddChildPrototypeTag(string pPrototypeTag)
{
	m_listChildrenPrototypeTag.push_back(pPrototypeTag);
}

void CGameObject::RemoveChild(CGameObject * pChild)
{
	if (m_listChildren.size() <= 0)
		return;

	uint64_t uuid = pChild->GetUUID();

	for (auto& iter = m_listChildren.begin(); iter != m_listChildren.end();)
	{
		if (uuid == (*iter)->GetUUID())
		{
			(*iter)->SetParent(nullptr);
			iter = m_listChildren.erase(iter);
			return;
		}
		else
			++iter;
	}
}

void CGameObject::RemoveParent()
{
	if (m_pParent != nullptr) {
		m_pParent->RemoveChild(this);
	}
}

void CGameObject::ClearChildren()
{
	for (auto& child : m_listChildren)
		child->SetParent(nullptr);

	m_listChildren.clear();
}

CGameObject * CGameObject::FindChildWithName(string name)
{
	auto iter = find_if(m_listChildren.begin(), m_listChildren.end(), [&](CGameObject* child) {
		return child->GetName() == name;
	});
	if (iter != m_listChildren.end())
		return (*iter);

	return nullptr;
}

HRESULT CGameObject::SetUpComponents(_uint iSceneIndex, string pPrototypeTag, string pComponentTag, CComponent ** pOut, void * pArg)
{
	CEngine*	pEngine = GET_INSTANCE(CEngine);

	CComponent*		pComponent = pEngine->CloneComponent(iSceneIndex, pPrototypeTag, pArg);
	if (nullptr == pComponent)
		return E_FAIL;

	if (m_Components.end() != m_Components.find(pComponentTag))
		return E_FAIL;
	else
	{
		m_Components.emplace(pComponentTag, pComponent);
		if (pOut)
			*pOut = pComponent;
		//SafeAddRef(pComponent);
	}

	RELEASE_INSTANCE(CEngine);

	return S_OK;
}

HRESULT CGameObject::AddComponent(_uint iSceneIndex, string pPrototypeTag, string pComponentTag, void* pArg)
{
	if (GetComponent(pComponentTag))
	{
		MSG_BOX("Component already exist");
		return E_FAIL;
	}

	CComponent*		pComponent = m_pEngine->CloneComponent(iSceneIndex, pPrototypeTag, pArg);
	if (nullptr == pComponent)
		return E_FAIL;

	if (nullptr == GetComponent(pComponentTag))
		m_Components.emplace(pComponentTag, pComponent);
	else
		return E_FAIL;

	return S_OK;
}

HRESULT CGameObject::AddComponent(string pComponentTag, CComponent * pComponent)
{
	if (GetComponent(pComponentTag))
	{
		MSG_BOX("Component already exist");
		return E_FAIL;
	}

	m_Components.emplace(pComponentTag, pComponent);

	return S_OK;
}

HRESULT CGameObject::AddModelComponent(_uint iSceneIndex, CComponent * pModel)
{
	if (nullptr != GetComponent("Com_Model"))
		return E_FAIL;

	m_Components.emplace("Com_Model", pModel);
	return S_OK;
}

HRESULT CGameObject::AddModelInsComponent(_uint iSceneIndex, CComponent * pModel)
{
	if (nullptr != GetComponent("Com_InsModel"))
		return E_FAIL;

	m_Components.emplace("Com_InsModel", pModel);
	return S_OK;
}

CComponent* CGameObject::GetComponent(string pComponentTag)
{
	auto iter = m_Components.find(pComponentTag);
	if (m_Components.end() == iter)
		return nullptr;
	else
		return (*iter).second;
}

HRESULT CGameObject::RemoveComponent(string pComponentTag)
{
	auto iter = m_Components.find(pComponentTag);
	if (m_Components.end() == iter)
		return E_FAIL;

	SafeRelease((*iter).second);
	m_Components.erase(iter);
	return S_OK;
}

unordered_map<string, class CComponent*> CGameObject::GetComponentList()
{
	return m_Components;
}



CGameObject * CGameObject::PrefabClone(void * pArg)
{
	return nullptr;
}

CGameObject * CGameObject::ChildrenPrefabClone(CGameObject * Parent, void * pArg)
{
	return nullptr;
}

void CGameObject::Free()
{
	for (auto& Pair : m_Components)
	{
//#ifdef _DEBUG
//
//		if (Pair.second)
//			cout << GetName() << "obj name "<< Pair.first << " com name " << Pair.second->getRefCnt() << " cnt" << "\n";
//#endif
		SafeRelease(Pair.second);
//#ifdef _DEBUG
//		if(Pair.second)
//			cout << GetName() << "obj name " << Pair.first << " com name " << Pair.second->getRefCnt() << " cnt" << "\n";
//#endif
	}
	m_Components.clear();
//#ifdef _DEBUG
//	cout << GetName() << " : obj name " << this->getRefCnt() << " cnt" << "\n";
//#endif

	m_pParent = nullptr;
}
