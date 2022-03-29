#include "stdafx.h"
#include "..\Public\Player.h"
#include "Camera_Fly.h"

#pragma region StateMachine
#include "StateMachine.h"
#include "Idle_Player.h"
#include "Walk_Player.h"
#include "Run_Player.h"
#include "Att_Player.h"
#include "Skill_Player.h"
#pragma endregion

#pragma region HUD
#include "HpBar.h"
#include "LevelFlag.h"
#include "Stat.h"
#include "Item.h"
#include "EventCheck.h"
#include "QuickSlot.h"
#include "Gold.h"
#include "SkillIcon.h"
#include "TargetOn.h"
#pragma endregion

#pragma region Equipment
#include "Equipment.h"
#include "Glove.h"
#include "Armor.h"
#include "Boots.h"
#include "Lower.h"
#pragma endregion

#pragma region Collider
#include "BasicCollider.h"
#pragma endregion

#pragma region Effect
#include "InstanceEffectFire.h"
#include "EffectBlood.h"
#include "EffectBloodDecal.h"
#pragma endregion

#include "DropRock.h"
USING(Client)


CPlayer::CPlayer(CGameObject* pObj)
	: CCharacter(pObj)
{
}


CPlayer * CPlayer::Create(CGameObject * pObj)
{
	CPlayer*		pInstance = new CPlayer(pObj);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed to Create CPlayer");
		SafeRelease(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CPlayer::Free()
{
	__super::Free();
	//	SafeRelease(m_pState);

		/*for (_uint i = 0; i < (_uint)CurState::Max; ++i)
			SafeRelease(m_pDynamicState[i]);
	*/
	//SafeRelease(m_pMenuHud);
	//SafeRelease(m_pInven);

	//for (int i = 0; i < quickSlotSize; i++) {
	//	SafeRelease(m_pQuickSlot[i]);
	//}
	if (m_pTargetOn) {
		m_pTargetOn->SetDead();
	}
}

HRESULT CPlayer::Initialize()
{
	m_pGameObject = CEngine::GetInstance()->FindGameObjectWithName(0, "Player");
	m_pTransform = static_cast<CTransform*>(m_pGameObject->GetComponent("Com_Transform"));
	m_pModel = static_cast<CModel*>(m_pGameObject->GetComponent("Com_Model"));
	m_pNavigation = static_cast<CNavigation*>(m_pGameObject->GetComponent("Com_Navigation"));
	m_pRendererCom = static_cast<CRenderer*>(m_pGameObject->GetComponent("Com_Renderer"));
	m_pCollider = static_cast<CCollider*>(m_pGameObject->GetComponent("Com_Collider"));
	m_pStatus = static_cast<CStat*>(m_pGameObject->GetComponent("Com_Stat"));
	m_pOBB = static_cast<CBasicCollider*>(m_pGameObject->GetComponent("Com_OBB"));
	m_pBox = static_cast<CBasicCollider*>(m_pGameObject->GetComponent("Com_OBB1"));
	CGameObject* pCamera = CEngine::GetInstance()->GetGameObjectInLayer(0, "LAYER_CAMERA").front();
	m_pCameraTransform = static_cast<CTransform*>(pCamera->GetComponent("Com_Transform"));
	XMStoreFloat4x4(&m_matRoot, XMMatrixIdentity());

	if (m_pCollider)
	{
		m_pController = m_pCollider->GetController();
		//	m_pController->getActor()->userData = this;
	}
	SetStateMachine();
	m_pState = m_pDynamicState[(_uint)CurState::IDLE];
	m_pState->Enter(*this);
	m_pModel->SetAnimationLoop((_uint)Player_State::LBCombo1, false, true);
	m_pModel->SetAnimationLoop((_uint)Player_State::LBCombo2, false, true);
	m_pModel->SetAnimationLoop((_uint)Player_State::LBCombo3, false, true);
	m_pModel->SetAnimationLoop((_uint)Player_State::LBCombo4_0, false, true);
	m_pModel->SetAnimationLoop((_uint)Player_State::LBCombo4_1, false, true);
	m_pModel->SetAnimationLoop((_uint)Player_State::RBCombo1, false, false);
	m_pModel->SetAnimationLoop((_uint)Player_State::RBCombo2, false, true);
	m_pModel->SetAnimationLoop((_uint)Player_State::RBCombo3, false, true);
	m_pModel->SetAnimationLoop((_uint)Player_State::RBCombo4, false, true);
	m_pModel->SetAnimationLoop((_uint)Player_State::LB2Combo1, false, true);
	m_pModel->SetAnimationLoop((_uint)Player_State::LB2Combo2, false, true);
	m_pModel->SetAnimationLoop((_uint)Player_State::LB2Combo3, false, true);
	m_pModel->SetAnimationLoop((_uint)Player_State::LB3Combo1_0, false, true);
	m_pModel->SetAnimationLoop((_uint)Player_State::LB3Combo1_1, false, true);
	m_pModel->SetAnimationLoop((_uint)Player_State::LB3Combo2, false, true);
	m_pModel->SetAnimationLoop((_uint)Player_State::LB3Combo3, false, true);
	m_pModel->SetAnimationLoop((_uint)Player_State::LB3Combo4, false, true);
	m_pModel->SetAnimationLoop((_uint)Player_State::Leap_Start, false, true);
	m_pModel->SetAnimationLoop((_uint)Player_State::Leap_End, false, true);
	m_pModel->SetAnimationLoop((_uint)Player_State::WhirlWind_Start, false, false);
	m_pModel->SetAnimationLoop((_uint)Player_State::WhirlWind_End, false, false);
	m_pModel->SetAnimationLoop((_uint)Player_State::Chop_Start, false, false);
	m_pModel->SetAnimationLoop((_uint)Player_State::Chop_End, false, false);
	m_pModel->SetAnimationLoop((_uint)Player_State::WarCry, false, false);
	m_pModel->SetAnimationLoop((_uint)Player_State::Evade, false, false);
	m_pModel->SetAnimationLoop((_uint)Player_State::CBEvade, false, false);

	m_iSkill[0] = (_uint)Player_State::Leap_Start;
	m_iSkill[1] = (_uint)Player_State::WhirlWind_Start;
	m_iSkill[2] = (_uint)Player_State::WarCry;
	m_iSkill[3] = (_uint)Player_State::Chop_Start;

	m_pEquip[(_uint)Equip::Glove] = CGlove::Create();
	m_pEquip[(_uint)Equip::Armor] = CArmor::Create();
	m_pEquip[(_uint)Equip::Boots] = CBoots::Create();
	m_pEquip[(_uint)Equip::Pants] = CLower::Create();

	for (_uint i = 0; i < (_uint)Equip::Max_Equip; ++i)
	{
		if (m_pEquip[i])
			CEngine::GetInstance()->AddScriptObject(m_pEquip[i], SCENE_STATIC);
	}

	// menu Create
	m_pMenuHud = CMenuHud::Create(m_pGameObject);
	m_pInven = CInventory::Create(m_pGameObject);
	m_pGold = CGold::Create(m_pGameObject);
	m_pHpBar = CHpBar::Create(m_pGameObject);
	m_pLevelFlag = LevelFlag::Create(m_pGameObject);
	m_pSkillIcon = CSkillIcon::Create(m_pGameObject);

	CSkillIcon::SKILLINFO info;
	info.coolTime = 7.5f;
	info.level = 1;
	info.Name = "storm";
	m_pSkillIcon->SetSkillInfo(0, info);
	info.coolTime = 15.f;
	info.level = 1;
	info.Name = "WhrilWind";
	m_pSkillIcon->SetSkillInfo(1, info);
	info.coolTime = 20.f;
	info.skillDuration = 10.f;
	info.Name = "Blood";
	m_pSkillIcon->SetSkillInfo(2, info);
	info.coolTime = 20.f;
	info.Name = "guillotine";
	m_pSkillIcon->SetSkillInfo(3, info);

	m_pMenuHud->LinkInventoryToHud(m_pInven);

	//Hud Create
	CEngine::GetInstance()->AddScriptObject(m_pMenuHud, SCENE_STATIC);
	CEngine::GetInstance()->AddScriptObject(m_pInven, SCENE_STATIC);
	CEngine::GetInstance()->AddScriptObject(m_pHpBar, SCENE_STATIC);
	CEngine::GetInstance()->AddScriptObject(m_pLevelFlag, SCENE_STATIC);
	CEngine::GetInstance()->AddScriptObject(m_pGold, SCENE_STATIC);

	for (_uint i = 0; i < quickSlotSize; i++) {
		m_pQuickSlot[i] = CQuickSlot::Create(nullptr);
		CEngine::GetInstance()->AddScriptObject(m_pQuickSlot[i], SCENE_STATIC);
	}

	CEventCheck::GetInstance()->SetPlayer(this);

	return S_OK;
}

void CPlayer::Update(_double dDeltaTime)
{
	if (CEngine::GetInstance()->GetCurSceneNumber() < SCENE_STAGE1) {
		m_pGameObject->SetActive(false);
		return;
	}
	else {
		m_pGameObject->SetActive(true);
	}

	if (!m_pGameObject)
		return;

	m_dAnimSpeed = 1.f;
	if (!g_Menu && !g_AnotherMenu) {
		PlayerMove(dDeltaTime);
		InputSkill();
		Input();
		Jump(dDeltaTime);
	}
	m_pState->Update(dDeltaTime, *this);
	this->UIInput();
	
	//////////////////Dissolve Test///////////////////////////////
	if (m_bDissolve)
	{
		m_fDissolveAcc += (_float)dDeltaTime;
		if (m_fDissolveAcc > 1.f)
			m_fDissolveAcc = 1.f;
	}
	else
	{
		m_fDissolveAcc -= (_float)dDeltaTime;
		if (m_fDissolveAcc < 0.f)
			m_fDissolveAcc = 0.f;
	}
	m_pModel->SetDissolve(m_fDissolveAcc);
	///////////////////////////////////////////////////////////////

	//#ifdef _DEBUG
	if (CEngine::GetInstance()->Get_DIKDown(DIK_NUMPADPLUS)) {
		m_pStatus->EarnGold(100);
	}
	//#endif // _DEBUG
	Collsion();
	Transform_ToWorldSpace();
	SearchMonster();
	CreateBlood();
	SlowMotion(dDeltaTime);
	SlowAttack(dDeltaTime);
}

void CPlayer::LateUpdate(_double dDeltaTime)
{
	__super::LateUpdate(dDeltaTime);

	if (CEngine::GetInstance()->GetCurSceneNumber() < SCENE_STAGE1) {
		m_pGameObject->SetActive(false);
		return;
	}
	else {
		m_pGameObject->SetActive(true);
	}
	if (m_pModel->Get_AnimIndex() == (_uint)Player_State::RBCombo2)
		m_dAnimSpeed = 2.f;
	else
		m_dAnimSpeed = 1.f;
	m_pModel->Play_Animation(dDeltaTime * m_dAnimSpeed);

	if (CEngine::GetInstance()->IsKeyDown(VK_F5)) {
		CStat* stats = static_cast<CStat*>(m_pGameObject->GetComponent("Com_Stat"));
		CStat::STAT objStat = stats->GetStatInfo();
		objStat.level++;
		stats->SetStatInfo(objStat);
	}

	if (CEngine::GetInstance()->IsKeyDown(VK_F6)) {
		CStat* stats = static_cast<CStat*>(m_pGameObject->GetComponent("Com_Stat"));
		CStat::STAT objStat = stats->GetStatInfo();
		objStat.hp -= 10.f;
		stats->SetStatInfo(objStat);
	}
}

void CPlayer::Render()
{

}

void CPlayer::SetStateMachine()
{
	m_pDynamicState[(_uint)CurState::IDLE] = new CIdle_Player;
	m_pDynamicState[(_uint)CurState::WALK] = new CWalk_Player;
	m_pDynamicState[(_uint)CurState::RUN] = new CRun_Player;
	m_pDynamicState[(_uint)CurState::ATT] = new CAtt_Player;
	m_pDynamicState[(_uint)CurState::Skill] = new CSkill_Player;
}

_fvector CPlayer::MatrixDecompose(_fmatrix mat, Decompose eDecompose)
{
	_vector scale, rotation, pos;
	XMMatrixDecompose(&scale, &rotation, &pos, mat);
	if (eDecompose == SCALE)
		return scale;
	else if (eDecompose == ROTATE)
		return rotation;
	else
		return pos;
}

void CPlayer::isFinish_Combo()
{
	m_iLB = -1;
	m_iRB = -1;
}

CStateMachine * CPlayer::GetState(CurState eState)
{
	return m_pDynamicState[(_uint)eState];
}

_float3 CPlayer::Get_CenterPosition()
{

	return _float3();
}

void CPlayer::Collsion()
{
	if (m_pState->Get_State() == CurState::ATT)
		m_pGameObject->Set_AttackState(true);
	else
		m_pGameObject->Set_AttackState(false);


}

_float3 CPlayer::GetLockOnPosition()
{
	if (m_listMonsters.size() > 0)
		return m_listMonsters.front()->GetCollisionPosition();
	return m_pGameObject->GetCollisionPosition();
}

void CPlayer::Input()
{
	CStateMachine* pState = m_pState->Input(*this);

	if (pState)
	{
		m_pState->End(*this);
		if (m_pState->Get_State() != pState->Get_State())
		{
			m_pState = pState;
		}
		m_pState->Enter(*this);
	}
	m_pState->Input(*this);
}

void CPlayer::PlayerCombo(_double dDeltaTime)
{
	//when Att Idle
	if (m_bCB)
	{
		m_dCombatTime += dDeltaTime;
		if (m_iLB >= 0/* && m_iMix < 0*/)
			InputLB(dDeltaTime);
		else if (m_iRB >= 0)
			InputRB(dDeltaTime);
		/*	else if (m_iMix >= 0)
				InputMix(dDeltaTime);*/
	}
	if (m_dCombatTime > 5.f)
	{
		m_iLB = -1;
		m_iRB = -1;
		m_bCB = false;

		m_dCombatTime = 0.f;
	}

	if (m_pState->Get_State() != CurState::ATT && m_iLB < 0 && m_iRB < 0)
	{
		//Start Combo
		if (CEngine::GetInstance()->IsMouseDown(0))
		{
			m_bCB = true;
			m_bDuring = false;
			m_dCombatTime = 0.f;
			m_iLB = 0;
		}
		else if (CEngine::GetInstance()->IsMouseDown(1))
		{
			m_bCB = true;
			m_bDuring = false;
			m_dCombatTime = 0.f;
			m_iRB = 0;
		}
	}
}

void CPlayer::InputLB(_double dDeltaTime)
{
	//During Combo
	if (m_bDuring)
	{
		m_dCombatTime = 0.f;
		if (m_iLB == 0)
		{
			if (m_pModel->GetCurrentKeyFrame() >= 28)
				m_bCombo = true;
		}
		if (m_iLB == 1)
		{
			if (m_pModel->GetCurrentKeyFrame() >= 37)
				m_bCombo = true;
		}
		if (m_iLB == 2)
		{
			if (m_pModel->Get_isFinished())
			{
				//if (!m_bMixCombo)
				m_bCombo = true;
			}
		}
		if (m_iLB == 3)
		{
			if (m_pModel->Get_isFinished())
			{
				//if (!m_bMixCombo)
				m_bCombo = true;
			}
		}
		if (m_iLB == 4)
		{
			if (m_pModel->Get_isFinished())
				m_bCombo = true;
		}
		if (m_bCombo)
		{
			++m_iLB;
			//m_bCombo = false;
			m_bDuring = false;
			m_bComboDelay = false;
		}

	}
	else if (m_bComboDelay)
	{
		m_dCOMBOTime += dDeltaTime;

		if (m_dCOMBOTime >= 1.f)
		{
			m_bCB = true;
			m_bComboDelay = false;
			m_dCOMBOTime = 0.f;
			m_iLB = -1;
		}

		if (CEngine::GetInstance()->IsMouseDown(0))
		{
			++m_iLB;
			m_dCombatTime = 0.f;
			m_dCOMBOTime = 0.f;
			m_bComboDelay = false;
		}
	}


	if (m_pState->Get_State() == CurState::ATT)
	{
		m_dCombatTime = 0.f;

		if (m_pModel->Get_isFinished())
		{
			m_bCB = true;
			if (m_iLB >= 3)
				m_bDuring = true;
			else
			{
				if (m_iLB < 4 && m_iLB > -1)
					m_bComboDelay = true;
				else
					m_bComboDelay = false;
			}
		}
		//Ing Att
		else if (CEngine::GetInstance()->IsMouseDown(0) && !m_bComboDelay)
		{
			m_bDuring = true;
		}
		//else if (CEngine::GetInstance()->IsMouseDown(1) && !m_bComboDelay)
		//{
		//	if (m_iLB == 1 || m_iLB == 2)
		//	{
		//		m_bMixCombo = true;
		//		++m_iMix;
		//	}
		//}
	}
	if (m_iLB > 5)
	{
		m_iLB = -1;
		m_bDuring = false;
		m_bComboDelay = false;
	}
}

void CPlayer::InputRB(_double dDeltaTime)
{
	if (m_bDuring)
	{
		m_dCombatTime = 0.f;
		if (m_iRB == 0)
		{
			if (m_pModel->GetCurrentKeyFrame() >= 37)
				m_bCombo = true;
		}
		if (m_iRB == 1)
		{
			if (m_pModel->GetCurrentKeyFrame() >= 18)
				m_bCombo = true;
		}
		if (m_iRB == 2)
		{
			if (m_pModel->GetCurrentKeyFrame() >= 24)
				m_bCombo = true;
		}
		if (m_iRB == 3)
		{
			if (m_pModel->Get_isFinished())
				m_bCombo = true;
		}

		if (m_bCombo)
		{
			++m_iRB;
			//m_bCombo = false;
			m_bDuring = false;
			m_bComboDelay = false;
		}

	}
	else if (m_bComboDelay)
	{
		m_dCOMBOTime += dDeltaTime;

		if (m_dCOMBOTime >= 1.f)
		{
			m_bCB = true;
			m_dCombatTime = 0.f;
			m_bComboDelay = false;
			m_dCOMBOTime = 0.f;
			m_iRB = -1;
		}

		if (CEngine::GetInstance()->IsMouseDown(1))
		{
			++m_iRB;
			m_dCombatTime = 0.f;
			m_dCOMBOTime = 0.f;
			m_bComboDelay = false;
		}
	}

	if (m_pState->Get_State() == CurState::ATT)
	{
		m_dCombatTime = 0.f;

		if (m_pModel->Get_isFinished())
		{
			m_bCB = true;
			if (m_iRB < 4 && m_iRB > -1)
				m_bComboDelay = true;
			else
				m_bComboDelay = false;
		}
		//Ing Att
		else if (CEngine::GetInstance()->IsMouseDown(1) && !m_bComboDelay)
			m_bDuring = true;
	}
	if (m_iRB >= 4)
	{
		m_iRB = -1;
		m_bDuring = false;
		m_bComboDelay = false;
	}
}

void CPlayer::InputMix(_double dDeltaTime)
{
	if (m_bDuring)
	{
		m_dCombatTime = 0.f;
		if (m_iMix == 0)
		{
			if (m_pModel->GetCurrentKeyFrame() >= 37)
				m_bCombo = true;
		}
		if (m_iMix == 1)
		{
			if (m_pModel->GetCurrentKeyFrame() >= 18)
				m_bCombo = true;
		}
		if (m_iMix == 2)
		{
			if (m_pModel->GetCurrentKeyFrame() >= 24)
				m_bCombo = true;
		}
		if (m_iMix == 3)
		{
			if (m_pModel->Get_isFinished())
				m_bCombo = true;
		}

		if (m_bCombo)
		{
			++m_iMix;
			m_bCombo = false;
			m_bDuring = false;

			m_bComboDelay = false;
		}

	}
	else if (m_bComboDelay)
	{
		m_dCOMBOTime += dDeltaTime;

		if (m_dCOMBOTime >= 2.f)
		{
			//m_bCB = true;
			m_bComboDelay = false;
			m_dCOMBOTime = 0.f;
			m_iRB = -1;
		}

		if (CEngine::GetInstance()->IsMouseDown(1))
		{
			++m_iMix;
			m_dCombatTime = 0.f;
			m_dCOMBOTime = 0.f;
			m_bComboDelay = false;
		}
	}

	if (m_pState->Get_State() == CurState::ATT)
	{
		if (m_pModel->Get_isFinished())
		{
			m_bCB = true;
			m_dCombatTime = 0.f;
			if (m_iMix < 4 && m_iMix > -1)
				m_bComboDelay = true;
			else
				m_bComboDelay = false;
		}
		//Ing Att
		else if (CEngine::GetInstance()->IsMouseDown(1) && !m_bComboDelay)
			m_bDuring = true;
	}
	if (m_iMix > 4)
	{
		m_iMix = -1;
		m_bDuring = false;
		m_bComboDelay = false;
	}
}

void CPlayer::SetUp_Skill(Skill eNumSlot, _int iInputvalue)
{
	if ((_uint)eNumSlot > (_uint)Skill::Max_Slot)
	{
		m_iSkill[(_uint)eNumSlot] = iInputvalue;
		for (_uint i = 0; i < (_uint)Skill::Max_Slot; ++i)
		{
			if ((_uint)eNumSlot != i)
			{
				if (m_iSkill[(_uint)eNumSlot] == m_iSkill[i])
					m_iSkill[i] = -1;
			}
		}
	}
}

_bool CPlayer::BuyItem(string itemName, ITEMINFO _item)
{
	_int curGold = m_pStatus->GetStatInfo().gold;
	if (curGold >= _item.price) {
		m_pStatus->EarnGold(-_item.price);
		CItem*	tempItem = new CItem(_item);
		if (!m_pInven->AddItem(itemName, tempItem)) {
			SafeDelete(tempItem);
		}
		//gold decrease please
		return true;
	}
	else {
		//fail
		return false;
	}

	return _bool();
}

_bool CPlayer::AddItemQuickSlot(CItem* item, _int idx)
{
	if (idx >= quickSlotSize)
		return false;
	m_pQuickSlot[idx]->RegistItem(item, idx);
	return true;
}

void CPlayer::HealHp(_float point)
{
	CStat* stat = static_cast<CStat*>(m_pGameObject->GetComponent("Com_Stat"));
	stat->HpHeal(point);
}

void CPlayer::Set_InvenRightLeft(_bool right)
{
	if (m_pMenuHud) {
		m_pMenuHud->SetInvenRightLeft(right);
	}
}


void CPlayer::PlayerMove(_double dDeltaTime)
{
	//ÇÃ·¹ÀÌ¾î ¿òÁ÷ÀÓ 
	_vector vLook, vRight, vPlayerLook, vEvadeLook;
	vLook = XMVector3Normalize(m_pCameraTransform->GetState(CTransform::STATE_LOOK));
	XMStoreFloat3(&m_vPlayerPos, vLook);
	vRight = XMVector3Normalize(m_pCameraTransform->GetState(CTransform::STATE_RIGHT));
	vPlayerLook = XMVector3Normalize(m_pTransform->GetState(CTransform::STATE_LOOK));
	vEvadeLook = vPlayerLook;
	PxVec3 vDir = PxVec3(0.f, 0.f, 0.f);
	PxControllerFilters filters;
	_float fSpeed = 0.f;
	_vector vUp = m_pTransform->GetState(CTransform::STATE::STATE_UP);
	if (Walk() && !m_bEvade)
	{
		if (CEngine::GetInstance()->IsKeyPressed('W'))
		{
			vPlayerLook = XMVectorLerp(vPlayerLook, vLook, 0.4f);
			vPlayerLook = XMVectorSetY(vPlayerLook, 0.f);
			memcpy(&vDir, &vPlayerLook, sizeof(PxVec3));
			fSpeed = 1.5f;
		}
		if (CEngine::GetInstance()->IsKeyPressed('S'))
		{
			vPlayerLook = XMVectorLerp(vPlayerLook, -vLook, 0.4f);
			vPlayerLook = XMVectorSetY(vPlayerLook, 0.f);
			memcpy(&vDir, &vPlayerLook, sizeof(PxVec3));
			fSpeed = 1.5f;
		}
		if (CEngine::GetInstance()->IsKeyPressed('A'))
		{
			vPlayerLook = XMVectorLerp(vPlayerLook, -vRight, 0.4f);
			vPlayerLook = XMVectorSetY(vPlayerLook, 0.f);
			memcpy(&vDir, &vPlayerLook, sizeof(PxVec3));
			fSpeed = 1.5f;
		}
		if (CEngine::GetInstance()->IsKeyPressed('D'))
		{
			vPlayerLook = XMVectorLerp(vPlayerLook, vRight, 0.4f);
			vPlayerLook = XMVectorSetY(vPlayerLook, 0.f);
			memcpy(&vDir, &vPlayerLook, sizeof(PxVec3));
			fSpeed = 1.5f;
		}

		if (CEngine::GetInstance()->Get_DIKState(DIK_LSHIFT)) //Sprint
		{
			fSpeed = 2.5f;
		}
		if (CEngine::GetInstance()->IsKeyDown(VK_CONTROL)) // Jump
		{
			m_bJump = true;
			m_fJumpSpeed = 0.05f;
		}

		if (CEngine::GetInstance()->IsKeyDown(VK_OEM_2))
		{
			if (!m_bMoveState)
				m_bMoveState = true;
			else
				m_bMoveState = false;
		}
	}

	vEvadeLook = SetEvadeDist();
	//PlayerCombo(dDeltaTime);


	if (!m_bEvade && !m_bEvadeDelay)
	{
		m_pStatus->Immortal(false);
		if (CEngine::GetInstance()->Get_DIKDown(DIK_SPACE)) // Evade
		{
			m_bEvade = true;
			vEvadeLook = XMVectorSetY(vEvadeLook, 0.f);
			vPlayerLook = vEvadeLook;
			m_pStatus->Immortal(true);
		}
	}

	if (m_bEvade)
	{
		m_bEvadeDelay = true;
		m_EvadeDelayTime = 0.f;
		isFinish_Combo();
		m_dAnimSpeed = 1.7f;
		m_pStatus->Immortal(true);
		switch (m_pModel->Get_AnimIndex())
		{
		case (_uint)Player_State::Evade:
			m_bCB = false;
			if (m_pModel->GetCurrentKeyFrame() < 10)
			{
				memcpy(&vDir, &vPlayerLook, sizeof(PxVec3));
				fSpeed = 4.f;
			}
			else
			{
				if (m_bMove)
					m_bEvade = false;
			}
			break;
		case (_uint)Player_State::CBEvade:
			if (m_pModel->GetCurrentKeyFrame() < 17)
			{
				memcpy(&vDir, &vPlayerLook, sizeof(PxVec3));
				fSpeed = 2.f;
			}
			else
			{
				if (m_bMove)
					m_bEvade = false;
			}
			break;
		}

		if (m_pModel->Get_isFinished()) {
			m_bEvade = false;
			m_pStatus->Immortal(false);
		}
	}

	if (m_bEvadeDelay)
	{
		m_EvadeDelayTime += dDeltaTime;

		if (m_EvadeDelayTime > 1.f)
		{
			m_bEvadeDelay = false;
			m_EvadeDelayTime = 0.f;
		}
	}
	if (!m_bEvade)
		PlayerCombo(dDeltaTime);
	if (m_bCombo)
	{
		vPlayerLook = vEvadeLook;
		m_bCombo = false;
	}
	/*if (CEngine::GetInstance()->IsKeyDown('K'))
		SetUpEquip("PlateArmor");
	if (CEngine::GetInstance()->IsKeyDown('L'))
		SetUpEquip("LeatherArmor");*/

	m_pTransform->SetLook(vPlayerLook);

	m_pController->move(vDir * fSpeed * (_float)dDeltaTime, 0.f, (_float)dDeltaTime, nullptr);
}

void CPlayer::Jump(_double dDeltaTime)
{

	PxControllerFilters filters;
	if (m_bJump)
	{
		if (m_fJumpSpeed >= 0)
		{
			m_fJumpSpeed -= _float(m_fSpeed * dDeltaTime);
		}
		else
		{
			m_bJump = false;
		}

		m_pController->move(PxVec3(0.f, m_fJumpSpeed, 0.f), 0.01f, PxF32(dDeltaTime), filters);
	}
	else
	{
		if (IsGravity()) {
			m_fJumpSpeed -= _float(m_fSpeed * (_float)dDeltaTime);
			m_pController->move(PxVec3(0.0f, m_fJumpSpeed, 0.f), 0.01f, PxF32(dDeltaTime), filters);
		}
		else {
			m_fJumpSpeed = 0.f;
		}
	}
}

void CPlayer::SetUpEquip(string Name)
{
	ITEMINFO Info = CEngine::GetInstance()->GetItemAsName(Name).second;
	_uint i = 0;
	if (Info.itemType == ITEMTYPE::EQUIP)
	{
		switch (Info.equipType)
		{
		case EQUIPTYPE::CHEST:
			i = 0;
			Equip_OnOff(Equip::Armor, Name, i);
			break;
		case EQUIPTYPE::GLOOVE:
			i = 1;
			Equip_OnOff(Equip::Glove, Name, i);
			break;
		case EQUIPTYPE::SHOOSE:
			i = 2;
			Equip_OnOff(Equip::Boots, Name, i);
			break;
		case EQUIPTYPE::HELMET:
			i = 4;
			Equip_OnOff(Equip::Helm, Name, i);
			break;
		case EQUIPTYPE::LOWER:
			i = 6;
			Equip_OnOff(Equip::Pants, Name, i);
			break;
		default:
			break;
		}
	}
}

void CPlayer::Equip_OnOff(Equip eEquipType, string Name, _uint NumMaterial)
{
	_bool bCheck = m_pEquip[(_uint)eEquipType]->Set_ModelCom(Name);

	if (bCheck)
	{
		if (m_pEquip[(_uint)eEquipType]->Get_GameObj()->GetActive())
		{
			//if(eEquipType != Equip::Armor)
			static_cast<CEmptyGameObject*>(m_pGameObject)->Set_Render(NumMaterial, false);
		}
		else
			static_cast<CEmptyGameObject*>(m_pGameObject)->Set_Render(NumMaterial, true);
	}
}

_vector CPlayer::SetEvadeDist()
{
	_vector vLook, vRight, vPlayerLook, vEvadeLook;
	vLook = XMVector3Normalize(m_pCameraTransform->GetState(CTransform::STATE_LOOK));
	vRight = XMVector3Normalize(m_pCameraTransform->GetState(CTransform::STATE_RIGHT));
	vPlayerLook = XMVector3Normalize(m_pTransform->GetState(CTransform::STATE_LOOK));
	vEvadeLook = vPlayerLook;

	if (CEngine::GetInstance()->Get_DIKState(DIK_W))
	{
		if (CEngine::GetInstance()->Get_DIKState(DIK_A))
			m_eEvadeDist = e7;
		else if (CEngine::GetInstance()->Get_DIKState(DIK_D))
			m_eEvadeDist = e9;
		else
			m_eEvadeDist = e8;
		m_bMove = true;
	}
	else if (CEngine::GetInstance()->Get_DIKState(DIK_S))
	{
		if (CEngine::GetInstance()->Get_DIKState(DIK_A))
			m_eEvadeDist = e1;
		else if (CEngine::GetInstance()->Get_DIKState(DIK_D))
			m_eEvadeDist = e3;
		else
			m_eEvadeDist = e2;
		m_bMove = true;
	}
	else if (CEngine::GetInstance()->Get_DIKState(DIK_A))
	{
		if (CEngine::GetInstance()->Get_DIKState(DIK_W))
			m_eEvadeDist = e7;
		else if (CEngine::GetInstance()->Get_DIKState(DIK_S))
			m_eEvadeDist = e1;
		else
			m_eEvadeDist = e4;
		m_bMove = true;
	}
	else if (CEngine::GetInstance()->Get_DIKState(DIK_D))
	{
		if (CEngine::GetInstance()->Get_DIKState(DIK_W))
			m_eEvadeDist = e9;
		else if (CEngine::GetInstance()->Get_DIKState(DIK_S))
			m_eEvadeDist = e3;
		else
			m_eEvadeDist = e6;
		m_bMove = true;
	}
	else
	{
		m_bMove = false;
		m_eEvadeDist = e5;
	}
	switch (m_eEvadeDist)
	{
	case e7:
		vEvadeLook = XMVector3Normalize(vLook - vRight);
		break;
	case e8:
		vEvadeLook = vLook;
		break;
	case e9:
		vEvadeLook = XMVector3Normalize(vLook - (-vRight));
		break;
	case e4:
		vEvadeLook = -vRight;
		break;
	case e5:
		vEvadeLook = vPlayerLook;
		break;
	case e6:
		vEvadeLook = vRight;
		break;
	case e1:
		vEvadeLook = XMVector3Normalize(-vLook - vRight);
		break;
	case e2:
		vEvadeLook = -vLook;
		break;
	case e3:
		vEvadeLook = XMVector3Normalize(-vLook - (-vRight));
		break;
	}

	return vEvadeLook;
}

_bool CPlayer::Walk()
{
	if (m_pState->Get_State() == CurState::IDLE || m_pState->Get_State() == CurState::RUN
		|| m_pState->Get_State() == CurState::WALK || m_pModel->Get_AnimIndex() == (_uint)Player_State::WhirlWind_ing)
		return true;
	else
		return false;
}

_bool CPlayer::IsGravity()
{
	_vector vPlayerPos = m_pTransform->GetState(CTransform::STATE_POSITION);
	_vector vRayDir = XMVectorSet(0.f, -1.f, 0.f, 0.f);
	PxRaycastBuffer buf;
	PxQueryFilterData filterData;
	filterData.data.word1 = CPxManager::GROUP4;
	filterData.data.word2 = CPxManager::GROUP4;
	filterData.flags |= PxQueryFlag::eANY_HIT;
	filterData.flags |= PxQueryFlag::ePREFILTER;
	_bool isCollied = false;
	PxRigidActor* actor = m_pController->getActor();

	if (CEngine::GetInstance()->Raycast(vPlayerPos, vRayDir, 0.06f, buf, filterData, &CPxQueryFilters(actor, CPxManager::GROUP4)))
	{
		if (buf.getAnyHit(0).distance <= 0.0f)
		{
			//cout << "playe collid with terrain\n";
			isCollied = false;
		}
		else {
			//cout << "playe no collid with terrain \n";
			isCollied = true;
		}
	}
	else {
		//	cout << "playe no collid with terrain \n";
		isCollied = true;
	}
	return isCollied;
}

void CPlayer::CreateBlood()
{
	if (m_pOBB->Get_isHit()) {

		_matrix Translation;
		_int random = rand() % 2;
		random += 1;
		Translation = XMMatrixTranslation(XMVectorGetX(m_pTransform->GetState(CTransform::STATE_POSITION)), XMVectorGetY(m_pTransform->GetState(CTransform::STATE_POSITION)) + ((float)random*0.1f), XMVectorGetZ(m_pTransform->GetState(CTransform::STATE_POSITION)));
		Translation = m_pTransform->Remove_Scale(Translation);


		CGameObject* EffectBlood = CEngine::GetInstance()->AddGameObjectToPrefab(CEngine::GetInstance()->GetCurSceneNumber(), "Prototype_Effect_IIBlood", "E_IIBlood", &Translation);
		CEngine::GetInstance()->AddScriptObject(CEffectBlood::Create(EffectBlood), CEngine::GetInstance()->GetCurSceneNumber());

		CGameObject* EffectBloodDecal = CEngine::GetInstance()->AddGameObjectToPrefab(CEngine::GetInstance()->GetCurSceneNumber(), "Prototype_Effect_BloodDecal", "E_BloodDecal");
		CEngine::GetInstance()->AddScriptObject(CEffectBloodDecal::Create(EffectBloodDecal, m_pTransform->GetState(CTransform::STATE_POSITION)), CEngine::GetInstance()->GetCurSceneNumber());
	}
}

void CPlayer::SlowMotion(_double deltaTime)
{
	_bool isSlow = m_pStatus->GetIsSlow();
	if (isSlow == true) {
		CEventCheck::GetInstance()->ZoomFov(m_slowEvadeTime, 60.f, 15.f);
		g_TickLate = 0.5f;
		m_slowEvadeDelta += deltaTime;
		if (m_slowEvadeTime < m_slowEvadeDelta) {
			m_slowEvadeDelta = 0;
			m_pStatus->SetSlow(false);
			g_TickLate = 1.f;
		}
	}
}

void CPlayer::SlowAttack(_double deltaTime)
{
	if (m_bSlowAttck) {
		g_TickLate = 0.5f;
		m_slowDelta += deltaTime;
		if (m_sloweTime < m_slowDelta) {
			m_slowDelta = 0;
			g_TickLate = 1.f;
			m_bSlowAttck = false;
		}
	}
}

void CPlayer::UIInput()
{
	_int i = -1;
	if (CEngine::GetInstance()->Get_DIKDown(DIK_F1))
		i = 0;
	else if (CEngine::GetInstance()->Get_DIKDown(DIK_F2))
		i = 1;
	else if (CEngine::GetInstance()->Get_DIKDown(DIK_F3))
		i = 2;
	else if (CEngine::GetInstance()->Get_DIKDown(DIK_F4))
		i = 3;

	if (i >= 0) {
		if (quickSlotSize > i)
			m_pQuickSlot[i]->UseItem(this);
	}

	if (m_pGold) {
		string gold = to_string(m_pStatus->GetStatInfo().gold);
		m_pGold->UpdateGoldString(gold);
	}
}
void CPlayer::InputSkill()
{
	if (CEngine::GetInstance()->IsKeyDown('1'))
	{
		if (m_pSkillIcon->IsCoolDown(0)) {
			m_bCB = true;
			m_iSkillIndex = m_iSkill[(_uint)Skill::Slot1];
			m_pSkillIcon->UseSkill(0);
			m_bUsableSkill = true;
		}
		else {
			m_bUsableSkill = false;
		}
	}
	if (CEngine::GetInstance()->IsKeyDown('2'))
	{
		if (m_pSkillIcon->IsCoolDown(1)) {
			m_bCB = true;
			m_iSkillIndex = m_iSkill[(_uint)Skill::Slot2];
			m_pSkillIcon->UseSkill(1);
			m_bUsableSkill = true;
		}
		else {
			m_bUsableSkill = false;
		}
	}
	if (CEngine::GetInstance()->IsKeyDown('3'))
	{
		if (m_pSkillIcon->IsCoolDown(2)) {
			m_bCB = true;
			m_iSkillIndex = m_iSkill[(_uint)Skill::Slot3];
			m_pSkillIcon->UseSkill(2);
			m_bUsableSkill = true;
		}
		else {
			m_bUsableSkill = false;
		}
	}
	if (CEngine::GetInstance()->IsKeyDown('4'))
	{
		if (m_pSkillIcon->IsCoolDown(3)) {
			m_bCB = true;
			m_iSkillIndex = m_iSkill[(_uint)Skill::Slot4];
			m_pSkillIcon->UseSkill(3);
			m_bUsableSkill = true;
		}
		else {
			m_bUsableSkill = false;
		}
	}

	if (CEngine::GetInstance()->IsKeyDown('5')) {
		m_bDissolve = !m_bDissolve;
		//static_cast<CEmptyGameObject*>(m_pGameObject)->SetRimLight(true);
	}
	//if (CEngine::GetInstance()->IsKeyDown('6')) {
	//	static_cast<CEmptyGameObject*>(m_pGameObject)->SetRimLight(false);
	//}


	//if (CEngine::GetInstance()->Get_DIKDown(DIK_NUMPAD0)) {
	//	CEngine::GetInstance()->AddGameObjectToPrefab(CEngine::GetInstance()->GetCurSceneNumber(), "Prototype_GameObecjt_Wolf", "Wolf");
	//}	
}

void CPlayer::Transform_ToWorldSpace()
{

	_vector		vPoint[8];
	_vector*	tempPoint = m_pBox->GetObbBox();
	for (int i = 0; i < 8; i++)
		vPoint[i] = tempPoint[i];
	Make_Plane(vPoint);
}

void CPlayer::Make_Plane(_fvector * pPoints)
{
	XMStoreFloat4(&m_Plane[0], XMPlaneFromPoints(pPoints[1], pPoints[2], pPoints[5]));
	XMStoreFloat4(&m_Plane[1], XMPlaneFromPoints(pPoints[4], pPoints[7], pPoints[0]));

	XMStoreFloat4(&m_Plane[2], XMPlaneFromPoints(pPoints[4], pPoints[5], pPoints[1]));
	XMStoreFloat4(&m_Plane[3], XMPlaneFromPoints(pPoints[3], pPoints[6], pPoints[7]));

	XMStoreFloat4(&m_Plane[4], XMPlaneFromPoints(pPoints[5], pPoints[4], pPoints[7]));
	XMStoreFloat4(&m_Plane[5], XMPlaneFromPoints(pPoints[0], pPoints[1], pPoints[2]));
}

void CPlayer::SearchMonster()
{
	if (m_pTargetOn) {
		if (m_listMonsters.size() > 0) {
			_float lenght = XMVectorGetX(XMVector3Length(XMLoadFloat3(&m_pGameObject->GetPosition()) - XMLoadFloat3(&m_listMonsters.front()->GetPosition())));
			if (lenght >= 4.f) {
				m_listMonsters.clear();
				m_pTargetOn->ReleaseThisUI();
				m_pTargetOn = nullptr;
			}
		}
	}
	if (CEngine::GetInstance()->Get_DIKDown(DIK_B)) {
		m_listMonsters.clear();
		CEngine* engine = CEngine::GetInstance();
		list<CGameObject*> monList = engine->GetGameObjectInLayer(engine->GetCurSceneNumber(), "Layer_Monster");
		for (auto& iter : monList) {
			if (isInFrustum(XMLoadFloat3(&iter->GetCollisionPosition()), 1.f)) {
				m_listMonsters.emplace_back(iter);
			}
		}
		sort(m_listMonsters.begin(), m_listMonsters.end(), [](CGameObject* _A, CGameObject* _B) {
			return _A->GetCollisionPosition().z < _B->GetCollisionPosition().z;
		});
		if (m_listMonsters.size() > 0) {
			if (m_pTargetOn) {
				m_pTargetOn->ReleaseThisUI();
				m_pTargetOn = nullptr;
				m_pTargetOn = CTargetOn::Create(m_pGameObject, m_listMonsters.front());
			}
			else {
				m_pTargetOn = CTargetOn::Create(m_pGameObject, m_listMonsters.front());
			}
		}
		else {
			if (m_pTargetOn) {
				m_pTargetOn->ReleaseThisUI();
				m_pTargetOn = nullptr;
			}
		}
	}

}

_bool CPlayer::isInFrustum(_fvector vPosition, _float fRange)
{
	for (_uint i = 0; i < 6; ++i)
	{
		if (fRange < XMVectorGetX(XMPlaneDotCoord(XMLoadFloat4(&m_Plane[i]), vPosition)))
			return false;
	}
	return true;
}

