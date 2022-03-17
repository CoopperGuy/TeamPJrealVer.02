#include "stdafx.h"
#include "..\Public\MonHpVIBuffer.h"

BEGIN(Client)
void CreatMonsterHPbarThread(CMonHpVIBuffer* script) {
	CEmptyGameObject*	monHud = static_cast<CEmptyGameObject*>(CEngine::GetInstance()->SpawnPrefab("U_MonHpVIBuffer"));
	CEmptyGameObject*	monHpBar = static_cast<CEmptyGameObject*>(CEngine::GetInstance()->SpawnPrefab("U_MonHpBar"));

	CEngine::GetInstance()->AddScriptObject(script, CEngine::GetInstance()->GetCurSceneNumber());

	script->pushHuds(monHud, monHpBar);
	script->pushBuffer(dynamic_cast<CVIBuffer_Rect*>(monHpBar->GetComponent("Com_VIBuffer")));
	script->SetHud_BarTransform(static_cast<CTransform*>(monHud->GetComponent("Com_Transform")) , static_cast<CTransform*>(monHpBar->GetComponent("Com_Transform")));
	script->SetIsEnd();

}
END

USING(Client)

CMonHpVIBuffer::CMonHpVIBuffer()
{
}

HRESULT CMonHpVIBuffer::Initailze(CGameObject * pArg)
{
	if (pArg != nullptr) {
		pTarget = pArg;
		m_pTargetStat = static_cast<CStat*>(pTarget->GetComponent("Com_Stat"));
		m_pTargetTrans = static_cast<CTransform*>(pTarget->GetComponent("Com_Transform"));
	}
	std::thread createHpbar(CreatMonsterHPbarThread, this);
	createHpbar.detach();
	return S_OK;
}

void CMonHpVIBuffer::Update(_double deltaTime)
{
	if (m_bDead)
		return;
	if (m_bisEnd) {
		if (m_pTargetStat) {
			m_fPercetage = m_pTargetStat->GetHpPercentage();
			if (m_fPercetage < m_fBackPercentage) {
				m_fBackPercentage -= _float(0.05f*deltaTime);
			}
			m_pHudBuffer[0]->GetShader()->SetUp_ValueOnShader("g_Percentage", &m_fPercetage, sizeof(_float));
			m_pHudBuffer[0]->GetShader()->SetUp_ValueOnShader("g_Back", &m_fBackPercentage, sizeof(_float));
		}
		if (m_pHudTrans) {
			_vector hpPos = XMVector3TransformCoord(XMVectorSet(0.f, 0.5f, 0.f, 1.f), m_pTargetTrans->GetWorldMatrix());
			m_pHudTrans->SetState(CTransform::STATE_POSITION, hpPos);
			if (m_pBarTrans) {
				m_pBarTrans->SetMatrix(m_pHudTrans->GetWorldMatrix());
			}
		}

		
	
	}
}

void CMonHpVIBuffer::LateUpdate(_double deltaTime)
{
	if (m_bisEnd) {
		if (pTarget) {
			if (pTarget->isDead()) {
				this->SetDead();
				m_pHp->DeleteRealObject();
				m_pHpBar->DeleteRealObject();
			}
		}
	}
}

void CMonHpVIBuffer::Render()
{
}

void CMonHpVIBuffer::SetHud_BarTransform(CTransform * Hudtrans, CTransform * BarTrans)
{
	m_pBarTrans = BarTrans;
	m_pHudTrans = Hudtrans;
}


CMonHpVIBuffer * CMonHpVIBuffer::Create(CGameObject * pTarget)
{
	CMonHpVIBuffer*	obj = new CMonHpVIBuffer();
	if (FAILED(obj->Initailze(pTarget))) {
		SAFE_RELEASE(obj);
		return nullptr;
	}
	return obj;
}

void CMonHpVIBuffer::Free()
{
	m_pHp->DeleteRealObject();
	m_pHpBar->DeleteRealObject();

}

