#pragma once
#include "Scene.h"
#include "PathFinding.h"
#include "Inventory.h"
#ifdef _DEBUG
#endif
BEGIN(Client)

class CScene_Test final : public CScene
{
private:
	explicit CScene_Test(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, _uint iSceneIndex);
	virtual ~CScene_Test() = default;
public:
	static CScene_Test* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, _uint iSceneIndex);
	virtual void Free() override;

public:
	virtual HRESULT Initialize() override;
	virtual _uint Update(_double TimeDelta)  override;
	virtual HRESULT Render() override;
private:
	HRESULT ReadyLayerLights(string pLayerTag);
	HRESULT ReadyLayerCamera(string pLayerTag);
	HRESULT ReadyLayerGrid(string pLayerTag);
	HRESULT ReadyScript();
	HRESULT ReadyLayerSky(string pLayerTag);
private:
	void TestScriptHud();
	void TestScriptPlayer();
	void TestScriptOhter();
	

private:
	CPathFinding* m_pPathFinding = nullptr;
	CGameObject* m_pTrajectory = nullptr;
	CInventory*	m_pInven = nullptr;
#ifdef _DEBUG
	_double testDelta = 0;
	_double	testTime = 3;
	class CComboHud*	testHud = nullptr;
#endif
};

END