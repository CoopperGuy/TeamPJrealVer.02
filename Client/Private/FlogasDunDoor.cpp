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
	pFlogasDunDoor = m_pTransform->GetState(CTransform::STATE_POSITION);

	m_pCollider = static_cast<CCollider*>(m_pFlogasDunDoor->GetComponent("Com_Collider"));

	m_pAlretUI = static_cast<CEmptyGameObject*>(CEngine::GetInstance()->SpawnPrefab("O_NPC_F"));
	_vector pos = XMVector3TransformCoord(XMVectorSet(0.f, 0.5f, -0.3f, 0.f), m_pTransform->GetWorldMatrix());
	_float3 float3Pos;
	XMStoreFloat3(&float3Pos, pos);
	m_pAlretUI->SetPosition(float3Pos);

	doorY = XMVectorGetY(pFlogasDunDoor);

	return S_OK;
}

void CFlogasDunDoor::Update(_double deltaTime)
{

	if (m_bDead)
		return;

	CTransform* playerTrans = static_cast<CTransform*>(m_pPlayer->GetComponent("Com_Transform"));
	pFlogasDunDoor = m_pTransform->GetState(CTransform::STATE_POSITION);

	_vector playerPos = playerTrans->GetState(CTransform::STATE_POSITION);
	_float dist = XMVectorGetZ(XMVector3Length(pFlogasDunDoor - playerPos));

	if (dist < 3.f && XMVectorGetZ(playerPos) <= -11.f) {
		if (m_pAlretUI) {
			m_pAlretUI->SetActive(true);
		}
		if (CEngine::GetInstance()->IsKeyDown('F')) {
			if (!m_bOpenDoor)
				m_bOpenDoor = true;
		}
		if (m_bOpenDoor && MaxHight >= XMVectorGetY(pFlogasDunDoor)) {
			doorY += 0.2f * deltaTime;
			m_pTransform->SetState(CTransform::STATE_POSITION, _vector{ XMVectorGetX(pFlogasDunDoor),doorY,XMVectorGetZ(pFlogasDunDoor) });

		}

	}
	else {
		if (m_pAlretUI) {
			m_pAlretUI->SetActive(false);
			m_bOpenDoor = false;
		}
		if (!m_bOpenDoor && MinHight <= XMVectorGetY(pFlogasDunDoor)) {
			m_pAlretUI->SetActive(false);
			doorY -= 0.2f * deltaTime;
			m_pTransform->SetState(CTransform::STATE_POSITION, _vector{ XMVectorGetX(pFlogasDunDoor),doorY,XMVectorGetZ(pFlogasDunDoor) });
		}
	}
	_float3 pos;
	XMStoreFloat3(&pos, pFlogasDunDoor);
	m_pCollider->SetRelativePos(pos);

}

void CFlogasDunDoor::LateUpdate(_double deltaTime)
{
	if (m_bDead)
	{
		this->SetDead();
		m_pFlogasDunDoor->SetDead();
	}
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
