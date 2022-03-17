#include "stdafx.h"
#include "Client_Struct.h"
#include "..\Public\EffectBlood.h"

USING(Client)

CEffectBlood::CEffectBlood()
{
}

CEffectBlood * CEffectBlood::Create(void * pArg)
{
	CEffectBlood*		pInstance = new CEffectBlood();

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Create CEffectBlood");
		SafeRelease(pInstance);
		return nullptr;
	}

	return pInstance;
}


HRESULT CEffectBlood::Initialize(void* pArg)
{
	if (pArg != nullptr) {
		m_pGameObject = CEngine::GetInstance()->FindGameObjectWithName(CEngine::GetInstance()->GetCurSceneNumber(), "Blood");
		if (m_pGameObject == nullptr)
			return E_FAIL;

		m_pTransform = static_cast<CTransform*>(m_pGameObject->GetComponent("Com_Transform"));

		_vector pos = { 0.f,0.f,0.f };
		memcpy(&pos, pArg, sizeof(_vector));
		m_pTransform->SetState(CTransform::STATE_POSITION, pos);
	}
	return S_OK;
}

void CEffectBlood::Update(_double deltaTime)
{
	if (m_bDead)
		return;

	if (m_pGameObject)
	{
	//	_bool spriteend = static_cast<CEmptyEffect*>(m_pGameObject)->GetSpriteEnd();
	/*	if (spriteend)
		{
			m_bDead = true;
		}*/
	}

}

void CEffectBlood::LateUpdate(_double deltaTime)
{
	if (m_bDead)
	{
		m_pGameObject->SetDead();
		//이건잠시 막아둠 
		/*__super::SetDead();*/
	}
}

void CEffectBlood::Render()
{
}


void CEffectBlood::Free()
{
	__super::Free();
}
