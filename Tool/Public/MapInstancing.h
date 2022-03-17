#pragma once
#include "ImGuiWindow.h"
#include "Gizmo.h"
#include "Navigation.h"
BEGIN(Tool)
class CMapInstancing : public CImGuiWindow
{
public:
	explicit CMapInstancing();
	virtual ~CMapInstancing() = default;
	virtual void Free() override;

public:
	virtual void Initialize() override;
	virtual void Update() override;
	virtual void LateUpdate() override;

private:
	_bool		m_bDrawActive = false;
	_float3 NaviMeshTemp[MAX_PATH];
	_int num = 0;

	vector<class CCell*>				Cells;
	_uint		m_iNumCells = 0;


	_int InstanceCount = 1;

private:
	class CGizmo*		m_pGizmo;
	class CMapInstancinggation*	pNavigation = nullptr;
	class CToolPlayer*	pPlayer = nullptr;


};
END