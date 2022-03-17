#pragma once
#include "Client_Defines.h"
#include "Scene.h"

BEGIN(Client)
class CPortal;
class CScene_Stage01 :
	public CScene
{
private:
	explicit CScene_Stage01(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, _uint iLevelIndex);
	virtual ~CScene_Stage01() = default;
public:
	virtual HRESULT Initialize() override;
	virtual _uint Update(_double TimeDelta)  override;
	virtual HRESULT Render() override;
public:
	HRESULT ReadyLayerCamera(string pLayerTag);
	HRESULT ReadyScript();
	HRESULT ReadyLayerSky(string pLayerTag);
private:
	CPortal*	m_pPortal = nullptr;

public:
	static CScene_Stage01* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, _uint iLevelIndex);
	virtual void Free() override;
};

END