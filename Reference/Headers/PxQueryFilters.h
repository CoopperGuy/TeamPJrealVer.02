#pragma once
#include "Base.h"
BEGIN(Engine)

class ENGINE_DLL CPxQueryFilters : public PxQueryFilterCallback , public CBase
{
public:
	CPxQueryFilters(PxRigidActor* _actor, _uint _filter) :filter(_filter), m_pActor(_actor) {

	}
	virtual ~CPxQueryFilters() = default;
	virtual PxQueryHitType::Enum preFilter(const PxFilterData & filterData, const PxShape * shape, const PxRigidActor * actor, PxHitFlags & queryFlags);
	virtual PxQueryHitType::Enum postFilter(const PxFilterData & filterData, const PxQueryHit & hit);

private:
	_uint	filter = 0;
	physx::PxRigidActor*	m_pActor = nullptr;

public:
	virtual void Free() override;
};

END