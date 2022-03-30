 #include "EnginePCH.h"
#include "..\Public\SceneSerializer.h"
#include "Engine.h"
#include "ModelManager.h"
#include <string.h>

#include "EmptyGameObject.h"
#include "EmptyMapObject.h"
#include "EmptyUI.h"
#include "EmptyEffect.h"
#include "Layer.h"
#include "InstanceModel.h"

#pragma region UICOM
#include "RectTransform.h"
#include "VIBuffer_RectUI.h"
#include "VIBuffer_Terrain.h"
#include "VIBuffer_RectEffect.h"
#pragma endregion

#pragma region COMPONENT
#include "Collider.h"
#include "BoxCollider.h"
#include "SphereCollider.h"
#include "CapsuleCollider.h"
#include "BasicCollider.h"
#pragma endregion

static string prefabPath = "../../Assets/MyPrefabs/";
std::mutex YAMLMutex;
CSceneSerializer::CSceneSerializer()
	: m_pEngine(CEngine::GetInstance())
{
	m_pDevice = m_pEngine->GetDevice();
	m_pDeviceContext = m_pEngine->GetDeviceContext();
}

void CSceneSerializer::Serialize(const string & filePath)
{
	YAML::Emitter out;

	out << YAML::BeginMap;
	out << YAML::Key << "Scene" << YAML::Value << "Untitled";
	out << YAML::Key << "GameObjects" << YAML::Value << YAML::BeginSeq;

	//list<CGameObject*> objList = m_pEngine->GetGameObjectInLayer(0, TEXT("LAYER_TOOL"));
	unordered_map<string, CLayer*>* layers = m_pEngine->GetLayers();
	for (auto& pair : *layers)
	{
		if (pair.first == "LAYER_TOOL")
			continue;

		list<CGameObject*> objList = (pair.second)->GetGameObjectList();
		for (auto& obj : objList)
		{
			out << YAML::BeginMap;

			out << YAML::Key << "Name" << YAML::Value << obj->GetName();
			out << YAML::Key << "UUID" << YAML::Value << obj->GetUUID();
			out << YAML::Key << "Active" << YAML::Value << obj->IsActive();
			out << YAML::Key << "Layer" << YAML::Value << obj->GetLayer();
			out << YAML::Key << "Instancing" << YAML::Value << obj->GetInstancing();

			if (dynamic_cast<CEmptyGameObject*>(obj))
				SerializeObject(out, obj);
			//else if (dynamic_cast<CEmptyMapObject*>(obj))
			//	SerializeMapObject(out, obj);
			else if (dynamic_cast<CEmptyEffect*>(obj))
				SerializeEffect(out, obj);
			else
				SerializeUI(out, obj);

			if (0 < obj->GetChildren().size())
			{
				out << YAML::Key << "Children";
				out << YAML::BeginSeq;

				list<CGameObject*> children = obj->GetChildren();
				for (auto& child : children)
					out << child->GetUUID();

				out << YAML::EndSeq;
			}

			out << YAML::EndMap;

		}
	}
	// Save GameObjects

	out << YAML::EndSeq;
	out << YAML::EndMap;

	std::ofstream fout(filePath);
	fout << out.c_str();
}

bool CSceneSerializer::Deserialize(const string & filePath, _uint curScene)
{
	//YAMLMutex.lock();
	std::ifstream stream(filePath);
	std::stringstream strStream;
	strStream << stream.rdbuf();

	YAML::Node data = YAML::Load(strStream.str());
	if (!data["Scene"])
		return false;

	string SceneName = data["Scene"].as<string>();

	auto gameObjects = data["GameObjects"];

	if (gameObjects)
	{
		for (auto obj : gameObjects)
		{
			CGameObject* deserializedObject = nullptr;
			if (obj["Type"].as<string>() == "UI")
				deserializedObject = DeserializeUI(obj, false, curScene);
			else if (obj["Type"].as<string>() == "Effect")
				deserializedObject = DeserializeEffect(obj, false, curScene);
			else
				deserializedObject = DeserializeObject(obj, false, curScene);
			if(deserializedObject)
				cout << deserializedObject->GetName() << "is created \n";
		}
	}


	if (gameObjects)
	{
		for (auto obj : gameObjects)
		{
			auto children = obj["Children"];
			if (children)
			{
				CGameObject* parent = m_pEngine->FindGameObjectWithUUID(curScene, obj["UUID"].as<uint64_t>());
				int seqSize = (_int)children.size();
				for (int i = 0; i < children.size(); ++i)
				{
					uint64_t uuid = children[i].as<uint64_t>();
					CGameObject* child = nullptr;
					if (child = m_pEngine->FindGameObjectWithUUID(curScene, uuid))
						parent->AddChild(child);
					if (dynamic_cast<CEmptyGameObject*>(child) || dynamic_cast<CEmptyEffect*>(child))
					{

						if (dynamic_cast<CEmptyEffect*>(child)) {
							dynamic_cast<CEmptyEffect*>(child)->SetParent(parent);
						}
						else {
							dynamic_cast<CEmptyGameObject*>(child)->SetParent(parent);
						}
						_matrix localMatrix =
							dynamic_cast<CTransform*>(child->GetComponent("Com_Transform"))->GetWorldMatrix();
						dynamic_cast<CTransform*>(child->GetComponent("Com_LocalTransform"))->SetMatrix(localMatrix);

					}
				}
			}
		}
	}
	//YAMLMutex.unlock();
	CModelManager::GetInstance()->WaitThreads();


	DeserializeInstancModel(curScene);
	return true;
}

void CSceneSerializer::SerializeObject(YAML::Emitter & out, CGameObject * obj)
{

	out << YAML::Key << "Type" << YAML::Value << "Object";
	out << YAML::Key << "PassIndex" << YAML::Value << obj->GetPassIndex();
	out << YAML::Key << "NumOBB" << YAML::Value << static_cast<CEmptyGameObject*>(obj)->GetNumOBB();
	if (obj->GetComponent("Com_Transform"))
	{
		CTransform* transform = nullptr;
		if (obj->GetParent() == nullptr) {
			transform = dynamic_cast<CTransform*>(obj->GetComponent("Com_Transform"));
		}
		else {
			transform = dynamic_cast<CTransform*>(obj->GetComponent("Com_LocalTransform"));
		}
		out << YAML::Key << "Com_Transform";
		out << YAML::BeginMap;

		XMMATRIX matrix = XMLoadFloat4x4(&transform->GetMatrix());
		//XMVECTOR tr, rt, sc;
		//XMMatrixDecompose(&sc, &rt, &tr, matrix);

		float _objMat[16];
		memcpy(_objMat, &matrix, sizeof(XMMATRIX));
		float matrixTranslation[3], matrixRotation[3], matrixScale[3];
		ImGuizmo::DecomposeMatrixToComponents(_objMat, matrixTranslation, matrixRotation, matrixScale);


		out << YAML::Key << "Translation";
		out << YAML::Value << YAML::Flow;
		out << YAML::BeginSeq << matrixTranslation[0] << matrixTranslation[1] << matrixTranslation[2] << YAML::EndSeq;

		out << YAML::Key << "Rotation";
		out << YAML::Value << YAML::Flow;
		out << YAML::BeginSeq << matrixRotation[0] << matrixRotation[1] << matrixRotation[2] << YAML::EndSeq;

		out << YAML::Key << "Scale";
		out << YAML::Value << YAML::Flow;
		out << YAML::BeginSeq << matrixScale[0] << matrixScale[1] << matrixScale[2] << YAML::EndSeq;


		out << YAML::EndMap;
	}

	if (obj->GetComponent("Com_Collider"))
	{
		CCollider* collider = dynamic_cast<CCollider*>(obj->GetComponent("Com_Collider"));

		// Check if RB exist
		out << YAML::Key << "Com_Collider";
		out << YAML::BeginMap;

		_float3 center = collider->GetRelativePos();
		out << YAML::Key << "Center";
		out << YAML::Value << YAML::Flow;
		out << YAML::BeginSeq << center.x << center.y << center.z << YAML::EndSeq;

		if (dynamic_cast<CBoxCollider*>(collider))
		{
			CBoxCollider* boxCollier = dynamic_cast<CBoxCollider*>(collider);
			_float3 boxSize = boxCollier->GetSize();

			out << YAML::Key << "Type" << YAML::Value << "Box";
			out << YAML::Key << "Size" << YAML::Value << YAML::Flow;
			out << YAML::BeginSeq << boxSize.x << boxSize.y << boxSize.z << YAML::EndSeq;
		}
		else if (dynamic_cast<CSphereCollider*>(collider))
		{
			CSphereCollider* sphereCollier = dynamic_cast<CSphereCollider*>(collider);
			_float radius = sphereCollier->GetSize();
			out << YAML::Key << "Type" << YAML::Value << "Sphere";
			out << YAML::Key << "Radius" << YAML::Value << radius;
		}
		else if (dynamic_cast<CCapsuleCollider*>(collider))
		{
			CCapsuleCollider* capsuleCollier = dynamic_cast<CCapsuleCollider*>(collider);
			pair<float, float> capuleSize = capsuleCollier->GetSize();
			out << YAML::Key << "Type" << YAML::Value << "Capsule";
			out << YAML::Key << "Radius" << YAML::Value << capuleSize.first;
			out << YAML::Key << "Height" << YAML::Value << capuleSize.second;
		}
		_float3 angle = collider->GetEuler();
		out << YAML::Key << "Angle";
		out << YAML::BeginSeq << angle.x << angle.y << angle.z << YAML::EndSeq;

		CCollider::RIGIDBODYDESC desc = collider->GetRigidBodyDesc();
		out << YAML::Key << "RigidBody";
		out << YAML::BeginMap;

		out << YAML::Key << "Enabled" << YAML::Value << desc.bEnabled;
		out << YAML::Key << "Gravity" << YAML::Value << desc.bGravity;
		out << YAML::Key << "Kinematic" << YAML::Value << desc.bKinematic;
		out << YAML::Key << "CharacterController" << YAML::Value << desc.bCC;

		out << YAML::EndMap;

		out << YAML::EndMap;

	}

	_int numObb = static_cast<CEmptyGameObject*>(obj)->GetNumOBB();
	_int i = 0;
	do {
		string obb = "Com_OBB";
		if (i != 0)
			obb += to_string(i);
		if (obj->GetComponent(obb))
		{
			CBasicCollider* Obb = dynamic_cast<CBasicCollider*>(obj->GetComponent(obb));

			// Check if RB exist
			out << YAML::Key << obb.c_str();
			out << YAML::BeginMap;

			_float3 center = Obb->GetPos();
			out << YAML::Key << "Center";
			out << YAML::Value << YAML::Flow;
			out << YAML::BeginSeq << center.x << center.y << center.z << YAML::EndSeq;

			_float3 boxSize = Obb->GetSize();

			out << YAML::Key << "Size" << YAML::Value << YAML::Flow;
			out << YAML::BeginSeq << boxSize.x << boxSize.y << boxSize.z << YAML::EndSeq;

			_bool attach = Obb->GetIsAttachBone();
			out << YAML::Key << "IsAttach" << YAML::Value << attach;

			string boneName = Obb->GetBoneName();
			out << YAML::Key << "BoneName" << YAML::Value << boneName.c_str();

			_float3 offset = Obb->GetOffset();
			out << YAML::Key << "Offset";
			out << YAML::Value << YAML::Flow;
			out << YAML::BeginSeq << offset.x << offset.y << offset.z << YAML::EndSeq;

			_int collType = (_int)Obb->GetCollisionType();
			out << YAML::Key << "CollisionType" << YAML::Value << collType;

			out << YAML::EndMap;
			if (numObb == 0)
				static_cast<CEmptyGameObject*>(obj)->SetNumOBB(1);
		}
		i++;
	} while (i < numObb);


	if (obj->GetComponent("Com_VIBuffer"))
	{
		CVIBuffer* viBuffer = dynamic_cast<CVIBuffer*>(obj->GetComponent("Com_VIBuffer"));

		out << YAML::Key << "Com_VIBuffer";
		out << YAML::BeginMap;

		if (dynamic_cast<CVIBuffer_Terrain*>(viBuffer))
		{
			CVIBuffer_Terrain* pTerrainBuffer = dynamic_cast<CVIBuffer_Terrain*>(viBuffer);
			out << YAML::Key << "Type" << YAML::Value << "Terrain";
			out << YAML::Key << "HeightMapPath" << YAML::Value << pTerrainBuffer->GetHeightMapPath();
			out << YAML::Key << "TexturePath" << YAML::Value << pTerrainBuffer->GetTexturePath();
		}
		else if (dynamic_cast<CVIBuffer_Rect*>(viBuffer)) {
			CVIBuffer_Rect* pRectBuffer = dynamic_cast<CVIBuffer_Rect*>(viBuffer);
			out << YAML::Key << "Type" << YAML::Value << "Rect";
			out << YAML::Key << "TexturePath" << YAML::Value << pRectBuffer->GetTextureFilePath();
		}

		out << YAML::EndMap;
	}

	if (obj->GetComponent("Com_Model"))
	{
		CModel* pModel = dynamic_cast<CModel*>(obj->GetComponent("Com_Model"));

		out << YAML::Key << "Com_Model";
		out << YAML::BeginMap;

		out << YAML::Key << "MeshFilePath" << YAML::Value << pModel->GetMeshFilePath();
		out << YAML::Key << "MeshFileName" << YAML::Value << pModel->GetMeshFileName();
		out << YAML::Key << "HasCollider" << YAML::Value << pModel->HasMeshCollider();
		out << YAML::Key << "Equipment" << YAML::Value << pModel->GetLinkEquip();

		out << YAML::EndMap;
	}

	if (obj->GetComponent("Com_Light"))
	{
		CLight* pLight = dynamic_cast<CLight*>(obj->GetComponent("Com_Light"));
		const LIGHTDESC* lightDesc = pLight->GetLightDesc();
		out << YAML::Key << "Com_Light";
		out << YAML::BeginMap;

		out << YAML::Key << "Type" << YAML::Value << lightDesc->eType;
		out << YAML::Key << "Range" << YAML::Value << lightDesc->fLightRange;
		out << YAML::Key << "Angle" << YAML::Value << lightDesc->fLightAngle;

		out << YAML::Key << "Diffuse";
		out << YAML::Value << YAML::Flow;
		out << YAML::BeginSeq << lightDesc->vDiffuse.x << lightDesc->vDiffuse.y << lightDesc->vDiffuse.z << lightDesc->vDiffuse.w << YAML::EndSeq;

		out << YAML::Key << "Ambient";
		out << YAML::Value << YAML::Flow;
		out << YAML::BeginSeq << lightDesc->vAmbient.x << lightDesc->vAmbient.y << lightDesc->vAmbient.z << lightDesc->vAmbient.w << YAML::EndSeq;

		out << YAML::Key << "Specular";
		out << YAML::Value << YAML::Flow;
		out << YAML::BeginSeq << lightDesc->vSpecular.x << lightDesc->vSpecular.y << lightDesc->vSpecular.z << lightDesc->vSpecular.w << YAML::EndSeq;

		out << YAML::EndMap;
	}

	if (obj->GetComponent("Com_NPCInfo")) {
		CNPCInfo* npcInfo = static_cast<CNPCInfo*>(obj->GetComponent("Com_NPCInfo"));
		_uint npcid = npcInfo->GetNPCID();
		_uint questId = npcInfo->GetNPCQuestID();
		_uint boxID = npcInfo->GetBoxID();

		out << YAML::Key << "Com_NPCInfo";
		out << YAML::BeginMap;

		out << YAML::Key << "NPCID" << YAML::Value << npcid;
		out << YAML::Key << "QuestID" << YAML::Value << questId;
		out << YAML::Key << "BoxID" << YAML::Value << boxID;
		out << YAML::EndMap;
	}

	if (obj->GetComponent("Com_Stat")) {
		CStat* objStatCom = static_cast<CStat*>(obj->GetComponent("Com_Stat"));
		CStat::STAT objStat = objStatCom->GetStatInfo();

		out << YAML::Key << "Com_Stat";
		out << YAML::BeginMap;

		out << YAML::Key << "Hp";
		out << YAML::Value << YAML::Flow;
		out << YAML::BeginSeq << objStat.hp << objStat.boHp << objStat.maxHp << objStat.levelHp << YAML::EndSeq;
		out << YAML::Key << "HpRegen";
		out << YAML::Value << YAML::Flow;
		out << YAML::BeginSeq << objStat.hpRegeneration << objStat.levelRegen << YAML::EndSeq;
		out << YAML::Key << "Atk";
		out << YAML::Value << YAML::Flow;
		out << YAML::BeginSeq << objStat.atk << objStat.boAtk << objStat.levelAtk << YAML::EndSeq;
		out << YAML::Key << "Arm";
		out << YAML::Value << YAML::Flow;
		out << YAML::BeginSeq << objStat.armor << objStat.boArmor << objStat.levelArmor << YAML::EndSeq;
		out << YAML::Key << "Exp";
		out << YAML::Value << YAML::Flow;
		out << YAML::BeginSeq << objStat.exp << objStat.maxExp << objStat.levelExp << YAML::EndSeq;
		out << YAML::Key << "Level" << YAML::Value << objStat.level;
		out << YAML::Key << "Gold" << YAML::Value << objStat.gold;

		out << YAML::EndMap;

	}

	if (obj->GetComponent("Com_Shop")) {
		CShop* shop = static_cast<CShop*>(obj->GetComponent("Com_Shop"));
		pair<_int, _int> range = shop->GetItemRange();
		ITEMTYPE type = shop->GetItemType();

		out << YAML::Key << "Com_Shop";
		out << YAML::BeginMap;
		out << YAML::Key << "Range";
		out << YAML::Value << YAML::Flow;
		out << YAML::BeginSeq << range.first << range.second << YAML::EndSeq;
		out << YAML::Key << "Type" << YAML::Value << (_int)type;

		out << YAML::EndMap;
	}

	if (obj->GetComponent("Com_Spawn")) {
		CSpawner*	spawner = static_cast<CSpawner*>(obj->GetComponent("Com_Spawn"));
		CSpawner::SPAWNER info = spawner->GetSpawnInfo();

		out << YAML::Key << "Com_Spawn";
		out << YAML::BeginMap;

		out << YAML::Key << "Name" << YAML::Value << info.prefabName;
		out << YAML::Key << "Time" << YAML::Value << info.spawnTime;

		out << YAML::EndMap;

	}

	{
		if (obj->GetInstancing())
		{
			InstaingDesc(obj, out);

			out << YAML::Key << "RenderGroup" << YAML::Value << dynamic_cast<CEmptyGameObject*>(obj)->GetRenderGroup();
			out << YAML::Key << "FrustumRange" << YAML::Value << dynamic_cast<CEmptyGameObject*>(obj)->GetFrustum();
		}
		else
		{
			out << YAML::Key << "RenderGroup" << YAML::Value << dynamic_cast<CEmptyGameObject*>(obj)->GetRenderGroup();
			out << YAML::Key << "FrustumRange" << YAML::Value << dynamic_cast<CEmptyGameObject*>(obj)->GetFrustum();
		}

	}
}

