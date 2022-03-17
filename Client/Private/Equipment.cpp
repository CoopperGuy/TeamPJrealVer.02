#include "stdafx.h"
#include "..\Public\Equipment.h"
#include "Player.h"

USING(Client)

CEquipment::CEquipment()
{
}

CEquipment::CEquipment(CGameObject * pObj)
	:CCharacter(pObj)
{
}

HRESULT CEquipment::Initialize()
{
	CEmptyGameObject* pPlayer = static_cast<CEmptyGameObject*>(CEngine::GetInstance()->GetGameObjectInLayer(0, "LAYER_PLAYER").front());
	if (pPlayer)
	{
		m_pTargetTransform = static_cast<CTransform*>(pPlayer->GetComponent("Com_RenderTransform"));
		m_pTargetModel = static_cast<CModel*>(pPlayer->GetComponent("Com_Model"));
	}
	return S_OK;
}

void CEquipment::Update(_double deltaTime)
{
}

void CEquipment::LateUpdate(_double deltaTime)
{
}

void CEquipment::Render()
{
}

void CEquipment::Linked_SkinnedInfoToTarget(CModel * pTarget)
{
	if (pTarget)
	{
		m_pModel->SetUp_SkinnedInfoToEquipment(pTarget);
	}
}

HRESULT CEquipment::Add_EquipList(string name)
{
	if (Find_EquipList(name))
		return E_FAIL;

	CGameObject* pGameObject = CEngine::GetInstance()->FindGameObjectWithName(SCENE_STATIC, name);
	CModel* pModel = static_cast<CModel*>(pGameObject->GetComponent("Com_Model"));
	pModel->SetUp_SkinnedInfoToEquipment(m_pTargetModel);
	static_cast<CEmptyGameObject*>(pGameObject)->SetTargetTransform(m_pTargetTransform);
	pGameObject->SetActive(false);
	m_EquipList.emplace(pModel->GetMeshFileName(),pModel);

	return S_OK;
}

CModel* CEquipment::Find_EquipList(string name)
{
	auto iter = m_EquipList.find(name + ".fbx");
	if (iter == m_EquipList.end())
		return nullptr;

	return iter->second;


}

_bool CEquipment::Set_ModelCom(string name)
{
	CModel* pModel = Find_EquipList(name);

	if (pModel)
	{
		if (m_pModel)
		{
			if (m_pModel->GetMeshFileName() == pModel->GetMeshFileName())
			{
				if(m_pGameObject->GetActive())
					ActiveEquip(false);
				else
					ActiveEquip(true);
				return true;
			}
			ActiveEquip(false);
		}
		m_pGameObject = CEngine::GetInstance()->FindGameObjectWithName(SCENE_STATIC, name);
		m_pModel = pModel;
		ActiveEquip(true);
	}
	else
	{
		MSG_BOX("Fuck! It's doesn't have");
		return false;
	}
	return true;
}

void CEquipment::Free()
{
}