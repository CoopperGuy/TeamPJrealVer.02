#pragma once

#include "Component.h"

BEGIN(Engine)
class CEmptyGameObject;

class ENGINE_DLL CSpawner :
	public CComponent
{
public:
	typedef struct tagSpawn {
		_float	spawnTime = 60.f;
		_float	spawnDelta = 0.f;
		_bool	isSpawned = false;
		string prefabName;
	}SPAWNER;
private:
	explicit CSpawner(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	explicit CSpawner(const CSpawner& rhs);
	virtual ~CSpawner() = default;

public:
	virtual HRESULT InitializePrototype();
	virtual HRESULT Initialize(void* pArg);
public:
	void Update(_double deltaTime);
public:
	void SetSpawnInfo(SPAWNER _info) { m_tSpawn = _info; }
public:
	SPAWNER GetSpawnInfo() { return m_tSpawn; }
private:
	CEmptyGameObject*		m_pSpawnedObject = nullptr;
	SPAWNER					m_tSpawn;
;
public:
	static CSpawner* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	virtual CComponent* Clone(void* pArg) override;
	virtual void Free() override;
};

END