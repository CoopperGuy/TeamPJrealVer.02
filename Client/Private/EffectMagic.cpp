#include "stdafx.h"
#include "Client_Struct.h"
#include "..\Public\EffectMagic.h"
#include "EmptyEffect.h"
#include "EffectMagicAf.h"
#include "MeteoFireBall.h"
USING(Client)

CEffectMagic::CEffectMagic()
{
}

CEffectMagic * CEffectMagic::Create(void * pArg, _vector pos)
{
	CEffectMagic*		pInstance = new CEffectMagic();

	if (FAILED(pInstance->Initialize(pArg, pos)))
	{
		MSG_BOX("Failed to Create CEffectMagic");
		SafeRelease(pInstance);
		return nullptr;
	}

	return pInstance;
}


HRESULT CEffectMagic::Initialize(void* pArg, _vector pos)
{
	if (pArg != nullptr) {

		m_pGameObject = (CGameObject*)pArg;
		if (m_pGameObject == nullptr)
			return E_FAIL;

		m_pTransform = static_cast<CTransform*>(m_pGameObject->GetComponent("Com_Transform"));
		//pos = XMVectorSetY(pos, 0.1f);

		_int  startrand = rand() % 2;
		if (0 == startrand)
			m_pTransform->SetState(CTransform::STATE_POSITION, _vector{ float(rand() % 4) ,0.1f , XMVectorGetZ(pos) - float(rand() % 5) });
		else
			m_pTransform->SetState(CTransform::STATE_POSITION, _vector{ float(rand() % 4) * -1 ,0.1f , XMVectorGetZ(pos) + float(rand() % 5) });
	}
	return S_OK;
}

void CEffectMagic::Update(_double deltaTime)
{
	if (m_bDead)
		return;

	if (!m_pGameObject)
		return;
	mypos = m_pTransform->GetState(CTransform::STATE_POSITION);
	m_dDeadTime += deltaTime;
	m_makefb += deltaTime;
	m_makedt += deltaTime;


	if (!makemeteo && m_makefb >= static_cast<CEmptyEffect*>(m_pGameObject)->GetFadeOutStartTime()) {
		makemeteo = true;
		m_makefb = 0;
	}

	if (makemeteo) {
		if (m_makedt >= 0.095) {
			_vector mypos = m_pTransform->GetState(CTransform::STATE_POSITION);

			auto Meteo = CEngine::GetInstance()->AddGameObjectToPrefab(0, "Prototype_GameObecjt_MeteoOBB", "O_MeteoOBB");
			CEngine::GetInstance()->AddScriptObject(CMeteoFireBall::Create(Meteo, mypos), CEngine::GetInstance()->GetCurSceneNumber());
			m_makedt = 0;
			makemeteo = false;
		}
	}


	if (m_dDeadTime >= static_cast<CEmptyEffect*>(m_pGameObject)->GetEffectDuration())
		m_bDead = true;
}


void CEffectMagic::LateUpdate(_double deltaTime)
{
	if (m_bDead)
	{



		this->SetDead();
		m_pGameObject->SetDead();
	}
}

void CEffectMagic::Render()
{
}


void CEffectMagic::Free()
{
	__super::Free();
}
