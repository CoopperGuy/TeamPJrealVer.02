#include "EnginePCH.h"
#include "..\public\Engine.h"
#include "TimerManager.h"
#include "GraphicDevice.h"
#include "SceneSerializer.h"
#include "InputManager.h"
#include "ModelManager.h"
#include "QuestData.h"

static string itemDBPath = "../../DB/DB.xlsx";
std::mutex PhysxMutex;
std::mutex GameObjectMutex;

BEGIN(Engine)
void DBLoadThread() {
	CXlsxRead::GetInstance()->Ready_XlsxLoader(itemDBPath);
	CQuestManager::GetInstance()->LoadQeustData();
	CTalkManager::GetInstance()->LoadTalkData();
}
END

IMPLEMENT_SINGLETON(CEngine)

CEngine::CEngine()
	: m_pTimerManager(CTimerManager::GetInstance())
	, m_pGraphicDevice(CGraphicDevice::GetInstance())
	, m_pSceneManager(CSceneManager::GetInstance())
	, m_pGameObjectManager(CGameObjectManager::GetInstance())
	, m_pPxManager(CPxManager::GetInstance())
	, m_pInputManager(CInputManager::GetInstance())
	, m_pComponentManager(CComponentManager::GetInstance())
	, m_pImGuiManager(CImGuiManager::GetInstance())
	, m_pSoundManager(CSound::GetInstance())
	, m_pPipeline(CPipeline::GetInstance())
	, m_pScriptObjectManager(CScriptObjectManager::GetInstance())
	, m_pLightManager(CLightManager::GetInstance())
	, m_pModelManager(CModelManager::GetInstance())
	, m_pTargetManager(CTargetManager::GetInstance())
	, m_pFrustum(CFrustum::GetInstance())
	, m_pXlsxRead(CXlsxRead::GetInstance())
	, m_pQuestManager(CQuestManager::GetInstance())
	, m_pTalkManager(CTalkManager::GetInstance())
	, m_pAlretManager(CAlretManager::GetInstance())
	, m_pInput_Device(CInput_Device::GetInstance())
{
	SafeAddRef(m_pTargetManager);
	SafeAddRef(m_pLightManager);
	SafeAddRef(m_pScriptObjectManager);
	SafeAddRef(m_pPipeline);
	SafeAddRef(m_pTimerManager);
	SafeAddRef(m_pInputManager);
	SafeAddRef(m_pGraphicDevice);
	SafeAddRef(m_pSceneManager);
	SafeAddRef(m_pPxManager);
	SafeAddRef(m_pGameObjectManager);
	SafeAddRef(m_pComponentManager);
	SafeAddRef(m_pImGuiManager);
	SafeAddRef(m_pSoundManager);
	SafeAddRef(m_pModelManager);
	SafeAddRef(m_pFrustum);
	SafeAddRef(m_pXlsxRead);
	SafeAddRef(m_pQuestManager);
	SafeAddRef(m_pTalkManager);
	SafeAddRef(m_pInput_Device);
	SafeAddRef(m_pAlretManager);
}

#pragma region TIMER_MANAGER

HRESULT CEngine::Initialize(_uint iNumScenes, USAGE usage)
{
	m_eUsage = usage;

	if (nullptr == m_pGameObjectManager)
		return E_FAIL;

	if (FAILED(m_pGameObjectManager->ReserveManager(iNumScenes)))
		return E_FAIL;

	if (FAILED(m_pScriptObjectManager->ReserveManager(iNumScenes)))
		return E_FAIL;

	if (FAILED(m_pComponentManager->ReserveManager(iNumScenes)))
		return E_FAIL;

	if (FAILED(m_pFrustum->Ready_FrustumInProjSpace()))
		return E_FAIL;

	//if (FAILED(m_pXlsxRead->Ready_XlsxLoader(itemDBPath)))
	//	return E_FAIL;
	std::thread dbLoad(DBLoadThread);
	dbLoad.detach();

	return S_OK;
}

_uint CEngine::Update(_double dTimeDelta)
{
	m_pInput_Device->SetUp_InputDeviceState();
	m_pInputManager->Update();

	m_pScriptObjectManager->Update(dTimeDelta);
	m_pGameObjectManager->Update(dTimeDelta);

	m_pLightManager->Update(dTimeDelta);
	m_pFrustum->Transform_ToWorldSpace();

	m_pScriptObjectManager->LateUpdate(dTimeDelta);
	m_pGameObjectManager->LateUpdate(dTimeDelta);
	
	return _uint();
}