void CSceneSerializer::SerializeUI(YAML::Emitter & out, CGameObject * obj)
{
	_float _max, _min, _spd;
	_bool isShrink = dynamic_cast<CEmptyUI*>(obj)->GetIsShrink();
	_bool isBreath = dynamic_cast<CEmptyUI*>(obj)->GetIsBreath();
	_bool xShrink = dynamic_cast<CEmptyUI*>(obj)->GetXShrink();
	_bool yShrink = dynamic_cast<CEmptyUI*>(obj)->GetYShrink();

	_max = dynamic_cast<CEmptyUI*>(obj)->GetShrinkMax();
	_min = dynamic_cast<CEmptyUI*>(obj)->GetShrinkMin();
	_spd = dynamic_cast<CEmptyUI*>(obj)->GetShrinkSpd();
	out << YAML::Key << "Type" << YAML::Value << "UI";
	out << YAML::Key << "SortingOrder" << YAML::Value << dynamic_cast<CEmptyUI*>(obj)->GetSortingOrder();
	out << YAML::Key << "PassIndex" << YAML::Value << obj->GetPassIndex();
	out << YAML::Key << "Hovering" << YAML::Value << dynamic_cast<CEmptyUI*>(obj)->GetIsHovering();
	if (isShrink) {
		out << YAML::Key << "Shrink" << YAML::BeginSeq << _max << _min << _spd << YAML::EndSeq;
		out << YAML::Key << "XShrink" << YAML::Value << xShrink;
		out << YAML::Key << "YShrink" << YAML::Value << yShrink;

		if (isBreath) {
			out << YAML::Key << "Breath" << YAML::Value << isBreath;
		}
	}
	if (obj->GetComponent("Com_Transform"))
	{
		CRectTransform* rectTransform = dynamic_cast<CRectTransform*>(obj->GetComponent("Com_Transform"));

		out << YAML::Key << "Com_Transform";
		out << YAML::BeginMap;

		CRectTransform::RECTTRANSFORMDESC desc = rectTransform->GetTransformDesc();
		out << YAML::Key << "Position";
		out << YAML::Value << YAML::Flow;
		out << YAML::BeginSeq << desc.posX << desc.posY << YAML::EndSeq;

		out << YAML::Key << "Size";
		out << YAML::Value << YAML::Flow;
		out << YAML::BeginSeq << desc.sizeX << desc.sizeY << YAML::EndSeq;

		out << YAML::EndMap;
	}

	if (obj->GetComponent("Com_VIBuffer"))
	{
		CVIBuffer_RectUI* buffer = dynamic_cast<CVIBuffer_RectUI*>(obj->GetComponent("Com_VIBuffer"));

		out << YAML::Key << "Com_VIBuffer";
		out << YAML::BeginMap;

		out << YAML::Key << "TexturePath" << YAML::Value << buffer->GetTextureFilePath(CVIBuffer_RectUI::TEXTURE_DIFFUSE);
		out << YAML::Key << "TextureMaskPath" << YAML::Value << buffer->GetTextureFilePath(CVIBuffer_RectUI::TEXUTRE_MASK);
		out << YAML::Key << "TextureNoisePath" << YAML::Value << buffer->GetTextureFilePath(CVIBuffer_RectUI::TEXUTRE_NOISE);
		out << YAML::Key << "TextureAnyPath" << YAML::Value << buffer->GetTextureFilePath(CVIBuffer_RectUI::TEXUTRE_NORMAL);

		_float4 color = buffer->GetColor();
		out << YAML::Key << "Color";
		out << YAML::Value << YAML::Flow;
		out << YAML::BeginSeq << color.x << color.y << color.z << color.w << YAML::EndSeq;

		out << YAML::EndMap;
	}
	if (obj->GetComponent("Com_Text"))
	{
		CText* text = dynamic_cast<CText*>(obj->GetComponent("Com_Text"));

		out << YAML::Key << "Com_Text";
		out << YAML::BeginMap;

		out << YAML::Key << "Text" << YAML::Value << text->GetText();

		_float4 color = text->GetColor();
		_float2 scale = text->GetScale();
		_bool	shaderCut = text->GetShaderCut();

		out << YAML::Key << "Color";
		out << YAML::Value << YAML::Flow;
		out << YAML::BeginSeq << color.x << color.y << color.z << color.w << YAML::EndSeq;

		out << YAML::Key << "Scale" << YAML::Flow;
		out << YAML::BeginSeq << scale.x << scale.y << YAML::EndSeq;

		out << YAML::Key << "LayerDepth" << YAML::Value << text->GetLayerDepth();

		out << YAML::Key << "IsCenter" << YAML::Value << text->GetIsCenter();

		out << YAML::Key << "ShaderCut" << YAML::Value << shaderCut;

		out << YAML::Key << "IsKorean" << YAML::Value << text->GetIsKorean();

		out << YAML::EndMap;

	}

}


