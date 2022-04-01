#include "stdafx.h"
#include "..\Public\DropRockSmall.h"
#include "PortalUI.h"
#include "EffectRockDust.h"
#include "EventCheck.h"
#include "EffectDropRock.h"
#include "EffectUrsaDust.h"
#include "EffectDropRockDust.h"
USING(Client)

CDropRockSmall::CDropRockSmall()
{
}

HRESULT CDropRockSmall::Initailze(CGameObject * pArg, _vector pos)
{

	if (pArg != nullptr) {

		m_pGameObject = (CGameObject*)pArg;
		if (m_pGameObject == nullptr)
			return E_FAIL;

		m_pTransform = static_cast<CTransform*>(m_pGameObject->GetComponent("Com_Transform"));

		randompos = rand() % 2;
		randomX = rand() % 10;


		pos = XMVectorSetY(pos, XMVectorGetY(m_pTransform->GetState(CTransform::STATE_POSITION)) + randomX*0.1);

		

		m_pTransform->SetState(CTransform::STATE_POSITION, pos);
		MyPos = m_pTransform->GetState(CTransform::STATE_POSITION);

		PosX = XMVectorGetX(m_pTransform->GetState(CTransform::STATE_POSITION));
		PosZ = XMVectorGetZ(m_pTransform->GetState(CTransform::STATE_POSITION));
		StartPosY = XMVectorGetY(m_pTransform->GetState(CTransform::STATE_POSITION));
		StartPosX = XMVectorGetX(m_pTransform->GetState(CTransform::STATE_POSITION));

		m_pTransform->SetScale(_float3(randomX * 0.001f, randomX * 0.001f, randomX * 0.001f));


		int random = rand() % 3;
		if (random == 0) {
			CGameObject* EffectRockDust = CEngine::GetInstance()->AddGameObjectToPrefab(CEngine::GetInstance()->GetCurSceneNumber(), "Prototype_GameObecjt_DropDust", "E_DropDust");
			CEngine::GetInstance()->AddScriptObject(CEffectDropRockDust::Create(EffectRockDust, m_pTransform->GetState(CTransform::STATE_POSITION)), CEngine::GetInstance()->GetCurSceneNumber());

			//_matrix offset = XMMatrixTranslation(XMVectorGetX(m_pTransform->GetState(CTransform::STATE_POSITION)) + (randomX), XMVectorGetY(m_pTransform->GetState(CTransform::STATE_POSITION)) + (randomX), XMVectorGetZ(m_pTransform->GetState(CTransform::STATE_POSITION)) + (randomX));
			//auto Dust = CEngine::GetInstance()->AddGameObjectToPrefab(CEngine::GetInstance()->GetCurSceneNumber(), "Prototype_Effect_UrsaeDust", "E_UrsaeDust");
			//CEngine::GetInstance()->AddScriptObject(CEffectUrsaDust::Create(Dust, m_pTransform->Remove_ScaleRotation(offset * m_pTransform->GetWorldMatrix())), CEngine::GetInstance()->GetCurSceneNumber());
		}
	}
	return S_OK;
}

void CDropRockSmall::Update(_double deltaTime)
{

	if (m_bDead)
		return;

	XMVector3Normalize(MyPos);

	PosY = StartPosY + (3.5f * Time - 0.5f * 9.8f * Time * Time);
	Time += (_float)deltaTime * 0.1* randomX; //½Ã°£°ªÀ» ÇØÁà¾ßÇÔ 



	if (randompos == 0) {
		PosX -= /*deltaTime**/0.003f * randomX;
		PosZ += /*deltaTime**/0.005f * randomX;
	}
	else {
		PosX += /*deltaTime**/0.003f * randomX;
		PosZ -= /*deltaTime**/0.005f * randomX;

	}
	m_pTransform->SetState(CTransform::STATE_POSITION, _vector{ PosX,PosY,PosZ });

	MyPos = m_pTransform->GetState(CTransform::STATE_POSITION);


}

void CDropRockSmall::LateUpdate(_double deltaTime)
{
	if (0.1f>=XMVectorGetY(m_pTransform->GetState(CTransform::STATE_POSITION)))
	{
		this->SetDead();
		m_pGameObject->SetDead();
	}
}


CDropRockSmall * CDropRockSmall::Create(CGameObject * pArg, _vector pos)
{
	CDropRockSmall*		pInstance = new CDropRockSmall();

	if (FAILED(pInstance->Initailze(pArg, pos)))
	{
		MSG_BOX("Failed to Create CDropRockSmall");
		SafeRelease(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CDropRockSmall::Free()
{
}
