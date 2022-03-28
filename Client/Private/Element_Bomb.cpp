#include "stdafx.h"
#include "..\Public\Element_Bomb.h"
#include "El_Flogas.h"
#include "Bomb_Center.h"

USING(Client)

CElement_Bomb::CElement_Bomb()
{
}

HRESULT CElement_Bomb::Initialize(CEmptyEffect* pThis, CGameObject* pTargat, CEl_Flogas& pObj)
{
	m_pThis = pThis;
	m_pElement = &pObj;
	CTransform* pTargetTrans = static_cast<CTransform*>(pTargat->GetComponent("Com_Transform"));
	m_pEffectTrans = static_cast<CTransform*>(m_pThis->GetComponent("Com_Transform"));
	_vector vPos = pTargetTrans->GetState(CTransform::STATE_POSITION);
	vPos = XMVectorSetY(vPos, 0.5f);
	m_pEffectTrans->SetState(CTransform::STATE_POSITION, vPos);

	CGameObject* pGameObject = CEngine::GetInstance()->AddGameObjectToPrefab(CEngine::GetInstance()->GetCurSceneNumber(), "Prototype_Effect_Bomb_Center", "E_Bomb_Center");
	CEngine::GetInstance()->AddScriptObject(CBomb_Center::Create((CEmptyEffect*)pGameObject, m_pEffectTrans, *this), CEngine::GetInstance()->GetCurSceneNumber());
	return S_OK;
}

void CElement_Bomb::Update(_double dDeltaTime)
{

}

void CElement_Bomb::LateUpdate(_double deltaTime)
{
	if (m_bRelease = m_pElement->Get_DeadMotion())
	{
		//cout << " Deleating Bomb " << "\n";
		this->SetDead();
		m_pThis->SetDead();
		//cout << " Deleated Bomb " << "\n";
	}
}



CBasicEffect * CElement_Bomb::Create(CEmptyEffect* pThis, CGameObject* pTargat, class CEl_Flogas& pObj)
{
	CElement_Bomb*		pInstance = new CElement_Bomb();

	if (FAILED(pInstance->Initialize(pThis, pTargat, pObj)))
	{
		MSG_BOX("Failed to Create CElement_Bomb");
		SafeRelease(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CElement_Bomb::Free()
{
}