void CSceneSerializer::SerializeEffect(YAML::Emitter & out, CGameObject * obj)
{
	out << YAML::Key << "Type" << YAML::Value << "Effect";
	out << YAML::Key << "PassIndex" << YAML::Value << obj->GetPassIndex();
	out << YAML::Key << "BillBord" << YAML::Value << static_cast<CEmptyEffect*>(obj)->GetBillBord();

	if (obj->GetComponent("Com_Transform"))
	{
		CTransform* transform = nullptr;
		if (obj->GetParent() == nullptr) {
			transform = dynamic_cast<CTransform*>(obj->GetComponent("Com_Transform"));
		}
		else {
			transform = dynamic_cast<CTransform*>(obj->GetComponent("Com_LocalTransform"));
		}

		//CTransform* transform = dynamic_cast<CTransform*>(obj->GetComponent("Com_Transform"));

		out << YAML::Key << "Com_Transform";
		out << YAML::BeginMap;

		XMMATRIX matrix = XMLoadFloat4x4(&transform->GetMatrix());
		//XMVECTOR tr, rt, sc;
		//XMMatrixDecompose(&sc, &rt, &tr, matrix);

		float _objMat[16];
		memcpy(_objMat, &matrix, sizeof(XMMATRIX));
		float matrixTranslation[3], matrixRotation[3], matrixScale[3];
		ImGuizmo::DecomposeMatrixToComponents(_objMat, matrixTranslation, matrixRotation, matrixScale);


		out << YAML::Key << "Translation";
		out << YAML::Value << YAML::Flow;
		out << YAML::BeginSeq << matrixTranslation[0] << matrixTranslation[1] << matrixTranslation[2] << YAML::EndSeq;

		out << YAML::Key << "Rotation";
		out << YAML::Value << YAML::Flow;
		out << YAML::BeginSeq << matrixRotation[0] << matrixRotation[1] << matrixRotation[2] << YAML::EndSeq;

		out << YAML::Key << "Scale";
		out << YAML::Value << YAML::Flow;
		out << YAML::BeginSeq << matrixScale[0] << matrixScale[1] << matrixScale[2] << YAML::EndSeq;


		out << YAML::EndMap;
	}

	if (obj->GetComponent("Com_Model"))
	{
		CModel* pModel = dynamic_cast<CModel*>(obj->GetComponent("Com_Model"));

		out << YAML::Key << "Com_Model";
		out << YAML::BeginMap;

		out << YAML::Key << "MeshFilePath" << YAML::Value << pModel->GetMeshFilePath();
		out << YAML::Key << "MeshFileName" << YAML::Value << pModel->GetMeshFileName();
		out << YAML::Key << "HasCollider" << YAML::Value << pModel->HasMeshCollider();
		out << YAML::Key << "EffectFilePath" << YAML::Value << pModel->GetEffectFilePath();

		out << YAML::EndMap;
	}

	if (obj->GetComponent("Com_OBB"))
	{
		CBasicCollider* Obb = dynamic_cast<CBasicCollider*>(obj->GetComponent("Com_OBB"));

		// Check if RB exist
		out << YAML::Key << "Com_OBB";
		out << YAML::BeginMap;

		_float3 center = Obb->GetPos();
		out << YAML::Key << "Center";
		out << YAML::Value << YAML::Flow;
		out << YAML::BeginSeq << center.x << center.y << center.z << YAML::EndSeq;

		_float3 boxSize = Obb->GetSize();

		out << YAML::Key << "Size" << YAML::Value << YAML::Flow;
		out << YAML::BeginSeq << boxSize.x << boxSize.y << boxSize.z << YAML::EndSeq;

		out << YAML::EndMap;
	}

	if (obj->GetComponent("Com_VIBuffer"))
	{
		//CVIBuffer_RectEffect* buffer = dynamic_cast<CVIBuffer_RectEffect*>(obj->GetComponent("Com_VIBuffer"));

		out << YAML::Key << "Com_VIBuffer";
		out << YAML::BeginMap;
		out << YAML::EndMap;
	}

	if (obj->GetComponent("Com_Decal"))
	{
		out << YAML::Key << "Com_Decal";
		out << YAML::BeginMap;
		out << YAML::EndMap;
	}

	if (obj->GetComponent("Com_PointInstance"))
	{
		out << YAML::Key << "Com_PointInstance";
		out << YAML::BeginMap;
		out << YAML::EndMap;		
	}

	if (obj->GetComponent("Com_RectInstance"))
	{
		CVIBuffer_RectInstance* pRectInst = static_cast<CVIBuffer_RectInstance*>(obj->GetComponent("Com_RectInstance"));

		out << YAML::Key << "Com_RectInstance";
		out << YAML::BeginMap;

		out << YAML::Key << "Shape" << YAML::Value << pRectInst->Get_CurShape();
		out << YAML::Key << "InstNum" << YAML::Value << pRectInst->Get_InstanceNum();

		out << YAML::Key << "Color";
		out << YAML::Value << YAML::Flow;
		out << YAML::BeginSeq << pRectInst->Get_Color().x << pRectInst->Get_Color().y << pRectInst->Get_Color().z << 0.f << YAML::EndSeq;

		out << YAML::Key << "Speed" << YAML::Value << pRectInst->Get_Speed();
		out << YAML::Key << "LifeTime" << YAML::Value << pRectInst->Get_LifeTime();
		out << YAML::Key << "Size" << YAML::Value << pRectInst->Get_Size();
		out << YAML::Key << "StartRadian" << YAML::Value << pRectInst->Get_StartRadian();
		out << YAML::Key << "RadiationAngle" << YAML::Value << pRectInst->Get_RadiationAngle();
				
		out << YAML::Key << "LerpColor" << YAML::Value << pRectInst->Get_LerpColor();

		_float4 srcColor = pRectInst->Get_SrcColor();
		out << YAML::Key << "SrcColor";
		out << YAML::Value << YAML::Flow;
		out << YAML::BeginSeq << srcColor.x << srcColor.y << srcColor.z << srcColor.w << YAML::EndSeq;

		_float4 destColor = pRectInst->Get_DestColor();
		out << YAML::Key << "DestColor";
		out << YAML::Value << YAML::Flow;
		out << YAML::BeginSeq << destColor.x << destColor.y << destColor.z << destColor.w << YAML::EndSeq;


		out << YAML::EndMap;
	}

	/* Effect_Setting */
	CEmptyEffect* pGameObj = static_cast<CEmptyEffect*>(obj);

	out << YAML::Key << "Effect_Setting";
	out << YAML::BeginMap;

	out << YAML::Key << "TextureDiffusePath" << YAML::Value << pGameObj->GetTextureFilePath(CEmptyEffect::TEXTURE_DIFFUSE);
	out << YAML::Key << "TextureMaskPath" << YAML::Value << pGameObj->GetTextureFilePath(CEmptyEffect::TEXTURE_MASK);
	out << YAML::Key << "TextureNoisePath" << YAML::Value << pGameObj->GetTextureFilePath(CEmptyEffect::TEXTURE_NOISE);
	out << YAML::Key << "TextureNormalPath" << YAML::Value << pGameObj->GetTextureFilePath(CEmptyEffect::TEXTURE_NORMAL);

	out << YAML::Key << "ScrollSpeedX";
	out << YAML::Value << YAML::Flow;
	out << YAML::BeginSeq << pGameObj->GetScrollSpeedX().x << pGameObj->GetScrollSpeedX().y << pGameObj->GetScrollSpeedX().z << YAML::EndSeq;

	out << YAML::Key << "ScrollSpeedY";
	out << YAML::Value << YAML::Flow;
	out << YAML::BeginSeq << pGameObj->GetScrollSpeedY().x << pGameObj->GetScrollSpeedY().y << pGameObj->GetScrollSpeedY().z << YAML::EndSeq;

	out << YAML::Key << "Distortion1";
	out << YAML::Value << YAML::Flow;
	out << YAML::BeginSeq << pGameObj->GetDistortion(0).x << pGameObj->GetDistortion(0).y << YAML::EndSeq;

	out << YAML::Key << "Distortion2";
	out << YAML::Value << YAML::Flow;
	out << YAML::BeginSeq << pGameObj->GetDistortion(1).x << pGameObj->GetDistortion(1).y << YAML::EndSeq;

	out << YAML::Key << "Distortion3";
	out << YAML::Value << YAML::Flow;
	out << YAML::BeginSeq << pGameObj->GetDistortion(2).x << pGameObj->GetDistortion(2).y << YAML::EndSeq;

	out << YAML::Key << "DistortionScale" << YAML::Value << pGameObj->GetDistortionScale();
	out << YAML::Key << "DistortionBias" << YAML::Value << pGameObj->GetDistortionBias();

	out << YAML::Key << "FadeOut" << YAML::Value << pGameObj->GetFadeOutEnable();
	out << YAML::Key << "FadeOutDuration" << YAML::Value << pGameObj->GetFadeOutDuration();
	out << YAML::Key << "FadeOutStratTime" << YAML::Value << pGameObj->GetFadeOutStartTime();
	
	out << YAML::Key << "FadeIn" << YAML::Value << pGameObj->GetFadeInEnable();
	out << YAML::Key << "FadeInDuration" << YAML::Value << pGameObj->GetFadeInDuration();
	out << YAML::Key << "FadeInStratTime" << YAML::Value << pGameObj->GetFadeInStartTime();
	
	out << YAML::Key << "EffectDuration" << YAML::Value << pGameObj->GetEffectDuration();

	out << YAML::Key << "SpriteNumX" << YAML::Value << pGameObj->GetSpriteX();
	out << YAML::Key << "SpriteNumY" << YAML::Value << pGameObj->GetSpriteY();
	out << YAML::Key << "SpriteTotal" << YAML::Value << pGameObj->GetSpriteTotal();
	out << YAML::Key << "SpriteSpeed" << YAML::Value << pGameObj->GetSpriteSpeed();
	//out << YAML::Key << "Alpha" << YAML::Value << pGameObj->GetAlpha();
	out << YAML::Key << "OffsetColor";
	out << YAML::Value << YAML::Flow;
	out << YAML::BeginSeq << pGameObj->GetOffsetColor().x << pGameObj->GetOffsetColor().y << pGameObj->GetOffsetColor().z << pGameObj->GetOffsetColor().w << YAML::EndSeq;

	out << YAML::EndMap;
}

CGameObject* CSceneSerializer::DeserializeUI(YAML::Node& obj, _bool bSpawn, _uint curScene)
{
	uint64_t uuid = 0;
	if (!bSpawn)
		uuid = obj["UUID"].as<uint64_t>();

	auto name = obj["Name"].as<string>();
	// auto uuid = obj["UUID"].as<uint64_t>();
	auto layer = obj["Layer"].as<string>();
	auto active = obj["Active"].as<_bool>();
	GameObjectMutex.lock();
	CGameObject* deserializedObject = m_pEngine->AddGameObject(curScene, "Prototype_EmptyUI", layer);
	deserializedObject->SetInfo(name, layer, uuid, active, curScene);
	GameObjectMutex.unlock();


	auto sortingOrder = obj["SortingOrder"].as<_int>();
	dynamic_cast<CEmptyUI*>(deserializedObject)->SetSortingOrder(sortingOrder);

	if (obj["PassIndex"])
	{
		auto renderIndex = obj["PassIndex"].as<_int>();
		deserializedObject->SetPassIndex(renderIndex);
	}
	if (obj["Hovering"])
	{
		auto hovering = obj["Hovering"].as<_bool>();
		dynamic_cast<CEmptyUI*>(deserializedObject)->SetIsHovering(hovering);
	}
	if (obj["Shrink"]) {
		auto shrink = obj["Shrink"];
		_float _max = shrink[0].as<_float>();
		_float _min = shrink[1].as<_float>();
		_float _spd = shrink[2].as<_float>();
		if (obj["XShrink"]) {
			auto xShrink = obj["XShrink"].as<_bool>();
			dynamic_cast<CEmptyUI*>(deserializedObject)->SetIsXShrink(xShrink);
		}
		if (obj["YShrink"]) {
			auto yShrink = obj["YShrink"].as<_bool>();
			dynamic_cast<CEmptyUI*>(deserializedObject)->SetIsYShrink(yShrink);
		}

		dynamic_cast<CEmptyUI*>(deserializedObject)->SetShrinkInfo(_max, _min, _spd);
		dynamic_cast<CEmptyUI*>(deserializedObject)->SetIsShrink(true);
	}
	if (obj["Breath"]) {
		auto breath = obj["Breath"].as<_bool>();
		dynamic_cast<CEmptyUI*>(deserializedObject)->SetIsBreath(breath);
	}
	auto transformCom = obj["Com_Transform"];
	if (transformCom)
	{
		CRectTransform::RECTTRANSFORMDESC desc;

		auto pos = transformCom["Position"];
		desc.posX = pos[0].as<float>();
		desc.posY = pos[1].as<float>();
		auto size = transformCom["Size"];
		desc.sizeX = size[0].as<float>();
		desc.sizeY = size[1].as<float>();
		desc.correctX = 1.f;
		desc.correctY = 1.f;
		dynamic_cast<CEmptyUI*>(deserializedObject)->SetRectTransform(desc);
	}

	auto viBuffer = obj["Com_VIBuffer"];
	if (viBuffer)
	{
		CComponent* pTransform = deserializedObject->GetComponent("Com_Transform");
		deserializedObject->AddComponent(0, "Prototype_VIBuffer_RectUI", "Com_VIBuffer", pTransform);
		CComponent* pVIBuffer = deserializedObject->GetComponent("Com_VIBuffer");

		string texturePath = viBuffer["TexturePath"].as<string>();
		if ("" != texturePath)
		{
			dynamic_cast<CVIBuffer_RectUI*>(pVIBuffer)->UpdateTexture(texturePath, CVIBuffer_RectUI::TEXTURE_DIFFUSE);
			texturePath.clear();
		}
		if (viBuffer["TextureMaskPath"]) {
			texturePath = viBuffer["TextureMaskPath"].as<string>();
			if ("" != texturePath)
			{
				dynamic_cast<CVIBuffer_RectUI*>(pVIBuffer)->UpdateTexture(texturePath, CVIBuffer_RectUI::TEXUTRE_MASK);
				texturePath.clear();
			}
		}
		if (viBuffer["TextureNoisePath"]) {
			texturePath = viBuffer["TextureNoisePath"].as<string>();
			if ("" != texturePath)
			{
				dynamic_cast<CVIBuffer_RectUI*>(pVIBuffer)->UpdateTexture(texturePath, CVIBuffer_RectUI::TEXUTRE_NOISE);
				texturePath.clear();
			}
		}
		if (viBuffer["TextureAnyPath"]) {
			texturePath = viBuffer["TextureAnyPath"].as<string>();
			if ("" != texturePath)
			{
				dynamic_cast<CVIBuffer_RectUI*>(pVIBuffer)->UpdateTexture(texturePath, CVIBuffer_RectUI::TEXUTRE_NORMAL);
				texturePath.clear();
			}
		}
		_float4 color;
		auto sequence = viBuffer["Color"];
		color.x = sequence[0].as<float>();
		color.y = sequence[1].as<float>();
		color.z = sequence[2].as<float>();
		color.w = sequence[3].as<float>();
		dynamic_cast<CVIBuffer_RectUI*>(pVIBuffer)->SetColor(color);
	}
	auto text = obj["Com_Text"];
	if (text)
	{
		CComponent* pTransform = deserializedObject->GetComponent("Com_Transform");
		deserializedObject->AddComponent(0, "Prototype_Text", "Com_Text", pTransform);
		CText* pText = dynamic_cast<CText*>(deserializedObject->GetComponent("Com_Text"));

		string strText = text["Text"].as<string>();

		_float4 color;
		auto sequenceColor = text["Color"];
		color.x = sequenceColor[0].as<float>();
		color.y = sequenceColor[1].as<float>();
		color.z = sequenceColor[2].as<float>();
		color.w = sequenceColor[3].as<float>();

		_float2 scale;
		auto sequenceScale = text["Scale"];
		scale.x = sequenceScale[0].as<float>();
		scale.y = sequenceScale[1].as<float>();

		_float	layerDepth = text["LayerDepth"].as<float>();
		pText->SetTextInfo(strText, layerDepth, color, scale);
		if (text["IsCenter"]) {
			_bool	isCenter = text["IsCenter"].as<bool>();
			pText->SetIsCenter(isCenter);
		}
		if (text["ShaderCut"]) {
			_bool	isShaderCut = text["ShaderCut"].as<bool>();
			pText->SetShader(isShaderCut);
		}
		if (text["IsKorean"]) {
			_bool	isKorean = text["IsKorean"].as<bool>();
			pText->SetIsKorean(isKorean);
		}
	}

	return deserializedObject;
}

CGameObject* CSceneSerializer::DeserializeObject(YAML::Node & obj, _bool bSpawn, _uint curScene)
{
	if (!bSpawn)
	{
		if (CEngine::GetInstance()->GetCurrentUsage() == CEngine::USAGE::USAGE_CLIENT)
		{
			_bool _bInstancing = false;
			if (obj["Instancing"])
				_bInstancing = obj["Instancing"].as<_bool>();

			if (_bInstancing)
			{
				InsModelInsertMap(obj);
				return nullptr;
			}
			else
				return NoneInstancingObject(obj, bSpawn, curScene);
		}
		else //Tool
			return NoneInstancingObject(obj, bSpawn, curScene);
	}
	else
		return NoneInstancingObject(obj, bSpawn, curScene);
}


