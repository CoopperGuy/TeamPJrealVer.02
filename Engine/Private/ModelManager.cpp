#include "EnginePCH.h"
#include "..\Public\ModelManager.h"
#include "Engine.h"
#include "InstanceModel.h"

IMPLEMENT_SINGLETON(CModelManager)
static atomic<int> iCurNumThread = 0;
static int iCurInNumThread = 0;
unsigned int APIENTRY ThreadCloneModel(void* pArg)
{
	CModelManager::MODELLOADDESC* desc = (CModelManager::MODELLOADDESC*)pArg;

	iCurNumThread.fetch_add(1);
	CModelManager::GetInstance()->CloneModelThread(
		desc->pObj, desc->pMeshFilePath, desc->pMeshFileName, desc->pShaderFilePath, desc->pEffectFilePath,
		desc->bMeshCollider, desc->pArg, desc->bEquipment); 

	SafeDelete(desc);
	iCurNumThread.fetch_sub(1);


	return 0;
}


CModelManager::CModelManager()
{
	InitializeCriticalSection(&m_CS);
	InitializeCriticalSection(&m_WaitCS);
	m_maxThread = std::thread::hardware_concurrency() + 1;

}

void CModelManager::Free()
{
	// Delete prototypes
	for (auto& model : m_mapModel) {
		SafeRelease(model.second);
		//cout << "model delete " << model.second << "\n";
	}
	m_Threads.clear();
	m_mapModel.clear();
	m_CurCloningObj.clear();
	SafeRelease(m_pDissolveTex);

	DeleteCriticalSection(&m_CS);
	DeleteCriticalSection(&m_WaitCS);


}

void CModelManager::CloneModel(CGameObject* pObj, string pMeshFilePath, string pMeshFileName, string pShaderFilePath, string pEffectFilePath, _bool meshCollider, void* pArg, _bool bEquipment)
{
	while (m_maxThread < iCurNumThread.load())
		Sleep(300);
	
	MODELLOADDESC* desc = new MODELLOADDESC(pObj, pMeshFilePath, pMeshFileName, pShaderFilePath, pEffectFilePath, meshCollider, pArg, m_CS, bEquipment);
	std::lock_guard<std::mutex> lock(m_Wait);
	m_Threads.emplace_back(std::thread(ThreadCloneModel, desc));
}




void CModelManager::CloneModelThread(CGameObject* pObj, string pMeshFilePath, string pMeshFileName, string pShaderFilePath, string pEffectFilePath, _bool meshCollider, void * pArg, _bool bEquipment)
{
	if (nullptr == m_pDissolveTex)
	{
		m_pDissolveTex = CTexture::Create(CEngine::GetInstance()->GetDevice(),
			CEngine::GetInstance()->GetDeviceContext(),
			CTexture::TYPE_TGA,
			"..\\..\\Assets\\Textures\\Dissolve\\Dissolve.tga");
	}
	CTransform*	transform;
	_matrix pivotMatrix = XMMatrixIdentity();
	if (nullptr != pArg) {
		transform = static_cast<CTransform*>(pArg);
		_vector scale, rotation, position;
		XMMatrixDecompose(&scale, &rotation, &position, transform->GetWorldMatrix());
		pivotMatrix = XMMatrixScalingFromVector(scale);
	}


	
	ID3D11Device * pDevice = CEngine::GetInstance()->GetDevice();
	ID3D11DeviceContext * pDeviceContext = CEngine::GetInstance()->GetDeviceContext();

	string fullPath = pMeshFilePath + pMeshFileName;

	if (m_mapModel.find(fullPath) == m_mapModel.end())
	{
		CModel* pModel = CModel::Create(pDevice, pDeviceContext);
		pModel->SetMeshCollider(meshCollider);
		pModel->SetLinkEquip(bEquipment);
		pModel->CreateBuffer(pMeshFilePath, pMeshFileName, pShaderFilePath, pEffectFilePath, pivotMatrix);
		std::lock_guard<std::mutex> locks(m_Mesh_Jobs);
		if (m_mapModel.find(fullPath) != m_mapModel.end()){
			SafeRelease(pModel);
			cout << "/////////////Same Model Inputed////////////\n";
		}
		else {
			m_mapModel.emplace(make_pair(fullPath, pModel));
		}

		auto& iter = m_CurCloningObj.find(fullPath);
		if (iter != m_CurCloningObj.end())
			iter->second = false;
	}
	// return cloned object
	pObj->AddModelComponent(0, m_mapModel[fullPath]->Clone(pArg));
}

