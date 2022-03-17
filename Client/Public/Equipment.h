#pragma once
#include "Character.h"
#include "Engine.h"

BEGIN(Client)

class CEquipment : public CCharacter
{
public:
	explicit CEquipment();
	explicit CEquipment(CGameObject* pObj);
	virtual ~CEquipment() = default;

public:
	virtual HRESULT Initialize();
	virtual void Update(_double deltaTime);
	virtual void LateUpdate(_double deltaTime);
	virtual void Render();

protected:
	void Linked_SkinnedInfoToTarget(CModel* pTarget);

public:
	 void ActiveEquip(_bool Active) { m_pGameObject->SetActive(Active); }
	 const string GetName() const { return m_EquipPath; }
	 const CGameObject* Get_GameObj() const { return m_pGameObject; }
public:
	HRESULT Add_EquipList(string name);
	CModel* Find_EquipList(string name);
	_bool Set_ModelCom(string name);

public:
	virtual void Free() override;

protected:
	CTransform* m_pTargetTransform = nullptr;
	CModel*		m_pTargetModel = nullptr;
	string		m_EquipPath = {};
	ITEMINFO	m_pEquipInfo = {};
	unordered_map<string, CModel*> m_EquipList;
};

END