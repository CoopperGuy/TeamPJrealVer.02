#include "stdafx.h"
#include "Client_Struct.h"
#include "..\Public\EffectGrab.h"
#include "EmptyEffect.h"

USING(Client)

CEffectGrab::CEffectGrab()
{
}

CEffectGrab * CEffectGrab::Create(void * pArg)
{
	CEffectGrab*		pInstance = new CEffectGrab();

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Create CEffectGrab");
		SafeRelease(pInstance);
		return nullptr;
	}

	return pInstance;
}


HRESULT CEffectGrab::Initialize(void* pArg)
{
	if (pArg != nullptr) {

		m_pGameObject = (CGameObject*)pArg;
		if (m_pGameObject == nullptr)
			return E_FAIL;
		m_pTransform = static_cast<CTransform*>(m_pGameObject->GetComponent("Com_Transform"));


		//flogas
		pTargetObj = CEngine::GetInstance()->FindGameObjectWithName(CEngine::GetInstance()->GetCurSceneNumber(), "Flogas");
		if (pTargetObj == nullptr)
			return E_FAIL;

		CTransform* pTargetTransform = static_cast<CTransform*>(pTargetObj->GetComponent("Com_Transform"));
		CModel* pTargetmodel = static_cast<CModel*>(pTargetObj->GetComponent("Com_Model"));

		_matrix targetbone = pTargetmodel->Get_BoneWithoutOffset("Bip01-L-Finger2");
		m_pTransform->SetMatrix(targetbone *pTargetTransform->GetWorldMatrix());

		m_fScale = { m_pTransform->GetScale(CTransform::STATE_RIGHT) , m_pTransform->GetScale(CTransform::STATE_UP) , m_pTransform->GetScale(CTransform::STATE_LOOK) };
	}
	return S_OK;
}

void CEffectGrab::Update(_double deltaTime)
{
	if (m_bDead)
		return;

	if (!m_pGameObject)
		return;

	m_fScale.y += plusscale;

	if(m_bSetFadeOut)
		m_pTransform->SetScale(m_fScale);

	if (m_fScale.y > m_fMaxScail)
	{
		m_bDead = true;
		/*if (m_bSetFadeOut)
		{
			SetFadeInOut(m_pGameObject);
			m_bSetFadeOut = false;
		}*/
	}

	//if (static_cast<CEmptyEffect*>(m_pGameObject)->GetFadeOutEnable()) {
	//	if (0.2 >= static_cast<CEmptyEffect*>(m_pGameObject)->GetFadeAlpha())
	//	{
	//		m_bDead = true;
	//	}
	//}

}

void CEffectGrab::LateUpdate(_double deltaTime)
{
	if (m_bDead)
	{
		this->SetDead();
		m_pGameObject->SetDead();
	}
}

void CEffectGrab::Render()
{
}

void CEffectGrab::SetFadeInOut(CGameObject * pObj)
{
	static_cast<CEmptyEffect*>(pObj)->SetFadeOutEnable(true);
	static_cast<CEmptyEffect*>(pObj)->SetFadeOutStartTime(0.f);
	static_cast<CEmptyEffect*>(pObj)->SetFadeOutDuration(0.5f);
	static_cast<CEmptyEffect*>(pObj)->SetEffectDuration(0.5f);
}

void CEffectGrab::Free()
{
	__super::Free();
}
