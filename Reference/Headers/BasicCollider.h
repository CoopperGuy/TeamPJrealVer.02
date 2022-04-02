#pragma once

#include "Component.h"
#include "DebugDraw.h"
#include "DebugBox.h"
#include "DebugCapsule.h"

/* 객체의 충돌을 위한 충돌체. */

BEGIN(Engine)

class ENGINE_DLL CBasicCollider final : public CComponent
{
public:
	enum STATES { STATES_IDEL, STATES_ATK, STATES_HITTED, STATES_END };
	enum TYPE { TYPE_AABB, TYPE_OBB, TYPE_SPHERE, TYPE_END };
	enum COLLSTATE { START, ING, FINESHED, STATEEND };
	enum COLLISIONTYPE {
		COLLSION_NONE = 0,
		COLLISION_FOUND = (1 << 0),
		COLLISION = (1 << 1),
		COLLISION_LOST = (1 << 2),
		COLLISION_END = (1 << 3)
	};
	typedef pair<CGameObject*, CBasicCollider*> OBJCOLLIDER;
public:
	struct CollisionRay
	{
		XMVECTOR origin;
		XMVECTOR direction;
	};

public:
	typedef struct tagColliderDesc
	{
		/* 상태행렬의 스케일값은 무시. */
		_float3		vSize = _float3(1.f, 1.f, 1.f);
	} COLLIDERDESCS;

	typedef struct tagOBBDesc
	{
		_float3		vCenter;
		_float3		vAlignAxis[3];
		_float3		vCenterAxis[3];
	}OBBDESC;

	typedef struct tagSphereDesc
	{
		_float3		Sphere_Center;
		_float		Sphere_Radius;
	}SPHEREDESC;


private:
	explicit CBasicCollider(ID3D11Device* pDevice, ID3D11DeviceContext*	pDeviceContext);
	explicit CBasicCollider(const CBasicCollider& rhs);
	virtual ~CBasicCollider() = default;
public:
	virtual HRESULT InitializePrototype(TYPE eType);
	virtual HRESULT InitializePrototype_Sphere(_fvector vCenter);
	virtual HRESULT Initialize(void* pArg) override;

public:
	_bool Update_State(_fmatrix TransformMatrix);
	_bool Collision_AABB(CBasicCollider* pTargetColCompute_OBBlider);
	_bool Collision_OBB(CBasicCollider* pTargetCollider);
	_bool Collision_Sphere(CBasicCollider* pTargetCollider);
	_bool Collision_OBBToReset(CBasicCollider* pMyCollider, CBasicCollider* pTargetCollider);
public:
	void Collision_CheckObj(list<OBJCOLLIDER>& _MyObj, list<OBJCOLLIDER>& _TargetObj);
	_bool Collision_OBB(CBasicCollider* pMyCollider, CBasicCollider* pTargetCollider);
	void Collision_PlayerToTarget(CBasicCollider* pMyCollider, list<OBJCOLLIDER>& _TargetObj);
	void CollisionWeaponeToTarget(list<OBJCOLLIDER>& pMyCollider, list<OBJCOLLIDER>& _TargetObj);
	void Collision_MonsterWeaponToPlayer(list<OBJCOLLIDER>& pMyCollider, list<OBJCOLLIDER>& _TargetObj);

public:
	void SetSTATE(STATES _state) { m_eState = _state; }
public:
	_int GetCollisionFlag() {
		return m_CollisionFlag;
	}
	_bool Get_isHit() { return m_isHit; }
	STATES GetSTATES() { return m_eState; }

public:
	HRESULT Render();
public:
	PROPERTY(GetSTATES, SetSTATE)	STATES p_States;
private:
	void SetupCollisionFlag(_bool tf);


private:
	STATES	m_eState = STATES::STATES_END;
	TYPE					m_eType = TYPE_END;
	COLLIDERDESCS			m_ColliderDesc;
	COLLSTATE				m_CollState = STATEEND;

