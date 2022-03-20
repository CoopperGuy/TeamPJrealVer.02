#include "stdafx.h"
#include "Client_Struct.h"
#include "..\Public\EffectSwordRing.h"
#include "EmptyEffect.h"
#include "Obb.h"
USING(Client)

CEffectSwordRing::CEffectSwordRing()
{
}

CEffectSwordRing * CEffectSwordRing::Create(void * pArg)
{
	CEffectSwordRing*		pInstance = new CEffectSwordRing();

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Create CEffectSwordRing");
		SafeRelease(pInstance);
		return nullptr;
	}

	return pInstance;
}


HRESULT CEffectSwordRing::Initialize(void* pArg)
{
	if (pArg != nullptr) {

		m_pGameObject = (CGameObject*)pArg;
		if (m_pGameObject == nullptr)
			return E_FAIL;

		CGameObject* pTargetObj = CEngine::GetInstance()->FindGameObjectWithName(CEngine::GetInstance()->GetCurSceneNumber(), "Flogas");
		if (pTargetObj == nullptr)
			return E_FAIL;

		CTransform* pTargetTransform = static_cast<CTransform*>(pTargetObj->GetComponent("Com_Transform"));

		TargetPos = pTargetTransform->GetState(CTransform::STATE_UP);

		m_pTransform = static_cast<CTransform*>(m_pGameObject->GetComponent("Com_Transform"));

		_vector pos = pTargetTransform->GetState(CTransform::STATE_POSITION);
		pos = XMVectorSetY(pos, XMVectorGetY(pos) + 0.2f);
		m_pTransform->SetState(CTransform::STATE_POSITION, pos);

		m_fScale = { m_pTransform->GetScale(CTransform::STATE_RIGHT) , m_pTransform->GetScale(CTransform::STATE_UP) , m_pTransform->GetScale(CTransform::STATE_LOOK) };
		
		CStat* stat =static_cast<CStat*>(pTargetObj->GetComponent("Com_Stat"));

		//pos ,size ,  dmg,collisiontpye, duration;
		m_pOBB = CObb::Create(pos,XMLoadFloat3(&m_fScale), stat->GetStatInfo().atk, ID::MONSTER_EFFECT, 100.f, nullptr);
	}
	return S_OK;
}

void CEffectSwordRing::Update(_double deltaTime)
{
	if (m_bDead)
		return;

	if (!m_pGameObject)
		return;

	{
		if (m_bSlashR) {
			m_fScrollX = _float3(-0.6f, -0.5f, 0.f);
			m_fScrollY = _float3(-0.6f, -0.5f, 0.f);
		}
		else {
			m_fScrollX = _float3(0.6f,1.5f, 0.f);
			m_fScrollY = _float3(0.6f,1.5f, 0.f);
		}
	}
	static_cast<CEmptyEffect*>(m_pGameObject)->SetScrollSpeedX(m_fScrollX);
	static_cast<CEmptyEffect*>(m_pGameObject)->SetScrollSpeedY(m_fScrollY);


	m_fScale.x += PlusScale;
	m_fScale.y += PlusScale * 0.95f;
	m_fScale.z += PlusScale;
	m_pTransform->RotateAxis(XMVectorSet(0.f,1.f,0.f,0.f), deltaTime * 2.f);
	m_pTransform->SetScale(m_fScale);
	m_pOBB->SetSize(m_fScale);
	if (m_fScale.y > m_fMaxScail)
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

void CEffectSwordRing::LateUpdate(_double deltaTime)
{
	if (m_bDead)
	{
		this->SetDead();
		m_pGameObject->SetDead();
		m_pOBB->SetupDead();
	}
}

void CEffectSwordRing::Render()
{
}

void CEffectSwordRing::SetFadeInOut(CGameObject * pObj)
{
	static_cast<CEmptyEffect*>(pObj)->SetFadeOutEnable(true);
	static_cast<CEmptyEffect*>(pObj)->SetFadeOutStartTime(0.f);
	static_cast<CEmptyEffect*>(pObj)->SetFadeOutDuration(0.8f);
	static_cast<CEmptyEffect*>(pObj)->SetEffectDuration(0.8f);
}

void CEffectSwordRing::Free()
{
	__super::Free();
}