void CEngine::ReleaseEngine()
{
	if (0 != CEngine::GetInstance()->DestroyInstance())
		MSG_BOX("Failed to Delete CEngine");

	if (0 != CSound::GetInstance()->DestroyInstance())
		MSG_BOX("Failed to Delete CSound");

	if (0 != CImGuiManager::GetInstance()->DestroyInstance())
		MSG_BOX("Failed to Delete CImGuiManager");

	if (0 != CInputManager::GetInstance()->DestroyInstance())
		MSG_BOX("Failed to Delete CInputManager");

	if (0 != CTimerManager::GetInstance()->DestroyInstance())
		MSG_BOX("Failed to Delete CTimer_Manager");

	if (0 != CSceneManager::GetInstance()->DestroyInstance())
		MSG_BOX("Failed to Delete CSceneManager");

	if (0 != CScriptObjectManager::GetInstance()->DestroyInstance())
		MSG_BOX("Failed to Delete CScriptObjectManager");

	if (0 != CGameObjectManager::GetInstance()->DestroyInstance())
		MSG_BOX("Failed to Delete CGameObjectManager");

	if (0 != CPipeline::GetInstance()->DestroyInstance())
		MSG_BOX("Failed to Delete CPipeline");

	if (0 != CModelManager::GetInstance()->DestroyInstance())
		MSG_BOX("Failed to Delete CModelManager");

	if (0 != CComponentManager::GetInstance()->DestroyInstance())
		MSG_BOX("Failed to Delete CComponentManager");

	if (0 != CPxManager::GetInstance()->DestroyInstance())
		MSG_BOX("Failed to Delete CPhysX");

	if (0 != CLightManager::GetInstance()->DestroyInstance())
		MSG_BOX("Failed to Delete CLightManager");

	if (0 != CFrustum::GetInstance()->DestroyInstance())
		MSG_BOX("Failed to Delete CFrustum");

	if (0 != CQuestManager::GetInstance()->DestroyInstance())
		MSG_BOX("Failed to Delete CQuestManager");

	if (0 != CTalkManager::GetInstance()->DestroyInstance())
		MSG_BOX("Failed to Delete CTalkManager");

	if (0 != CXlsxRead::GetInstance()->DestroyInstance())
		MSG_BOX("Failed to Delete CXlsxRead");

	if (0 != CTargetManager::GetInstance()->DestroyInstance())
		MSG_BOX("Failed to Delete CTargetManager");

	if (0 != CInput_Device::GetInstance()->DestroyInstance())
		MSG_BOX("Failed to Delete CInput_Device");

	if (0 != CAlretManager::GetInstance()->DestroyInstance())
		MSG_BOX("Failed to Delete CAlretManager");

	if (0 != CGraphicDevice::GetInstance()->DestroyInstance())
		MSG_BOX("Failed to Delete CGraphic_Device");


	
}

HRESULT CEngine::ReadyDevice(HWND hWnd, _uint iWidth, _uint iHeight , HINSTANCE hInst)
{
	m_pGraphicDevice->ReadyGraphicDevice(hWnd, iWidth, iHeight);
	if (FAILED(m_pInput_Device->Ready_Input_Device(hInst, hWnd)))
		return E_FAIL;
	// m_pPxManager->Initialize();
	InitializeImGui(hWnd, GetDevice(), GetDeviceContext());
	return S_OK;
}

ID3D11Device * CEngine::GetDevice()
{
	return m_pGraphicDevice->GetDevice();
}

ID3D11DeviceContext * CEngine::GetDeviceContext()
{
	return m_pGraphicDevice->GetDeviceContext();
}

IDXGISwapChain * CEngine::GetSwapChain()
{
	return m_pGraphicDevice->GetSwapChain();
}

ID3D11RenderTargetView * CEngine::GetRenderTargetView()
{
	return m_pGraphicDevice->GetRenderTargetView();
}

ID3D11DepthStencilView * CEngine::GetDepthStencilRenderTargetView()
{
	return m_pGraphicDevice->GetDepthStencilRenderTargetView();
}


SpriteBatch * CEngine::GetSpriteBatch()
{
	return m_pGraphicDevice->GetSpriteBatch();;
}

