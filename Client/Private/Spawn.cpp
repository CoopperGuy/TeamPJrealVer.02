#include "stdafx.h"
#include "..\Public\Spawn.h"
#include "Wolf.h"

BEGIN(Client)
void SpawnObject(CSpawn* script) {
	if (script->GetCreateCnt() <= 0)
		return;
	script->SetCreateCnt(0);
	script->SpawnObjects();
	script->SetCreateCnt(1);
}

END

USING(Client)

CSpawn::CSpawn()
{
}

HRESULT CSpawn::Initailze(CGameObject * pArg)
{
	if (pArg) {
		m_pThis = static_cast<CEmptyGameObject*>(pArg);
		m_pSpawn = static_cast<CSpawner*>(m_pThis->GetComponent("Com_Spawn"));
		m_pSpawnPos = static_cast<CTransform*>(m_pThis->GetComponent("Com_Transform"));
		m_SpawnTime = m_pSpawn->GetSpawnInfo().spawnTime;
	}
	CEngine::GetInstance()->AddScriptObject(this, CEngine::GetInstance()->GetCurSceneNumber());
	return S_OK;

}

void CSpawn::Update(_double deltaTime)
{
	if (m_pSpawn) {
		if (!m_bisSpawned) {
			std::thread spawnObj(SpawnObject, this);
			spawnObj.detach();
		}
		if (m_iSpawnScript && m_iSpawnScript->IsDead()) {
			m_iSpawnScript = nullptr;
		}
		if (!m_iSpawnScript) {
			m_SpawnDelta += deltaTime;
			if (m_SpawnDelta > m_SpawnTime) {
				m_bisSpawned = false;
				m_SpawnDelta = 0;
			}
		}
	}


}

void CSpawn::LateUpdate(_double deltaTime)
{
}

void CSpawn::Render()
{
}

void CSpawn::SpawnObjects()
{
	CSpawner::SPAWNER info = m_pSpawn->GetSpawnInfo();
	_float3 pos;
	XMStoreFloat3(&pos, m_pSpawnPos->GetState(CTransform::STATE_POSITION));
	if (info.prefabName == "WOLF") {
		m_iSpawnScript = CWolf::Create(nullptr,pos);
	}
	m_bisSpawned = true;
}

CSpawn * CSpawn::Create(CGameObject * pTarget)
{
	CSpawn*	obj = new CSpawn();
	if (FAILED(obj->Initailze(pTarget))) {
		SAFE_RELEASE(obj);
		return nullptr;
	}
	return obj;
}

void CSpawn::Free()
{
}

