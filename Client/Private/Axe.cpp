#include "stdafx.h"
#include "..\Public\Axe.h"
#include "BasicCollider.h"
#include "Player.h"
#include "Wolf.h"
#include "Model.h"
#include "EventCheck.h"
#include "Camera_Fly.h"
#include "ShoulerAtk.h"
#include "ImpactGround.h"
#include "ImpactBeam.h"
#include "ImpactShort.h"
USING(Client)

CAxe::CAxe()
{
}

CAxe::CAxe(CGameObject * pObj)
{
}

HRESULT CAxe::Initialize()
{
	pAxe = CEngine::GetInstance()->FindGameObjectWithName(SCENE_STATIC, "Axe");
	pPlayer = CEngine::GetInstance()->FindGameObjectWithName(SCENE_STATIC, "Player");
	m_pTransform = static_cast<CTransform*>(pAxe->GetComponent("Com_Transform"));
	m_pTargetTransform = static_cast<CTransform*>(pPlayer->GetComponent("Com_RenderTransform"));
	m_pTargetModel = static_cast<CModel*>(pPlayer->GetComponent("Com_Model"));
	m_pCollider = static_cast<CCollider*>(pAxe->GetComponent("Com_Collider"));
	m_pModel = static_cast<CModel*>(pAxe->GetComponent("Com_Model"));
	m_pOBB = static_cast<CBasicCollider*>(pAxe->GetComponent("Com_OBB"));


	XMStoreFloat4x4(&m_matRightBone, XMMatrixIdentity());
	
	Create_Trail();
	
	return S_OK;
}

void CAxe::Update(_double deltaTime)
{
	Set_Attack();
}

void CAxe::LateUpdate(_double deltaTime)
{
	State_Att();

	if (m_pTrailBuffer)
		m_pTrailBuffer->Update(deltaTime, XMLoadFloat4x4(&m_matRightBone) * XMLoadFloat4x4(&m_pTargetTransform->GetMatrix()));
	Set_TrailOnOff();
}

void CAxe::Render()
{
}

