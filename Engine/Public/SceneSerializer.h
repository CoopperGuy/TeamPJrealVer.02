#pragma once
//#include "Engine_Defines.h"
#include "Engine.h"
#include "GameObject.h"

BEGIN(Engine)

class ENGINE_DLL CSceneSerializer
{
public:
	// TODO: Constructor must take a scene as parameter
	CSceneSerializer();

public:
	void Serialize(const string& filePath);
	bool Deserialize(const string& filePath, _uint curScene = 0);
	void SerializeObject(YAML::Emitter& out, CGameObject* obj);
	void SerializeUI(YAML::Emitter& out, CGameObject* obj);
	void SerializeEffect(YAML::Emitter& out, CGameObject* obj);
	void SerializeCamera(YAML::Emitter& out, CGameObject* obj);

	void SerializePrefab(CGameObject* obj);
	void DeserializePrefab();
	HRESULT CreatePrefab(string pPrototypeTag, YAML::Node data, _uint curScene = 0);
	CGameObject* SpawnPrefab(YAML::Node data, _uint curScene = 0);

	CGameObject* DeserializeUI(YAML::Node& obj, _bool bSpawn = false, _uint curScene = 0);
	CGameObject* DeserializeObject(YAML::Node& obj,_bool bSpawn = false, _uint curScene = 0);
	CGameObject* DeserializeCamera(YAML::Node& obj, _bool bSpawn = false, _uint curScene = 0);
	CGameObject* DeserializeEffect(YAML::Node& obj, _bool bSpawn = false, _uint curScene = 0);

	CGameObject* DeserializePrototypeEffect(string pPrototypeTag, YAML::Node& obj, _bool bSpawn = false, _uint curScene = 0);
	CGameObject* DeserializePrototypeGameObject(string pPrototypeTag, YAML::Node& obj, _bool bSpawn = false, _uint curScene = 0);

	CGameObject* NoneInstancingObject(YAML::Node& obj, _bool bSpawn = false, _uint curScene = 0);

	void DeserializeInstancModel(_uint curScene);
	void InstaingDesc(CGameObject* obj, YAML::Emitter & out);
	void InsModelInsertMap(YAML::Node& obj);

private:
	CEngine*				m_pEngine = nullptr;
	ID3D11Device*			m_pDevice = nullptr;
	ID3D11DeviceContext*	m_pDeviceContext = nullptr;

	multimap<string, _float4x4>InstanceAllList = {};
};

END