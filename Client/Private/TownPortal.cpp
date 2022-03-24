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
	DistanceWithDirection();
}

void CTownPortal::LateUpdate(_double deltaTime)
{
}

_float CTownPortal::DistanceWithDirection()
{
	CTransform* playerTrans = static_cast<CTransform*>(m_pPlayer->GetComponent("Com_Transform"));

	_vector playerLook = playerTrans->GetState(CTransform::STATE_LOOK);
	_vector playerPos = playerTrans->GetState(CTransform::STATE_POSITION);
	_vector portalPos = static_cast<CTransform*>(m_pPortal->GetComponent("Com_Transform"))->GetState(CTransform::STATE_POSITION);
	_vector dir = XMVectorSetY(XMVector3Normalize(playerPos - portalPos), 0.f);
	_float dist = XMVectorGetX(XMVector3Length(portalPos - playerPos));

	playerLook = XMVector3Normalize(XMVectorSetY(playerLook, 0.f));

	if (dist < 0.6f) {
		_float degree = XMConvertToDegrees(XMVectorGetX(XMVector3Dot(dir, playerLook)));
		if (CEngine::GetInstance()->Get_DIKDown(DIK_F)) {
			CEventCheck::GetInstance()->SetChangeScene(true);
			CEventCheck::GetInstance()->SetSceneNumber(SCENE::SCENE_STAGE1);
			static_cast<CCollider*>(m_pPlayer->GetComponent("Com_Collider"))->SetPosition(_float3(0.f, 5.f, 27.f));
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
