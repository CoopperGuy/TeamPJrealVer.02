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
	const _bool	GetIsEquip() { return m_bisEquip; }

public:
	virtual void Free() override;

protected:
	unordered_map<string, CModel*> m_EquipList;
protected:
	CTransform* m_pTargetTransform = nullptr;
	CModel*		m_pTargetModel = nullptr;
protected:
	ITEMINFO	m_pEquipInfo{};
	string		m_EquipPath{};
protected:
	_bool		m_bisEquip = false;
	_bool		m_bWeapon = false;
};

END