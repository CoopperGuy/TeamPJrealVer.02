#pragma once

#include "Component.h"
#include "Transform.h"
#include "VIBuffer.h"

BEGIN(Engine)

class ENGINE_DLL CCollider abstract : public CComponent
{
public:
	typedef struct tagRigidBody
	{
		_bool bEnabled;
		_bool bGravity;
		_bool bKinematic;
		_bool bCC;
	}RIGIDBODYDESC;

public:
	explicit CCollider(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	explicit CCollider(const CCollider& rhs);
	virtual ~CCollider() = default;
public:
	virtual CComponent* Clone(void* pArg) PURE;
	virtual void Free() override;
public:
	virtual HRESULT InitializePrototype();
	virtual HRESULT Initialize(void* pArg);
	virtual _uint	LateUpdate(double deltaTime);
	virtual HRESULT Render();

public:
	virtual HRESULT SetUpDebugLine() PURE;
	virtual void SetUpRigidActor(void* pShapeInfo, RIGIDBODYDESC desc, class CGameObject* obj) PURE;

public:
	RIGIDBODYDESC&	GetRigidBodyDesc() { return m_RigidBodyDesc; }
	PxRigidActor*	GetRigidActor() { return m_pRigidActor; }
	PxController* GetController() { return m_pController; }
	PxExtendedVec3 GetPosition() { 
		if (m_pController)
			return m_pController->getPosition();
		else
			return PxExtendedVec3(0.f, 0.f, 0.f);
	}
	_float3&	GetRelativePos() { return m_vRelativePos; }
	_float3	GetEuler();

public:
	void		SetRelativePos(_float3 center) { m_vRelativePos = center; }
	void		SetPosition(_float3 pos);
	void		SetEuler(_float3 euler);
	void		ReleaseController();
public:
	void		QuaternionToEuler(_float4 quaternion, _float3& euler);
	void		EulerToQuaternion(_float3 euler, _float4& quaternion);
protected:
	class CEngine*		m_pEngine;
protected:
	PxRigidActor*	m_pRigidActor = nullptr;
	PxController*	m_pController = nullptr;
	CComponent*		m_pDebugLine = nullptr;
	CTransform*		m_pObjTransform = nullptr;
protected:
	RIGIDBODYDESC	m_RigidBodyDesc;
	_float4x4		m_pxMat;
	_float4			m_quaternion;
	_float3			m_vRelativePos = { 0.f, 0.f, 0.f };
	_float3			m_preFramePosition;
	_float3			m_euler;
	_float3			m_Size;

	//protected:
	//	PxPhysics*					m_pPhysics;
	//	PxControllerManager*		m_pControllerManager;
};

END