CGameObject * CSceneSerializer::DeserializeEffect(YAML::Node & obj, _bool bSpawn, _uint curScene)
{
	uint64_t uuid = 0;
	if (!bSpawn)
		uuid = obj["UUID"].as<uint64_t>();

	auto name = obj["Name"].as<string>();
	auto layer = obj["Layer"].as<string>();
	auto active = obj["Active"].as<_bool>();
	int renderGroup = 3;

	GameObjectMutex.lock();
	CGameObject* deserializedObject = m_pEngine->AddGameObject(curScene, "Prototype_EmptyEffect", layer);
	deserializedObject->SetInfo(name, layer, uuid, active, curScene);
	GameObjectMutex.unlock();

	if (obj["BillBord"])
	{
		auto billBord = obj["BillBord"].as<_bool>();
		static_cast<CEmptyEffect*>(deserializedObject)->SetBillBord(billBord);
	}

	if (obj["PassIndex"])
	{
		auto renderIndex = obj["PassIndex"].as<_int>();
		deserializedObject->SetPassIndex(renderIndex);
	}

	deserializedObject->SetInfo(name, layer, uuid, active, 0);

	auto transformCom = obj["Com_Transform"];
	if (transformCom)
	{
		//_float3 tr, rt, sc;
		float tr[3], rt[3], sc[3];
		auto sequence = transformCom["Translation"];
		tr[0] = sequence[0].as<float>();
		tr[1] = sequence[1].as<float>();
		tr[2] = sequence[2].as<float>();
		sequence = transformCom["Rotation"];
		rt[0] = sequence[0].as<float>();
		rt[1] = sequence[1].as<float>();
		rt[2] = sequence[2].as<float>();
		sequence = transformCom["Scale"];
		sc[0] = sequence[0].as<float>();
		sc[1] = sequence[1].as<float>();
		sc[2] = sequence[2].as<float>();

		float _objMat[16]; XMFLOAT4X4 objMat;
		ImGuizmo::RecomposeMatrixFromComponents(tr, rt, sc, _objMat);
		memcpy(&objMat, _objMat, sizeof(XMFLOAT4X4));

		CComponent* pTransform = deserializedObject->GetComponent("Com_Transform");
		static_cast<CTransform*>(pTransform)->SetMatrix(objMat);
	}

	auto modelCom = obj["Com_Model"];
	if (modelCom)
	{
		string meshFilePath = modelCom["MeshFilePath"].as<string>();
		string meshFileName = modelCom["MeshFileName"].as<string>();
		string shaderFilePath = "../../Assets/Shader/Shader_Effect.fx";
		string EffectFilePath = modelCom["EffectFilePath"].as<string>();

		auto shaderPath = modelCom["ShaderPath"];
		if (shaderPath)
			shaderFilePath = modelCom["ShaderPath"].as<string>();
		_bool hasCollider = modelCom["HasCollider"].as<_bool>();

		if (!bSpawn)
			m_pEngine->CloneModel(deserializedObject, meshFilePath, meshFileName, shaderFilePath, EffectFilePath, hasCollider, deserializedObject->GetComponent("Com_Transform"));
		else
			CModelManager::GetInstance()->CloneModelThread(deserializedObject, meshFilePath, meshFileName, shaderFilePath, EffectFilePath, hasCollider, deserializedObject->GetComponent("Com_Transform"));
	}

	auto OBB = obj["Com_OBB"];
	if (OBB)
	{
		if (deserializedObject->AddComponent(0, "Prototype_OBBCollider", "Com_OBB"))
			MSG_BOX("Failed to Com_OBB");

		CComponent* pCollider = deserializedObject->GetComponent("Com_OBB");

		auto center_n = OBB["Center"];
		_float3 Pos;
		Pos.x = center_n[0].as<float>();
		Pos.y = center_n[1].as<float>();
		Pos.z = center_n[2].as<float>();

		dynamic_cast<CBasicCollider*>(pCollider)->SetPos(Pos);

		auto size_n = OBB["Size"];
		_float3 size;
		size.x = size_n[0].as<float>();
		size.y = size_n[1].as<float>();
		size.z = size_n[2].as<float>();

		dynamic_cast<CBasicCollider*>(pCollider)->SetSize(size);
	}

	auto RectCom = obj["Com_VIBuffer"];
	if (RectCom)
	{
		if (deserializedObject->AddComponent(0, "Prototype_VIBuffer_RectEffect", "Com_VIBuffer", deserializedObject->GetComponent("Com_Transform")))
			MSG_BOX("Failed to AddComponent Prototype_VIBuffer_RectEffect");

		//string DiffuseFilePath = RectCom["TextureDiffusePath"].as<string>();

		//CVIBuffer_RectEffect* pRectBuffer = dynamic_cast<CVIBuffer_RectEffect*>(deserializedObject->GetComponent("Com_VIBuffer"));
		//pRectBuffer->SetTexture(DiffuseFilePath);
	}

	auto DecalCom = obj["Com_Decal"];
	if (DecalCom)
	{
		if (deserializedObject->AddComponent(0, "Prototype_VIBuffer_Decal", "Com_Decal", deserializedObject->GetComponent("Com_Transform")))
			MSG_BOX("Failed to AddComponent Prototype_VIBuffer_Decal");
	}

	auto PointInstanceCom = obj["Com_PointInstance"];
	if (PointInstanceCom)
	{
		if (deserializedObject->AddComponent(0, "Prototype_VIBuffer_PointInstance", "Com_PointInstance", deserializedObject->GetComponent("Com_Transform")))
			MSG_BOX("Failed to AddComponent Prototype_VIBuffer_PointInstance");
	}

	auto RectInstanceCom = obj["Com_RectInstance"];
	if (RectInstanceCom)
	{
		if (deserializedObject->AddComponent(0, "Prototype_VIBuffer_RectInstance", "Com_RectInstance", deserializedObject->GetComponent("Com_Transform")))
			MSG_BOX("Failed to AddComponent Prototype_VIBuffer_RectInstance");

		_float4 vColor = { 0.f, 0.f, 0.f, 0.f };
		_float4 vSrcColor{};
		_float4 vDestColor{};
		_float fSpeed = RectInstanceCom["Speed"].as<float>();
		_float fSize = RectInstanceCom["Size"].as<float>();
		_float fStartRadian = RectInstanceCom["StartRadian"].as<float>();
		_float fRadiationAngle = RectInstanceCom["RadiationAngle"].as<float>();
		_float fLifeTime = RectInstanceCom["LifeTime"].as<float>();
		_int iInstNum = RectInstanceCom["InstNum"].as<int>();
		_int iShape = RectInstanceCom["Shape"].as<int>();
		_bool isLerp = RectInstanceCom["LerpColor"].as<bool>();

		auto sequence = RectInstanceCom["Color"];
		vColor.x = sequence[0].as<float>();
		vColor.y = sequence[1].as<float>();
		vColor.z = sequence[2].as<float>();

		auto srcColor = RectInstanceCom["SrcColor"];
		vSrcColor.x = srcColor[0].as<float>();
		vSrcColor.y = srcColor[1].as<float>();
		vSrcColor.z = srcColor[2].as<float>();
		vSrcColor.w = 0.f;

		auto destColor = RectInstanceCom["DestColor"];
		vDestColor.x = destColor[0].as<float>();
		vDestColor.y = destColor[1].as<float>();
		vDestColor.z = destColor[2].as<float>();
		vDestColor.w = 0.f;

		CVIBuffer_RectInstance* pRectInst = static_cast<CVIBuffer_RectInstance*>(deserializedObject->GetComponent("Com_RectInstance"));

		pRectInst->Set_Color(vColor);
		pRectInst->Set_SrcColor(vSrcColor);
		pRectInst->Set_DestColor(vDestColor);
		pRectInst->Set_Speed(fSpeed);
		pRectInst->Set_Size(fSize);
		pRectInst->Set_StartRadian(fStartRadian);
		pRectInst->Set_RadiationAngle(fRadiationAngle);
		pRectInst->Set_LifeTime(fLifeTime);
		pRectInst->Set_InstanceNum(iInstNum);
		pRectInst->Set_lerpColor(isLerp);
		if (iShape == 0)
		{
			pRectInst->Set_Shape(CVIBuffer_RectInstance::RADIATION);
			pRectInst->Initialize_Radiation();
		}
		else
		{
			pRectInst->Set_Shape(CVIBuffer_RectInstance::CONE);
			pRectInst->Initialize_Cone();
		}
	}

	auto EffectSetting = obj["Effect_Setting"];
	if (EffectSetting)
	{
		string DiffuseFilePath = EffectSetting["TextureDiffusePath"].as<string>();
		string MaskFilePath = EffectSetting["TextureMaskPath"].as<string>();
		string NoiseFilePath = EffectSetting["TextureNoisePath"].as<string>();
		string NormalFilePath = "";
		if (EffectSetting["TextureNormalPath"])
			NormalFilePath = EffectSetting["TextureNormalPath"].as<string>();

		_float4 vOffsetColor = { 0.f, 0.f, 0.f, 0.f };
		_float3 vScrollSpeedX;
		_float3 vScrollSpeedY;
		_float2 vDistortion1;
		_float2 vDistortion2;
		_float2 vDistortion3;
		_float fDistortionScale = EffectSetting["DistortionScale"].as<float>();
		_float fDistortionBias = EffectSetting["DistortionBias"].as<float>();

		auto sequence = EffectSetting["ScrollSpeedX"];
		vScrollSpeedX.x = sequence[0].as<float>();
		vScrollSpeedX.y = sequence[1].as<float>();
		vScrollSpeedX.z = sequence[2].as<float>();
		sequence = EffectSetting["ScrollSpeedY"];
		vScrollSpeedY.x = sequence[0].as<float>();
		vScrollSpeedY.y = sequence[1].as<float>();
		vScrollSpeedY.z = sequence[2].as<float>();
		sequence = EffectSetting["Distortion1"];
		vDistortion1.x = sequence[0].as<float>();
		vDistortion1.y = sequence[1].as<float>();
		sequence = EffectSetting["Distortion2"];
		vDistortion2.x = sequence[0].as<float>();
		vDistortion2.y = sequence[1].as<float>();
		sequence = EffectSetting["Distortion3"];
		vDistortion3.x = sequence[0].as<float>();
		vDistortion3.y = sequence[1].as<float>();

		if (EffectSetting["OffsetColor"])
		{
			sequence = EffectSetting["OffsetColor"];
			vOffsetColor.x = sequence[0].as<float>();
			vOffsetColor.y = sequence[1].as<float>();
			vOffsetColor.z = sequence[2].as<float>();
			vOffsetColor.w = sequence[3].as<float>();
		}

		CEmptyEffect* pEffect = static_cast<CEmptyEffect*>(deserializedObject);

		pEffect->SetTexture(DiffuseFilePath, CEmptyEffect::TEXTURE_DIFFUSE);
		pEffect->SetTexture(MaskFilePath, CEmptyEffect::TEXTURE_MASK);
		pEffect->SetTexture(NoiseFilePath, CEmptyEffect::TEXTURE_NOISE);
		pEffect->SetTexture(NormalFilePath, CEmptyEffect::TEXTURE_NORMAL);
		pEffect->SetScrollSpeedX(vScrollSpeedX);
		pEffect->SetScrollSpeedY(vScrollSpeedY);
		pEffect->setDistortion(0, vDistortion1);
		pEffect->setDistortion(1, vDistortion2);
		pEffect->setDistortion(2, vDistortion3);
		pEffect->SetDistortionScale(fDistortionScale);
		pEffect->SetDistortionBias(fDistortionBias);
		pEffect->SetOffsetColor(vOffsetColor);

		if (EffectSetting["FadeOut"]) {
			_bool	FadeOut = EffectSetting["FadeOut"].as<_bool>();
			pEffect->SetFadeOutEnable(FadeOut);
		}
		if (EffectSetting["FadeOutDuration"]) {
			_float	FadeOutDuration = EffectSetting["FadeOutDuration"].as<_float>();
			pEffect->SetFadeOutDuration(FadeOutDuration);
		}
		if (EffectSetting["FadeOutStratTime"]) {
			_float	FadeOutStratTime = EffectSetting["FadeOutStratTime"].as<_float>();
			pEffect->SetFadeOutStartTime(FadeOutStratTime);
		}
		if (EffectSetting["FadeIn"]) {
			_bool	FadeIn = EffectSetting["FadeIn"].as<_bool>();
			pEffect->SetFadeInEnable(FadeIn);
		}
		if (EffectSetting["FadeInDuration"]) {
			_float	FadeInDuration = EffectSetting["FadeInDuration"].as<_float>();
			pEffect->SetFadeInDuration(FadeInDuration);
		}
		if (EffectSetting["FadeInStratTime"]) {
			_float	FadeInStratTime = EffectSetting["FadeInStratTime"].as<_float>();
			pEffect->SetFadeInStartTime(FadeInStratTime);
		}
		if (EffectSetting["EffectDuration"]) {
			_float	EffectDuration = EffectSetting["EffectDuration"].as<_float>();
			pEffect->SetEffectDuration(EffectDuration);
		}
		if (EffectSetting["SpriteNumX"]) {
			_int	NumX = EffectSetting["SpriteNumX"].as<_int>();
			pEffect->SetSpriteX(NumX);
		}
		if (EffectSetting["SpriteNumY"]) {
			_int	NumY = EffectSetting["SpriteNumY"].as<_int>();
			pEffect->SetSpriteY(NumY);
		}
		if (EffectSetting["SpriteTotal"]) {
			_int	NumTotal = EffectSetting["SpriteTotal"].as<_int>();
			pEffect->SetSpriteTotal(NumTotal);
		}
		if (EffectSetting["SpriteSpeed"]) {
			_float	fSpriteSpeed = EffectSetting["SpriteSpeed"].as<_float>();
			pEffect->SetSpriteSpeed(fSpriteSpeed);
		}		
	}

	return deserializedObject;
}