SpriteFont * CEngine::GetSpriteFont()
{
	return m_pGraphicDevice->GetSpriteFont();;
}

SpriteFont * CEngine::GetSpriteKoreanFont()
{
	return m_pGraphicDevice->GetSpriteKoreanFont();
}

_float2 CEngine::GetCurrentWindowSize()
{
	return m_pGraphicDevice->GetCurrentWindowSize();
}

XMMATRIX CEngine::GetViewMatrix()
{
	return m_pGraphicDevice->GetViewMatrix();
}

XMMATRIX CEngine::GetProjectionMatrix()
{
	return m_pGraphicDevice->GetProjectionMatrix();
}


HRESULT CEngine::ClearBackBufferView(_float4 vColor)
{
	if (nullptr == m_pGraphicDevice)
		return E_FAIL;

	return m_pGraphicDevice->Clear_BackBufferView(vColor);
}

HRESULT CEngine::ClearDepthStencilView(_float fDepth, _uint iStencil)
{
	if (nullptr == m_pGraphicDevice)
		return E_FAIL;

	return m_pGraphicDevice->Clear_DepthStencilView(fDepth, iStencil);
}

void CEngine::SetRTV()
{
	return m_pGraphicDevice->SetRTV();
}

void CEngine::SetRTV2()
{
	return m_pGraphicDevice->SetRTV2();
}

void CEngine::Render()
{
	if (nullptr == m_pGraphicDevice)
		return;

	return m_pGraphicDevice->Render();
}

void CEngine::RenderClient()
{
	if (nullptr == m_pGraphicDevice)
		return;

	return m_pGraphicDevice->RenderClient();
}



HRESULT CEngine::Present()
{
	if (nullptr == m_pGraphicDevice)
		return E_FAIL;

	return m_pGraphicDevice->Present();
}

HRESULT CEngine::ChangeResolution(_uint iWidth, _uint iHeight)
{
	return m_pGraphicDevice->ChangeResolution(iWidth, iHeight);
}

void CEngine::ChangeProj(_uint iWidth, _uint iHeight)
{
	m_pGraphicDevice->ChangeProj(iWidth, iHeight);
}


HRESULT CEngine::AddTimers(string pTimerTag)
{
	if (nullptr == m_pTimerManager)
		return E_FAIL;

	return m_pTimerManager->AddTimers(pTimerTag);
}

_double CEngine::ComputeDeltaTime(string pTimerTag)
{
	if (nullptr == m_pTimerManager)
		return 0.0;

	return m_pTimerManager->ComputeDeltaTime(pTimerTag);
}

void CEngine::DeserializePrefab()
{
	CSceneSerializer serializer;
	serializer.DeserializePrefab();
}

void CEngine::SerializeScene(const string & filePath)
{
	CSceneSerializer serializer;
	serializer.Serialize(filePath);
}

void CEngine::DeserializeScene(const string & filePath, _uint scnenNumber)
{
	CSceneSerializer serializer;
	serializer.Deserialize(filePath,scnenNumber);
}

HRESULT CEngine::SetUpCurrentScene(CScene * pCurrentScene, _uint sceneNum)
{
	if (nullptr == m_pSceneManager)
		return E_FAIL;

	return m_pSceneManager->SetUpCurrentScene(pCurrentScene, sceneNum);
}

void CEngine::SetCurSceneNumber(_uint idx)
{
	if (nullptr == m_pSceneManager)
		return;

	return m_pSceneManager->SetCurSceneNumber(idx);
}

_uint CEngine::UpdateScene(_double DeltaTime)
{	
	if (nullptr == m_pSceneManager)
		return -1;

	return m_pSceneManager->UpdateScene(DeltaTime);
}

HRESULT CEngine::RenderScene()
{
	if (nullptr == m_pSceneManager)
		return E_FAIL;

	return m_pSceneManager->RenderScene();
}

_uint CEngine::GetCurSceneNumber()
{
	if (nullptr == m_pSceneManager)
		return E_FAIL;

	return m_pSceneManager->GetCurSceneNumber();
}

void CEngine::WaitThread()
{
	if (m_pModelManager)
		m_pModelManager->WaitThreads();
}

HRESULT CEngine::AddPrototype(string pPrototypeTag, CGameObject * pPrototype)
{
	if (nullptr == m_pGameObjectManager)
		return E_FAIL;

	return m_pGameObjectManager->AddPrototype(pPrototypeTag, pPrototype);

}

