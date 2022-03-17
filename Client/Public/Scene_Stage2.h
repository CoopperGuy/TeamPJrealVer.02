#pragma once
#include "Client_Defines.h"
#include "Scene.h"

BEGIN(Client)
class CPortal;
class CScene_Stage02 :
	public CScene
{
private:
	explicit CScene_Stage02(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, _uint iLevelIndex);
	virtual ~CScene_Stage02() = default;
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
	static CScene_Stage02* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, _uint iLevelIndex);
	virtual void Free() override;
};

END