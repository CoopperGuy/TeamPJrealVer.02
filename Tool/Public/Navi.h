#pragma once
#include "ImGuiWindow.h"
#include "Gizmo.h"
#include "Navigation.h"
BEGIN(Tool)
class CNavi : public CImGuiWindow
{
public:
	explicit CNavi();
	virtual ~CNavi() = default;
	virtual void Free() override;

public:
	virtual void Initialize() override;
	virtual void Update() override;
	virtual void LateUpdate() override;


private:
	HRESULT Navigation_Save();
	HRESULT Navigation_Load();
	HRESULT Delete_All ();
	HRESULT Delete_Last();

private:
	void DrawNavi();


private:
	_bool		m_bDrawActive = false;
	_float3 NaviMeshTemp[MAX_PATH];
	_int num = 0;

	vector<class CCell*>				Cells;
	_uint		m_iNumCells = 0;
private:
	class CGizmo*		m_pGizmo;
	class CNavigation*	pNavigation = nullptr;
	class CToolPlayer*	pPlayer = nullptr;


};
END