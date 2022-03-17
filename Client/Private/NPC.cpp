#include "stdafx.h"
#include "..\Public\NPC.h"

USING(Client)

CNPC::CNPC()
{
}

CNPC::CNPC(CGameObject * pObj)
	:CCharacter(pObj)
{
}

HRESULT CNPC::Initailze(CGameObject * pArg)
{
	list<CGameObject*> layer = CEngine::GetInstance()->GetGameObjectInLayer(0, "LAYER_PLAYER");
	if(layer.size() > 0)
		m_pPlayerTarget = static_cast<CEmptyGameObject*>(layer.front());
	if (m_pPlayerTarget)
		m_pPlayerTransform =static_cast<CTransform*>(m_pPlayerTarget->GetComponent("Com_Transform"));
	
	return S_OK;
}

void CNPC::Update(_double deltaTime)
{
}

void CNPC::LateUpdate(_double deltaTime)
{
}

_float CNPC::CalculatePlayerDistance(CTransform* npcTransform)
{
	if (m_pPlayerTransform) {
		_vector npcPos = npcTransform->GetState(CTransform::STATE_POSITION);
		_vector playerPos = m_pPlayerTransform->GetState(CTransform::STATE_POSITION);
		_float distance = XMVectorGetX(XMVector3Length(npcPos - playerPos));
		return distance;
	}

	return 10000.f;
}

void CNPC::Free()
{
}

