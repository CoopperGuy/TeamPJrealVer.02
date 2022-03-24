#include "stdafx.h"
#include "..\Public\Scene_Tool.h"
#include "Engine.h"
#include "Navigation.h"
//#pragma region COMPONENTS
//#include "Transform.h"
//#include "RectTransform.h"
//#include "SphereCollider.h"
//#include "BoxCollider.h"
//#include "CapsuleCollider.h"
//#include "VIBuffer_LineSphere.h"
//#include "VIBuffer_LineBox.h"
//#include "VIBuffer_LineCapsule.h"
//#include "VIBuffer_RectUI.h"
//#include "VIBuffer_Terrain.h"
//#include "Text.h"
//#include "Model.h"
//#pragma endregion 
//
//#pragma region OBJECTS
//#include "EmptyGameObject.h"
//#include "EmptyUI.h"
//#pragma endregion
#include "VIBufferTriangle.h"
#include "Camera_Tool.h"
#include "ToolPlayer.h"
#include "EmptyMapObject.h"
#include "InstanceModel.h"
#include "InsTempModel.h"
#include "BasicCollider.h"
#include "Stat.h"
#include "Shop.h"
//#include "VIBuffer_RectInstance.h"

USING(Tool)
extern HINSTANCE hInst;
CScene_Tool::CScene_Tool(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext, _uint iLevelIndex)
	: CScene(pDevice, pDeviceContext, iLevelIndex)
{
}


HRESULT CScene_Tool::Initialize()
{
	__super::Initialize();

	CEngine::GetInstance()->PxInitialize();

	if (FAILED(ReadyPrototypeComponent()))
		return E_FAIL;

	if (FAILED(ReadyPrototypeGameObject()))
		return E_FAIL;

	if (FAILED(Ready_Layer_Camera("LAYER_TOOL")))
		return E_FAIL;

	ReadyLayerLights("LAYER_LIGHTS");

	return S_OK;
}

_uint CScene_Tool::Update(_double TimeDelta)
{
	__super::Update(TimeDelta);

	return _uint();
}

HRESULT CScene_Tool::Render()
{
	__super::Render();

	return S_OK;
}

