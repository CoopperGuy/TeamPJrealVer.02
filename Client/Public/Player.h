#pragma once
#include "Character.h"
#include "StateMachine.h"
#include "MenuHud.h"
#include "Inventory.h"

BEGIN(Client)

class CStateMachine;
class LevelFlag;
class CHpBar;
class CQuickSlot;
class CGold;
class CSkillIcon;
class CTargetOn;
class CPlayer : public CCharacter
{
	enum Decompose{SCALE, ROTATE, POS};
	enum Evade_Dist {e7,e8,e9,e4,e5,e6,e1,e2,e3,Evade_END};
private:
	explicit CPlayer();
	explicit CPlayer(CGameObject* pObj);
	virtual ~CPlayer() = default;

public:
	static CPlayer* Create(CGameObject* pObj);
	virtual void Free() override;

public:
	HRESULT Initialize();
	virtual void Update(_double dDeltaTime);
	virtual void LateUpdate(_double dDeltaTime);
	virtual void Render();


public:
	void SetStateMachine();
	void SetUp_AnimIndex(_uint Index) { m_pModel->SetUp_AnimationIndex(Index); }
	_fvector MatrixDecompose(_fmatrix mat, Decompose eDecompose);

public:
	void isFinish_Combo();
	void PlayerCombo(_double dDeltaTime);
	void InputLB(_double dDeltaTime);
	void InputRB(_double dDeltaTime);
	void InputMix(_double dDeltaTime);
	void SetUp_Skill(Skill eNumSlot, _int iInputvalue);
	_int Get_Skill() { return m_iSkillIndex; }
	//inven,UI,etc..
public:
	_bool BuyItem(string itemName, ITEMINFO _item);
	_bool SellItem(string name, ITEMTYPE type, EQUIPTYPE equipType = EQUIPTYPE::TYPE_END);
	_bool AddItemQuickSlot(CItem* item, _int idx);
	void HealHp(_float point);
	//set
public:
	void Set_During() { m_bDuring = false; }
	void Set_InvenRightLeft(_bool right);
	void Set_Evade() { m_bEvade = false; }
	void Set_AnimSpeed(_double dSpeed) { m_dAnimSpeed = dSpeed; }
	void Set_SlowAttack(_bool _bSlowAttack) { m_bSlowAttck = _bSlowAttack; }
	void Set_Hit() { m_bHit = false, m_bDown = false, m_bDownIng = false; }
	//get
public:
	const _int Get_LBComboIndex() const { return m_iLB; }
	const _int Get_RBComboIndex() const { return m_iRB; }
	_bool& Get_AttackState() { return m_bCB; }
	const _bool Get_During() const { return m_bDuring; }
	CStateMachine* GetState(CurState eState);
	const _bool Get_MoveState() const { return m_bMoveState; }
	const _bool Get_Combat() const { return m_bCB; }
	const _fmatrix Get_RootMatrix() const { return XMLoadFloat4x4(&m_matRoot); }
	const _bool Get_Evade()const { return m_bEvade; }
	_bool Get_AttState() { return m_AttState; }
	_float3 Get_CenterPosition();
	void Collsion();
	_bool GetUsableSkill() { return m_bUsableSkill; }
	_float3 GetLockOnPosition();
	_bool Get_SlowAttack() { return m_bSlowAttck; }
	_bool Get_Hit() { return m_bHit; }
	_bool Get_Down() { return m_bDown; }
	_bool Get_Downing() { return m_bDownIng; }
	const CStat::STAT GetStatus() const;

public:
	void SetUpEquip(string Name);

private:
	void EquipmentsStatusUpdate();
	void Input();
	void PlayerMove(_double dDeltaTime);
	void Jump(_double dDeltaTime);
	void Equip_OnOff(Equip eEquipType, string Name, _uint NumMaterial);
	_vector SetEvadeDist();
	_bool Walk();
	_bool IsGravity();
	void CreateBlood();
	void SlowMotion(_double deltaTime);
	void SlowAttack(_double deltaTime);
private:
	void UIInput();
	void InputSkill();
private:
	void Transform_ToWorldSpace();
	void Make_Plane(_fvector* pPoints);
	void SearchMonster();
	_bool isInFrustum(_fvector vPosition, _float fRange = 0.f);

private:
	PxVec3 PlayerDirection = PxVec3(0.f, 0.f, 0.f);
	CRenderer*			m_pRendererCom = nullptr;

	//Component
private:
	CTransform* m_pCameraTransform = nullptr;
	CStat*		m_pStatus = nullptr;
	//state
private:
	_float m_fJumpSpeed		= 0.f;
	_float m_fSpeed			= 0.098f;
	//combo
	_int m_iLB				= -1;
	_int m_iRB				= -1;
	_int m_iMix				= -1;
	_double m_dCOMBOTime	= 0.f;
	_double m_dCombatTime	= 0.f;
	_double m_dKnockback	= 0.0;
private:
	_bool m_bSlowAttck		= false;
	//
	_bool m_bEvadeDelay		= false;
	_bool m_bMove			= false;
	_bool m_bJump			= false;
	_bool m_bMoveState		= false;
	_bool m_AttState		= false;
	//combo
	_bool m_bCB				= false;
	_bool m_bCombo			= false;
	_bool m_bComboDelay		= false;
	_bool m_bDuring			= false;
	_bool m_bMixCombo		= false;
	//
	_bool m_bEvade			= false;
	_bool m_bUsableSkill	= false;
	_bool m_bHit			= false;
	_bool m_bDown			= false;
	_bool m_bDownIng		= false;
	//
	_bool m_bDissolve = false;
	_float m_fDissolveAcc = 0.f;
private:
	_uint m_iIndex							= 0;
	_double m_RootTime						= 0.f;
	_double m_EvadeDelayTime				= 0.f;

	_double m_dAnimSpeed					= 0.f;
	//skill
	_int m_iSkill[(_uint)Skill::Max_Slot]	= {};
	_int m_iSkillIndex						= 0;
private:
	_double m_slowEvadeDelta = 0;
	_double m_slowEvadeTime = 0.2;
private:
	_double m_slowDelta = 0;
	_double m_sloweTime = 0.1;
private:
	_float4x4 m_matRoot			= {};
	_float4x4 m_matWorld		= {};
	PxExtendedVec3 m_vPxPos		= {};
	_float3		 m_vPlayerPos	= {};

private:
	_float3			m_vPoint[8];
	_float4			m_Plane[6];
	
	vector<CGameObject*> m_listMonsters;

private:
	const static _uint quickSlotSize = 1;

private://hud
	CMenuHud*	m_pMenuHud = nullptr;
	CInventory*	m_pInven = nullptr;
	CHpBar*		m_pHpBar = nullptr;
	LevelFlag*	m_pLevelFlag = nullptr;
	CGold*		m_pGold = nullptr;
	CQuickSlot*	m_pQuickSlot[quickSlotSize] = { nullptr, };
	CSkillIcon* m_pSkillIcon = nullptr;
	CTargetOn*	m_pTargetOn = nullptr;
private:
	CBasicCollider* m_pOBB = nullptr;
	CBasicCollider* m_pBox = nullptr;
private:
	class CEquipment* m_pEquip[(_uint)Equip::Max_Equip] = {nullptr};
	CItem*			m_pEquipItemList[(_uint)EQUIPTYPE::TYPE_END] = { nullptr };
	CStateMachine*	m_pState = nullptr;
	CStateMachine*	m_pDynamicState[(_uint)CurState::Max];
	Evade_Dist		m_eEvadeDist = Evade_END;
};
END
