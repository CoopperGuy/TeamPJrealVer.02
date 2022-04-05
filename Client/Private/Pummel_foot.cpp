#include "stdafx.h"
#include "..\Public\Pummel_foot.h"
#include "EventCheck.h"
#include "Obb.h"
#include "Dustparticle_Cone.h"

USING(Client)

CPummel_foot::CPummel_foot()
{
}

HRESULT CPummel_foot::Initialize(CEmptyEffect* pThis, CGameObject* pTarget)
{
	m_pThis = pThis;
	m_pTargetTransform = static_cast<CTransform*>(pTarget->GetComponent("Com_Transform"));
	m_pEffectTrans = static_cast<CTransform*>(m_pThis->GetComponent("Com_Transform"));
	CModel* pModel = static_cast<CModel*>(pTarget->GetComponent("Com_Model"));
	mat = m_pTargetTransform->GetWorldMatrix();
	mat.r[0] = XMVector3Normalize(mat.r[0]) * m_pEffectTrans->GetScale(CTransform::STATE_RIGHT);
	mat.r[1] = XMVector3Normalize(mat.r[1]) * m_pEffectTrans->GetScale(CTransform::STATE_UP);
	mat.r[2] = XMVector3Normalize(mat.r[2]) * -m_pEffectTrans->GetScale(CTransform::STATE_LOOK);
	mat.r[3] = (pModel->Get_BoneMatrix("Bip01-L-Foot") * m_pTargetTransform->GetWorldMatrix()).r[3];
	_float4 color = m_pThis->GetOffsetColor();
	color.w = 0.f;
	m_pThis->SetOffsetColor(color);
	m_pEffectTrans->SetMatrix(mat);
	_vector vTargetPos = m_pEffectTrans->GetState(CTransform::STATE_POSITION);
	_float3 vPos = {};
	XMStoreFloat3(&vPos, vTargetPos);
	m_pEffectTrans->SetState(CTransform::STATE_POSITION, vTargetPos);
	mat.r[0] = XMVector3Normalize(mat.r[0]);
	mat.r[1] = XMVector3Normalize(mat.r[1]);
	mat.r[2] = XMVector3Normalize(mat.r[2]);
	mat *= XMMatrixRotationY(90.f);
	for (_uint i = 0; i < 10; ++i)
	{
		_uint j = rand() %2;
		vPos = { vPos.x + (_float)j * 0.01f, vPos.y, vPos.z + (_float)j * 0.03f };
		mat.r[3] = XMLoadFloat3(&vPos);
		CGameObject* pGameObject = CEngine::GetInstance()->AddGameObjectToPrefab(CEngine::GetInstance()->GetCurSceneNumber(), "Prototype_GameObecjt_Ursa_Dustparticle_Cone", "E_Dustparticle_Cone", &mat);
		CEngine::GetInstance()->AddScriptObject(CDustparticle_Cone::Create((CEmptyEffect*)pGameObject, m_pGameObject), CEngine::GetInstance()->GetCurSceneNumber());
	}
	return S_OK;
}

void CPummel_foot::Update(_double dDeltaTime)
{
	m_DurationDelta += (_float)dDeltaTime;

	if (m_DurationDelta > 0.2)
	{
		if (m_bCreate)
		{
			_vector vTargetPos = m_pEffectTrans->GetState(CTransform::STATE_POSITION);
			_float3 vPos = {};
			XMStoreFloat3(&vPos, vTargetPos);
			for (_uint i = 0; i < 10; ++i)
			{
				_uint j = rand() % 2;
				vPos = { vPos.x + (_float)j * 0.01f, vPos.y, vPos.z + (_float)j * 0.03f };
				mat.r[3] = XMLoadFloat3(&vPos);
				CGameObject* pGameObject = CEngine::GetInstance()->AddGameObjectToPrefab(CEngine::GetInstance()->GetCurSceneNumber(), "Prototype_GameObecjt_Ursa_Dustparticle_Cone", "E_Dustparticle_Cone", &mat);
				CEngine::GetInstance()->AddScriptObject(CDustparticle_Cone::Create((CEmptyEffect*)pGameObject, m_pGameObject), CEngine::GetInstance()->GetCurSceneNumber());
			}
		}
	}
}

void CPummel_foot::LateUpdate(_double dDeltaTime)
{

	if (m_DurationDelta > 0.3f)
	{
		this->SetDead();
		m_pThis->SetDead();
	}
}



CBasicEffect * CPummel_foot::Create(CEmptyEffect* pThis, CGameObject* pTarget)
{
	CPummel_foot*		pInstance = new CPummel_foot();

	if (FAILED(pInstance->Initialize(pThis, pTarget)))
	{
		MSG_BOX("Failed to Create CPummel_foot");
		SafeRelease(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CPummel_foot::Free()
{
}
