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


	//	_matrix posMatrix = {};
	//	//_matrix targetbone = pTargetmodel->Get_TransformationMatrix("Bip01-L-Calf");
	////뼈못찾는듯 다른뼈로 해보장
	//	memcpy(&posMatrix, &targetbone, sizeof(XMMATRIX));
	//	m_pTransform->SetMatrix(posMatrix);
	}
	return S_OK;
}

void CEffectDust::Update(_double deltaTime)
{
	if (m_bDead)
		return;

	if (!m_pGameObject)
		return;
}

void CEffectDust::LateUpdate(_double deltaTime)
{

	
	if (m_bAnimationEnd)
	{
		SetFadeInOut(m_pGameObject);
		m_bSet = true;
		if (m_bSet)
		{
			FadeInStartTime += deltaTime;
			if (FadeInStartTime > static_cast<CEmptyEffect*>(m_pGameObject)->GetFadeOutDuration())
			{
				m_bDead = true;
				m_bSet = false;
				m_bAnimationEnd = false;
			}
		}

	}


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
