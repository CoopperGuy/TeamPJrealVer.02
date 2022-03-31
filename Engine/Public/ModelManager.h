#pragma once
#include "Base.h"
#include "Model.h"
#include "GameObject.h"
BEGIN(Engine)
class CModel;
class CModelManager final : public CBase
{
public:
	typedef struct tagModelDesc
	{
		tagModelDesc(CGameObject* _pObj, string _pMeshFilePath,
			string _pMeshFileName, string _pShaderFilePath, string _pEffectFilePath,
			_bool _meshCollider, void* _pArg, CRITICAL_SECTION _cs, _bool bEquipment = false)
			: pObj(_pObj), pMeshFilePath(_pMeshFilePath), pMeshFileName(_pMeshFileName),
			pShaderFilePath(_pShaderFilePath), pEffectFilePath(_pEffectFilePath), bMeshCollider(_meshCollider), pArg(_pArg)
			,cs(_cs), bEquipment(bEquipment) {}

		CGameObject*	pObj;
		string			pMeshFilePath;
		string			pMeshFileName;
		string			pShaderFilePath;
		string			pEffectFilePath;
		_bool			bMeshCollider; 
		void*			pArg;
		CRITICAL_SECTION cs;
		_bool			bEquipment;
	}MODELLOADDESC;

	DECLARE_SINGLETON(CModelManager)
private:
	CModelManager();
	virtual ~CModelManager() = default;
	virtual void Free() override;

public:
	void CloneModel(CGameObject* pObj, string pMeshFilePath, string pMeshFileName, string pShaderFilePath = "../../Assets/Shader/Shader_Mesh.fx", string pEffectFilePath = "", _bool meshCollider = false, void* pArg = nullptr, _bool bEquipment = false);
	void CloneModelIns(CGameObject * pObj, string pMeshFilePath, string pMeshFileName, string pShaderFilePath, string pEffectFilePath, _bool meshCollider, _bool HasIns, _bool Building, _bool BG, _bool Ground, void * pArg);
	void CloneModelThread(CGameObject* pObj, string pMeshFilePath, string pMeshFileName, string pShaderFilePath = "../../Assets/Shader/Shader_Mesh.fx", string pEffectFilePath = "", _bool meshCollider = false, void* pArg = nullptr, _bool bEquipment = false);
	void CloneModelThreadIns(CGameObject* pObj, string pMeshFilePath, string pMeshFileName, 
		string pShaderFilePath = "../../Assets/Shader/Shader_Mesh.fx", string pEffectFilePath = "", 
		_bool meshCollider = false, _bool HasIns = true, void* pArg = nullptr);
	void WaitThreads();
	void CreaetModel(string pMeshFilePath, string pMeshFileName, string pShaderFilePath = "../../Assets/Shader/Shader_Mesh.fx", void* pArg = nullptr);
	
	CTexture* GetDissoveTex() { return m_pDissolveTex; }
public:
	CModel* GetCloneModel(string fullPathName, void* pArg);
private:
	unordered_map<string, CModel*> m_mapModel;
private:
	class CTexture* m_pDissolveTex = nullptr;
	CModel* m_pTargetModel = nullptr;

	list<HANDLE>				thread_handles;
	vector<std::thread>			m_Threads;
	unordered_map<string, _bool>			m_CurCloningObj;
	CRITICAL_SECTION		m_CS;
	CRITICAL_SECTION		m_WaitCS;
	unordered_map<string, CRITICAL_SECTION> m_keys;

	size_t					m_maxThread = 6;

	std::mutex				m_Mesh_Jobs;
	std::mutex				m_Wait;
};

END