CAxe * CAxe::Create(CGameObject * pObj)
{
	CAxe*		pInstance = new CAxe(pObj);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed to Create CAxe");
		SafeRelease(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CAxe::Free()
{
}


void CAxe::Set_Attack()
{
	/*CGameObject* pPlayer = CEngine::GetInstance()->FindGameObjectWithName(SCENE_STATIC, "Player");
	CComponent* Player = static_cast<CPlayer*>*/
}

void CAxe::State_Att()
{
	_matrix Scale, Rotate, Translation;
	Scale = XMMatrixScaling(1.5f, 1.5f, 1.5f);
	Translation = XMMatrixTranslation(0.f, 0.05f,0.f);
	Rotate = XMMatrixRotationRollPitchYaw(0.05f, 90.f, -0.05f);
	_matrix TargetRootBone = XMMatrixIdentity();
	_matrix OffsetMatrix = XMMatrixIdentity();
	OffsetMatrix = Scale * Rotate * Translation;
	//BN_Weapon_R
	XMStoreFloat4x4(&m_matRightBone, m_pModel->Get_TransformationMatrix("AN_PC_WP_TAX1013_A00_BC") * m_pTargetModel->Get_BoneWithoutOffset("BN_Weapon_R"));
	m_pTransform->SetMatrix(OffsetMatrix * XMLoadFloat4x4(&m_matRightBone) * XMLoadFloat4x4(&m_pTargetTransform->GetMatrix()));
}

void CAxe::State_Idle()
{
}

void CAxe::Create_Trail()
{
	m_pTrail = CEngine::GetInstance()->AddGameObject(SCENE_STATIC, "Prototype_EmptyEffect", "Player_Trail");
	if (m_pTrail == nullptr)
		return;

	CEmptyEffect* pEffect = static_cast<CEmptyEffect*>(m_pTrail);
	
	pEffect->SetPassIndex(3);
	pEffect->SetTexture("../../Assets/Textures/Effect/Diffuse/LV_ElRano_Object_SpermaPropB_E_LBR.dds", CEmptyEffect::TEXTURE_DIFFUSE);
	pEffect->SetTexture("../../Assets/Textures/Effect/Mask/Trun_FX_Trail02_Tex_HKJ.jpg", CEmptyEffect::TEXTURE_MASK);
	pEffect->SetTexture("../../Assets/Textures/Effect/Noise/Trail.dds", CEmptyEffect::TEXTURE_NOISE);

	pEffect->SetScrollSpeedX(_float3(0.5f, 0.5f, 0.f));
	pEffect->SetScrollSpeedY(_float3(0.f, 0.f, 0.f));
	pEffect->setDistortion(0, _float2(0.1f, 0.2f));
	pEffect->setDistortion(1, _float2(0.1f, 0.3f));
	pEffect->setDistortion(2, _float2(0.1f, 0.1f));
	pEffect->SetDistortionScale(4.f);
	pEffect->SetDistortionBias(1.f);

	_matrix WeaponTrans = XMLoadFloat4x4(&m_matRightBone) * XMLoadFloat4x4(&m_pTargetTransform->GetMatrix());
	m_pTrail->AddComponent(0, "Prototype_VIBuffer_Trail", "Com_Trail", &WeaponTrans);
	m_pTrailBuffer = static_cast<CVIBuffer_Trail*>(m_pTrail->GetComponent("Com_Trail"));
}

void CAxe::Set_TrailOnOff()
{
	CModel*	playerModel = static_cast<CModel*>(pPlayer->GetComponent("Com_Model"));
	CStat*	playerStat = static_cast<CStat*>(pPlayer->GetComponent("Com_Stat"));
	CTransform*	playerTrans = static_cast<CTransform*>(pPlayer->GetComponent("Com_Transform"));
	Player_State playerState = (Player_State)playerModel->Get_AnimIndex();
	m_pTrailBuffer->SetIsActive(false);
	playerStat->SetSTATE(CStat::STATES_IDEL);
	_vector thisPos = m_pTransform->GetState(CTransform::STATE_POSITION);
	switch (playerState)
	{
	case Client::Player_State::WakeUp_Bad:
		break;
	case Client::Player_State::Normal_Idle1:
		break;
	case Client::Player_State::Normal_Idle2:
		break;
	case Client::Player_State::Evade:
		break;
	case Client::Player_State::Walk:
		break;
	case Client::Player_State::Run:
		break;
	case Client::Player_State::Sprint_ing:
		break;
	case Client::Player_State::Sprint_end:
		break;
	case Client::Player_State::Run_Stop:
		break;
	case Client::Player_State::Jump:
		break;
	case Client::Player_State::Fall:
		break;
	case Client::Player_State::Fall_ing:
		break;
	case Client::Player_State::Land_Stop:
		break;
	case Client::Player_State::Land_Run:
		break;
	case Client::Player_State::Sprint_Fall:
		break;
	case Client::Player_State::Sprint_Fall_ing:
		break;
	case Client::Player_State::Sprint_Land_Run:
		break;
	case Client::Player_State::ML_Start:
		break;
	case Client::Player_State::ML_ing:
		break;
	case Client::Player_State::ML_End:
		break;
	case Client::Player_State::LL_Start:
		break;
	case Client::Player_State::LL_ing:
		break;
	case Client::Player_State::LL_End:
		break;
	case Client::Player_State::Interact_Relam1:
		break;
	case Client::Player_State::Interact_Relam2:
		break;
	case Client::Player_State::Interact_Relam3:
		break;
	case Client::Player_State::Hit_F:
		break;
	case Client::Player_State::Hit_B:
		break;
	case Client::Player_State::Stand_Die:
		break;
	case Client::Player_State::LBCombo1: {
		_int keyFrame = playerModel->GetCurrentKeyFrame();
		if (keyFrame >= 2 && keyFrame <= 23) {
			m_pTrailBuffer->SetIsActive(true);
			playerStat->SetSTATE(CStat::STATES_ATK);
			playerStat->SetDMGRatio(0.5f);
		}
		if(keyFrame == 2)
			CEventCheck::GetInstance()->ShakeCamera(CCamera_Fly::SHAKE::SHAKE_LEFT, 1.f, 0.03f, 3.f);
		break;
	}
	case Client::Player_State::LBCombo2: {
		_int keyFrame = playerModel->GetCurrentKeyFrame();
		if (keyFrame >= 3 && keyFrame <= 29) {
			m_pTrailBuffer->SetIsActive(true);
			playerStat->SetSTATE(CStat::STATES_ATK);
			playerStat->SetDMGRatio(0.65f);
			//CEventCheck::GetInstance()->ShakeCamera(CCamera_Fly::SHAKE::SHAKE_LEFT, 1.f, 0.1f);
		}
		if(keyFrame == 3)
			CEventCheck::GetInstance()->ShakeCamera(CCamera_Fly::SHAKE::SHAKE_LEFT, 1.f, 0.03f, 3.f);

		break;
	}
	case Client::Player_State::LBCombo3: {
		_int keyFrame = playerModel->GetCurrentKeyFrame();
		if (keyFrame >= 2 && keyFrame <= 11) {
			m_pTrailBuffer->SetIsActive(true);
			playerStat->SetSTATE(CStat::STATES_ATK);
			playerStat->SetDMGRatio(0.35f);
		}
		if(keyFrame == 5)
			CEventCheck::GetInstance()->ShakeCamera(CCamera_Fly::SHAKE::SHAKE_ING, 6.f, 0.02f);
		break;
	}
	case Client::Player_State::LBCombo4_0: {
		break;
	}
	case Client::Player_State::LBCombo4_1: {
		_int keyFrame = playerModel->GetCurrentKeyFrame();
		if (keyFrame >= 3 && keyFrame <= 23) {
			m_pTrailBuffer->SetIsActive(true);
			playerStat->SetSTATE(CStat::STATES_ATK);
			playerStat->SetDMGRatio(1.f);
		}
		if (keyFrame == 14) {
			CEventCheck::GetInstance()->ShakeCamera(CCamera_Fly::SHAKE::SHAKE_ING, 6.f, 0.02f);
			CGameObject* pGameObject = CEngine::GetInstance()->AddGameObjectToPrefab(CEngine::GetInstance()->GetCurSceneNumber(), "Prototype_Effect_ImpactGround", "E_ImpactGround2");
			CEngine::GetInstance()->AddScriptObject(CImpactGround::Create((CEmptyEffect*)pGameObject, pPlayer), CEngine::GetInstance()->GetCurSceneNumber());
		}

		break;
	}
	case Client::Player_State::RBCombo1: {
		_int keyFrame = playerModel->GetCurrentKeyFrame();
		if (keyFrame >= 13 && keyFrame <= 22) {
			m_pTrailBuffer->SetIsActive(true);
			playerStat->SetSTATE(CStat::STATES_ATK);
			playerStat->SetDMGRatio(0.6f);
		}
		if (keyFrame == 13)
			CEventCheck::GetInstance()->ShakeCamera(CCamera_Fly::SHAKE::SHAKE_LEFT, 1.f, 0.02f, 3.f);

		break;
	}
	case Client::Player_State::RBCombo2: {
		_int keyFrame = playerModel->GetCurrentKeyFrame();
		playerStat->SetDMGRatio(0.5f);
		if (keyFrame >= 8) {
			CEventCheck::GetInstance()->ShakeCamera(CCamera_Fly::SHAKE::SHAKE_ING, 4.f, 0.03f);
		}
		if (keyFrame == 0) {
			CGameObject* pGameObject = CEngine::GetInstance()->AddGameObjectToPrefab(CEngine::GetInstance()->GetCurSceneNumber(), "Prototype_Effect_shoulderEffect", "E_shoulderEffect");
			CEngine::GetInstance()->AddScriptObject(CShoulerAtk::Create((CEmptyEffect*)pGameObject, pPlayer), CEngine::GetInstance()->GetCurSceneNumber());
		}
		break;
	}
	case Client::Player_State::RBCombo3: {
		_int keyFrame = playerModel->GetCurrentKeyFrame();
		if (keyFrame >= 0 && keyFrame <= 13) {
			m_pTrailBuffer->SetIsActive(true);
			playerStat->SetSTATE(CStat::STATES_ATK);
			playerStat->SetDMGRatio(0.7f);
		}
		if (keyFrame == 13)
			CEventCheck::GetInstance()->ShakeCamera(CCamera_Fly::SHAKE::SHAKE_ING, 5.f, 0.02f);
		break;
	}
	case Client::Player_State::RBCombo4: {
		_int keyFrame = playerModel->GetCurrentKeyFrame();
		if (keyFrame >= 12 && keyFrame <= 22) {
			m_pTrailBuffer->SetIsActive(true);
			playerStat->SetSTATE(CStat::STATES_ATK);
			playerStat->SetDMGRatio(1.1f);
		}
		if (keyFrame == 19) {
			CEventCheck::GetInstance()->ShakeCamera(CCamera_Fly::SHAKE::SHAKE_ING, 6.f, 0.02f);
		}
		if (keyFrame == 18) {
			CGameObject* pGameObject = CEngine::GetInstance()->AddGameObjectToPrefab(CEngine::GetInstance()->GetCurSceneNumber(), "Prototype_Effect_ImpactBeam", "E_ImpactBeam00");
			CEngine::GetInstance()->AddScriptObject(CImpactBeam::Create((CEmptyEffect*)pGameObject, pPlayer), CEngine::GetInstance()->GetCurSceneNumber());
		}
		break;
	}
	case Client::Player_State::LB2Combo1:
		break;
	case Client::Player_State::LB2Combo2:
		break;
	case Client::Player_State::LB2Combo3:
		break;
	case Client::Player_State::LB3Combo1_0:
		break;
	case Client::Player_State::LB3Combo1_1:
		break;
	case Client::Player_State::LB3Combo2:
		break;
	case Client::Player_State::LB3Combo3:
		break;
	case Client::Player_State::LB3Combo4:
		break;
	case Client::Player_State::Leap_Start:
		break;
	case Client::Player_State::Leap_End:
		break;
	case Client::Player_State::WhirlWind_Start: {
		_int keyFrame = playerModel->GetCurrentKeyFrame();
		if (keyFrame >= 16) {
			m_pTrailBuffer->SetIsActive(true);
		}
		break;
	}
	case Client::Player_State::WhirlWind_ing: {
		m_pTrailBuffer->SetIsActive(true);
		playerStat->SetSTATE(CStat::STATES_ATK);
		playerStat->SetDMGRatio(0.1f);
		break;
	}
	case Client::Player_State::WhirlWind_End: {
		_int keyFrame = playerModel->GetCurrentKeyFrame();
		if (keyFrame >= 0 && keyFrame <= 9) {
			m_pTrailBuffer->SetIsActive(true);
		}
		break;
	}
	case Client::Player_State::WarCry:
		break;
	case Client::Player_State::Chop_Start: {
		break;
	}
	case Client::Player_State::Chop_ing1: {
		m_pTrailBuffer->SetIsActive(true);
		_int keyFrame = playerModel->GetCurrentKeyFrame();
		playerStat->SetSTATE(CStat::STATES_ATK);
		playerStat->SetDMGRatio(0.2f);
		if (keyFrame == 5) {
			CGameObject* pGameObject = CEngine::GetInstance()->AddGameObjectToPrefab(CEngine::GetInstance()->GetCurSceneNumber(), "Prototype_Effect_ImpactShort", "E_ImpactShort");
			CEngine::GetInstance()->AddScriptObject(CImpactShort::Create((CEmptyEffect*)pGameObject, pPlayer), CEngine::GetInstance()->GetCurSceneNumber());

		}
		break;
	}
	case Client::Player_State::Chop_ing2: {
		m_pTrailBuffer->SetIsActive(true);
		_int keyFrame = playerModel->GetCurrentKeyFrame();
		playerStat->SetSTATE(CStat::STATES_ATK);
		playerStat->SetDMGRatio(0.2f);
		if (keyFrame == 5) {
			CGameObject* pGameObject = CEngine::GetInstance()->AddGameObjectToPrefab(CEngine::GetInstance()->GetCurSceneNumber(), "Prototype_Effect_ImpactShort", "E_ImpactShort");
			CEngine::GetInstance()->AddScriptObject(CImpactShort::Create((CEmptyEffect*)pGameObject, pPlayer), CEngine::GetInstance()->GetCurSceneNumber());

		}
		break;
	}
	case Client::Player_State::Chop_End: {
		break;
	}
	case Client::Player_State::Weapon_On:
		break;
	case Client::Player_State::Weapon_Off:
		break;
	case Client::Player_State::CB_Idle:
		break;
	case Client::Player_State::CBEvade:
		break;
	case Client::Player_State::CBWalk:
		break;
	case Client::Player_State::CBRun:
		break;
	case Client::Player_State::CBSprint_ing:
		break;
	case Client::Player_State::CBRun_Stop:
		break;
	case Client::Player_State::CBJump:
		break;
	case Client::Player_State::CBFall:
		break;
	case Client::Player_State::CBFall_ing:
		break;
	case Client::Player_State::CBLand_Stop:
		break;
	case Client::Player_State::CBLand_Run:
		break;
	case Client::Player_State::Dead:
		break;
	case Client::Player_State::Player_End:
		break;
	}
}
