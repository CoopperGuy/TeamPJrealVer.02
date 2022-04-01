#pragma once

#include "SceneManager.h"
#include "GameObjectManager.h"
#include "ComponentManager.h"
#include "ImGuiManager.h"
#include "Pipeline.h"
#include "Sound.h"
#include "ScriptObjectManager.h"
#include "LightManager.h"
#include "PxManager.h"
#include "TargetManager.h"
#include "Frustum.h"
#include "XlsxRead.h"
#include "QuestManager.h"
#include "TalkManager.h"
#include "QuestData.h"
#include "AlretManager.h"
#include "Input_Device.h"
#include "PxQueryFilters.h"
#include "CameraManager.h"


BEGIN(Engine)

class CQuestData;
class CPxQueryFilters;
class ENGINE_DLL CEngine final : public CBase
{
	DECLARE_SINGLETON(CEngine)private:
public:
	enum class USAGE { USAGE_CLIENT, USAGE_TOOL, USAGE_NONE };
private:
	CEngine();
	virtual ~CEngine() = default;

public:
	HRESULT			Initialize(_uint iNumScenes, USAGE usage);
	_uint			Update(_double dTimeDelta);
	static void		ReleaseEngine();
	USAGE			GetCurrentUsage() { return m_eUsage; }

#pragma region DEVICE_MANAGER
public:
	HRESULT						ReadyDevice(HWND hWnd, _uint iWidth, _uint iHeight, HINSTANCE hInst);
	ID3D11Device*				GetDevice();
	ID3D11DeviceContext*		GetDeviceContext();
	IDXGISwapChain*				GetSwapChain();
	ID3D11RenderTargetView*		GetRenderTargetView();
	ID3D11DepthStencilView*		GetDepthStencilRenderTargetView();
	SpriteBatch*				GetSpriteBatch();
	SpriteFont*					GetSpriteFont();
	SpriteFont*					GetSpriteKoreanFont();
	_float2						GetCurrentWindowSize();


	XMMATRIX					GetViewMatrix();
	XMMATRIX					GetProjectionMatrix();

	HRESULT						ClearBackBufferView(_float4 vColor);
	HRESULT						ClearDepthStencilView(_float fDepth, _uint iStencil);

	void						SetRTV();
	void						SetRTV2();
	void						Render();
	void						RenderClient();
	HRESULT						Present();
	HRESULT						ChangeResolution(_uint iWidth, _uint iHeight);
	void						ChangeProj(_uint iWidth, _uint iHeight);
#pragma endregion

#pragma region TIMER_MANAGER
public:
	HRESULT AddTimers(string pTimerTag);
	_double ComputeDeltaTime(string pTimerTag);
#pragma endregion

#pragma region SCENE
public:
	void		DeserializePrefab();
	void		SerializeScene(const string& filePath);
	void		DeserializeScene(const string& filePath, _uint scnenNumber);
	HRESULT		SetUpCurrentScene(class CScene* pCurrentScene, _uint scnenNum);
	void		SetCurSceneNumber(_uint idx);
	_uint		UpdateScene(_double DeltaTime);
	HRESULT		RenderScene();
	_uint		GetCurSceneNumber();
	void		WaitThread();
#pragma endregion

#pragma region GAMEOBJECT_MANAGER
public:
	HRESULT AddPrototype(string pPrototypeTag, class CGameObject* pPrototype);
	CGameObject* ClonePrototype(string pFindPrototypeTag, string pPrototypeTag, void* pArg = nullptr);
	CGameObject* AddGameObject(_uint iSceneIndex, string pPrototypeTag, string pLayerTag, void* pArg = nullptr);
	void	ClearGameObjectManager(_uint iSceneIndex);
	list<class CGameObject*> GetGameObjectInLayer(_uint iSceneIndex, string pLayerTag);
	unordered_map<string, CLayer*>* GetLayers();
	void AddGameObjectWithName(_uint iSceneIndex, string name, CGameObject* pObj);
	void AddGameObjectWithUUID(_uint iSceneIndex, uint64_t uuid, CGameObject* pObj);
	CGameObject* FindGameObjectWithName(_uint iSceneIndex, string name);
	CGameObject* FindGameObjectWithUUID(_uint iSceneIndex, uint64_t uuid);

