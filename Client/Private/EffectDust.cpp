#include "stdafx.h"
#include "Client_Struct.h"
#include "..\Public\EffectDust.h"
#include "EmptyEffect.h"

USING(Client)

CEffectDust::CEffectDust()
{
}

CEffectDust * CEffectDust::Create(void * pArg)
{
	CEffectDust*		pInstance = new CEffectDust();

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Create CEffectDust");
		SafeRelease(pInstance);
		return nullptr;
	}

	return pInstance;
}


HRESULT CEffectDust::Initialize(void* pArg)
{
	if (pArg != nullptr) {


		m_pGameObject = (CGameObject*)pArg;
		if (m_pGameObject == nullptr)
			return E_FAIL;

		CGameObject* pTargetObj = CEngine::GetInstance()->FindGameObjectWithName(CEngine::GetInstance()->GetCurSceneNumber(), "Flogas");
		if (pTargetObj == nullptr)
			return E_FAIL;

		CModel* pTargetmodel = static_cast<CModel*>(m_pGameObject->GetComponent("Com_Model"));

		m_pTransform = static_cast<CTransform*>(m_pGameObject->GetComponent("Com_Transform"));

		m_pTransform->SetState(CTransform::STATE_POSITION, _vector{ 0.f,0.f,0.f });
	}
	return S_OK;
}

void CEffectDust::Update(_double deltaTime)
{
	if (m_bDead)
		return;

	if (!m_pGameObject)
		return;

	m_fFrame += 14.f*deltaTime;

	if (m_fFrame >= 14)
		m_bDead = true;
}

void CEffectDust::LateUpdate(_double deltaTime)
{


	if (m_bDead)
	{
		this->SetDead();
		m_pGameObject->SetDead();
	}
}

void CEffectDust::Render()
{
}

void CEffectDust::SetGameObject(CGameObject* obj)
{
	m_pGameObject = obj;
}
void CEffectDust::SetTransform(CTransform * trans)
{
	m_pTransform = trans;
}

void CEffectDust::SetFadeInOut(CGameObject * pObj)
{
	static_cast<CEmptyEffect*>(pObj)->SetFadeOutEnable(true);
	static_cast<CEmptyEffect*>(pObj)->SetFadeOutStartTime(0.f);
	static_cast<CEmptyEffect*>(pObj)->SetFadeOutDuration(1.5f);
	static_cast<CEmptyEffect*>(pObj)->SetEffectDuration(1.5f);
}

void CEffectDust::Free()
{
	__super::Free();
}