CGameObject * CSceneSerializer::DeserializePrototypeEffect(string pPrototypeTag, YAML::Node & obj, _bool bSpawn, _uint curScene)
{
	uint64_t uuid = 0;
	if (!bSpawn)
		uuid = obj["UUID"].as<uint64_t>();

	auto name = obj["Name"].as<string>();
	auto layer = obj["Layer"].as<string>();
	auto active = obj["Active"].as<_bool>();
	int renderGroup = 3;

	GameObjectMutex.lock();
	CGameObject* deserializedObject = m_pEngine->ClonePrototype("Prototype_EmptyEffect", pPrototypeTag);
	deserializedObject->SetPrefabInfo(name, layer, uuid, active, curScene);
	GameObjectMutex.unlock();


	if (obj["PassIndex"])
	{
		auto renderIndex = obj["PassIndex"].as<_int>();
		deserializedObject->SetPassIndex(renderIndex);
	}

	if (obj["BillBord"])
	{
		auto billBord = obj["BillBord"].as<_bool>();
		static_cast<CEmptyEffect*>(deserializedObject)->SetBillBord(billBord);
	}

	auto transformCom = obj["Com_Transform"];
	if (transformCom)
	{
		//_float3 tr, rt, sc;
		float tr[3], rt[3], sc[3];
		auto sequence = transformCom["Translation"];
		tr[0] = sequence[0].as<float>();
		tr[1] = sequence[1].as<float>();
		tr[2] = sequence[2].as<float>();
		sequence = transformCom["Rotation"];
		rt[0] = sequence[0].as<float>();
		rt[1] = sequence[1].as<float>();
		rt[2] = sequence[2].as<float>();
		sequence = transformCom["Scale"];
		sc[0] = sequence[0].as<float>();
		sc[1] = sequence[1].as<float>();
		sc[2] = sequence[2].as<float>();

		float _objMat[16]; XMFLOAT4X4 objMat;
		ImGuizmo::RecomposeMatrixFromComponents(tr, rt, sc, _objMat);
		memcpy(&objMat, _objMat, sizeof(XMFLOAT4X4));

		CComponent* pTransform = deserializedObject->GetComponent("Com_Transform");
		dynamic_cast<CTransform*>(pTransform)->SetMatrix(objMat);
	}

	auto modelCom = obj["Com_Model"];
	if (modelCom)
	{
		string meshFilePath = modelCom["MeshFilePath"].as<string>();
		string meshFileName = modelCom["MeshFileName"].as<string>();
		string shaderFilePath = "../../Assets/Shader/Shader_Effect.fx";
		string EffectFilePath = modelCom["EffectFilePath"].as<string>();

		auto shaderPath = modelCom["ShaderPath"];
		if (shaderPath)
			shaderFilePath = modelCom["ShaderPath"].as<string>();
		_bool hasCollider = modelCom["HasCollider"].as<_bool>();

		if (!bSpawn)
			m_pEngine->CloneModel(deserializedObject, meshFilePath, meshFileName, shaderFilePath, EffectFilePath, hasCollider, deserializedObject->GetComponent("Com_Transform"));
		else
			CModelManager::GetInstance()->CloneModelThread(deserializedObject, meshFilePath, meshFileName, shaderFilePath, EffectFilePath, hasCollider, deserializedObject->GetComponent("Com_Transform"));
	}

	auto RectCom = obj["Com_VIBuffer"];
	if (RectCom)
	{
		if (deserializedObject->AddComponent(0, "Prototype_VIBuffer_RectEffect", "Com_VIBuffer", deserializedObject->GetComponent("Com_Transform")))
			MSG_BOX("Failed to AddComponent Prototype_VIBuffer_RectEffect");

		//string DiffuseFilePath = RectCom["TextureDiffusePath"].as<string>();

		CVIBuffer_RectEffect* pRectBuffer = dynamic_cast<CVIBuffer_RectEffect*>(deserializedObject->GetComponent("Com_VIBuffer"));
		//pRectBuffer->SetTexture(DiffuseFilePath);
	}

	auto DecalCom = obj["Com_Decal"];
	if (DecalCom)
	{
		if (deserializedObject->AddComponent(0, "Prototype_VIBuffer_Decal", "Com_Decal", deserializedObject->GetComponent("Com_Transform")))
			MSG_BOX("Failed to AddComponent Prototype_VIBuffer_Decal");
	}

	auto PointInstanceCom = obj["Com_PointInstance"];
	if (PointInstanceCom)
	{
		if (deserializedObject->AddComponent(0, "Prototype_VIBuffer_PointInstance", "Com_PointInstance", deserializedObject->GetComponent("Com_Transform")))
			MSG_BOX("Failed to AddComponent Prototype_VIBuffer_PointInstance");
	}

	auto RectInstanceCom = obj["Com_RectInstance"];
	if (RectInstanceCom)
	{
		if (deserializedObject->AddComponent(0, "Prototype_VIBuffer_RectInstance", "Com_RectInstance", deserializedObject->GetComponent("Com_Transform")))
			MSG_BOX("Failed to AddComponent Prototype_VIBuffer_RectInstance");

		_float4 vColor = { 0.f, 0.f, 0.f, 0.f };
		_float fSpeed = RectInstanceCom["Speed"].as<float>();
		_float fSize = RectInstanceCom["Size"].as<float>();
		_float fStartRadian = RectInstanceCom["StartRadian"].as<float>();
		_float fRadiationAngle = RectInstanceCom["RadiationAngle"].as<float>();
		_float fLifeTime = RectInstanceCom["LifeTime"].as<float>();
		_int iShape = RectInstanceCom["Shape"].as<int>();
		_int iInstNum = RectInstanceCom["InstNum"].as<int>();

		auto sequence = RectInstanceCom["Color"];
		vColor.x = sequence[0].as<float>();
		vColor.y = sequence[1].as<float>();
		vColor.z = sequence[2].as<float>();

		CVIBuffer_RectInstance* pRectInst = static_cast<CVIBuffer_RectInstance*>(deserializedObject->GetComponent("Com_RectInstance"));

		pRectInst->Set_Color(vColor);
		pRectInst->Set_Speed(fSpeed);
		pRectInst->Set_Size(fSize);
		pRectInst->Set_StartRadian(fStartRadian);
		pRectInst->Set_RadiationAngle(fRadiationAngle);
		pRectInst->Set_LifeTime(fLifeTime);
		pRectInst->Set_InstanceNum(iInstNum);

		if (iShape == 0)
		{
			pRectInst->Set_Shape(CVIBuffer_RectInstance::RADIATION);
			pRectInst->Initialize_Radiation();
		}
		else
		{
			pRectInst->Set_Shape(CVIBuffer_RectInstance::CONE);
			pRectInst->Initialize_Cone();
		}
	}

	auto EffectSetting = obj["Effect_Setting"];
	if (EffectSetting)
	{
		string DiffuseFilePath = EffectSetting["TextureDiffusePath"].as<string>();
		string MaskFilePath = EffectSetting["TextureMaskPath"].as<string>();
		string NoiseFilePath = EffectSetting["TextureNoisePath"].as<string>();
		string NormalFilePath = "";
		if (EffectSetting["TextureNormalPath"])
			NormalFilePath = EffectSetting["TextureNormalPath"].as<string>();

		_float4 vOffsetColor = { 0.f, 0.f, 0.f, 0.f };
		_float3 vScrollSpeedX;
		_float3 vScrollSpeedY;
		_float2 vDistortion1;
		_float2 vDistortion2;
		_float2 vDistortion3;
		_float fDistortionScale = EffectSetting["DistortionScale"].as<float>();
		_float fDistortionBias = EffectSetting["DistortionBias"].as<float>();
		

		auto sequence = EffectSetting["ScrollSpeedX"];
		vScrollSpeedX.x = sequence[0].as<float>();
		vScrollSpeedX.y = sequence[1].as<float>();
		vScrollSpeedX.z = sequence[2].as<float>();
		sequence = EffectSetting["ScrollSpeedY"];
		vScrollSpeedY.x = sequence[0].as<float>();
		vScrollSpeedY.y = sequence[1].as<float>();
		vScrollSpeedY.z = sequence[2].as<float>();
		sequence = EffectSetting["Distortion1"];
		vDistortion1.x = sequence[0].as<float>();
		vDistortion1.y = sequence[1].as<float>();
		sequence = EffectSetting["Distortion2"];
		vDistortion2.x = sequence[0].as<float>();
		vDistortion2.y = sequence[1].as<float>();
		sequence = EffectSetting["Distortion3"];
		vDistortion3.x = sequence[0].as<float>();
		vDistortion3.y = sequence[1].as<float>();

		if (EffectSetting["OffsetColor"])
		{
			sequence = EffectSetting["OffsetColor"];
			vOffsetColor.x = sequence[0].as<float>();
			vOffsetColor.y = sequence[1].as<float>();
			vOffsetColor.z = sequence[2].as<float>();
			vOffsetColor.w = sequence[3].as<float>();
		}

		CEmptyEffect* pEffect = static_cast<CEmptyEffect*>(deserializedObject);

		pEffect->SetTexture(DiffuseFilePath, CEmptyEffect::TEXTURE_DIFFUSE);
		pEffect->SetTexture(MaskFilePath, CEmptyEffect::TEXTURE_MASK);
		pEffect->SetTexture(NoiseFilePath, CEmptyEffect::TEXTURE_NOISE);
		pEffect->SetTexture(NormalFilePath, CEmptyEffect::TEXTURE_NORMAL);
		pEffect->SetScrollSpeedX(vScrollSpeedX);
		pEffect->SetScrollSpeedY(vScrollSpeedY);
		pEffect->setDistortion(0, vDistortion1);
		pEffect->setDistortion(1, vDistortion2);
		pEffect->setDistortion(2, vDistortion3);
		pEffect->SetDistortionScale(fDistortionScale);
		pEffect->SetDistortionBias(fDistortionBias);
		pEffect->SetOffsetColor(vOffsetColor);


		if (EffectSetting["FadeOut"]) {
			_bool	FadeOut = EffectSetting["FadeOut"].as<_bool>();
			pEffect->SetFadeOutEnable(FadeOut);
		}
		if (EffectSetting["FadeOutDuration"]) {
			_float	FadeOutDuration = EffectSetting["FadeOutDuration"].as<_float>();
			pEffect->SetFadeOutDuration(FadeOutDuration);
		}
		if (EffectSetting["FadeOutStratTime"]) {
			_float	FadeOutStratTime = EffectSetting["FadeOutStratTime"].as<_float>();
			pEffect->SetFadeOutStartTime(FadeOutStratTime);
		}
		if (EffectSetting["FadeIn"]) {
			_bool	FadeIn = EffectSetting["FadeIn"].as<_bool>();
			pEffect->SetFadeInEnable(FadeIn);
		}
		if (EffectSetting["FadeInDuration"]) {
			_float	FadeInDuration = EffectSetting["FadeInDuration"].as<_float>();
			pEffect->SetFadeInDuration(FadeInDuration);
		}
		if (EffectSetting["FadeInStratTime"]) {
			_float	FadeInStratTime = EffectSetting["FadeInStratTime"].as<_float>();
			pEffect->SetFadeInStartTime(FadeInStratTime);
		}
		if (EffectSetting["EffectDuration"]) {
			_float	EffectDuration = EffectSetting["EffectDuration"].as<_float>();
			pEffect->SetEffectDuration(EffectDuration);
		}
		if (EffectSetting["SpriteNumX"]) {
			_int	NumX = EffectSetting["SpriteNumX"].as<_int>();
			pEffect->SetSpriteX(NumX);
		}
		if (EffectSetting["SpriteNumY"]) {
			_int	NumY = EffectSetting["SpriteNumY"].as<_int>();
			pEffect->SetSpriteY(NumY);
		}
		if (EffectSetting["SpriteTotal"]) {
			_int	NumTotal = EffectSetting["SpriteTotal"].as<_int>();
			pEffect->SetSpriteTotal(NumTotal);
		}
		if (EffectSetting["SpriteSpeed"]) {
			_float	fSpriteSpeed = EffectSetting["SpriteSpeed"].as<_float>();
			pEffect->SetSpriteSpeed(fSpriteSpeed);
		}
	}

	return deserializedObject;
}

