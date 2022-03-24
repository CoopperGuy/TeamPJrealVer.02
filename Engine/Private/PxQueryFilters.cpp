#include "EnginePCH.h"
#include "..\Public\PxQueryFilters.h"

USING(Engine)

PxQueryHitType::Enum CPxQueryFilters::preFilter(const PxFilterData & filterData, const PxShape * shape, const PxRigidActor * actor, PxHitFlags & queryFlags)
{
	if (actor == m_pActor)
		return PxQueryHitType::eNONE;
	if (filterData.word1 & filter || filterData.word2 & filter)
		return PxQueryHitType::eBLOCK;
	return PxQueryHitType::eNONE;
}

PxQueryHitType::Enum CPxQueryFilters::postFilter(const PxFilterData & filterData, const PxQueryHit & hit)
{
	return PxQueryHitType::Enum();
}

void CPxQueryFilters::Free()
{
}