HRESULT CScene_Tool::ReadyPrototypeComponent()
{
	if (FAILED(m_pEngine->AddPrototype(0, "Prototype_Transform", CTransform::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;
	if (FAILED(m_pEngine->AddPrototype(0, "Prototype_RectTransform", CRectTransform::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;

	/* Prepare DebugLine */
	if (FAILED(m_pEngine->AddPrototype(0, "Prototype_VIBuffer_LineSphere", CVIBuffer_LineSphere::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;
	if (FAILED(m_pEngine->AddPrototype(0, "Prototype_VIBuffer_LineBox", CVIBuffer_LineBox::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;
	if (FAILED(m_pEngine->AddPrototype(0, "Prototype_VIBuffer_LineCapsule", CVIBuffer_LineCapsule::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;
	if (FAILED(m_pEngine->AddPrototype(0, "Prototype_VIBuffer_RectUI", CVIBuffer_RectUI::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;
	if (FAILED(m_pEngine->AddPrototype(0, "Prototype_VIBuffer_RectEffect", CVIBuffer_RectEffect::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;
	if (FAILED(m_pEngine->AddPrototype(0, "Prototype_VIBuffer_Terrain", CVIBuffer_Terrain::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;
	if (FAILED(m_pEngine->AddPrototype(0, "Prototype_VIBuffer_Rect", CVIBuffer_Rect::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;
	if (FAILED(m_pEngine->AddPrototype(0, "Prototype_VIBuffer_RectInstance", CVIBuffer_RectInstance::Create(m_pDevice, m_pDeviceContext, "../../Assets/Shader/Shader_RectInstance.fx"))))
		return E_FAIL;
	if (FAILED(m_pEngine->AddPrototype(0, "Prototype_VIBuffer_Decal", CVIBuffer_Cube::Create(m_pDevice, m_pDeviceContext, "../../Assets/Shader/Shader_Decal.fx"))))
		return E_FAIL;
	if (FAILED(m_pEngine->AddPrototype(0, "Prototype_VIBuffer_PointInstance", CVIBuffer_PointInstance::Create(m_pDevice, m_pDeviceContext, "../../Assets/Shader/Shader_PointInstance.fx", 10))))
		return E_FAIL;

	/* Prepare Collider*/
	if (FAILED(m_pEngine->AddPrototype(0, "Prototype_SphereCollider", CSphereCollider::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;
	if (FAILED(m_pEngine->AddPrototype(0, "Prototype_BoxCollider", CBoxCollider::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;
	if (FAILED(m_pEngine->AddPrototype(0, "Prototype_CapsuleCollider", CCapsuleCollider::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;

	if (FAILED(m_pEngine->AddPrototype(0, "Prototype_Text", CText::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;
	if (FAILED(m_pEngine->AddPrototype(0, "Prototype_Model", CModel::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;

	///*Instance Object*/
	//if (FAILED(m_pEngine->AddPrototype(0, "Prototype_InstanceTempModel", CInsTempModel::Create(m_pDevice, m_pDeviceContext))))
	//	return E_FAIL;

	if (FAILED(m_pEngine->AddPrototype(0, "Prototype_InstanceModel", CInstanceModel::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;


	/* Gameobject Navigation */
	if (FAILED(m_pEngine->AddPrototype(0, "Prototype_Navigation", CNavigation::Create_ForTool(m_pDevice, m_pDeviceContext))))
		return E_FAIL;

	/*NPCInfo*/
	if (FAILED(m_pEngine->AddPrototype(0, "Prototype_NPCInfo", CNPCInfo::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;

	/*Stat*/
	if (FAILED(m_pEngine->AddPrototype(0, "Prototype_Stat", CStat::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;

	/*Shop*/
	if (FAILED(m_pEngine->AddPrototype(0, "Prototype_Shop", CShop::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;

	/*Spawn*/

	if (FAILED(m_pEngine->AddPrototype(0, "Prototype_Spawn", CSpawner::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;

	/*Collider*/
	if (FAILED(m_pEngine->AddPrototype(0, "Prototype_OBBCollider", CBasicCollider::Create(m_pDevice, m_pDeviceContext, CBasicCollider::TYPE_OBB))))
		return E_FAIL;

	if (FAILED(m_pEngine->AddPrototype(0, "Prototype_AABBCollider", CBasicCollider::Create(m_pDevice, m_pDeviceContext, CBasicCollider::TYPE_AABB))))
		return E_FAIL;
}

HRESULT CScene_Tool::ReadyPrototypeGameObject()
{
	CEngine*	pEngine = GET_INSTANCE(CEngine);

	/* Gameobject Prototype */
	if (FAILED(pEngine->AddPrototype("Prototype_EmptyGameObject", CEmptyGameObject::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;
	if (FAILED(pEngine->AddPrototype("Prototype_EmptyUI", CEmptyUI::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;
	if (FAILED(pEngine->AddPrototype("Prototype_EmptyEffect", CEmptyEffect::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;
	if (FAILED(pEngine->AddPrototype("Prototype_EmptyMapObject", CEmptyMapObject::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;

	RELEASE_INSTANCE(CEngine);

	return S_OK;
}

HRESULT CScene_Tool::Ready_Layer_Camera(string pLayerTag)
{
	CEngine*		pEngine = GET_INSTANCE(CEngine);

	if (FAILED(pEngine->AddPrototype("GameObject_Camera_Fly", CCamera_Tool::Create(m_pDevice, m_pDeviceContext))))
		return E_FAIL;

	CCamera::CAMERADESC		CameraDesc;
	ZeroMemory(&CameraDesc, sizeof(CCamera::CAMERADESC));

	CameraDesc.vEye = _float3(0.f, 1.f, -5.f);
	CameraDesc.vAt = _float3(0.f, 0.f, 0.f);
	CameraDesc.vAxisY = _float3(0.f, 1.f, 0.f);

	if (nullptr == pEngine->AddGameObject(0, "GameObject_Camera_Fly", pLayerTag, &CameraDesc))
		return E_FAIL;

	RELEASE_INSTANCE(CEngine);

	return S_OK;
}
HRESULT CScene_Tool::ReadyLayerLights(string pLayerTag)
{
	CEngine*		pEngine = GET_INSTANCE(CEngine);

	//LIGHTDESC			LightDesc;
	//ZeroMemory(&LightDesc, sizeof(LIGHTDESC));

	//LightDesc.vLightDir = _float4(1.f, -1.f, 1.f, 0.f);
	//LightDesc.vDiffuse = _float4(1.f, 1.f, 1.f, 1.f);
	//LightDesc.vAmbient = _float4(1.f, 1.f, 1.f, 1.f);
	//LightDesc.vSpecular = _float4(1.f, 1.f, 1.f, 1.f);

	//if (FAILED(pEngine->AddLight(m_pDevice, m_pDeviceContext, LightDesc)))
	//	return E_FAIL;

	//LightDesc.eType = LIGHTDESC::LIGHT_POINT;
	//LightDesc.vLightPos = _float4(5.f, 5.f, 0.f, 1.f);
	//LightDesc.fLightRange = 10.f;
	//LightDesc.vDiffuse = _float4(0.f, 0.f, 1.f, 1.f);
	//LightDesc.vAmbient = _float4(0.4f, 0.4f, 0.4f, 1.f);
	//LightDesc.vSpecular = _float4(1.f, 1.f, 1.f, 1.f);

	//if (FAILED(pEngine->AddLight(m_pDevice, m_pDeviceContext, LightDesc)))
	//	return E_FAIL;


	RELEASE_INSTANCE(CEngine);

	return S_OK;
}


CScene_Tool * CScene_Tool::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext, _uint iLevelIndex)
{
	CScene_Tool*		pInstance = new CScene_Tool(pDevice, pDeviceContext, iLevelIndex);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed to Creating CScene_Tool");
		SafeRelease(pInstance);
	}

	return pInstance;
}

void CScene_Tool::Free()
{
	__super::Free();
}
