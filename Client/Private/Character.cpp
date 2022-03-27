#include "stdafx.h"
#include "..\Public\Character.h"

USING(Client)
CCharacter::CCharacter()
{
}

CCharacter::CCharacter(CGameObject * pObj)
	: m_pGameObject(pObj)
{
}

CCharacter * CCharacter::Create(CGameObject * pObj)
{
	return nullptr;
}

void CCharacter::Free()
{
}

void CCharacter::SetObjectTransform(CGameObject * pObj, BONEDESC * pBone)
{
	if (pObj)
	{
		_float4x4 pos;
		_matrix matPos;

		if (m_pModel->IsSimulatingRagdoll())
			matPos = pBone->pHierarchyNode->Get_CombinedTransformationMatrix();
		else
			matPos = pBone->pHierarchyNode->Get_CombinedTransformationMatrix() * m_pTransform->GetWorldMatrix();
		XMStoreFloat4x4(&pos, matPos);

		static_cast<CTransform*>(pObj->GetComponent("Com_Transform"))->SetMatrix(pos);
	}
}

_vector CCharacter::GetBonePosition(BONEDESC * pBone)
{
	_vector s, r, t;
	_matrix mat = pBone->pHierarchyNode->Get_CombinedTransformationMatrix() * m_pTransform->GetWorldMatrix();
	XMMatrixDecompose(&s, &r, &t, mat);

	return t;
}

void CCharacter::Update(_double deltaTime)
{
	
}

void CCharacter::LateUpdate(_double deltaTime)
{
}
