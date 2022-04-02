#include "stdafx.h"
#include "..\Public\Obb.h"
#include "BasicCollider.h"
#include "Obb.h"
USING(Client)

CObb::CObb()
{
}



HRESULT CObb::Initailze(_float3 position, _float3 size, _float dmg, ID _collisionType, _float _duration, CTransform * pArg)
{
	CEngine* pEngine = CEngine::GetInstance();
	m_pTargetTransform = pArg;
	m_pObj =static_cast<CEmptyGameObject*>(pEngine->AddGameObjectToPrefab(pEngine->GetCurSceneNumber(), "Prototype_GameObject_OBBs", "O_OBBs", &XMMatrixTranslationFromVector(XMLoadFloat3(&position))));
	m_pOBB = static_cast<CBasicCollider*>(m_pObj->GetComponent("Com_OBB"));
	m_pStat = static_cast<CStat*>(m_pObj->GetComponent("Com_Stat"));
	m_pTransform  = static_cast<CTransform*>(m_pObj->GetComponent("Com_Transform"));

	m_pOBB->SetSize(size);
	m_pOBB->SetCollisionType(_collisionType);
	CStat::STAT stat = m_pStat->GetStatInfo();
	stat.level = 1;
	stat.levelAtk = dmg;
	stat.maxHp = 1000000;
	stat.levelHp = 1000000;
	stat.hp = 1000000;
	m_pStat->SetStatInfo(stat);
	m_fDuration = _duration;

	m_pOBB->p_States = (CBasicCollider::STATES_ATK);
	pEngine->AddScriptObject(this, pEngine->GetCurSceneNumber());
	return S_OK;
}

HRESULT CObb::Initailze(_fvector position, _fvector _size, _float dmg, ID _collisionType, _float _duration, CTransform * pArg)
{
	CEngine* pEngine = CEngine::GetInstance();
	if(pArg)
		m_pTargetTransform = pArg;

	m_pObj = static_cast<CEmptyGameObject*>(pEngine->AddGameObjectToPrefab(pEngine->GetCurSceneNumber(), "Prototype_GameObject_OBBs", "O_OBBs", &XMMatrixTranslationFromVector(position)));
	m_pOBB = static_cast<CBasicCollider*>(m_pObj->GetComponent("Com_OBB"));
	m_pStat = static_cast<CStat*>(m_pObj->GetComponent("Com_Stat"));
	m_pTransform = static_cast<CTransform*>(m_pObj->GetComponent("Com_Transform"));

	_float3 size;
	XMStoreFloat3(&size, _size);
	m_pOBB->SetSize(size);
	m_pOBB->SetCollisionType(_collisionType);
	CStat::STAT stat = m_pStat->GetStatInfo();
	stat.level = 1;
	stat.levelAtk = dmg;
	stat.maxHp = 1000000;
	stat.levelHp = 1000000;
	stat.hp = 1000000;	
	m_pStat->SetStatInfo(stat);
	m_fDuration = _duration;

	m_pOBB->p_States = (CBasicCollider::STATES_ATK);

	pEngine->AddScriptObject(this, pEngine->GetCurSceneNumber());
	return S_OK;
}

void CObb::Update(_double deltaTime)
{
	m_pOBB->p_States = (CBasicCollider::STATES_ATK);

	if(m_pTargetTransform)
		m_pTransform->SetMatrix(m_pTargetTransform->GetWorldMatrix());
	
}

void CObb::LateUpdate(_double deltaTime)
{
	m_fDelta += (_float)deltaTime;
	if (m_fDuration < m_fDelta) {
		this->SetDead();
		m_pObj->SetDead();
	}
}

void CObb::Render()
{
}

void CObb::SetSize(_fvector _size)
{
	_float3 size;
	XMStoreFloat3(&size, _size);
	this->SetSize(size);
}

void CObb::SetSize(_float3 _size)
{
	m_pOBB->SetSize(_size);
}

void CObb::SetupDead()
{
	this->SetDead();
	if(m_pObj)
		m_pObj->SetDead();
}

void CObb::SetPosision(_float3 _pos)
{
	m_pObj->SetPosition(_pos);
}

void CObb::SetMatrix(_fmatrix matWorld)
{
	m_pObj->SetWorldMatrix(matWorld);
}

void CObb::SetMatrix(_float4x4 matWorld)
{
	m_pObj->SetWorldMatrix(matWorld);
}

CObb * CObb::Create(_float3 position, _float3 size, _float dmg, ID _collisionType, _float _duration, CTransform * pTarget)
{
	CObb*	obj = new CObb();
	if (FAILED(obj->Initailze(position, size, dmg, _collisionType, _duration, pTarget))) {
		SAFE_RELEASE(obj);
		return nullptr;
	}
	return obj;
}

CObb * CObb::Create(_fvector position, _fvector size, _float dmg, ID _collisionType, _float _duration, CTransform * pTarget)
{
	CObb*	obj = new CObb();
	if (FAILED(obj->Initailze(position, size, dmg, _collisionType, _duration, pTarget))) {
		SAFE_RELEASE(obj);
		return nullptr;
	}
	return obj;
}


void CObb::Free()
{
}