CGameObject* CEngine::ClonePrototype(string pFindPrototypeTag, string pPrototypeTag, void* pArg)
{
	if (nullptr == m_pGameObjectManager)
		return nullptr;

	return m_pGameObjectManager->ClonePrototype(pFindPrototypeTag, pPrototypeTag, pArg);
}

CGameObject* CEngine::AddGameObject(_uint iSceneIndex, string pPrototypeTag, string pLayerTag, void * pArg)
{
	if (nullptr == m_pGameObjectManager)
		return nullptr;


	return m_pGameObjectManager->AddGameObject(iSceneIndex, pPrototypeTag, pLayerTag, pArg);
}

void CEngine::ClearGameObjectManager(_uint iSceneIndex)
{
	if (nullptr == m_pGameObjectManager)
		return;

	m_pGameObjectManager->Clear(iSceneIndex);
}

list<class CGameObject*> CEngine::GetGameObjectInLayer(_uint iSceneIndex, string pLayerTag)
{
	return m_pGameObjectManager->GetGameObjectInLayer(iSceneIndex, pLayerTag);
}

unordered_map<string, CLayer*>* CEngine::GetLayers()
{
	return m_pGameObjectManager->GetLayers();
}

void CEngine::AddGameObjectWithName(_uint iSceneIndex, string name, CGameObject * pObj)
{
	m_pGameObjectManager->AddGameObjectWithName(name, pObj, iSceneIndex);
}

void CEngine::AddGameObjectWithUUID(_uint iSceneIndex, uint64_t uuid, CGameObject * pObj)
{
	m_pGameObjectManager->AddGameObjectWithUUID(uuid, pObj, iSceneIndex);
}

CGameObject * CEngine::FindGameObjectWithName(_uint iSceneIndex, string name)
{
	return m_pGameObjectManager->FindGameObjectWithName(iSceneIndex, name);
}

CGameObject * CEngine::FindGameObjectWithUUID(_uint iSceneIndex, uint64_t uuid)
{
	return m_pGameObjectManager->FindGameObjectWithUUID(iSceneIndex, uuid);
}

void CEngine::AddPrefab(string name, YAML::Node node)
{
	m_pGameObjectManager->AddPrefab(name, node);
}

CGameObject* CEngine::SpawnPrefab(string name)
{
	return m_pGameObjectManager->SpawnPrefab(name);
}

HRESULT CEngine::CreatePrefab(string pPrototypeTag, string name)
{
	return m_pGameObjectManager->CreatePrefab(pPrototypeTag, name);
}

CGameObject* CEngine::AddGameObjectToPrefab(_uint iSceneIndex, string pPrototypeTag, string pLayerTag, void * pArg)
{
	if (m_pGameObjectManager == nullptr)
		return nullptr;

	return m_pGameObjectManager->AddGameObjectToPrefab(iSceneIndex, pPrototypeTag, pLayerTag, pArg);
}

void CEngine::AddOBBList(ID Type, CGameObject * _pGameObject, CBasicCollider* _obb)
{
	if (m_pGameObjectManager == nullptr)
		return;

	m_pGameObjectManager->AddOBBCollsionList(Type, _pGameObject, _obb);
}

void CEngine::CollsionCheck()
{
	if (m_pGameObjectManager == nullptr)
		return;

	m_pGameObjectManager->CollsionCheck();
}



IScriptObject * CEngine::AddScriptObject(IScriptObject * pObj, _uint iNumScenes)
{
	return m_pScriptObjectManager->AddObject(pObj, iNumScenes);
}

void CEngine::ClearScriptObject(_uint iSceneNum)
{
	m_pScriptObjectManager->ClearScriptObject(iSceneNum);
}



void CEngine::InitializeInput()
{
	m_pInputManager->Initialize();
}

void CEngine::UpdateInput()
{
	m_pInputManager->Update();
}

bool CEngine::IsKeyUp(_uint key)
{
	return m_pInputManager->IsKeyUp(key);
}

bool CEngine::IsKeyDown(_uint key)
{
	return m_pInputManager->IsKeyDown(key);
}

bool CEngine::IsKeyPressed(_uint key)
{
	return m_pInputManager->IsKeyPressed(key);
}

bool CEngine::IsKeyToggled(_uint key)
{
	return m_pInputManager->IsKeyToggled(key);
}

