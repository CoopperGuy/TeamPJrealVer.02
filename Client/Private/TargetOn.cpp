#include "stdafx.h"
#include "..\Public\TargetOn.h"

USING(Client)

CTargetOn::CTargetOn()
{
}

HRESULT CTargetOn::Initailze(CGameObject* player, CGameObject * pArg)
{
	m_pThisUI = static_cast<CEmptyUI*>(CEngine::GetInstance()->SpawnPrefab("U_TargetOnUI"));
	m_pThisUI->SetisRender(false);
	m_pPlayer = static_cast<CEmptyGameObject*>(player);
	m_pTarget = static_cast<CEmptyGameObject*>(pArg);

	_vector targetPos = XMLoadFloat3(&m_pTarget->GetCollisionPosition()) - XMVectorSet(0.f,0.25f,0.f,0.f);
	_matrix view = CEngine::GetInstance()->GetTransform(CPipeline::D3DTS_VIEW);
	_matrix proj = CEngine::GetInstance()->GetTransform(CPipeline::D3DTS_PROJ);
	targetPos = XMVector3TransformCoord(targetPos, view);
	targetPos = XMVector3TransformCoord(targetPos, proj);

	_float4x4 transform;
	transform._11 = 1.f;
	transform._22 = 1.f;

	transform._41 = XMVectorGetX(targetPos) * (1280 >> 1) + (1280 >> 1);
	transform._42 = -XMVectorGetY(targetPos) * (720 >> 1) + (720 >> 1);
	m_pTargetTrans = static_cast<CRectTransform*>(m_pThisUI->GetComponent("Com_Transform"));
	CRectTransform::RECTTRANSFORMDESC desc = m_pTargetTrans->GetTransformDesc();
	desc.posX = transform._41;
	desc.posY = transform._42;
	m_pTargetTrans->SetTransformMat(desc);
	//_float3 fUIPos;
	//XMStoreFloat3(&fUIPos, uiPos);
	//m_pThisUI->SetPosition(fUIPos);

	list<CGameObject*> child = m_pThisUI->GetChildren();
	for (auto&iter : child) {
		m_Child.emplace_back(static_cast<CEmptyUI*>(iter));
	}
	CEngine::GetInstance()->AddScriptObject(this,CEngine::GetInstance()->GetCurSceneNumber());
	return S_OK;
}

void CTargetOn::Update(_double deltaTime)
{
	m_pThisUI->SetisRender(true);
	_vector targetPos = XMLoadFloat3(&m_pTarget->GetCollisionPosition()) - XMVectorSet(0.f, 0.25f, 0.f, 0.f);
	_matrix view = CEngine::GetInstance()->GetTransform(CPipeline::D3DTS_VIEW);
	_matrix proj = CEngine::GetInstance()->GetTransform(CPipeline::D3DTS_PROJ);
	targetPos = XMVector3TransformCoord(targetPos, view);
	targetPos = XMVector3TransformCoord(targetPos, proj);

	_float4x4 transform;
	transform._11 = 1.f;
	transform._22 = 1.f;

	transform._41 = XMVectorGetX(targetPos) * (1280 >> 1) + (1280 >> 1);
	transform._42 = -XMVectorGetY(targetPos) * (720 >> 1) + (720 >> 1);
	CRectTransform::RECTTRANSFORMDESC desc = m_pTargetTrans->GetTransformDesc();
	desc.posX = transform._41;
	desc.posY = transform._42;
	m_pTargetTrans->SetTransformMat(desc);
	
	_float2 offset = m_Child[TARGETS_UP]->GetTransformOffst();
	if (offset.y <= -m_fAimOffset) {
		offset.y += (_float)deltaTime * m_fAimOffset;
		m_Child[TARGETS_UP]->SetTransformOffst(offset.x, offset.y);
	}
	
	offset = m_Child[TARGETS_DOWN]->GetTransformOffst();
	if (offset.y >= m_fAimOffset) {
		offset.y -= (_float)deltaTime * m_fAimOffset;
		m_Child[TARGETS_DOWN]->SetTransformOffst(offset.x, offset.y);
	}

	offset = m_Child[TARGETS_LEFT]->GetTransformOffst();
	if (offset.x <= -m_fAimOffset) {
		offset.x += (_float)deltaTime * m_fAimOffset;
		m_Child[TARGETS_LEFT]->SetTransformOffst(offset.x, offset.y);
	}

	offset = m_Child[TARGETS_RIGHT]->GetTransformOffst();
	if (offset.x >= m_fAimOffset) {
		offset.x -= (_float)deltaTime * m_fAimOffset;
		m_Child[TARGETS_RIGHT]->SetTransformOffst(offset.x, offset.y);
	}


}

void CTargetOn::LateUpdate(_double deltaTime)
{
	
}

void CTargetOn::Render()
{
}

void CTargetOn::ReleaseThisUI()
{
	this->SetDead();
	m_pThisUI->SetisRender(false);
}

CTargetOn * CTargetOn::Create(CGameObject* player,CGameObject * pTarget)
{
	CTargetOn*	obj = new CTargetOn();
	if (FAILED(obj->Initailze(player, pTarget))) {
		SAFE_RELEASE(obj);
		return nullptr;
	}
	return obj;
}

void CTargetOn::Free()
{
	if(m_pThisUI)
		m_pThisUI->SetDead();
}

