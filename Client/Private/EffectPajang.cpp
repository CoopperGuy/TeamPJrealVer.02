#include "stdafx.h"
#include "Client_Struct.h"
#include "..\Public\EffectPajang.h"
#include "EmptyEffect.h"
#include "Obb.h"
USING(Client)

CEffectPajang::CEffectPajang()
{
}

CEffectPajang * CEffectPajang::Create(void * pArg)
{
	CEffectPajang*		pInstance = new CEffectPajang();

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Create CEffectPajang");
		SafeRelease(pInstance);
		return nullptr;
	}

	return pInstance;
}


HRESULT CEffectPajang::Initialize(void* pArg)
{
	if (pArg != nullptr) {

		m_pGameObject = (CGameObject*)pArg;
		if (m_pGameObject == nullptr)
			return E_FAIL;
		m_pTransform = static_cast<CTransform*>(m_pGameObject->GetComponent("Com_Transform"));

		CGameObject* pTargetObj = CEngine::GetInstance()->FindGameObjectWithName(CEngine::GetInstance()->GetCurSceneNumber(), "Flogas");
		if (pTargetObj == nullptr)
			return E_FAIL;

		CTransform* pTargetTransform = static_cast<CTransform*>(pTargetObj->GetComponent("Com_Transform"));
		CModel* pTargetmodel = static_cast<CModel*>(pTargetObj->GetComponent("Com_Model"));

		//_matrix targetbone = pTargetmodel->Get_BoneWithoutOffset("Bip01-L-Finger2");
		_matrix targetbone = XMMatrixIdentity();
		memcpy(&targetbone.r[3], &pTargetmodel->Get_BoneWithoutOffset("Bip01-L-Finger21").r[3],sizeof(_vector));
		targetbone.r[3] = XMVectorSetY(targetbone.r[3], 1.f);
		//memccpy(&m_pTransform->GetWorldMatrix(), &targetbone, 0, sizeof(_matrix));
		m_pTransform->SetMatrix(targetbone *pTargetTransform->GetWorldMatrix());
		//m_pTransform->SetUpRotation(_vector{ 0.f,0.f,1.f }, 90.f);

		m_fScale = { m_pTransform->GetScale(CTransform::STATE_RIGHT) , m_pTransform->GetScale(CTransform::STATE_UP) ,0.f };

		_vector pos = m_pTransform->GetState(CTransform::STATE_POSITION);
		pos = XMVectorSetY(pos, XMVectorGetY(pos) - 0.6f);
		m_pTransform->SetState(CTransform::STATE_POSITION, pos);
		CStat* stat =static_cast<CStat*>(pTargetObj->GetComponent("Com_Stat"));
		m_pOBB = CObb::Create(pos, XMLoadFloat3(&m_fScale), stat->GetStatInfo().atk, ID::MONSTER_EFFECT, 100.f, nullptr);
	}
	return S_OK;
}

void CEffectPajang::Update(_double deltaTime)
{
	if (m_bDead)
		return;

	if (!m_pGameObject)
		return;

	m_fScale.x += PlusScale;
	//m_fScale.y += PlusScale;
	m_fScale.z += PlusScale;
	m_pTransform->SetScale(m_fScale);

	if (m_fScale.x >= m_fMaxScail)
	{
		if (m_bSetFadeOut)
		{
			SetFadeInOut(m_pGameObject);
			m_bSetFadeOut = false;
		}
	}

	if (static_cast<CEmptyEffect*>(m_pGameObject)->GetFadeOutEnable()) {
		m_fDeadTime += deltaTime;
		//if (0.15 >= static_cast<CEmptyEffect*>(m_pGameObject)->GetFadeAlpha())
		if (m_fDeadTime >= static_cast<CEmptyEffect*>(m_pGameObject)->GetFadeOutDuration())
		{
			m_bDead = true;
		}
	}

}

void CEffectPajang::LateUpdate(_double deltaTime)
{
	if (m_bDead)
	{
		this->SetDead();
		m_pGameObject->SetDead();
	}
}

void CEffectPajang::Render()
{
}

void CEffectPajang::SetFadeInOut(CGameObject * pObj)
{
	static_cast<CEmptyEffect*>(pObj)->SetFadeOutEnable(true);
	static_cast<CEmptyEffect*>(pObj)->SetFadeOutStartTime(0.f);
	static_cast<CEmptyEffect*>(pObj)->SetFadeOutDuration(2.f);
	static_cast<CEmptyEffect*>(pObj)->SetEffectDuration(2.f);
}

void CEffectPajang::Free()
{
	__super::Free();
}