CGameObject * CSceneSerializer::DeserializePrototypeGameObject(string pPrototypeTag, YAML::Node & obj, _bool bSpawn, _uint curScene)
{
	auto name = obj["Name"].as<string>();
	auto uuid = obj["UUID"].as<uint64_t>();
	auto layer = obj["Layer"].as<string>();
	auto active = obj["Active"].as<_bool>();
	int numOBBs = 0;
	if (obj["NumOBB"])
		numOBBs = obj["NumOBB"].as<_int>();
	int renderGroup = 1;
	_float	FrustumRange = 1.f;
	_bool _bInstancing = false;
	if (obj["Instancing"])
		_bInstancing = obj["Instancing"].as<_bool>();
	if (obj["RenderGroup"])
		renderGroup = obj["RenderGroup"].as<_int>();
	if (obj["FrustumRange"])
		FrustumRange = obj["FrustumRange"].as<_float>();



	GameObjectMutex.lock();
	CGameObject* deserializedObject = m_pEngine->ClonePrototype("Prototype_EmptyGameObject", pPrototypeTag);
	deserializedObject->SetPrefabInfo(name, layer, uuid, active, curScene);
	GameObjectMutex.unlock();

	if (obj["PassIndex"])
	{
		auto renderIndex = obj["PassIndex"].as<_int>();
		deserializedObject->SetPassIndex(renderIndex);
	}

	dynamic_cast<CEmptyGameObject*>(deserializedObject)->SetRenderGroup((CRenderer::RENDER)renderGroup);
	dynamic_cast<CEmptyGameObject*>(deserializedObject)->SetFrustum(FrustumRange);
	dynamic_cast<CEmptyGameObject*>(deserializedObject)->SetNumOBB(numOBBs);

	auto transformCom = obj["Com_Transform"];
	if (transformCom)
	{
		//_float3 tr, rt, sc;
		float tr[3], rt[3], sc[3];
		auto sequence = transformCom["Translation"];
		tr[0] = sequence[0].as<float>();
		tr[1] = sequence[1].as<float>();
		tr[2] = sequence[2].as<float>();
		sequence = transformCom["Rotation"];
		rt[0] = sequence[0].as<float>();
		rt[1] = sequence[1].as<float>();
		rt[2] = sequence[2].as<float>();
		sequence = transformCom["Scale"];
		sc[0] = sequence[0].as<float>();
		sc[1] = sequence[1].as<float>();
		sc[2] = sequence[2].as<float>();

		float _objMat[16]; XMFLOAT4X4 objMat;
		ImGuizmo::RecomposeMatrixFromComponents(tr, rt, sc, _objMat);
		memcpy(&objMat, _objMat, sizeof(XMFLOAT4X4));

		CComponent* pTransform = deserializedObject->GetComponent("Com_Transform");
		dynamic_cast<CTransform*>(pTransform)->SetMatrix(objMat);
	}
	auto colliderCom = obj["Com_Collider"];
	if (colliderCom)
	{
		PhysxMutex.lock();
		auto centerInfo = colliderCom["Center"];
		_float3 center = { 0, 0, 0 };
		center.x = centerInfo[0].as<float>();
		center.y = centerInfo[1].as<float>();
		center.z = centerInfo[2].as<float>();

		auto angleInfo = colliderCom["Angle"];
		_float3 angle = { 0, 0, 0 };
		if (angleInfo) {
			angle.x = angleInfo[0].as<float>();
			angle.y = angleInfo[1].as<float>();
			angle.z = angleInfo[2].as<float>();
		}


		CCollider::RIGIDBODYDESC desc;
		ZeroMemory(&desc, sizeof(desc));

		auto rigidBody = colliderCom["RigidBody"];
		desc.bEnabled = rigidBody["Enabled"].as<bool>();
		desc.bGravity = rigidBody["Gravity"].as<bool>();
		desc.bKinematic = rigidBody["Kinematic"].as<bool>();
		desc.bCC = rigidBody["CharacterController"].as<bool>();

		string type = colliderCom["Type"].as<string>();
		if (type == "Box")
		{
			if (deserializedObject->AddComponent(0, "Prototype_BoxCollider", "Com_Collider", deserializedObject->GetComponent("Com_Transform")))
				MSG_BOX("Failed to AddComponent BoxCollider");

			auto sequence = colliderCom["Size"];
			_float3 size;
			size.x = sequence[0].as<float>();
			size.y = sequence[1].as<float>();
			size.z = sequence[2].as<float>();

			CComponent* pCollider = deserializedObject->GetComponent("Com_Collider");
			dynamic_cast<CCollider*>(pCollider)->SetRelativePos(center);
			dynamic_cast<CCollider*>(pCollider)->SetEuler(angle);
			dynamic_cast<CBoxCollider*>(pCollider)->SetUpRigidActor(&size, desc, deserializedObject);

		}
		else if (type == "Sphere")
		{
			if (deserializedObject->AddComponent(0, "Prototype_SphereCollider", "Com_Collider", deserializedObject->GetComponent("Com_Transform")))
				MSG_BOX("Failed to AddComponent Prototype_SphereCollider");

			_float radius = colliderCom["Radius"].as<float>();
			CComponent* pCollider = deserializedObject->GetComponent("Com_Collider");
			dynamic_cast<CCollider*>(pCollider)->SetRelativePos(center);
			dynamic_cast<CSphereCollider*>(pCollider)->SetUpRigidActor(&radius, desc, deserializedObject);
		}
		else if (type == "Capsule")
		{
			if (deserializedObject->AddComponent(0, "Prototype_CapsuleCollider", "Com_Collider", deserializedObject->GetComponent("Com_Transform")))
				MSG_BOX("Failed to AddComponent Prototype_CapsuleCollider");

			pair<float, float> capsuleSize;
			capsuleSize.first = colliderCom["Radius"].as<float>();
			capsuleSize.second = colliderCom["Height"].as<float>();
			CComponent* pCollider = deserializedObject->GetComponent("Com_Collider");
			dynamic_cast<CCollider*>(pCollider)->SetRelativePos(center);
			dynamic_cast<CCapsuleCollider*>(pCollider)->SetUpRigidActor(&capsuleSize, desc, deserializedObject);
		}
		PhysxMutex.unlock();
	}

	_int i = 0;
	do {
		string obb = "Com_OBB";
		if (i != 0)
			obb += to_string(i);
		auto OBB = obj[obb.c_str()];
		if (OBB)
		{
			if (deserializedObject->AddComponent(0, "Prototype_OBBCollider", obb))
				MSG_BOX("Failed to Com_OBB");

			CComponent* pCollider = deserializedObject->GetComponent(obb);

			auto center_n = OBB["Center"];
			_float3 Pos;
			Pos.x = center_n[0].as<float>();
			Pos.y = center_n[1].as<float>();
			Pos.z = center_n[2].as<float>();

			dynamic_cast<CBasicCollider*>(pCollider)->SetPos(Pos);

			auto size_n = OBB["Size"];
			_float3 size;
			size.x = size_n[0].as<float>();
			size.y = size_n[1].as<float>();
			size.z = size_n[2].as<float>();

			dynamic_cast<CBasicCollider*>(pCollider)->SetSize(size);

			auto attach = OBB["IsAttach"];
			if (attach) {
				_bool atc = attach.as<_bool>();
				dynamic_cast<CBasicCollider*>(pCollider)->SetIsAttachBone(atc);
			}

			auto boneName = OBB["BoneName"];
			if (boneName) {
				string name = boneName.as<string>();
				dynamic_cast<CBasicCollider*>(pCollider)->SetBoneName(name);
			}

			auto offset_n = OBB["Offset"];
			if (offset_n) {
				_float3 offset;
				offset.x = offset_n[0].as<_float>();
				offset.y = offset_n[1].as<_float>();
				offset.z = offset_n[2].as<_float>();
				dynamic_cast<CBasicCollider*>(pCollider)->SetOffset(offset);
			}

			auto CollType = OBB["CollisionType"];
			if (CollType) {
				_int coltype = CollType.as<_int>();
				dynamic_cast<CBasicCollider*>(pCollider)->SetCollisionType((ID)coltype);
			}
		}
		i++;
	} while (i < numOBBs);

	auto AABB = obj["Com_AABB"];
	if (AABB)
	{
		if (deserializedObject->AddComponent(0, "Prototype_AABBCollider", "Com_AABB"))
			MSG_BOX("Failed to Com_AABB");

		CComponent* pCollider = deserializedObject->GetComponent("Com_AABB");

		auto center_n = AABB["Center"];
		_float3 Pos;
		Pos.x = center_n[0].as<float>();
		Pos.y = center_n[1].as<float>();
		Pos.z = center_n[2].as<float>();

		dynamic_cast<CBasicCollider*>(pCollider)->SetPos(Pos);

		auto size_n = AABB["Size"];
		_float3 size;
		size.x = size_n[0].as<float>();
		size.y = size_n[1].as<float>();
		size.z = size_n[2].as<float>();

		dynamic_cast<CBasicCollider*>(pCollider)->SetSize(size);
	}



	auto viBufferCom = obj["Com_VIBuffer"];
	if (viBufferCom)
	{
		string type = viBufferCom["Type"].as<string>();
		if (type == "Terrain")
		{
			if (deserializedObject->AddComponent(0, "Prototype_VIBuffer_Terrain", "Com_VIBuffer", deserializedObject->GetComponent("Com_Transform")))
				MSG_BOX("Failed to AddComponent Prototype_VIBuffer_Terrain");

			string heightMapPath = viBufferCom["HeightMapPath"].as<string>();
			string texturePath = viBufferCom["TexturePath"].as<string>();

			CVIBuffer_Terrain* pTerrainBuffer = dynamic_cast<CVIBuffer_Terrain*>(deserializedObject->GetComponent("Com_VIBuffer"));
			pTerrainBuffer->SetHeightMapPath(heightMapPath);
			pTerrainBuffer->SetTexturePath(texturePath);
		}
		else {
			if (deserializedObject->AddComponent(0, "Prototype_VIBuffer_Rect", "Com_VIBuffer", deserializedObject->GetComponent("Com_Transform")))
				MSG_BOX("Failed to AddComponent Prototype_VIBuffer_Terrain");

			string texturePath = viBufferCom["TexturePath"].as<string>();


			CVIBuffer_Rect* pRectBuffer = dynamic_cast<CVIBuffer_Rect*>(deserializedObject->GetComponent("Com_VIBuffer"));
			pRectBuffer->UpdateTexture(texturePath);
		}
	}

	if (CEngine::GetInstance()->GetCurrentUsage() == CEngine::USAGE::USAGE_TOOL)
	{ //Original
		auto modelCom = obj["Com_Model"];
		if (modelCom)
		{
			string meshFilePath = modelCom["MeshFilePath"].as<string>();
			string meshFileName = modelCom["MeshFileName"].as<string>();
			string shaderFilePath = "";
			string EffectFilePath = "";

			auto shaderPath = modelCom["ShaderPath"];
			if (shaderPath)
				shaderFilePath = modelCom["ShaderPath"].as<string>();
			_bool hasCollider = modelCom["HasCollider"].as<_bool>();
			_bool hasInstancing = _bInstancing;
			deserializedObject->SetInstacing(_bInstancing);
			_bool bEquipment = false;
			if (modelCom["Equipment"])
			{
				bEquipment = modelCom["Equipment"].as<_bool>();
			}

			if (!bSpawn)
				m_pEngine->CloneModel(deserializedObject, meshFilePath, meshFileName, shaderFilePath, EffectFilePath, hasCollider, deserializedObject->GetComponent("Com_Transform"), bEquipment);

			else
				CModelManager::GetInstance()->CloneModelThread(deserializedObject, meshFilePath, meshFileName, shaderFilePath, EffectFilePath, hasCollider, deserializedObject->GetComponent("Com_Transform"), bEquipment);
		}

	}
	else //Client
	{
		auto modelCom = obj["Com_Model"];
		if (modelCom)
		{
			string meshFilePath = modelCom["MeshFilePath"].as<string>();
			string meshFileName = modelCom["MeshFileName"].as<string>();
			string shaderFilePath = "";
			string EffectFilePath = "";

			auto shaderPath = modelCom["ShaderPath"];
			if (shaderPath)
				shaderFilePath = modelCom["ShaderPath"].as<string>();
			_bool hasCollider = modelCom["HasCollider"].as<_bool>();
			_bool bEquipment = false;
			if (modelCom["Equipment"])
			{
				bEquipment = modelCom["Equipment"].as<_bool>();
			}
			if (!bSpawn)
				m_pEngine->CloneModel(deserializedObject, meshFilePath, meshFileName, shaderFilePath, EffectFilePath, hasCollider, deserializedObject->GetComponent("Com_Transform"), bEquipment);
			else
				CModelManager::GetInstance()->CloneModelThread(deserializedObject, meshFilePath, meshFileName, shaderFilePath, EffectFilePath, hasCollider, deserializedObject->GetComponent("Com_Transform"), bEquipment);
		}
	}

	auto lightCom = obj["Com_Light"];
	if (lightCom)
	{
		LIGHTDESC			lightDesc;
		lightDesc.eType = (LIGHTDESC::TYPE)lightCom["Type"].as<_int>();
		lightDesc.fLightRange = lightCom["Range"].as<_float>();
		lightDesc.fLightAngle = lightCom["Angle"].as<_float>();
		auto sequence = lightCom["Diffuse"];
		lightDesc.vDiffuse.x = sequence[0].as<_float>();
		lightDesc.vDiffuse.y = sequence[1].as<_float>();
		lightDesc.vDiffuse.z = sequence[2].as<_float>();
		lightDesc.vDiffuse.w = sequence[3].as<_float>();
		sequence = lightCom["Ambient"];
		lightDesc.vAmbient.x = sequence[0].as<_float>();
		lightDesc.vAmbient.y = sequence[1].as<_float>();
		lightDesc.vAmbient.z = sequence[2].as<_float>();
		lightDesc.vAmbient.w = sequence[3].as<_float>();
		sequence = lightCom["Specular"];
		lightDesc.vSpecular.x = sequence[0].as<_float>();
		lightDesc.vSpecular.y = sequence[1].as<_float>();
		lightDesc.vSpecular.z = sequence[2].as<_float>();
		lightDesc.vSpecular.w = sequence[3].as<_float>();

		CComponent* pLight = CLight::Create(CEngine::GetInstance()->GetDevice(), CEngine::GetInstance()->GetDeviceContext(), lightDesc, dynamic_cast<CTransform*>(deserializedObject->GetComponent("Com_Transform")));
		if (FAILED(deserializedObject->AddComponent("Com_Light", pLight)))
			MSG_BOX("Failed to AddComponent");
	}
	auto npcInfo = obj["Com_NPCInfo"];
	if (npcInfo) {
		_uint npcID = npcInfo["NPCID"].as<_uint>();
		_uint questID = npcInfo["QuestID"].as<_uint>();
		_uint boxID = npcInfo["BoxID"].as<_uint>();
		deserializedObject->AddComponent(0, "Prototype_NPCInfo", "Com_NPCInfo");

		CNPCInfo* npcInfo = static_cast<CNPCInfo*>(deserializedObject->GetComponent("Com_NPCInfo"));
		npcInfo->SetNPCID(npcID);
		npcInfo->SetNPCQuestID(questID);
		npcInfo->SetBoxID(boxID);
	}
	auto StatInfo = obj["Com_Stat"];
	if (StatInfo) {
		CStat::STAT objStat{};

		auto Hp = StatInfo["Hp"];
		objStat.hp = Hp[0].as<_float>();
		objStat.boHp = Hp[1].as<_float>();
		objStat.maxHp = Hp[2].as<_float>();
		objStat.levelHp = Hp[3].as<_float>();

		auto HpRegen = StatInfo["HpRegen"];
		objStat.hpRegeneration = HpRegen[0].as<_float>();
		objStat.levelRegen = HpRegen[1].as<_float>();

		auto Atk = StatInfo["Atk"];
		objStat.atk = Atk[0].as<_float>();
		objStat.boAtk = Atk[1].as<_float>();
		objStat.levelAtk = Atk[2].as<_float>();

		auto Armor = StatInfo["Arm"];
		objStat.armor = Armor[0].as<_float>();
		objStat.boArmor = Armor[1].as<_float>();
		objStat.levelArmor = Armor[2].as<_float>();

		auto Exp = StatInfo["Exp"];
		objStat.exp = Exp[0].as<_float>();
		objStat.maxExp = Exp[1].as<_float>();
		objStat.levelExp = Exp[2].as<_float>();

		auto Level = StatInfo["Level"];
		objStat.level = Level.as<_int>();

		if (StatInfo["Gold"]) {
			objStat.gold = StatInfo["Gold"].as<_int>();
		}
		deserializedObject->AddComponent(0, "Prototype_Stat", "Com_Stat", deserializedObject->GetComponent("Com_Transform"));
		CStat* stats = static_cast<CStat*>(deserializedObject->GetComponent("Com_Stat"));
		stats->SetStatInfo(objStat);
	}

	auto Shop = obj["Com_Shop"];
	if (Shop) {
		ITEMTYPE type;
		_int start, end;
		auto range = Shop["Range"];

		start = range[0].as<_int>();
		end = range[1].as<_int>();

		type = ITEMTYPE(Shop["Type"].as<_int>());

		deserializedObject->AddComponent(0, "Prototype_Shop", "Com_Shop");
		CShop* shopInfo = static_cast<CShop*>(deserializedObject->GetComponent("Com_Shop"));
		shopInfo->SetItemRange(start, end);
		shopInfo->SetItemType(type);
	}

	auto spawn = obj["Com_Spawn"];
	if (spawn) {
		CSpawner::SPAWNER info;

		info.prefabName = spawn["Name"].as<string>();
		info.spawnTime = spawn["Time"].as<_float>();

		deserializedObject->AddComponent(0, "Prototype_Spawn", "Com_Spawn");
		CSpawner* Spawn = static_cast<CSpawner*>(deserializedObject->GetComponent("Com_Spawn"));
		Spawn->SetSpawnInfo(info);
	}

	return deserializedObject;
}

