#include "stdafx.h"
#include "Client_Struct.h"
#include "..\Public\InstanceEffectFire.h"
#include "EmptyEffect.h"

USING(Client)

CInstanceEffectFire::CInstanceEffectFire()
{
}

CInstanceEffectFire * CInstanceEffectFire::Create(void * pArg)
{
	CInstanceEffectFire*		pInstance = new CInstanceEffectFire();

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Create CInstanceEffectFire");
		SafeRelease(pInstance);
		return nullptr;
	}

	return pInstance;
}


HRESULT CInstanceEffectFire::Initialize(void* pArg)
{
	if (pArg != nullptr) 
	{
		m_pGameObject = (CGameObject*)pArg;
		if (m_pGameObject == nullptr)
			return E_FAIL;

		/*CGameObject* pTargetObj = CEngine::GetInstance()->FindGameObjectWithName(CEngine::GetInstance()->GetCurSceneNumber(), "Flogas");
		if (pTargetObj == nullptr)
			return E_FAIL;*/

		/*static_cast<CEmptyEffect*>(m_pGameObject)->SetFadeOutEnable(true);
		static_cast<CEmptyEffect*>(m_pGameObject)->SetFadeInEnable(true);
		static_cast<CEmptyEffect*>(m_pGameObject)->SetEffectDuration(0.5f);
		static_cast<CEmptyEffect*>(m_pGameObject)->SetFadeInStartTime(0.f);
		static_cast<CEmptyEffect*>(m_pGameObject)->SetFadeInDuration(0.2f);
		static_cast<CEmptyEffect*>(m_pGameObject)->SetFadeOutStartTime(0.3f);
		static_cast<CEmptyEffect*>(m_pGameObject)->SetFadeOutDuration(0.2f);*/

		//CTransform* pTargetTransform = static_cast<CTransform*>(pTargetObj->GetComponent("Com_Transform"));
	}
	return S_OK;
}

void CInstanceEffectFire::Update(_double deltaTime)
{
	if (m_bDead)
		return;
	
	//m_DeadTimeAcc += deltaTime;

	if (!m_pGameObject)
		return;
}

void CInstanceEffectFire::LateUpdate(_double deltaTime)
{
	/*if (m_DeadTimeAcc > 0.5)
		m_bDead = true;
	
	if (m_bDead)
	{
		this->SetDead();
		m_pGameObject->SetDead();
	}*/
}

void CInstanceEffectFire::Render()
{
}

void CInstanceEffectFire::SetGameObject(CGameObject* obj)
{
	m_pGameObject = obj;
}
void CInstanceEffectFire::SetTransform(CTransform * trans)
{
	m_pTransform = trans;
}

void CInstanceEffectFire::SetFadeInOut(CGameObject * pObj)
{
	static_cast<CEmptyEffect*>(pObj)->SetFadeInEnable(true);
	static_cast<CEmptyEffect*>(pObj)->SetFadeOutEnable(true);
	static_cast<CEmptyEffect*>(pObj)->SetFadeInStartTime(0.f);
	static_cast<CEmptyEffect*>(pObj)->SetFadeInDuration(1.f);
	static_cast<CEmptyEffect*>(pObj)->SetFadeOutStartTime(2.f);
	static_cast<CEmptyEffect*>(pObj)->SetFadeOutDuration(1.f);
	static_cast<CEmptyEffect*>(pObj)->SetEffectDuration(3.f);
}

void CInstanceEffectFire::Free()
{
	__super::Free();
}
