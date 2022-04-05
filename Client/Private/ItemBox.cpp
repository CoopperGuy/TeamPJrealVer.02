#include "stdafx.h"
#include "..\Public\ItemBox.h"
#include "PortalUI.h"
#include "Sound.h"
#include "EventCheck.h"
#include "ItemDropEffect.h"
USING(Client)

CItemBox::CItemBox()
{
}

HRESULT CItemBox::Initailze(CGameObject * pArg, _vector pos)
{
	m_pItemBox = (CEmptyGameObject*)pArg;
	m_pPlayer = static_cast<CEmptyGameObject*>(CEngine::GetInstance()->FindGameObjectWithName(SCENE_STATIC, "Player"));
	m_pTransform = static_cast<CTransform*>(m_pItemBox->GetComponent("Com_Transform"));

	CTransform* playerpos = static_cast<CTransform*>(m_pPlayer->GetComponent("Com_Transform"));
	pos = playerpos->GetState(CTransform::STATE_POSITION);


	if (m_pItemBox == nullptr || m_pPlayer == nullptr)
		return E_FAIL;

	m_pTransform->SetState(CTransform::STATE_POSITION, pos);
	m_pModel = static_cast<CModel*>(m_pItemBox->GetComponent("Com_Model"));


	m_pAlretUI = static_cast<CEmptyGameObject*>(CEngine::GetInstance()->SpawnPrefab("O_NPC_FSmall"));

	_vector position = XMVector3TransformCoord(XMVectorSet(0.f, 0.2f, 0.f, 0.f), m_pTransform->GetWorldMatrix());
	_float3 float3Pos;
	XMStoreFloat3(&float3Pos, position);
	m_pAlretUI->SetPosition(float3Pos);

	m_vMyPos = m_pTransform->GetState(CTransform::STATE_POSITION);

	CGameObject* ItempDropEff = CEngine::GetInstance()->AddGameObjectToPrefab(CEngine::GetInstance()->GetCurSceneNumber(), "Prototype_Effect_ItemDrop", "E_ItemDrop");
	CEngine::GetInstance()->AddScriptObject(m_pItemDropEff = CItemDropEffect::Create(ItempDropEff, pos), CEngine::GetInstance()->GetCurSceneNumber());


	return S_OK;
}

void CItemBox::Update(_double deltaTime)
{

	if (m_bDead)
		return;

	CTransform* playerTrans = static_cast<CTransform*>(m_pPlayer->GetComponent("Com_Transform"));
	_vector playerPos = playerTrans->GetState(CTransform::STATE_POSITION);
	_float dist = XMVectorGetZ(XMVector3Length(m_vMyPos - playerPos));


	if (dist < 1.f && !m_bGetItem) {
		m_pAlretUI->SetActive(true);
		static_cast<CEmptyGameObject*>(m_pItemBox)->SetRimLight(true, DirectX::Colors::Gold, 1.f);
	}
	else {
		m_pAlretUI->SetActive(false);
		static_cast<CEmptyGameObject*>(m_pItemBox)->SetRimLight(false, DirectX::Colors::Gold, 1.f);
	}


	if (CEngine::GetInstance()->Get_DIKDown(DIK_F)) {
		m_bGetItem = true;
	}


	if (!m_bGetItem) {
		m_pModel->SetUp_AnimationIndex(0);
		m_pModel->Play_Animation(deltaTime);
	}

	else {


		m_pAlretUI->SetActive(false);

		if (m_pModel->Get_AnimIndex() == 0)
			m_pModel->SetUp_AnimationIndex(1);

		if (m_pModel->Get_isFinished() && !m_bDissolve) {
			deaddt += deltaTime;
			m_pModel->Play_Animation(0);
			m_bDissolve = true;
		}
		else
			m_pModel->Play_Animation(deltaTime);
	}

}

void CItemBox::LateUpdate(_double deltaTime)
{

	if (m_bDissolve)
	{


		m_fDissolveAcc += (_float)deltaTime * 0.5f;
		if (m_fDissolveAcc > 0.925f) {

		if (m_pItemDropEff)
			m_pItemDropEff->SetDead();

			m_pItemBox->SetDead();
			this->SetDead();
		}

		m_pModel->SetDissolve(m_fDissolveAcc);
	}
}


CItemBox * CItemBox::Create(CGameObject * pTarget, _vector pos)
{
	CItemBox*	obj = new CItemBox();
	if (FAILED(obj->Initailze(pTarget, pos))) {
		SAFE_RELEASE(obj);
		return nullptr;
	}
	return obj;
}

void CItemBox::Free()
{
}