bool CEngine::IsMouseUp(DWORD mouse)
{
	return m_pInputManager->IsMouseUp(mouse);
}

bool CEngine::IsMouseDown(DWORD mouse)
{
	return m_pInputManager->IsMouseDown(mouse);
}

bool CEngine::IsMousePressed(DWORD mouse)
{
	return m_pInputManager->IsMousePressed(mouse);
}

_float3 CEngine::GetMouseMoveValue()
{
	return m_pInputManager->GetMouseMoveValue();
}

_float3 CEngine::GetMousePosition()
{
	return m_pInputManager->GetMousePosition();
}

vector<bitset<256>> CEngine::Get_Key(_uint iIndex)
{
	return m_pInputManager->Get_Key(iIndex);
}

void CEngine::InputProc(const HWND hWnd, const UINT message, const WPARAM wParam, const LPARAM lParam)
{
	m_pInputManager->InputProc(hWnd, message, wParam, lParam);
}

HRESULT CEngine::AddPrototype(_uint iSceneIndex, string pPrototypeTag, CComponent * pPrototype)
{
	if (nullptr == m_pComponentManager)
		return E_FAIL;

	return m_pComponentManager->AddPrototype(iSceneIndex, pPrototypeTag, pPrototype);
}

CComponent * CEngine::CloneComponent(_uint iSceneIndex, string pPrototypeTag, void * pArg)
{
	if (nullptr == m_pComponentManager)
		return nullptr;

	return m_pComponentManager->CloneComponent(iSceneIndex, pPrototypeTag, pArg);
}

void CEngine::ClearComponentManager(_uint iSceneIndex)
{
	if (nullptr == m_pComponentManager)
		return;

	m_pComponentManager->Clear(iSceneIndex);
}

void CEngine::CloneModel(CGameObject* pObj, string pMeshFilePath, string pMeshFileName, string pShaderFilePath, string pEffectFilePath, _bool meshCollider, void* pArg, _bool bEquipment)
{
	m_pModelManager->CloneModel(pObj, pMeshFilePath, pMeshFileName, pShaderFilePath, pEffectFilePath, meshCollider, pArg, bEquipment);
}


const LIGHTDESC * CEngine::GetLightDesc(_uint iIndex) const
{
	if (nullptr == m_pLightManager)
		return nullptr;

	return m_pLightManager->GetLightDesc(iIndex);
}

HRESULT CEngine::AddLight(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext, const LIGHTDESC & LightDesc)
{
	if (nullptr == m_pLightManager)
		return E_FAIL;

	return m_pLightManager->AddLight(pDevice, pDeviceContext, LightDesc);
}



HRESULT CEngine::Add_RenderTarget(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext, string pRenderTargetTag, _uint iWidth, _uint iHeight, DXGI_FORMAT Format, _float4 vClearColor)
{
	return m_pTargetManager->Add_RenderTarget(pDevice, pDeviceContext, pRenderTargetTag, iWidth, iHeight, Format, vClearColor);
}

HRESULT CEngine::Add_MRT(string pMRTTag, string pRenderTargetTag)
{
	return m_pTargetManager->Add_MRT(pMRTTag, pRenderTargetTag);
}

HRESULT CEngine::Begin_MRT(ID3D11DeviceContext * pDeviceContext, string pMRTTag)
{
	return m_pTargetManager->Begin_MRT(pDeviceContext, pMRTTag);
}

HRESULT CEngine::End_MRT(ID3D11DeviceContext * pDeviceContext)
{
	return m_pTargetManager->End_MRT(pDeviceContext);
}

HRESULT CEngine::Ready_DebugBuffer(string pTargetTag, _float fX, _float fY, _float fWidth, _float fHeight)
{
	return m_pTargetManager->Ready_DebugBuffer(pTargetTag, fX, fY, fWidth, fHeight);
}

HRESULT CEngine::Render_DebugBuffers(string pMRTTag)
{
	return m_pTargetManager->Render_DebugBuffers(pMRTTag);
}

ID3D11ShaderResourceView * CEngine::GetShaderResourceView(string pTargetTag)
{
	return m_pTargetManager->GetShaderResourceView(pTargetTag);
}


_matrix CEngine::GetTransform(CPipeline::TYPE eType)
{
	if (nullptr == m_pPipeline)
		return XMMatrixIdentity();

	return m_pPipeline->Get_Transform(eType);
}