CGameObject* CSceneSerializer::NoneInstancingObject(YAML::Node & obj, _bool bSpawn, _uint curScene)
{
	auto name = obj["Name"].as<string>();
	auto uuid = obj["UUID"].as<uint64_t>();
	auto layer = obj["Layer"].as<string>();
	auto active = obj["Active"].as<_bool>();
	int numOBBs = 0;
	if(obj["NumOBB"])
		numOBBs = obj["NumOBB"].as<_int>();
	int renderGroup = 1;
	_float	FrustumRange = 1.f;
	_bool _bInstancing = false;
	if (obj["Instancing"])
		_bInstancing = obj["Instancing"].as<_bool>();
	if (obj["RenderGroup"])
		renderGroup = obj["RenderGroup"].as<_int>();
	if (obj["FrustumRange"])
		FrustumRange = obj["FrustumRange"].as<_float>();
	
	GameObjectMutex.lock();
	CGameObject* deserializedObject = m_pEngine->AddGameObject(curScene, "Prototype_EmptyGameObject", layer);
	deserializedObject->SetInfo(name, layer, uuid, active, curScene);
	GameObjectMutex.unlock();

	if (obj["PassIndex"])
	{
		auto renderIndex = obj["PassIndex"].as<_int>();
		deserializedObject->SetPassIndex(renderIndex);
	}


	dynamic_cast<CEmptyGameObject*>(deserializedObject)->SetRenderGroup((CRenderer::RENDER)renderGroup);
	dynamic_cast<CEmptyGameObject*>(deserializedObject)->SetFrustum(FrustumRange);
	dynamic_cast<CEmptyGameObject*>(deserializedObject)->SetNumOBB(numOBBs);
	auto transformCom = obj["Com_Transform"];
	if (transformCom)
	{
		//_float3 tr, rt, sc;
		float tr[3], rt[3], sc[3];
		auto sequence = transformCom["Translation"];
		tr[0] = sequence[0].as<float>();
		tr[1] = sequence[1].as<float>();
		tr[2] = sequence[2].as<float>();
		sequence = transformCom["Rotation"];
		rt[0] = sequence[0].as<float>();
		rt[1] = sequence[1].as<float>();
		rt[2] = sequence[2].as<float>();
		sequence = transformCom["Scale"];
		sc[0] = sequence[0].as<float>();
		sc[1] = sequence[1].as<float>();
		sc[2] = sequence[2].as<float>();

		float _objMat[16]; XMFLOAT4X4 objMat;
		ImGuizmo::RecomposeMatrixFromComponents(tr, rt, sc, _objMat);
		memcpy(&objMat, _objMat, sizeof(XMFLOAT4X4));

		CComponent* pTransform = deserializedObject->GetComponent("Com_Transform");
		dynamic_cast<CTransform*>(pTransform)->SetMatrix(objMat);
	}
	auto colliderCom = obj["Com_Collider"];
	if (colliderCom)
	{
		PhysxMutex.lock();
		auto centerInfo = colliderCom["Center"];
		_float3 center = { 0, 0, 0 };
		center.x = centerInfo[0].as<float>();
		center.y = centerInfo[1].as<float>();
		center.z = centerInfo[2].as<float>();

		auto angleInfo = colliderCom["Angle"];
		_float3 angle = { 0, 0, 0 };
		if (angleInfo) {
			angle.x = angleInfo[0].as<float>();
			angle.y = angleInfo[1].as<float>();
			angle.z = angleInfo[2].as<float>();
		}


		CCollider::RIGIDBODYDESC desc;
		ZeroMemory(&desc, sizeof(desc));

		auto rigidBody = colliderCom["RigidBody"];
		desc.bEnabled = rigidBody["Enabled"].as<bool>();
		desc.bGravity = rigidBody["Gravity"].as<bool>();
		desc.bKinematic = rigidBody["Kinematic"].as<bool>();
		desc.bCC = rigidBody["CharacterController"].as<bool>();

		string type = colliderCom["Type"].as<string>();
		if (type == "Box")
		{
			if (deserializedObject->AddComponent(0, "Prototype_BoxCollider", "Com_Collider", deserializedObject->GetComponent("Com_Transform")))
				MSG_BOX("Failed to AddComponent BoxCollider");

			auto sequence = colliderCom["Size"];
			_float3 size;
			size.x = sequence[0].as<float>();
			size.y = sequence[1].as<float>();
			size.z = sequence[2].as<float>();

			CComponent* pCollider = deserializedObject->GetComponent("Com_Collider");
			dynamic_cast<CCollider*>(pCollider)->SetRelativePos(center);
			dynamic_cast<CCollider*>(pCollider)->SetEuler(angle);
			dynamic_cast<CBoxCollider*>(pCollider)->SetUpRigidActor(&size, desc, deserializedObject);
			
		}
		else if (type == "Sphere")
		{
			if (deserializedObject->AddComponent(0, "Prototype_SphereCollider", "Com_Collider", deserializedObject->GetComponent("Com_Transform")))
				MSG_BOX("Failed to AddComponent Prototype_SphereCollider");

			_float radius = colliderCom["Radius"].as<float>();
			CComponent* pCollider = deserializedObject->GetComponent("Com_Collider");
			dynamic_cast<CCollider*>(pCollider)->SetRelativePos(center);
			dynamic_cast<CSphereCollider*>(pCollider)->SetUpRigidActor(&radius, desc, deserializedObject);
		}
		else if (type == "Capsule")
		{
			if (deserializedObject->AddComponent(0, "Prototype_CapsuleCollider", "Com_Collider", deserializedObject->GetComponent("Com_Transform")))
				MSG_BOX("Failed to AddComponent Prototype_CapsuleCollider");

			pair<float, float> capsuleSize;
			capsuleSize.first = colliderCom["Radius"].as<float>();
			capsuleSize.second = colliderCom["Height"].as<float>();
			CComponent* pCollider = deserializedObject->GetComponent("Com_Collider");
			dynamic_cast<CCollider*>(pCollider)->SetRelativePos(center);
			dynamic_cast<CCapsuleCollider*>(pCollider)->SetUpRigidActor(&capsuleSize, desc, deserializedObject);
		}
		PhysxMutex.unlock();
	}
	_int i = 0;
	do {
		string obb = "Com_OBB";
		if (i != 0)
			obb += to_string(i);
		auto OBB = obj[obb.c_str()];
		if (OBB)
		{
			if (deserializedObject->AddComponent(0, "Prototype_OBBCollider", obb))
				MSG_BOX("Failed to Com_OBB");

			CComponent* pCollider = deserializedObject->GetComponent(obb);

			auto center_n = OBB["Center"];
			_float3 Pos;
			Pos.x = center_n[0].as<float>();
			Pos.y = center_n[1].as<float>();
			Pos.z = center_n[2].as<float>();

			dynamic_cast<CBasicCollider*>(pCollider)->SetPos(Pos);

			auto size_n = OBB["Size"];
			_float3 size;
			size.x = size_n[0].as<float>();
			size.y = size_n[1].as<float>();
			size.z = size_n[2].as<float>();

			dynamic_cast<CBasicCollider*>(pCollider)->SetSize(size);

			auto attach = OBB["IsAttach"];
			if (attach) {
				_bool atc = attach.as<_bool>();
				dynamic_cast<CBasicCollider*>(pCollider)->SetIsAttachBone(atc);
			}

			auto boneName = OBB["BoneName"];
			if (boneName) {
				string name = boneName.as<string>();
				dynamic_cast<CBasicCollider*>(pCollider)->SetBoneName(name);
			}

			auto offset_n = OBB["Offset"];
			if (offset_n) {
				_float3 offset;
				offset.x = offset_n[0].as<_float>();
				offset.y = offset_n[1].as<_float>();
				offset.z = offset_n[2].as<_float>();
				dynamic_cast<CBasicCollider*>(pCollider)->SetOffset(offset);
			}

			auto CollType = OBB["CollisionType"];
			if (CollType) {
				_int coltype = CollType.as<_int>();
				dynamic_cast<CBasicCollider*>(pCollider)->SetCollisionType((ID)coltype);
			}
		}
		i++;
	} while (i < numOBBs);


	auto AABB = obj["Com_AABB"];
	if (AABB)
	{
		if (deserializedObject->AddComponent(0, "Prototype_AABBCollider", "Com_AABB"))
			MSG_BOX("Failed to Com_AABB");

		CComponent* pCollider = deserializedObject->GetComponent("Com_AABB");

		auto center_n = AABB["Center"];
		_float3 Pos;
		Pos.x = center_n[0].as<float>();
		Pos.y = center_n[1].as<float>();
		Pos.z = center_n[2].as<float>();

		dynamic_cast<CBasicCollider*>(pCollider)->SetPos(Pos);

		auto size_n = AABB["Size"];
		_float3 size;
		size.x = size_n[0].as<float>();
		size.y = size_n[1].as<float>();
		size.z = size_n[2].as<float>();

		dynamic_cast<CBasicCollider*>(pCollider)->SetSize(size);
	}



	auto viBufferCom = obj["Com_VIBuffer"];
	if (viBufferCom)
	{
		string type = viBufferCom["Type"].as<string>();
		if (type == "Terrain")
		{
			if (deserializedObject->AddComponent(0, "Prototype_VIBuffer_Terrain", "Com_VIBuffer", deserializedObject->GetComponent("Com_Transform")))
				MSG_BOX("Failed to AddComponent Prototype_VIBuffer_Terrain");

			string heightMapPath = viBufferCom["HeightMapPath"].as<string>();
			string texturePath = viBufferCom["TexturePath"].as<string>();

			CVIBuffer_Terrain* pTerrainBuffer = dynamic_cast<CVIBuffer_Terrain*>(deserializedObject->GetComponent("Com_VIBuffer"));
			pTerrainBuffer->SetHeightMapPath(heightMapPath);
			pTerrainBuffer->SetTexturePath(texturePath);
		}
		else {
			if (deserializedObject->AddComponent(0, "Prototype_VIBuffer_Rect", "Com_VIBuffer", deserializedObject->GetComponent("Com_Transform")))
				MSG_BOX("Failed to AddComponent Prototype_VIBuffer_Terrain");

			string texturePath = viBufferCom["TexturePath"].as<string>();


			CVIBuffer_Rect* pRectBuffer = dynamic_cast<CVIBuffer_Rect*>(deserializedObject->GetComponent("Com_VIBuffer"));
			pRectBuffer->UpdateTexture(texturePath);
		}
	}

	if (CEngine::GetInstance()->GetCurrentUsage() == CEngine::USAGE::USAGE_TOOL)
	{ //Original
		auto modelCom = obj["Com_Model"];
		if (modelCom)
		{
			string meshFilePath = modelCom["MeshFilePath"].as<string>();
			string meshFileName = modelCom["MeshFileName"].as<string>();
			string shaderFilePath = "";
			string EffectFilePath = "";

			auto shaderPath = modelCom["ShaderPath"];
			if (shaderPath)
				shaderFilePath = modelCom["ShaderPath"].as<string>();
			_bool hasCollider = modelCom["HasCollider"].as<_bool>();
			_bool hasInstancing = _bInstancing;
			deserializedObject->SetInstacing(_bInstancing);
			_bool bEquipment = false;
			if (modelCom["Equipment"])
			{
				bEquipment = modelCom["Equipment"].as<_bool>();
			}

			if (!bSpawn)
				m_pEngine->CloneModel(deserializedObject, meshFilePath, meshFileName, shaderFilePath, EffectFilePath, hasCollider, deserializedObject->GetComponent("Com_Transform"), bEquipment);

			else
				CModelManager::GetInstance()->CloneModelThread(deserializedObject, meshFilePath, meshFileName, shaderFilePath, EffectFilePath, hasCollider, deserializedObject->GetComponent("Com_Transform"), bEquipment);
		}

	}
	else //Client
	{
		auto modelCom = obj["Com_Model"];
		if (modelCom)
		{
			string meshFilePath = modelCom["MeshFilePath"].as<string>();
			string meshFileName = modelCom["MeshFileName"].as<string>();
			string shaderFilePath = "";
			string EffectFilePath = "";

			auto shaderPath = modelCom["ShaderPath"];
			if (shaderPath)
				shaderFilePath = modelCom["ShaderPath"].as<string>();
			_bool hasCollider = modelCom["HasCollider"].as<_bool>();
			_bool bEquipment = false;
			if (modelCom["Equipment"])
			{
				bEquipment = modelCom["Equipment"].as<_bool>();
			}
			if (!bSpawn)
				m_pEngine->CloneModel(deserializedObject, meshFilePath, meshFileName, shaderFilePath, EffectFilePath, hasCollider, deserializedObject->GetComponent("Com_Transform"), bEquipment);
			else
				CModelManager::GetInstance()->CloneModelThread(deserializedObject, meshFilePath, meshFileName, shaderFilePath, EffectFilePath, hasCollider, deserializedObject->GetComponent("Com_Transform"), bEquipment);
		}
	}

	auto lightCom = obj["Com_Light"];
	if (lightCom)
	{
		LIGHTDESC			lightDesc;
		lightDesc.eType = (LIGHTDESC::TYPE)lightCom["Type"].as<_int>();
		lightDesc.fLightRange = lightCom["Range"].as<_float>();
		lightDesc.fLightAngle = lightCom["Angle"].as<_float>();
		auto sequence = lightCom["Diffuse"];
		lightDesc.vDiffuse.x = sequence[0].as<_float>();
		lightDesc.vDiffuse.y = sequence[1].as<_float>();
		lightDesc.vDiffuse.z = sequence[2].as<_float>();
		lightDesc.vDiffuse.w = sequence[3].as<_float>();
		sequence = lightCom["Ambient"];
		lightDesc.vAmbient.x = sequence[0].as<_float>();
		lightDesc.vAmbient.y = sequence[1].as<_float>();
		lightDesc.vAmbient.z = sequence[2].as<_float>();
		lightDesc.vAmbient.w = sequence[3].as<_float>();
		sequence = lightCom["Specular"];
		lightDesc.vSpecular.x = sequence[0].as<_float>();
		lightDesc.vSpecular.y = sequence[1].as<_float>();
		lightDesc.vSpecular.z = sequence[2].as<_float>();
		lightDesc.vSpecular.w = sequence[3].as<_float>();

		CComponent* pLight = CLight::Create(CEngine::GetInstance()->GetDevice(), CEngine::GetInstance()->GetDeviceContext(), lightDesc, dynamic_cast<CTransform*>(deserializedObject->GetComponent("Com_Transform")));
		if (FAILED(deserializedObject->AddComponent("Com_Light", pLight)))
			MSG_BOX("Failed to AddComponent");
	}
	auto npcInfo = obj["Com_NPCInfo"];
	if (npcInfo) {
		_uint npcID = npcInfo["NPCID"].as<_uint>();
		_uint questID = npcInfo["QuestID"].as<_uint>();
		_uint boxID = npcInfo["BoxID"].as<_uint>();
		deserializedObject->AddComponent(0, "Prototype_NPCInfo", "Com_NPCInfo");

		CNPCInfo* npcInfo = static_cast<CNPCInfo*>(deserializedObject->GetComponent("Com_NPCInfo"));
		npcInfo->SetNPCID(npcID);
		npcInfo->SetNPCQuestID(questID);
		npcInfo->SetBoxID(boxID);
	}
	auto StatInfo = obj["Com_Stat"];
	if (StatInfo) {
		CStat::STAT objStat{};

		auto Hp = StatInfo["Hp"];
		objStat.hp = Hp[0].as<_float>();
		objStat.boHp = Hp[1].as<_float>();
		objStat.maxHp = Hp[2].as<_float>();
		objStat.levelHp = Hp[3].as<_float>();

		auto HpRegen = StatInfo["HpRegen"];
		objStat.hpRegeneration = HpRegen[0].as<_float>();
		objStat.levelRegen = HpRegen[1].as<_float>();

		auto Atk = StatInfo["Atk"];
		objStat.atk = Atk[0].as<_float>();
		objStat.boAtk = Atk[1].as<_float>();
		objStat.levelAtk = Atk[2].as<_float>();

		auto Armor = StatInfo["Arm"];
		objStat.armor = Armor[0].as<_float>();
		objStat.boArmor = Armor[1].as<_float>();
		objStat.levelArmor = Armor[2].as<_float>();

		auto Exp = StatInfo["Exp"];
		objStat.exp = Exp[0].as<_float>();
		objStat.maxExp = Exp[1].as<_float>();
		objStat.levelExp = Exp[2].as<_float>();

		auto Level = StatInfo["Level"];
		objStat.level = Level.as<_int>();

		if (StatInfo["Gold"]) {
			objStat.gold = StatInfo["Gold"].as<_int>();
		}
		deserializedObject->AddComponent(0, "Prototype_Stat", "Com_Stat", deserializedObject->GetComponent("Com_Transform"));
		CStat* stats = static_cast<CStat*>(deserializedObject->GetComponent("Com_Stat"));
		stats->SetStatInfo(objStat);
	}

	auto Shop = obj["Com_Shop"];
	if (Shop) {
		ITEMTYPE type;
		_int start, end;
		auto range = Shop["Range"];

		start = range[0].as<_int>();
		end = range[1].as<_int>();

		type = ITEMTYPE(Shop["Type"].as<_int>());

		deserializedObject->AddComponent(0, "Prototype_Shop", "Com_Shop");
		CShop* shopInfo = static_cast<CShop*>(deserializedObject->GetComponent("Com_Shop"));
		shopInfo->SetItemRange(start, end);
		shopInfo->SetItemType(type);
	}

	auto spawn = obj["Com_Spawn"];
	if (spawn) {
		CSpawner::SPAWNER info;

		info.prefabName = spawn["Name"].as<string>();
		info.spawnTime = spawn["Time"].as<_float>();

		deserializedObject->AddComponent(0, "Prototype_Spawn", "Com_Spawn");
		CSpawner* Spawn = static_cast<CSpawner*>(deserializedObject->GetComponent("Com_Spawn"));
		Spawn->SetSpawnInfo(info);
	}

	return deserializedObject;
}


