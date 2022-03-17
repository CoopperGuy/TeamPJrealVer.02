#pragma once
#include "Scene.h"

BEGIN(Client)

class CScene_Kim :
	public CScene
{
private:
	explicit CScene_Kim(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, _uint iLevelIndex);
	virtual ~CScene_Kim() = default;
public:
	virtual HRESULT Initialize() override;
	virtual _uint Update(_double TimeDelta)  override;
	virtual HRESULT Render() override;
public:
	HRESULT ReadyScript();
public:
	static CScene_Kim* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, _uint iLevelIndex);
	virtual void Free() override;
};

END