_vector CEngine::GetCamPosition()
{
	if (nullptr == m_pPipeline)
		return XMVectorZero();

	return m_pPipeline->Get_CamPosition();
}

void CEngine::InitializeImGui(HWND hWnd, ID3D11Device * device, ID3D11DeviceContext * deviceContext)
{
	m_pImGuiManager->Initialize(hWnd, device, deviceContext);
}

void CEngine::UpdateImGui()
{
	m_pImGuiManager->Update();
}

void CEngine::AddWindow(string name, CImGuiWindow * window)
{
	m_pImGuiManager->AddWindow(name, window);
}

CImGuiWindow * CEngine::GetWindow(string name)
{
	return m_pImGuiManager->GetWindow(name);
}

void CEngine::PlaySoundW(string pSoundKey, CHANNELID eID)
{
	m_pSoundManager->PlaySound(pSoundKey, eID);
}

void CEngine::PlayBGM(string pSoundKey)
{
	m_pSoundManager->PlayBGM(pSoundKey);
}

void CEngine::StopSound(CHANNELID eID)
{
	m_pSoundManager->StopSound(eID);
}

void CEngine::StopAll()
{
	m_pSoundManager->StopAll();
}


void CEngine::PxInitialize()
{
	m_pPxManager->Initialize();
}

void CEngine::UpdatePx(_double dDeltaTime)
{
	m_pPxManager->Update(dDeltaTime);
}

PxPhysics * CEngine::GetPhysics()
{
	return m_pPxManager->GetPhysics();
}

PxMaterial * CEngine::GetMaterial()
{
	return m_pPxManager->GetMaterial();
}

PxControllerManager * CEngine::GetControllerManager()
{
	return m_pPxManager->GetControllerManager();
}

void CEngine::AddActor(PxRigidActor * pActor)
{
	m_pPxManager->AddActor(pActor);
}

void CEngine::AddAggregateActor(PxRigidActor * pActor)
{
	m_pPxManager->AddAggregateActor(pActor);
}

_bool CEngine::Raycast(_vector origin, _vector unitDir, _float maxDistance, PxRaycastBuffer & hit, PxQueryFilterData& filterData)
{
	return m_pPxManager->Raycast(origin, unitDir, maxDistance, hit, filterData);
}

_bool CEngine::Raycast(PxVec3 origin, PxVec3 unitDir, _float maxDistance, PxRaycastBuffer & hit, PxQueryFilterData & filterData)
{
	return m_pPxManager->Raycast(origin, unitDir, maxDistance, hit, filterData);
}

_bool CEngine::Raycast(_vector origin, _vector unitDir, _float maxDistance, PxRaycastBuffer & hit, PxQueryFilterData & filterData, CPxQueryFilters * _callback)
{
	return m_pPxManager->Raycast(origin, unitDir, maxDistance, hit, filterData, _callback);
}

_bool CEngine::Overlap(const _float3 & point, PxU32 layerMask, _float3 direciton)
{
	return m_pPxManager->Overlap(point, layerMask, direciton);
}

_bool CEngine::isInFrustum(_fvector Position,_float range)
{
	if (m_pFrustum == nullptr)
		return false;
	return m_pFrustum->isIn(Position, range);
}

