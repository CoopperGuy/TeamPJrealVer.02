#include "stdafx.h"
#include "Client_Struct.h"
#include "..\Public\EffectFlyLaser.h"
#include "EmptyEffect.h"

USING(Client)

CEffectFlyLaser::CEffectFlyLaser()
{
}

CEffectFlyLaser * CEffectFlyLaser::Create(void * pArg)
{
	CEffectFlyLaser*		pInstance = new CEffectFlyLaser();

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Create CEffectFlyLaser");
		SafeRelease(pInstance);
		return nullptr;
	}

	return pInstance;
}


HRESULT CEffectFlyLaser::Initialize(void* pArg)
{
	if (pArg != nullptr) {

		m_pGameObject = (CGameObject*)pArg;
		if (m_pGameObject == nullptr)
			return E_FAIL;

		CGameObject* pTargetObj = CEngine::GetInstance()->FindGameObjectWithName(CEngine::GetInstance()->GetCurSceneNumber(), "Flogas");
		if (pTargetObj == nullptr)
			return E_FAIL;

		CTransform* pTargetTransform = static_cast<CTransform*>(pTargetObj->GetComponent("Com_Transform"));

		TargetPos = pTargetTransform->GetState(CTransform::STATE_UP);

		m_pTransform = static_cast<CTransform*>(m_pGameObject->GetComponent("Com_Transform"));

		_vector pos = pTargetTransform->GetState(CTransform::STATE_POSITION);
		pos = XMVectorSetY(pos, XMVectorGetY(pos) - 0.9f);
		pos = XMVectorSetZ(pos, XMVectorGetZ(pos) + 3.5f);
		m_pTransform->SetState(CTransform::STATE_POSITION, pos);
		
		list<CGameObject*> childList = m_pGameObject->GetChildren();

		SetFadeInOut(m_pGameObject);

		for (auto child : childList)
		{
			SetFadeInOut(child);
		}

		m_fScale = { m_pTransform->GetScale(CTransform::STATE_RIGHT) , m_pTransform->GetScale(CTransform::STATE_UP) , m_pTransform->GetScale(CTransform::STATE_LOOK) };
	}
	return S_OK;
}

void CEffectFlyLaser::Update(_double deltaTime)
{
	if (m_bDead)
		return;

	if (!m_pGameObject)
		return;
}

void CEffectFlyLaser::LateUpdate(_double deltaTime)
{

	if (m_bDead)
	{
		this->SetDead();
		m_pGameObject->SetDead();
	}
}

void CEffectFlyLaser::Render()
{
}

void CEffectFlyLaser::SetGameObject(CGameObject* obj)
{
	m_pGameObject = obj;
}
void CEffectFlyLaser::SetTransform(CTransform * trans)
{
	m_pTransform = trans;
}

void CEffectFlyLaser::SetFadeInOut(CGameObject * pObj)
{
	static_cast<CEmptyEffect*>(pObj)->SetFadeInEnable(true);
	static_cast<CEmptyEffect*>(pObj)->SetFadeOutEnable(true);
	static_cast<CEmptyEffect*>(pObj)->SetFadeInStartTime(0.f);
	static_cast<CEmptyEffect*>(pObj)->SetFadeInDuration(1.f);
	static_cast<CEmptyEffect*>(pObj)->SetFadeOutStartTime(2.f);
	static_cast<CEmptyEffect*>(pObj)->SetFadeOutDuration(1.f);
	static_cast<CEmptyEffect*>(pObj)->SetEffectDuration(3.f);
}

void CEffectFlyLaser::Free()
{
	__super::Free();
}
