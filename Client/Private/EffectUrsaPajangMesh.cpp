#include "stdafx.h"
#include "Client_Struct.h"
#include "..\Public\EffectUrsaPajangMesh.h"
#include "EmptyEffect.h"
#include "EffectMagicAf.h"
#include "MeteoFireBall.h"
USING(Client)

CEffectUrsaPajangMesh::CEffectUrsaPajangMesh()
{
}

CEffectUrsaPajangMesh * CEffectUrsaPajangMesh::Create(void * pArg, _vector pos)
{
	CEffectUrsaPajangMesh*		pInstance = new CEffectUrsaPajangMesh();

	if (FAILED(pInstance->Initialize(pArg, pos)))
	{
		MSG_BOX("Failed to Create CEffectUrsaPajangMesh");
		SafeRelease(pInstance);
		return nullptr;
	}

	return pInstance;
}


HRESULT CEffectUrsaPajangMesh::Initialize(void* pArg, _vector pos)
{
	if (pArg != nullptr) {

		m_pGameObject = (CGameObject*)pArg;
		if (m_pGameObject == nullptr)
			return E_FAIL;

		m_pTransform = static_cast<CTransform*>(m_pGameObject->GetComponent("Com_Transform"));
		pos = XMVectorSetY(pos, 0.1f);

		m_pTransform->SetState(CTransform::STATE_POSITION, pos);
		
		StartScail = { m_pTransform->GetScale(CTransform::STATE_RIGHT),m_pTransform->GetScale(CTransform::STATE_UP),m_pTransform->GetScale(CTransform::STATE_LOOK) };
		list<CGameObject*> childList = m_pGameObject->GetChildren();

	}
	return S_OK;
}

void CEffectUrsaPajangMesh::Update(_double deltaTime)
{
	if (m_bDead)
		return;

	if (!m_pGameObject)
		return;

	StartScail.x += 0.2f;
	StartScail.y += 0.2f;

	for (auto iter : childList)
	{
		CTransform* child = static_cast<CTransform*>(iter->GetComponent("Com_Transfrom"));
		child->SetScale(StartScail);
	}
	m_pTransform->SetScale(StartScail);
}


void CEffectUrsaPajangMesh::LateUpdate(_double deltaTime)
{
	m_dDeadTime += deltaTime;
	if (m_dDeadTime>=0.8f)
	{
		this->SetDead();
		m_pGameObject->SetDead();
	}
}

void CEffectUrsaPajangMesh::Render()
{
}


void CEffectUrsaPajangMesh::Free()
{
	__super::Free();
}
