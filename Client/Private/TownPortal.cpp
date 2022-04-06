#include "stdafx.h"
#include "..\Public\TownPortal.h"
#include "EventCheck.h"
USING(Client)

CTownPortal::CTownPortal()
{
}

HRESULT CTownPortal::Initailze(CGameObject * pArg)
{
	m_pPlayer = static_cast<CEmptyGameObject*>(CEngine::GetInstance()->FindGameObjectWithName(SCENE_STATIC, "Player"));
	m_pPortal = static_cast<CEmptyGameObject*>(CEngine::GetInstance()->FindGameObjectWithName(CEngine::GetInstance()->GetCurSceneNumber(), "TownPortal"));

	CEngine::GetInstance()->AddScriptObject(this, CEngine::GetInstance()->GetCurSceneNumber());
	m_pPortal->SetActive(true);
	return S_OK;
}

void CTownPortal::Update(_double deltaTime)
{
	if(m_pPortal->IsActive())
		DistanceWithDirection();
}

void CTownPortal::LateUpdate(_double deltaTime)
{
}

_float CTownPortal::DistanceWithDirection()
{
	CTransform* playerTrans = static_cast<CTransform*>(m_pPlayer->GetComponent("Com_Transform"));

	_vector playerLook = playerTrans->GetState(CTransform::STATE_LOOK);
	_vector playerPos = XMVectorSetY(playerTrans->GetState(CTransform::STATE_POSITION), 0.f);
	_vector portalPos = XMVectorSetY(static_cast<CTransform*>(m_pPortal->GetComponent("Com_Transform"))->GetState(CTransform::STATE_POSITION), 0.f);
	_vector dir = XMVector3Normalize(playerPos - portalPos);
	_float dist = XMVectorGetX(XMVector3Length(portalPos - playerPos));

	if (dist < 0.6f) {
		if (CEngine::GetInstance()->Get_DIKDown(DIK_F)) {
			CEventCheck::GetInstance()->SetChangeScene(true);
			CEventCheck::GetInstance()->SetSceneNumber(SCENE::SCENE_STAGE1);
			static_cast<CCollider*>(m_pPlayer->GetComponent("Com_Collider"))->SetPosition(_float3(0.f, 1.f, 30.f));
		}
	}

	return _float();
}

CTownPortal * CTownPortal::Create(CGameObject * pTarget)
{
	CTownPortal*	obj = new CTownPortal();
	if (FAILED(obj->Initailze(pTarget))) {
		SAFE_RELEASE(obj);
		return nullptr;
	}
	return obj;
}

void CTownPortal::Free()
{
}