	/*Prefab*/
	void AddPrefab(string name, YAML::Node node);
	CGameObject* SpawnPrefab(string name);
	HRESULT CreatePrefab(string pPrototypeTag, string name);
	CGameObject* AddGameObjectToPrefab(_uint iSceneIndex, string pPrototypeTag, string pLayerTag, void* pArg = nullptr);
	
	void AddOBBList(ID Type, CGameObject* _pGameObject, CBasicCollider* _obb);

	void CollsionCheck();
#pragma endregion

#pragma region SCRIPTOBJECT
	IScriptObject * AddScriptObject(IScriptObject* pObj, _uint iNumScenes);
	void ClearScriptObject(_uint iSceneNum);
#pragma endregion 

#pragma region INPUT
	void InitializeInput();
	void UpdateInput();

	bool IsKeyUp(_uint key);
	bool IsKeyDown(_uint key);
	bool IsKeyPressed(_uint key);
	bool IsKeyToggled(_uint key);
	bool IsMouseUp(DWORD mouse);
	bool IsMouseDown(DWORD mouse);
	bool IsMousePressed(DWORD mouse);

	_float3 GetMouseMoveValue();
	_float3 GetMousePosition();
	vector<bitset<256>> Get_Key(_uint iIndex);
	void InputProc(const HWND hWnd, const UINT message,
		const WPARAM wParam, const LPARAM lParam);
#pragma endregion

#pragma region COMPONENT
	HRESULT AddPrototype(_uint iSceneIndex, string pPrototypeTag, class CComponent* pPrototype);
	class CComponent* CloneComponent(_uint iSceneIndex, string pPrototypeTag, void* pArg = nullptr);
	void ClearComponentManager(_uint iSceneIndex);
	void CloneModel(CGameObject* pObj, string pMeshFilePath, string pMeshFileName, string pShaderFilePath, string pEffectFilePath, _bool meshCollider, void* pArg = nullptr, _bool bEquipment = false);
#pragma endregion

#pragma region LIGHT
	const LIGHTDESC* GetLightDesc(_uint iIndex = 0) const;
	HRESULT AddLight(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, const LIGHTDESC& LightDesc);
#pragma endregion

#pragma region RENDERTARGET
	HRESULT Add_RenderTarget(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, string pRenderTargetTag, _uint iWidth, _uint iHeight, DXGI_FORMAT Format, _float4 vClearColor);
	HRESULT Add_MRT(string pMRTTag, string pRenderTargetTag);
	HRESULT Begin_MRT(ID3D11DeviceContext* pDeviceContext, string pMRTTag);
	HRESULT End_MRT(ID3D11DeviceContext* pDeviceContext);
	HRESULT Ready_DebugBuffer(string pTargetTag, _float fX, _float fY, _float fWidth, _float fHeight);
	HRESULT Render_DebugBuffers(string pMRTTag);
	ID3D11ShaderResourceView* GetShaderResourceView(string pTargetTag);
#pragma endregion

#pragma region PIPELINE
	_matrix GetTransform(CPipeline::TYPE eType);
	_vector GetCamPosition();
#pragma endregion

#pragma region IMGUI
	void InitializeImGui(HWND hWnd, ID3D11Device* device, ID3D11DeviceContext* deviceContext);
	void UpdateImGui();
	void AddWindow(string name, class CImGuiWindow* window);
	CImGuiWindow* GetWindow(string name);
#pragma endregion

#pragma region SOUND
	void PlaySoundW(string pSoundKey, CHANNELID eID);
	void PlayBGM(string pSoundKey);
	void StopSound(CHANNELID eID);
	void StopAll();
#pragma endregion

#pragma region PHYSX
	void					PxInitialize();
	void					UpdatePx(_double dDeltaTime);

	PxPhysics*				GetPhysics();
	PxMaterial*				GetMaterial();
	PxControllerManager*	GetControllerManager();
	PxScene*				GetScene();
	PxCooking*				GetCooking();
	void					AddActor(PxRigidActor* pActor);
	void					AddAggregateActor(PxRigidActor* pActor);

