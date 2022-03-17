#include "stdafx.h"
#include "..\Public\ToolCharacter.h"

USING(Tool)
CToolCharacter::CToolCharacter()
{
}

CToolCharacter::CToolCharacter(CGameObject * pObj)
	: m_pGameObject(pObj)
{
}

CToolCharacter * CToolCharacter::Create(CGameObject * pObj)
{
	return nullptr;
}

void CToolCharacter::Free()
{
}

void CToolCharacter::SetObjectTransform(CGameObject * pObj, BONEDESC * pBone)
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

		dynamic_cast<CTransform*>(pObj->GetComponent("Com_Transform"))->SetMatrix(pos);
	}
}

_vector CToolCharacter::GetBonePosition(BONEDESC * pBone)
{
	_vector s, r, t;
	_matrix mat = pBone->pHierarchyNode->Get_CombinedTransformationMatrix() * m_pTransform->GetWorldMatrix();
	XMMatrixDecompose(&s, &r, &t, mat);

	return t;
}
