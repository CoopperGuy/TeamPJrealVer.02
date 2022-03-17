#include "stdafx.h"
#include "..\Public\Haribo.h"

USING(Tool)

CHaribo::CHaribo(CGameObject* pObj)
	: CToolCharacter(pObj)
{
}


CHaribo * CHaribo::Create(CGameObject * pObj)
{
	CHaribo*		pInstance = new CHaribo(pObj);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed to Create CHaribo");
		SafeRelease(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CHaribo::Free()
{
	__super::Free();
}

void CHaribo::Set_Attach(_fmatrix WorldMatrix)
{
	//_matrix myMatrix = m_pTransform->GetWorldMatrix();
	//XMStoreFloat4x4(&_matrix, Remove_Scale(WorldMatrix));
}

_fmatrix CHaribo::Remove_ScaleRotation(_fmatrix TransformMatrix)
{
	return _fmatrix();
}

_fmatrix CHaribo::Remove_OnlyScale(_fmatrix TransformMatrix)
{
	return _fmatrix();
}

_fmatrix CHaribo::Remove_Scale(_fmatrix TransformMatrix)
{
	return _fmatrix();
}

HRESULT CHaribo::Initialize()
{

	m_pGameObject = CEngine::GetInstance()->FindGameObjectWithName(CEngine::GetInstance()->GetCurSceneNumber(), "Haribo");
	if (nullptr == m_pGameObject)
		return E_NOINTERFACE;

	m_pTransform = dynamic_cast<CTransform*>(m_pGameObject->GetComponent("Com_Transform"));
	m_pModel = dynamic_cast<CModel*>(m_pGameObject->GetComponent("Com_Model"));


	return S_OK;
}

void CHaribo::Update(_double dDeltaTime)
{
	if (!m_pGameObject)
		return;

	auto PlayerOject = CEngine::GetInstance()->FindGameObjectWithName(CEngine::GetInstance()->GetCurSceneNumber(), "Player");
	if (nullptr == m_pGameObject)
		return ;

	CTransform* m_pTargetTransform = dynamic_cast<CTransform*>(PlayerOject->GetComponent("Com_Transform"));
	CModel* m_pTargetModel = dynamic_cast<CModel*>(PlayerOject->GetComponent("Com_Model"));


	_matrix		ColliderMatrix;
	_matrix		BoneMatrix;
	_matrix		TransMatrix;
	_matrix		RotationMatrix;

	TransMatrix = XMMatrixTranslation(0.f, 0.f, 0.f);
	BoneMatrix = m_pTargetModel->Get_BoneMatrix("Hand_1_R");
	//m_pTransform->Set_Attach(TransMatrix /*RotationMatrix*/* BoneMatrix * m_pTargetTransform->GetWorldMatrix()); //첫번째 오프셋 

}


void CHaribo::LateUpdate(_double dDeltaTime)
{

}

void CHaribo::Render()
{
}

