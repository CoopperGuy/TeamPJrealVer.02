#include "stdafx.h"
#include "Client_Struct.h"
#include "..\Public\EffectSoilDust.h"
#include "EffectUrsaDust.h"
#include "EffectSoilDecal.h"

USING(Client)

CEffectSoilDust::CEffectSoilDust()
{
}

CEffectSoilDust * CEffectSoilDust::Create(void * pArg, _matrix pos)
{
	CEffectSoilDust*		pInstance = new CEffectSoilDust();

	if (FAILED(pInstance->Initialize(pArg, pos)))
	{
		MSG_BOX("Failed to Create CEffectSoilDust");
		SafeRelease(pInstance);
		return nullptr;
	}

	return pInstance;
}


HRESULT CEffectSoilDust::Initialize(void* pArg, _matrix pos)
{
	if (pArg != nullptr) {

		m_pGameObject = (CGameObject*)pArg;
		if (m_pGameObject == nullptr)
			return E_FAIL;

		m_pTransform = static_cast<CTransform*>(m_pGameObject->GetComponent("Com_Transform"));

		m_pTransform->SetMatrix(pos);

		m_pTransform->SetScale(_float3(1.f, 2.f,1.f));


		MyPos = m_pTransform->GetState(CTransform::STATE_POSITION);

		MyPos = XMVectorSetY(MyPos, 0.8f);

		m_pTransform->SetState(CTransform::STATE_POSITION, MyPos);
		
		
		MakeEffet();
	}
	return S_OK;
}

void CEffectSoilDust::Update(_double deltaTime)
{
	if (m_bDead)
		return;

	if (!m_pGameObject)
		return;

}

void CEffectSoilDust::LateUpdate(_double deltaTime)
{
	if (static_cast<CEmptyEffect*>(m_pGameObject)->GetSpriteEnd())
	{
		this->SetDead();
		m_pGameObject->SetDead();
	}
}

void CEffectSoilDust::Render()
{
}


void CEffectSoilDust::Free()
{
	__super::Free();
}

void CEffectSoilDust::MakeEffet()
{

#pragma region Make Dust

	_matrix Translation;
	_int random = rand() % 3;
	_int Num = rand() % 2;
	random += 1;
	random = random*0.1f;

	if (Num == 0)
		Translation = XMMatrixTranslation(XMVectorGetX(m_pTransform->GetState(CTransform::STATE_POSITION)) + random, XMVectorGetY(m_pTransform->GetState(CTransform::STATE_POSITION)), XMVectorGetZ(m_pTransform->GetState(CTransform::STATE_POSITION)) + random);
	else
		Translation = XMMatrixTranslation(XMVectorGetX(m_pTransform->GetState(CTransform::STATE_POSITION)) - random, XMVectorGetY(m_pTransform->GetState(CTransform::STATE_POSITION)), XMVectorGetZ(m_pTransform->GetState(CTransform::STATE_POSITION)) - random);

	Translation = m_pTransform->Remove_Scale(Translation);
	for (int i = 0; i < 7; ++i) {
		auto Dust = CEngine::GetInstance()->AddGameObjectToPrefab(CEngine::GetInstance()->GetCurSceneNumber(), "Prototype_Effect_UrsaeDust", "E_UDust", &Translation);
		CEngine::GetInstance()->AddScriptObject(CEffectUrsaDust::Create(Dust, MyPos), CEngine::GetInstance()->GetCurSceneNumber());
	}
#pragma endregion

//#pragma region MakeDecal
//	auto DustDecal= CEngine::GetInstance()->AddGameObjectToPrefab(CEngine::GetInstance()->GetCurSceneNumber(), "Prototype_GameObecjt_SoilDecal", "E_SoilDecal");
//	CEngine::GetInstance()->AddScriptObject(CEffectSoilDecal::Create(DustDecal, MyPos), CEngine::GetInstance()->GetCurSceneNumber());
//#pragma endregion







}
