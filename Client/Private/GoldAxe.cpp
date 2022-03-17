#include "stdafx.h"
#include "..\Public\GoldAxe.h"

USING(Client)

CGoldAxe::CGoldAxe()
{
}

CGoldAxe::CGoldAxe(CGameObject * pObj)
{
}

HRESULT CGoldAxe::Initialize()
{
	//CGameObject* pAxe = CEngine::GetInstance()->FindGameObjectWithName(CEngine::GetInstance()->GetCurSceneNumber(), "Axe");
	//CGameObject* pPlayer = CEngine::GetInstance()->FindGameObjectWithName(CEngine::GetInstance()->GetCurSceneNumber(), "Player");
	//m_pTransform = dynamic_cast<CTransform*>(pAxe->GetComponent("Com_Transform"));
	//m_pTargetTransform = dynamic_cast<CTransform*>(pPlayer->GetComponent("Com_Transform"));
	//m_pTargetModel = dynamic_cast<CModel*>(pPlayer->GetComponent("Com_Model"));
	//m_pCollider = dynamic_cast<CCollider*>(pAxe->GetComponent("Com_Collider"));
	//m_pModel = dynamic_cast<CModel*>(pAxe->GetComponent("Com_Model"));
	//XMStoreFloat4x4(&m_matRightBone, XMMatrixIdentity());
	return S_OK;
}

void CGoldAxe::Update(_double deltaTime)
{

	//XMStoreFloat4x4(&m_matRightBone, m_pModel->Get_TransformationMatrix("AN_PC_WP_TAX1013_A00_BC") * m_pTargetModel->Get_BoneWithoutOffset("BN_Weapon_R"));
	////XMStoreFloat4x4(&m_matLeftBone, /*m_pTargetModel->Get_BoneWithoutOffset("BN_WP_L_02_Off") 
	////				**/ m_pTargetModel->Get_BoneWithoutOffset("BN_Weapon_L"));
	///*_matrix Mat = XMLoadFloat4x4(&m_matRightBone) * XMLoadFloat4x4(&m_matLeftBone) * XMLoadFloat4x4(&m_pTargetTransform->GetMatrix());*/
	//m_pTransform->SetMatrix(XMLoadFloat4x4(&m_matRightBone) * XMLoadFloat4x4(&m_pTargetTransform->GetMatrix()));
	////m_pTransform->SetMatrix(XMLoadFloat4x4(&m_matRightBone)/* * XMLoadFloat4x4(&m_matLeftBone) 
	////						*/* XMLoadFloat4x4(&m_pTargetTransform->GetMatrix()));
}

void CGoldAxe::LateUpdate(_double deltaTime)
{
	//m_pModel->Play_Animation(deltaTime);
}

void CGoldAxe::Render()
{
}

CGoldAxe * CGoldAxe::Create(CGameObject * pObj)
{
	CGoldAxe*		pInstance = new CGoldAxe(pObj);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed to Create CGoldAxe");
		SafeRelease(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CGoldAxe::Free()
{
}