BOOL CEngine::MouseRayCast(_float3 & out, HWND hWnd, _uint iWinCX, _uint iWinCY, const _matrix & matWorld)
{
	POINT pt/* = { (LONG)iWinCX >> 1, (LONG)iWinCY >> 1 }*/;
	GetCursorPos(&pt);
	ScreenToClient(hWnd, &pt);

	// 뷰포트 -> 투영스페이스
	_float3 vMouse = _float3(0.f, 0.f, 0.f);
	vMouse.x = pt.x / (iWinCX * 0.5f) - 1.f;
	vMouse.y = 1.f - pt.y / (iWinCY * 0.5f);

	// 투영스페이스 -> 뷰스페이스
	_matrix matProj;
	matProj = XMMatrixInverse(nullptr, GetProjectionMatrix());
	XMStoreFloat3(&vMouse, (XMVector3TransformCoord(XMLoadFloat3(&vMouse), matProj)));
	//XMLoadFloat3(&vMouse) = XMVector3TransformCoord(XMLoadFloat3(&vMouse), matProj);

	// 뷰스페이스에서 광선(ray)의 정보를 설정한다.
	_vector vRayPos = XMVectorSet(0, 0, 0, 1.f);
	_vector vRayDir = XMLoadFloat3(&vMouse) - vRayPos;

	// 뷰스페이스 -> 월드스페이스
	_matrix matView;
	matView = XMMatrixInverse(nullptr, GetViewMatrix());
	vRayPos = XMVector3TransformCoord(vRayPos, matView);
	vRayDir = XMVector3TransformNormal(vRayDir, matView);

	// 월드스페이스 -> 로컬스페이스
	_matrix matInverse;
	matInverse = XMMatrixInverse(nullptr, matWorld);
	vRayPos = XMVector3TransformCoord(vRayPos, matInverse);
	vRayDir = XMVector3TransformNormal(vRayDir, matInverse);

	//평면방정식
	_vector Plane = XMPlaneFromPoints(_vector{ 0.f, 0.f ,0.f }, _vector{ 0.f, 0.f ,1.f }, _vector{ 1.f, 0 ,0.f });
	//위치 (방향으로 얼만큼 쏠건지 정하고 위치를 옮겨주자)
	_vector point2 = (vRayPos + vRayDir * 1000.f);

	_vector RayResult = XMPlaneIntersectLine(Plane, vRayPos, point2);
	if (!XMVector4IsNaN(RayResult)) //평면 // 시작위치// 끝위치
	{
		RayResult = XMVector3TransformCoord(RayResult, matWorld);
		XMStoreFloat3(&out, RayResult);
		return true;
	}
	return false;
}

HRESULT CEngine::Ready_XlsxLoader(string _path)
{
	if (m_pXlsxRead == nullptr) {
		return E_FAIL;
	}
	return m_pXlsxRead->Ready_XlsxLoader(_path);
}

pair<string, ITEMINFO> CEngine::GetItemAsName(string name)
{
	if (m_pXlsxRead == nullptr) {
		return pair<string, ITEMINFO>{};
	}
	return m_pXlsxRead->GetItemAsName(name);
}

list<pair<string, ITEMINFO>> CEngine::GetItemTypeList(ITEMTYPE _type)
{
	if (m_pXlsxRead == nullptr) {
		return list<pair<string, ITEMINFO>>{};
	}
	return m_pXlsxRead->GetItemTypeList(_type);
}

HRESULT CEngine::LoadQeustData()
{
	if (m_pQuestManager == nullptr) {
		return E_FAIL;
	}
	return m_pQuestManager->LoadQeustData();
}

HRESULT CEngine::LoadTalkData()
{
	if (m_pTalkManager == nullptr) {
		return E_FAIL;
	}
	return m_pTalkManager->LoadTalkData();
}

string CEngine::GetTalk(_int talkID, _int talkIdx)
{
	if (m_pTalkManager == nullptr) {
		return string();
	}
	return m_pTalkManager->GetTalk(talkID, talkIdx);
}

vector<string> CEngine::GetTalk(_int talkID)
{
	if (m_pTalkManager == nullptr) {
		return vector<string>();
	}
	return m_pTalkManager->GetTalk(talkID);
}

CQuestData* CEngine::GetQuestData(_int _questID)
{
	if (m_pQuestManager == nullptr) {
		return nullptr;
	}
	return m_pQuestManager->GetQuestData(_questID);
}

_int CEngine::GetTalkSize(_int talkID)
{
	if (m_pTalkManager == nullptr) {
		return 0;
	}
	return (_int)m_pTalkManager->GetTalkSize(talkID);
}

void CEngine::AcceptQuest(_int _qusetID)
{
	if (m_pQuestManager == nullptr) {
		return;
	}
	m_pQuestManager->AcceptQuest(_qusetID);
}

void CEngine::AddDeadObject(string objName)
{
	if (m_pAlretManager == nullptr) {
		return;
	}
	m_pAlretManager->AddDeadObject(objName);
}

void CEngine::AddAcceptQuest(_int questID, CQuestData* questData)
{
	if (m_pAlretManager == nullptr) {
		return;
	}
	m_pAlretManager->AddAcceptQuest(questID,questData);
}

void CEngine::AddClearQuest(_int questID, CQuestData* questData)
{
	if (m_pAlretManager == nullptr) {
		return;
	}
	m_pAlretManager->AddClearQuest(questID, questData);
}

