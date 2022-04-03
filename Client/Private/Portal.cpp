#include "stdafx.h"
#include "..\Public\Portal.h"
#include "PortalUI.h"
USING(Client)

CPortal::CPortal()
{
}

HRESULT CPortal::Initailze(CGameObject * pArg)
{
	m_pPlayer = static_cast<CEmptyGameObject*>(CEngine::GetInstance()->FindGameObjectWithName(SCENE_STATIC, "Player"));
	m_pPortal = static_cast<CEmptyGameObject*>(CEngine::GetInstance()->FindGameObjectWithName(CEngine::GetInstance()->GetCurSceneNumber(), "Portal"));

	m_pPortalUI = CPortalUI::Create(nullptr);
	CEngine::GetInstance()->AddScriptObject(m_pPortalUI, CEngine::GetInstance()->GetCurSceneNumber());
	return S_OK;
}

void CPortal::Update(_double deltaTime)
{
	DistanceWithDirection();
}

void CPortal::LateUpdate(_double deltaTime)
{
}

_float CPortal::DistanceWithDirection()
{
	CTransform* playerTrans = static_cast<CTransform*>(m_pPlayer->GetComponent("Com_Transform"));

	_vector playerLook = playerTrans->GetState(CTransform::STATE_LOOK);
	_vector playerPos = XMVectorSetY(playerTrans->GetState(CTransform::STATE_POSITION), 0.f);
	_vector portalPos = XMVectorSetY(static_cast<CTransform*>(m_pPortal->GetComponent("Com_Transform"))->GetState(CTransform::STATE_POSITION), 0.f);
	_vector dir = XMVector3Normalize(playerPos - portalPos);
	_float dist = XMVectorGetX(XMVector3Length(portalPos - playerPos));

	playerLook = XMVector3Normalize(XMVectorSetY(playerLook, 0.f));

	if (dist < 0.5f)
	{

		if (CEngine::GetInstance()->Get_DIKDown(DIK_F))
		{
			CEngine::GetInstance()->PlaySoundW("PortalOpen.ogg", CHANNELID::UI10);
			m_pPortalUI->SetIsActive(true);
			g_AnotherMenu = true;
			/*	m_eSceneNum = SCENE::SCENE_STAGE2;
				m_bChangeScene = true;
				static_cast<CCollider*>(m_pPlayer->GetComponent("Com_Collider"))->SetPosition(_float3(0.f, 2.f, -10.f));*/
		}

	}
	if (m_pPortalUI) {
		CPortalUI::SCENES eScene = m_pPortalUI->GetSCENES();
		switch (eScene)
		{
		case Client::CPortalUI::STAGE01:
			break;
		case Client::CPortalUI::STAGE02:
			m_eSceneNum = SCENE_STAGE2;
			m_bChangeScene = true;
			static_cast<CCollider*>(m_pPlayer->GetComponent("Com_Collider"))->SetPosition(_float3(0.f, 0.f, -18.f));
			m_pPortalUI->SetIsActive(false);
			m_pPortalUI->SetSCENES(CPortalUI::SCENES_END);
			g_AnotherMenu = false;
			break;
		case Client::CPortalUI::STAGE03:
			m_eSceneNum = SCENE_STAGE3;
			m_bChangeScene = true;
			static_cast<CCollider*>(m_pPlayer->GetComponent("Com_Collider"))->SetPosition(_float3(0.f, 0.f, 0.f));
			m_pPortalUI->SetIsActive(false);
			m_pPortalUI->SetSCENES(CPortalUI::SCENES_END);
			g_AnotherMenu = false;
			break;
		}
	}

	return _float();
}


CPortal * CPortal::Create(CGameObject * pTarget)
{
	CPortal*	obj = new CPortal();
	if (FAILED(obj->Initailze(pTarget))) {
		SAFE_RELEASE(obj);
		return nullptr;
	}
	return obj;
}

void CPortal::Free()
{
}