void CModelManager::CloneModelThreadIns(CGameObject * pObj, string pMeshFilePath, string pMeshFileName, string pShaderFilePath, string pEffectFilePath, _bool meshCollider, _bool HasIns, void * pArg)
{
	if (nullptr == m_pDissolveTex)
	{
		m_pDissolveTex = CTexture::Create(CEngine::GetInstance()->GetDevice(),
			CEngine::GetInstance()->GetDeviceContext(),
			CTexture::TYPE_TGA,
			"..\\..\\Assets\\Textures\\Dissolve\\Dissolve.DDS");
	}
	CTransform*	transform;
	_matrix pivotMatrix = XMMatrixIdentity();
	if (nullptr != pArg) {
		transform = static_cast<CTransform*>(pArg);
		_vector scale, rotation, position;
		XMMatrixDecompose(&scale, &rotation, &position, transform->GetWorldMatrix());
		pivotMatrix = XMMatrixScalingFromVector(scale);
	}



	ID3D11Device * pDevice = CEngine::GetInstance()->GetDevice();
	ID3D11DeviceContext * pDeviceContext = CEngine::GetInstance()->GetDeviceContext();

	string fullPath = pMeshFilePath + pMeshFileName;

	if (m_mapModel.find(fullPath) == m_mapModel.end())
	{
		CModel* pModel = CModel::Create(pDevice, pDeviceContext);
		pModel->SetMeshCollider(meshCollider);

		pModel->Set_Instancing(HasIns);

		pModel->CreateBuffer(pMeshFilePath, pMeshFileName, pShaderFilePath, pEffectFilePath, pivotMatrix);
		m_mapModel.emplace(fullPath, pModel);

		auto& iter = m_CurCloningObj.find(fullPath);
		if (iter != m_CurCloningObj.end())
			iter->second = false;
	}

	// return cloned object
	pObj->AddModelComponent(0, m_mapModel[fullPath]->Clone(pArg));
}


void CModelManager::WaitThreads()
{
	
	for (auto& iter : m_Threads) {
		iter.join();
	}
	m_Threads.clear();
}

void CModelManager::CreaetModel(string pMeshFilePath, string pMeshFileName, string pShaderFilePath, void * pArg)
{
	if (nullptr == m_pDissolveTex)
	{
		m_pDissolveTex = CTexture::Create(CEngine::GetInstance()->GetDevice(),
			CEngine::GetInstance()->GetDeviceContext(),
			CTexture::TYPE_TGA,
			"..\\..\\Assets\\Textures\\Dissolve\\Dissolve.tga");
	}
	ID3D11Device * pDevice = CEngine::GetInstance()->GetDevice();
	ID3D11DeviceContext * pDeviceContext = CEngine::GetInstance()->GetDeviceContext();

	string fullPath = pMeshFilePath + pMeshFileName;

	if (m_mapModel.find(fullPath) == m_mapModel.end())
	{
		CModel* pModel = CModel::Create(pDevice, pDeviceContext);
		pModel->CreateBuffer(pMeshFilePath, pMeshFileName, pShaderFilePath, "", XMMatrixIdentity());
		m_mapModel.emplace(fullPath, pModel);

		auto& iter = m_CurCloningObj.find(fullPath);
		if (iter != m_CurCloningObj.end())
			iter->second = false;
	}
}

CModel * CModelManager::GetCloneModel(string fullPathName, void* pArg)
{
	if (m_mapModel.find(fullPathName) == m_mapModel.end())
	{
		return nullptr;
	}

	return static_cast<CModel*>(m_mapModel[fullPathName]->Clone(pArg));
}