void CEngine::RemoveAcceptQuest(_int questID)
{
	if (m_pAlretManager == nullptr) {
		return;
	}
	m_pAlretManager->RemoveAcceptQuest(questID);
}

_bool CEngine::SearchAcceptQuest(_int questID)
{
	if (m_pAlretManager == nullptr) {
		return false;
	}
	return m_pAlretManager->SearchAcceptQuest(questID);
}

_bool CEngine::SearchClearQuest(_int questID)
{
	if (m_pAlretManager == nullptr) {
		return false;
	}
	return m_pAlretManager->SearchClearQuest(questID);
}

CDeadInfo CEngine::GetDeadObjInfo(string objName)
{
	if (m_pAlretManager == nullptr) {
		return CDeadInfo();
	}
	return m_pAlretManager->GetDeadObjInfo(objName);
}

unordered_map<_int, CQuestData*> CEngine::GetQuestList()
{
	if (m_pAlretManager == nullptr) {
		return unordered_map<_int, CQuestData*>();
	}
	return std::move(m_pAlretManager->GetQuestList());
}

void CEngine::SetQuestList(unordered_map<_int, CQuestData*>&& quest)
{
	if (m_pAlretManager == nullptr) {
		return ;
	}
	m_pAlretManager->SetQuestList(std::move(quest));
}

void CEngine::QuestClear(_int questID)
{
	if (m_pAlretManager == nullptr) {
		return;
	}
	m_pAlretManager->QuestClear(questID);
}

_bool CEngine::Get_DIKState(_ubyte byKeyID)
{
	if (m_pInput_Device == nullptr) {
		return false;
	}
	return m_pInput_Device->Get_DIKState(byKeyID);
}

_bool CEngine::Get_DIKDown(_ubyte _byKeyID)
{
	if (m_pInput_Device == nullptr) {
		return false;
	}
	return m_pInput_Device->Get_DIKDown(_byKeyID);
}

_bool CEngine::Get_DIKUp(_ubyte _byKeyID)
{
	if (m_pInput_Device == nullptr) {
		return false;
	}
	return m_pInput_Device->Get_DIKUp(_byKeyID);
}

_long CEngine::Get_MouseMoveState(CInput_Device::MOUSEMOVESTATE eMoveState)
{
	if (m_pInput_Device == nullptr)
		return 0;

	return m_pInput_Device->Get_MouseMoveState(eMoveState);
}

_byte CEngine::Get_MouseButtonState(CInput_Device::MOUSEBUTTONSTATE eButtonState)
{
	if (m_pInput_Device == nullptr)
		return 0;

	return m_pInput_Device->Get_MouseButtonState(eButtonState);
}

_byte CEngine::Get_MouseButtonStateUp(CInput_Device::MOUSEBUTTONSTATE eButtonState)
{
	if (m_pInput_Device == nullptr)
		return 0;

	return m_pInput_Device->Get_MouseButtonStateUp(eButtonState);
}

_byte CEngine::Get_MouseButtonStateDown(CInput_Device::MOUSEBUTTONSTATE eButtonState)
{
	if (m_pInput_Device == nullptr)
		return 0;

	return m_pInput_Device->Get_MouseButtonStateDown(eButtonState);
}

PxScene * CEngine::GetScene()
{
	return m_pPxManager->GetScene();
}

PxCooking * CEngine::GetCooking()
{
	return m_pPxManager->GetCooking();
}

#pragma endregion

void CEngine::Free()
{
	SafeRelease(m_pXlsxRead);
	SafeRelease(m_pQuestManager);
	SafeRelease(m_pTalkManager);
	SafeRelease(m_pTargetManager);
	SafeRelease(m_pLightManager);
	SafeRelease(m_pScriptObjectManager);
	SafeRelease(m_pPipeline);
	SafeRelease(m_pComponentManager);
	SafeRelease(m_pGameObjectManager);
	SafeRelease(m_pSceneManager);
	SafeRelease(m_pTimerManager);
	SafeRelease(m_pInputManager);
	SafeRelease(m_pInput_Device);
	SafeRelease(m_pGraphicDevice);
	SafeRelease(m_pPxManager);
	SafeRelease(m_pImGuiManager);
	SafeRelease(m_pSoundManager);
	SafeRelease(m_pModelManager);
	SafeRelease(m_pFrustum);
	SafeRelease(m_pAlretManager);
}
