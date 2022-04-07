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

HRESULT CDropRockSmall::Initailze(CGameObject * pArg, _vector pos, _bool mini, _bool bone)
{

	if (pArg != nullptr) {

		m_pGameObject = (CGameObject*)pArg;
		if (m_pGameObject == nullptr)
			return E_FAIL;

		m_pTransform = static_cast<CTransform*>(m_pGameObject->GetComponent("Com_Transform"));

		randompos = rand() % 2;
		randomX = rand() % 10 + 1;


		pos = XMVectorSetY(pos, XMVectorGetY(m_pTransform->GetState(CTransform::STATE_POSITION)) + randomX*0.1f);
		
		m_pTransform->SetState(CTransform::STATE_POSITION, pos);
		MyPos = m_pTransform->GetState(CTransform::STATE_POSITION);

		PosX = XMVectorGetX(m_pTransform->GetState(CTransform::STATE_POSITION));
		PosZ = XMVectorGetZ(m_pTransform->GetState(CTransform::STATE_POSITION));
		StartPosY = XMVectorGetY(m_pTransform->GetState(CTransform::STATE_POSITION));
		StartPosX = XMVectorGetX(m_pTransform->GetState(CTransform::STATE_POSITION));

		if (bone == false)
		{
			if (mini)
				m_pTransform->SetScale(_float3(randomX * 0.0005f, randomX * 0.0005f, randomX * 0.0005f));
			else
				m_pTransform->SetScale(_float3(randomX * 0.001f, randomX * 0.001f, randomX * 0.001f));
		}

		m_bBone = bone;
		//int random = rand() % 3;
		//if (random == 0) {
		//	/*CGameObject* EffectRockDust = CEngine::GetInstance()->AddGameObjectToPrefab(CEngine::GetInstance()->GetCurSceneNumber(), "Prototype_GameObecjt_DropDust", "E_DropDust");
		//	CEngine::GetInstance()->AddScriptObject(CEffectDropRockDust::Create(EffectRockDust, m_pTransform->GetState(CTransform::STATE_POSITION)), CEngine::GetInstance()->GetCurSceneNumber());*/

		//	//_matrix offset = XMMatrixTranslation(XMVectorGetX(m_pTransform->GetState(CTransform::STATE_POSITION)) + (randomX), XMVectorGetY(m_pTransform->GetState(CTransform::STATE_POSITION)) + (randomX), XMVectorGetZ(m_pTransform->GetState(CTransform::STATE_POSITION)) + (randomX));
		//	//auto Dust = CEngine::GetInstance()->AddGameObjectToPrefab(CEngine::GetInstance()->GetCurSceneNumber(), "Prototype_Effect_UrsaeDust", "E_UrsaeDust");
		//	//CEngine::GetInstance()->AddScriptObject(CEffectUrsaDust::Create(Dust, m_pTransform->Remove_ScaleRotation(offset * m_pTransform->GetWorldMatrix())), CEngine::GetInstance()->GetCurSceneNumber());
		//}
	}
	return S_OK;
}

void CDropRockSmall::Update(_double deltaTime)
{

	if (m_bDead)
		return;

	XMVector3Normalize(MyPos);

	PosY = StartPosY + (3.5f * Time - 0.5f * 9.8f * Time * Time);
	Time += (_float)deltaTime * 0.1f* randomX; //½Ã°£°ªÀ» ÇØÁà¾ßÇÔ 
	fAngleAcc += (_float)deltaTime;

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

	if (m_bBone == true)
	{
		_vector vLook = m_pTransform->GetState(CTransform::STATE_LOOK);
		_vector vAxisY = XMVectorSet(0.f, 1.f, 0.f, 0.f);
		_vector vRight;
		_vector vUp;

		vLook = XMVector3Normalize(vLook);
		vRight = XMVector3Normalize(XMVector3Cross(vAxisY, vLook));
		vUp = XMVector3Normalize(XMVector3Cross(vLook, vRight));
		_matrix RotationMatrix1 = XMMatrixRotationAxis(vLook, XMConvertToRadians(fAngleAcc * 10.f));
		_matrix RotationMatrix2 = XMMatrixRotationAxis(vRight, XMConvertToRadians(fAngleAcc * 10.f));
		_matrix RotationMatrix3 = XMMatrixRotationAxis(vUp, XMConvertToRadians(fAngleAcc * 10.f));
		_matrix RotationMatrix = RotationMatrix1 * RotationMatrix2 * RotationMatrix3;

		m_pTransform->SetState(CTransform::STATE_RIGHT, XMVector4Transform(vRight * m_pTransform->GetScale(CTransform::STATE_RIGHT), RotationMatrix));
		m_pTransform->SetState(CTransform::STATE_UP, XMVector4Transform(vUp * m_pTransform->GetScale(CTransform::STATE_UP), RotationMatrix));
		m_pTransform->SetState(CTransform::STATE_LOOK, XMVector4Transform(vLook * m_pTransform->GetScale(CTransform::STATE_LOOK), RotationMatrix));
	}
}

void CDropRockSmall::LateUpdate(_double deltaTime)
{
	if (0.1f>=XMVectorGetY(m_pTransform->GetState(CTransform::STATE_POSITION)))
	{
		this->SetDead();
		m_pGameObject->SetDead();
	}
}


CDropRockSmall * CDropRockSmall::Create(CGameObject * pArg, _vector pos, _bool mini, _bool bone)
{
	CDropRockSmall*		pInstance = new CDropRockSmall();

	if (FAILED(pInstance->Initailze(pArg, pos, mini, bone)))
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
