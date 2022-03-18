#include "stdafx.h"
#include "..\Public\FlogasDunDoor.h"
#include "PortalUI.h"
USING(Client)

CFlogasDunDoor::CFlogasDunDoor()
{
}

HRESULT CFlogasDunDoor::Initailze(CGameObject * pArg)
{
	m_pFlogasDunDoor = static_cast<CEmptyGameObject*>(CEngine::GetInstance()->FindGameObjectWithName(CEngine::GetInstance()->GetCurSceneNumber(), "FlogasDunDoor"));
	m_pPlayer = static_cast<CEmptyGameObject*>(CEngine::GetInstance()->FindGameObjectWithName(SCENE_STATIC, "Player"));
	m_pTransform = static_cast<CTransform*>(m_pFlogasDunDoor->GetComponent("Com_Transform"));
	if (m_pFlogasDunDoor == nullptr || m_pPlayer == nullptr)
		return E_FAIL;
	_vector pFlogasDunDoor = m_pTransform->GetState(CTransform::STATE_POSITION);
	return S_OK;
}

void CFlogasDunDoor::Update(_double deltaTime)
{
	CTransform* playerTrans = static_cast<CTransform*>(m_pPlayer->GetComponent("Com_Transform"));

	_vector playerPos = playerTrans->GetState(CTransform::STATE_POSITION);
	_float dist = XMVectorGetZ(XMVector3Length(pFlogasDunDoor - playerPos));

	cout << dist << endl;

	//if (dist < 15.f) {
	//	if (MaxHight>XMVectorGetY(pFlogasDunDoor)) {
	//		m_pTransform->SetState(CTransform::STATE_POSITION, _vector{ XMVectorGetX(pFlogasDunDoor),XMVectorGetY(pFlogasDunDoor) + float(deltaTime),XMVectorGetZ(pFlogasDunDoor) });
	//	}
	//	else
	//	{
	//		m_pTransform->SetState(CTransform::STATE_POSITION, _vector{ XMVectorGetX(pFlogasDunDoor),XMVectorGetY(pFlogasDunDoor) - float(deltaTime),XMVectorGetZ(pFlogasDunDoor) });
	//	}
	//}
}

void CFlogasDunDoor::LateUpdate(_double deltaTime)
{
}


CFlogasDunDoor * CFlogasDunDoor::Create(CGameObject * pTarget)
{
	CFlogasDunDoor*	obj = new CFlogasDunDoor();
	if (FAILED(obj->Initailze(pTarget))) {
		SAFE_RELEASE(obj);
		return nullptr;
	}
	return obj;
}

void CFlogasDunDoor::Free()
{
}