	_bool					Raycast(_vector origin, _vector unitDir, _float maxDistance, PxRaycastBuffer& hit, PxQueryFilterData& filterData);
	_bool					Raycast(PxVec3 origin, PxVec3 unitDir, _float maxDistance, PxRaycastBuffer& hit, PxQueryFilterData& filterData);
	_bool					Raycast(_vector origin, _vector unitDir, _float maxDistance, PxRaycastBuffer& hit, PxQueryFilterData& filterData, CPxQueryFilters* _callback);
	_bool					Overlap(const _float3 & point, PxU32 layerMask,_float3 direciton);
#pragma endregion

#pragma	region FRUSTUM
	_bool isInFrustum(_fvector Position,_float range);
#pragma	endregion

#pragma region MouseRayCast
	BOOL MouseRayCast(_float3 &out, HWND hWnd, _uint iWinCX, _uint iWinCY, const _matrix & matWorld);
#pragma endregion

#pragma  region XLSXREAD
	HRESULT Ready_XlsxLoader(string _path);
	pair<string, ITEMINFO> GetItemAsName(string name);
	list<pair<string, ITEMINFO>> GetItemTypeList(ITEMTYPE _type);
#pragma  endregion

#pragma region QUEST
	HRESULT	LoadQeustData();
	HRESULT	LoadTalkData();
	string GetTalk(_int talkID, _int talkIdx);
	vector<string> GetTalk(_int talkID);
	CQuestData*	GetQuestData(_int _questID);
	_int	GetTalkSize(_int talkID);
	void AcceptQuest(_int _qusetID);
#pragma endregion

#pragma region ALRET
	void AddDeadObject(string objName);
	void AddAcceptQuest(_int questID, CQuestData* questData);
	void AddClearQuest(_int questID, CQuestData* questData);
	void RemoveAcceptQuest(_int questID);
	_bool SearchAcceptQuest(_int questID);
	_bool SearchClearQuest(_int qeustID);
	CDeadInfo GetDeadObjInfo(string objName);
	unordered_map<_int, CQuestData*>	GetQuestList();
	void SetQuestList(unordered_map<_int, CQuestData*>&& quest);
	void QuestClear(_int questID);
#pragma  endregion

#pragma region INPUTDEVICE
	_bool Get_DIKState(_ubyte byKeyID);
	_bool Get_DIKDown(_ubyte _byKeyID);
	_bool Get_DIKUp(_ubyte _byKeyID);
	_long Get_MouseMoveState(CInput_Device::MOUSEMOVESTATE eMoveState);
	_byte Get_MouseButtonState(CInput_Device::MOUSEBUTTONSTATE eButtonState);
	_byte Get_MouseButtonStateUp(CInput_Device::MOUSEBUTTONSTATE eButtonState);
	_byte Get_MouseButtonStateDown(CInput_Device::MOUSEBUTTONSTATE eButtonState);

#pragma endregion
#pragma region CAMERA
	void	ActiveCameraByIndex(_int _idx);
	HRESULT	AddCamera(CCamera* _camera);
	HRESULT	SetDefaultCamera();
#pragma endregion
private:
	class CTimerManager*			m_pTimerManager = nullptr;
	class CGraphicDevice*			m_pGraphicDevice = nullptr;
	class CSceneManager*			m_pSceneManager = nullptr;
	class CGameObjectManager*		m_pGameObjectManager = nullptr;
	class CComponentManager*		m_pComponentManager = nullptr;
	class CInputManager*			m_pInputManager = nullptr;
	class CPipeline*				m_pPipeline = nullptr;
	class CScriptObjectManager*		m_pScriptObjectManager = nullptr;
	class CLightManager*			m_pLightManager = nullptr;
	class CModelManager*			m_pModelManager = nullptr;
	class CTargetManager*			m_pTargetManager = nullptr;

	class CPxManager*				m_pPxManager = nullptr;
	class CImGuiManager*			m_pImGuiManager = nullptr;
	class CSound*					m_pSoundManager = nullptr;
	CQuestManager*					m_pQuestManager = nullptr;
	CTalkManager*					m_pTalkManager = nullptr;
	CAlretManager*					m_pAlretManager = nullptr;
	CInput_Device*					m_pInput_Device = nullptr;
	CameraManager*					m_pCameraManager = nullptr;
	CFrustum*						m_pFrustum = nullptr;
	CXlsxRead*						m_pXlsxRead = nullptr;
	USAGE							m_eUsage;

public:
	virtual void Free() override;
};

END