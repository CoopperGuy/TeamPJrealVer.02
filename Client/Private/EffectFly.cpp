#include "stdafx.h"
#include "Client_Struct.h"
#include "..\Public\EffectFly.h"
#include "EmptyEffect.h"

USING(Client)

CEffectFly::CEffectFly()
{
}

CEffectFly * CEffectFly::Create(void * pArg)
{
	CEffectFly*		pInstance = new CEffectFly();

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Create CEffectFly");
		SafeRelease(pInstance);
		return nullptr;
	}

	return pInstance;
}


HRESULT CEffectFly::Initialize(void* pArg)
{
	if (pArg != nullptr) {

		m_pGameObject = (CGameObject*)pArg;
		if (m_pGameObject == nullptr)
			return E_FAIL;

		CGameObject* pTargetObj = CEngine::GetInstance()->FindGameObjectWithName(CEngine::GetInstance()->GetCurSceneNumber(), "Flogas");
		if (pTargetObj == nullptr)
			return E_FAIL;

		CTransform* pTargetTransform = static_cast<CTransform*>(pTargetObj->GetComponent("Com_Transform"));


		m_pTransform = static_cast<CTransform*>(m_pGameObject->GetComponent("Com_Transform"));

		_vector pos = {};
		memcpy(&pos, &pTargetTransform->GetState(CTransform::STATE_POSITION), sizeof(_vector));
		//pos = XMVectorSetY(pos, XMVectorGetY(pos) + 3.f);
		m_pTransform->SetState(CTransform::STATE_POSITION, pos);

		_float posx = XMVectorGetX(pTargetTransform->GetState(CTransform::STATE_POSITION));
		_float posz = XMVectorGetZ(pTargetTransform->GetState(CTransform::STATE_POSITION));

		static_cast<CEmptyEffect*>(m_pGameObject)->SetFadeInEnable(false);

	}
	return S_OK;
}

void CEffectFly::Update(_double deltaTime)
{
	if (m_bDead)
		return;

	if (!m_pGameObject)
		return;

	if (m_bAnimationEnd) {
		if (m_bSetFadeInOut)
		{
			list<CGameObject*> childList = m_pGameObject->GetChildren();

			SetFadeInOut(m_pGameObject);

			//for (auto child : childList)
			//{
			//	SetFadeInOut(child);
			//}
			m_bSetFadeInOut = false;

		}
		if (!m_bSetFadeInOut)
			DeadTime += deltaTime;

		if (DeadTime > static_cast<CEmptyEffect*>(m_pGameObject)->GetFadeOutDuration())
		{
			m_bDead = true;
			m_bAnimationEnd = false;
		}
	}
	else
		static_cast<CEmptyEffect*>(m_pGameObject)->SetFadeInEnable(false);

}

void CEffectFly::LateUpdate(_double deltaTime)
{

	if (m_bDead)
	{
		this->SetDead();
		m_pGameObject->SetDead();
	}
}

void CEffectFly::Render()
{
}

void CEffectFly::SetGameObject(CGameObject* obj)
{
	m_pGameObject = obj;
}
void CEffectFly::SetTransform(CTransform * trans)
{
	m_pTransform = trans;
}

void CEffectFly::SetFadeInOut(CGameObject * pObj)
{
	static_cast<CEmptyEffect*>(pObj)->SetFadeOutEnable(true);
	static_cast<CEmptyEffect*>(pObj)->SetFadeOutStartTime(0.f);
	static_cast<CEmptyEffect*>(pObj)->SetFadeOutDuration(2.f);
	static_cast<CEmptyEffect*>(pObj)->SetEffectDuration(2.f);
}

void CEffectFly::Free()
{
	__super::Free();
}