void CSceneSerializer::SerializePrefab(CGameObject * obj)
{
	YAML::Emitter out;

	out << YAML::BeginMap;
	out << YAML::Key << "Scene" << YAML::Value << "Untitled";
	out << YAML::Key << "GameObjects" << YAML::Value << YAML::BeginSeq;

	out << YAML::BeginMap;

	out << YAML::Key << "Name" << YAML::Value << obj->GetName();
	out << YAML::Key << "UUID" << YAML::Value << obj->GetUUID();
	out << YAML::Key << "Active" << YAML::Value << obj->IsActive();
	out << YAML::Key << "Layer" << YAML::Value << obj->GetLayer();

	if (dynamic_cast<CEmptyGameObject*>(obj))
		SerializeObject(out, obj);
	else if (dynamic_cast<CEmptyEffect*>(obj))
		SerializeEffect(out, obj);
	else
		SerializeUI(out, obj);

	if (0 < obj->GetChildren().size())
	{
		out << YAML::Key << "Children";
		out << YAML::BeginSeq;



		list<CGameObject*> children = obj->GetChildren();
		for (auto& child : children)
		{
			out << YAML::BeginMap;

			out << YAML::Key << "Name" << YAML::Value << child->GetName();
			out << YAML::Key << "UUID" << YAML::Value << child->GetUUID();
			out << YAML::Key << "Active" << YAML::Value << child->IsActive();
			out << YAML::Key << "Layer" << YAML::Value << child->GetLayer();

			if (dynamic_cast<CEmptyGameObject*>(child))
				SerializeObject(out, child);
			else if (dynamic_cast<CEmptyEffect*>(child))
				SerializeEffect(out, child);
			else
				SerializeUI(out, child);

			out << YAML::EndMap;
		}


		out << YAML::EndSeq;
	}

	out << YAML::EndMap;

	out << YAML::EndSeq;
	out << YAML::EndMap;

	string filePath = prefabPath + obj->GetName() + ".prefab";

	std::ofstream fout(filePath);
	fout << out.c_str();
}

void CSceneSerializer::DeserializePrefab()
{
	// get all file name in directory

	for (auto& iter : FILESYSTEM::directory_iterator(prefabPath))
	{
		FILESYSTEM::path filePath = iter.path();
		if (filePath.extension().string() != ".prefab")
			continue;

		string strFilePath = iter.path().string();
		string fileName = iter.path().stem().string();

		std::ifstream stream(strFilePath);
		std::stringstream strStream;
		strStream << stream.rdbuf();

		YAML::Node data = YAML::Load(strStream.str());

		m_pEngine->AddPrefab(fileName, data);
	}
	return;
}

HRESULT CSceneSerializer::CreatePrefab(string pPrototypeTag, YAML::Node data, _uint curScene)
{
	if (!data["Scene"])
		return E_FAIL;
	CGameObject* deserializedPrototype = nullptr;

	string SceneName = data["Scene"].as<string>();
	_uint iNumChild = 0;

	auto gameObjects = data["GameObjects"];
	if (gameObjects)
	{
		for (auto obj : gameObjects)
		{

			if (obj["Type"].as<string>() == "UI")
				return E_FAIL;
			else if (obj["Type"].as<string>() == "Effect")
				deserializedPrototype = DeserializePrototypeEffect(pPrototypeTag, obj, false, 0);
			else
				deserializedPrototype = DeserializePrototypeGameObject(pPrototypeTag, obj, false, 0);
			if(deserializedPrototype)
				cout << "prefab prototype created  : " << deserializedPrototype->GetName() << "\n";
			auto children = obj["Children"];
			if (children)
			{
				CGameObject* pChildObj = nullptr;

				for (auto& child : children)
				{
					if (child["Type"].as<string>() == "UI")
					{
						pChildObj = DeserializeUI(child, true, curScene);
						dynamic_cast<CEmptyUI*>(pChildObj)->SetParent(deserializedPrototype);
					}
					else if (child["Type"].as<string>() == "Effect")
					{						
						deserializedPrototype->AddChildPrototypeTag(pPrototypeTag + to_string(iNumChild));
						pChildObj = DeserializePrototypeEffect(pPrototypeTag + to_string(iNumChild), child, false, curScene);
					}
					else
					{
						deserializedPrototype->AddChildPrototypeTag(pPrototypeTag + to_string(iNumChild));
						pChildObj = DeserializePrototypeGameObject(pPrototypeTag + to_string(iNumChild), child, false, curScene);
					}

					iNumChild++;
				}
			}
		}
	}
	return S_OK;
}


CGameObject * CSceneSerializer::SpawnPrefab(YAML::Node data, _uint curScene)
{
	if (!data["Scene"])
		return false;
	CGameObject* deserializedObject = nullptr;

	string SceneName = data["Scene"].as<string>();

	auto gameObjects = data["GameObjects"];
	if (gameObjects)
	{
		for (auto obj : gameObjects)
		{

			if (obj["Type"].as<string>() == "UI")
				deserializedObject = DeserializeUI(obj, true, curScene);
			else if (obj["Type"].as<string>() == "Effect")
			{
				deserializedObject = DeserializeEffect(obj, true, curScene);
			}
			else
				deserializedObject = DeserializeObject(obj, true, curScene);

			auto children = obj["Children"];
			if (children)
			{
				CGameObject* pChildObj = nullptr;

				for (auto& child : children)
				{
					if (child["Type"].as<string>() == "UI")
					{
						pChildObj = DeserializeUI(child, true, curScene);
						dynamic_cast<CEmptyUI*>(pChildObj)->SetParent(deserializedObject);
					}
					else if (child["Type"].as<string>() == "Effect")
					{
						pChildObj = DeserializeEffect(child, true, curScene);
						dynamic_cast<CEmptyEffect*>(pChildObj)->SetParent(deserializedObject);
					}
					else
					{
						pChildObj = DeserializeObject(child, true, curScene);
						dynamic_cast<CEmptyGameObject*>(pChildObj)->SetParent(deserializedObject);
					}

					pChildObj->SetParent(deserializedObject);
					deserializedObject->AddChild(pChildObj);
					if (dynamic_cast<CEmptyGameObject*>(pChildObj) || dynamic_cast<CEmptyEffect*>(pChildObj))
					{
						_matrix localMatrix =
							dynamic_cast<CTransform*>(pChildObj->GetComponent("Com_Transform"))->GetWorldMatrix();
						dynamic_cast<CTransform*>(pChildObj->GetComponent("Com_LocalTransform"))->SetMatrix(localMatrix);
					}
				}
			}
		}
	}
	return deserializedObject;
}


void CSceneSerializer::DeserializeInstancModel(_uint curScene)
{
	if (InstanceAllList.size() <= 0)
		return;


	string firstKey;
	_int instancecount = 0;
	list <string*> keylist = {};
	map<string, int> TempObj = {};
	vector<_float4x4> Transfroms;
	for (auto i = InstanceAllList.begin(); i != InstanceAllList.end(); i++)
	{
		firstKey = i->first;
		TempObj.insert({ firstKey, 0 });
	}

	for (auto iter : TempObj)
	{
		INSMODELDESC			InsDesc;
		ZeroMemory(&InsDesc, sizeof(INSMODELDESC));

		for (auto it = InstanceAllList.lower_bound(iter.first); it != InstanceAllList.upper_bound(iter.first); it++)
		{
			_int Insct = (_int)InstanceAllList.count(iter.first);

			char	szTextureFilePath[MAX_PATH] = "";

			char	szTextureFileName[MAX_PATH] = "";
			char	szExt[MAX_PATH] = "";

			_splitpath(iter.first.c_str(), nullptr, szTextureFilePath, szTextureFileName, szExt);
			strcat_s(szTextureFileName, szExt); // 이름 + 확장자 

			char		szFullPath[MAX_PATH] = "";
			strcpy_s(InsDesc.pMeshFilePath, szTextureFilePath);
			strcpy_s(InsDesc.pMeshFileName, MAX_PATH, szTextureFileName);

			InsDesc._InstancingCount = Insct;
			InsDesc.pTransfroms.emplace_back(it->second);
		}
		CGameObject* InsObject = m_pEngine->AddGameObject(curScene, "Prototype_EmptyMapObject", "MapObject");
		InsObject->AddComponent(0, "Prototype_InstanceModel", "Com_InsModel", &InsDesc);
	}

	//쓰레드 자식 죽으면 부모한테 전달 > 부모 죽이고 해야한다. 
}

void CSceneSerializer::InstaingDesc(CGameObject* obj, YAML::Emitter & out)
{
	if (obj->GetComponent("Com_Transform"))
	{
		CTransform* transform = nullptr;
		transform = dynamic_cast<CTransform*>(obj->GetComponent("Com_Transform"));

		XMMATRIX matrix = XMLoadFloat4x4(&transform->GetMatrix());
		//XMVECTOR tr, rt, sc;
		//XMMatrixDecompose(&sc, &rt, &tr, matrix);

		float _objMat[16];
		memcpy(_objMat, &matrix, sizeof(XMMATRIX));
		float matrixTranslation[3], matrixRotation[3], matrixScale[3];
		ImGuizmo::DecomposeMatrixToComponents(_objMat, matrixTranslation, matrixRotation, matrixScale);


		out << YAML::Key << "Translation";
		out << YAML::Value << YAML::Flow;
		out << YAML::BeginSeq << matrixTranslation[0] << matrixTranslation[1] << matrixTranslation[2] << YAML::EndSeq;

		out << YAML::Key << "Rotation";
		out << YAML::Value << YAML::Flow;
		out << YAML::BeginSeq << matrixRotation[0] << matrixRotation[1] << matrixRotation[2] << YAML::EndSeq;

		out << YAML::Key << "Scale";
		out << YAML::Value << YAML::Flow;
		out << YAML::BeginSeq << matrixScale[0] << matrixScale[1] << matrixScale[2] << YAML::EndSeq;
	}

	if (obj->GetComponent("Com_Model"))
	{
		CModel* pModel = dynamic_cast<CModel*>(obj->GetComponent("Com_Model"));

		out << YAML::Key << "MeshFilePath" << YAML::Value << pModel->GetMeshFilePath();
		out << YAML::Key << "MeshFileName" << YAML::Value << pModel->GetMeshFileName();
		out << YAML::Key << "HasCollider" << YAML::Value << pModel->HasMeshCollider();
	}
}

void CSceneSerializer::InsModelInsertMap(YAML::Node & obj)
{
	_float4x4 objMat;
	string meshFilePath;

	if (obj["MeshFilePath"])
	{
		meshFilePath = obj["MeshFilePath"].as<string>();
		string meshFileName = obj["MeshFileName"].as<string>();

		meshFilePath += meshFileName;
	}

	if (obj["Translation"])
	{
		//_float3 tr, rt, sc;
		float tr[3], rt[3], sc[3];
		auto sequence = obj["Translation"];
		tr[0] = sequence[0].as<float>();
		tr[1] = sequence[1].as<float>();
		tr[2] = sequence[2].as<float>();
		sequence = obj["Rotation"];
		rt[0] = sequence[0].as<float>();
		rt[1] = sequence[1].as<float>();
		rt[2] = sequence[2].as<float>();
		sequence = obj["Scale"];
		sc[0] = sequence[0].as<float>();
		sc[1] = sequence[1].as<float>();
		sc[2] = sequence[2].as<float>();

		float _objMat[16];/* XMFLOAT4X4 objMat;*/
		ImGuizmo::RecomposeMatrixFromComponents(tr, rt, sc, _objMat);
		memcpy(&objMat, _objMat, sizeof(XMFLOAT4X4));

	}

	InstanceAllList.emplace(meshFilePath, objMat);
}


