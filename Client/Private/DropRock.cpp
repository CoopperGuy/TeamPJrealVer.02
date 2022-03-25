#include "stdafx.h"
#include "..\Public\DropRock.h"
#include "PortalUI.h"
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

		switch (randompos)
		{
		case 0:
			pos = XMVectorSetX(pos, -10.f);
			break;
		case 1:
			pos = XMVectorSetX(pos, 10.f);
			break;

		}

		pos = XMVectorSetY(pos, 8.f);
		m_pTransform->SetState(CTransform::STATE_POSITION, pos);
		MyPos = m_pTransform->GetState(CTransform::STATE_POSITION);

		PosX = XMVectorGetX(m_pTransform->GetState(CTransform::STATE_POSITION));
		PosZ = XMVectorGetZ(m_pTransform->GetState(CTransform::STATE_POSITION));
		StartPosY = XMVectorGetY(m_pTransform->GetState(CTransform::STATE_POSITION));
		StartPosX = XMVectorGetX(m_pTransform->GetState(CTransform::STATE_POSITION));
	}
	return S_OK;
}

void CDropRock::Update(_double deltaTime)
{

	if (m_bDead)
		return;

	XMVector3Normalize(MyPos);

	PosY = StartPosY + (3.f * Time - 0.5f * 9.8f * Time * Time);
	Time += (_float)deltaTime * 1.5f; //시간값을 해줘야함 

	if(StartPosX >=0)
		PosX -= /*deltaTime**/0.2f;
	else
		PosX += /*deltaTime**/0.2f;

	m_pTransform->SetState(CTransform::STATE_POSITION, _vector{ PosX,PosY,PosZ });

}

void CDropRock::LateUpdate(_double deltaTime)
{
	if (0>=XMVectorGetY(m_pTransform->GetState(CTransform::STATE_POSITION)))
	{
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