	BoundingBox*			m_pBB = nullptr;
	BoundingSphere*			m_pSphere = nullptr;
	_bool					m_isCollision = false;
	_bool					m_isHit = false;
	_bool					m_bStartHit = false;
	_bool					m_bIsDownAttack = false;
	_bool					m_bIsDown = false;

	_float4x4				m_TransformMatrix;

	_float3					m_vMin, m_vMax;
	_float3					m_vPoint[8];
	_float3					m_vOriginPoint[8];
	_float3					m_vTempSize;
private:
	_int					m_CollisionFlag = COLLISIONTYPE::COLLSION_NONE;
	ID						m_CollisionType = ID::IDEND;
private:
	BasicEffect*			m_pEffect = nullptr;
	ID3D11InputLayout*		m_pInputLayOut = nullptr;

private:
	CGameObject*			m_pUserData = nullptr;
private:
	list<CGameObject*>	m_collsinlist;
private:
	DirectX::PrimitiveBatch<DirectX::VertexPositionColor>*				m_pBatch = nullptr;
	typedef DirectX::PrimitiveBatch<DirectX::VertexPositionColor>		BATCH;

private:
	_fmatrix Remove_ScaleRotation(_fmatrix TransformMatrix);
	_fmatrix Remove_Scale(_fmatrix TransformMatrix);

	OBBDESC Compute_OBB(_fvector* pPoints);

public:
	void Set_Radius(_float vRadius);

	_float3	GetSize() { return m_pBB->Extents; }
	void	SetSize(_float3 m_Size) { m_pBB->Extents = m_Size; m_vTempSize = m_Size; }

	_float3	GetPos() { return m_pBB->Center; }
	void	SetPos(_float3 m_Pos) { m_pBB->Center = m_Pos; }

	void SetType(TYPE mType) { m_eType = mType; }
	TYPE GetType() { return m_eType; }

public: //collsion
	void SetCollState(COLLSTATE _pCollState) { m_CollState = _pCollState; }
	COLLSTATE GetCollState() { return m_CollState; }
	void OBBOnEnter(CGameObject* pTarget);
	void OBBOnStay();
	void OBBOnExit();
	_bool	Get_Collision() { return m_isCollision; }
	void	Set_Collision(_bool pcollset) { m_isCollision = pcollset; }
	void	ClearList();
public:
	void SetIsAttachBone(_bool attach) { m_isAttachBone = attach; }
	void SetBoneName(string name) { m_strBoneName = name; }
	void SetOffset(_float3 offset) { m_Offset = offset; }
	void SetCollisionType(ID type) { m_CollisionType = type; }
	void SetHit(_bool pHit) { m_isHit = pHit; }
	void SetStartHit(_bool _bStartHIt) { m_bStartHit = _bStartHIt; }
	void SetIsDownAttack(_bool _bDownAttack) { m_bIsDownAttack = _bDownAttack; }
	void SetIsDown(_bool _bDown) { m_bIsDown = _bDown; }
public:
	_bool	GetIsAttachBone() { return m_isAttachBone; }
	_bool	GetStartHit() { return m_bStartHit; }
	_bool	GetIsDownAttack() { return m_bIsDownAttack; }
	_bool	GetIsDown() { return m_bIsDown; }
	ID		GetCollisionType() { return m_CollisionType; }
	_float3 GetOffset() { return m_Offset; }
	_vector GetVecOffset() { return XMLoadFloat3(&m_Offset); }
	string	GetBoneName() { return m_strBoneName; }
	_vector* GetObbBox();
	
private:
	_bool	m_isAttachBone = false;
	_float3	m_Offset;
	string	m_strBoneName;
public:
	static CBasicCollider* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, TYPE eType);
	static CBasicCollider* Create_Sphere(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, _fvector vCenter);
	virtual CComponent* Clone(void* pArg) override;
	virtual void Free() override;
};

END