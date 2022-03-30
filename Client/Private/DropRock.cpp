#include "stdafx.h"
#include "..\Public\DropRock.h"
#include "PortalUI.h"
#include "EffectRockDust.h"
#include "EventCheck.h"
#include "EffectDropRock.h"
#include "DropRockSmall.h"
USING(Client)

CDropRock::CDropRock()
{
}

HRESULT CDropRock::Initailze(CGameObject * pArg, _vector pos)
{

	if (pArg != nullptr) {

		m_pGameObject = (CGameObject*)pArg;
		if (m_pGameObject == nullptr)
			return E_FAIL;

		m_pTransform = static_cast<CTransform*>(m_pGameObject->GetComponent("Com_Transform"));

		int randompos = rand() % 2;
		int randomX = rand() % 5;
		randomX += 5;

		switch (randompos)
		{
		case 0:
			pos = XMVectorSetX(pos, randomX * -1.f);

			break;
		case 1:
			pos = XMVectorSetX(pos, randomX);
			break;

		}

		pos = XMVectorSetY(pos, 10.f);
		m_pTransform->SetState(CTransform::STATE_POSITION, pos);
		MyPos = m_pTransform->GetState(CTransform::STATE_POSITION);

		PosX = XMVectorGetX(m_pTransform->GetState(CTransform::STATE_POSITION));
		PosZ = XMVectorGetZ(m_pTransform->GetState(CTransform::STATE_POSITION));
		StartPosY = XMVectorGetY(m_pTransform->GetState(CTransform::STATE_POSITION));
		StartPosX = XMVectorGetX(m_pTransform->GetState(CTransform::STATE_POSITION));

		CGameObject* EffectRock = CEngine::GetInstance()->AddGameObjectToPrefab(CEngine::GetInstance()->GetCurSceneNumber(), "Prototype_Effect_DropRockEff", "E_RockEff2");
		CEngine::GetInstance()->AddScriptObject(m_pDropRockEff = CEffectDropRock::Create(EffectRock, MyPos), CEngine::GetInstance()->GetCurSceneNumber());

		if (m_pDropRockEff == nullptr)
		{
			MSG_BOX("CDropRock Class :: m_pDropRockEff is nullptr");
			return E_FAIL;
		}

	}
	return S_OK;
}

void CDropRock::Update(_double deltaTime)
{

	if (m_bDead)
		return;

	_vector pos = m_pTransform->GetState(CTransform::STATE_POSITION);


	if (m_pDropRockEff)
		m_pDropRockEff->Set_Pos(pos);


	XMVector3Normalize(MyPos);


	tempsp += 0.001f;

	PosY = StartPosY + (3.f * Time - 0.5f * 9.8f * Time * Time);
	Time += ((_float)deltaTime * 1.2f)+ tempsp; //시간값을 해줘야함 

	if (StartPosX >= 0)
		PosX -= /*deltaTime**/0.3f;
	else
		PosX += /*deltaTime**/0.3f;

	m_pTransform->SetState(CTransform::STATE_POSITION, _vector{ PosX,PosY,PosZ });

	MyPos = m_pTransform->GetState(CTransform::STATE_POSITION);


}

void CDropRock::LateUpdate(_double deltaTime)
{
	if (0.15f >= XMVectorGetY(m_pTransform->GetState(CTransform::STATE_POSITION)))
	{
		CEventCheck::GetInstance()->ShakeUpDown(5, 0.05f);

		/*	if (m_pDropRockEff)
				m_pDropRockEff->SetDead();*/

		_matrix Translation = XMMatrixTranslation(XMVectorGetX(m_pTransform->GetState(CTransform::STATE_POSITION)), XMVectorGetY(m_pTransform->GetState(CTransform::STATE_POSITION)), XMVectorGetZ(m_pTransform->GetState(CTransform::STATE_POSITION)));
		Translation = m_pTransform->Remove_Scale(Translation);

		CGameObject* EffectRockDust = CEngine::GetInstance()->AddGameObjectToPrefab(CEngine::GetInstance()->GetCurSceneNumber(), "Prototype_Effect_RockDust", "E_RockEff", &Translation);
		CEngine::GetInstance()->AddScriptObject(CEffectRockDust::Create(EffectRockDust), CEngine::GetInstance()->GetCurSceneNumber());

		for (int i = 0; i < 8; ++i) {
			CGameObject* RockSmall = CEngine::GetInstance()->AddGameObjectToPrefab(CEngine::GetInstance()->GetCurSceneNumber(), "Prototype_GameObecjt_RockSmall", "O_RockSmall", &Translation);
			CEngine::GetInstance()->AddScriptObject(CDropRockSmall::Create(RockSmall, m_pTransform->GetState(CTransform::STATE_POSITION)), CEngine::GetInstance()->GetCurSceneNumber());
		}
		this->SetDead();
		m_pGameObject->SetDead();
	}
}


CDropRock * CDropRock::Create(CGameObject * pArg, _vector pos)
{
	CDropRock*		pInstance = new CDropRock();

	if (FAILED(pInstance->Initailze(pArg, pos)))
	{
		MSG_BOX("Failed to Create CDropRock");
		SafeRelease(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CDropRock::Free()
{
}
