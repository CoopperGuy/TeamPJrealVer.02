#include "EnginePCH.h"
#include "..\Public\ModelManager.h"
#include "Engine.h"
#include "InstanceModel.h"

IMPLEMENT_SINGLETON(CModelManager)
static int iCurNumThread = 0;
static int iCurInNumThread = 0;
unsigned int APIENTRY ThreadCloneModel(void* pArg)
{
	CModelManager::MODELLOADDESC* desc = (CModelManager::MODELLOADDESC*)pArg;

	++iCurNumThread;

	CModelManager::GetInstance()->CloneModelThread(
		desc->pObj, desc->pMeshFilePath, desc->pMeshFileName, desc->pShaderFilePath, desc->pEffectFilePath,
		desc->bMeshCollider, desc->pArg, desc->bEquipment); 

	SafeDelete(desc);
	--iCurNumThread;


	return 0;
}


CModelManager::CModelManager()
{
	InitializeCriticalSection(&m_CS);
	InitializeCriticalSection(&m_WaitCS);

}

void CModelManager::Free()
{
	// Delete prototypes
	for (auto& model : m_mapModel) {
		SafeRelease(model.second);
		//cout << "model delete " << model.second << "\n";
	}

	m_mapModel.clear();

	SafeRelease(m_pDissolveTex);

	DeleteCriticalSection(&m_CS);
	DeleteCriticalSection(&m_WaitCS);


}

void CModelManager::CloneModel(CGameObject* pObj, string pMeshFilePath, string pMeshFileName, string pShaderFilePath, string pEffectFilePath, _bool meshCollider, void* pArg, _bool bEquipment)
{
	while (10 < iCurNumThread)
		Sleep(100);
	
	string fullPath = pMeshFilePath + pMeshFileName;
	auto& iter = m_CurCloningObj.find(fullPath);
	if (iter != m_CurCloningObj.end())
	{
		while(iter->second == true)
			Sleep(100);
	}
	else
		m_CurCloningObj.emplace(fullPath, true);

	//if (m_CurCloningObj.find(fullPath) != m_CurCloningObj.end())
	//{
	//	while (iter->second != true)
	//		Sleep(1000);
	//}
	//else
	//{
	//	m_CurCloningObj.emplace(fullPath, true);
	//}

	MODELLOADDESC* desc = new MODELLOADDESC(pObj, pMeshFilePath, pMeshFileName, pShaderFilePath, pEffectFilePath, meshCollider, pArg, m_CS, bEquipment);
	thread_handles.emplace_back((HANDLE)_beginthreadex(nullptr, 0, ThreadCloneModel, desc, 0, nullptr));
}

void CModelManager::CloneModelIns(CGameObject* pObj, string pMeshFilePath, string pMeshFileName, string pShaderFilePath, string pEffectFilePath, _bool meshCollider, _bool HasIns, _bool Building, _bool BG, _bool Ground, void* pArg)
{
	while (10 < iCurNumThread)
		Sleep(100);

	string fullPath = pMeshFilePath + pMeshFileName;
	auto& iter = m_CurCloningObj.find(fullPath);
	if (iter != m_CurCloningObj.end())
	{
		while (iter->second == false)
			Sleep(100);
	}
	else
		m_CurCloningObj.emplace(fullPath, true);

	//if (m_CurCloningObj.find(fullPath) != m_CurCloningObj.end())
	//{
	//	while (iter->second != true)
	//		Sleep(1000);
	//}
	//else
	//{
	//	m_CurCloningObj.emplace(fullPath, true);
	//}

	MODELLOADDESC* desc = new MODELLOADDESC(pObj, pMeshFilePath, pMeshFileName, pShaderFilePath, pEffectFilePath, meshCollider, pArg, m_CS);
	thread_handles.emplace_back((HANDLE)_beginthreadex(nullptr, 0, ThreadCloneModel, desc, 0, nullptr));
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
		EnterCriticalSection(&m_CS);
		m_mapModel.emplace(fullPath, pModel);
		LeaveCriticalSection(&m_CS);

		auto& iter = m_CurCloningObj.find(fullPath);
		if (iter != m_CurCloningObj.end())
			iter->second = false;
	}
	// return cloned object
	EnterCriticalSection(&m_CS);
	pObj->AddModelComponent(0, m_mapModel[fullPath]->Clone(pArg));
	LeaveCriticalSection(&m_CS);
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
	EnterCriticalSection(&m_WaitCS);
	while (!thread_handles.empty())
	{
		list<HANDLE> threads_left;
		auto& iter = thread_handles.begin();
		for (; iter != thread_handles.end();) {
			DWORD rc = WaitForSingleObject(*iter, INFINITE);

			if (rc == WAIT_OBJECT_0)
			{
				CloseHandle(*iter);
				iter = thread_handles.erase(iter);
				continue;

			}
			else if (rc == WAIT_TIMEOUT)
			{
				threads_left.emplace_back(*iter);
				iter = thread_handles.erase(iter);
				continue;
			}
			else
			{
				CloseHandle(*iter);
				iter = thread_handles.erase(iter);
				continue;

			}

			iter++;
		}
	}
	LeaveCriticalSection(&m_WaitCS);

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
