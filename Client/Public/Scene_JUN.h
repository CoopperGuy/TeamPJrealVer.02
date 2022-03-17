#pragma once
#include "Scene.h"

BEGIN(Client)

class CScene_JUN :
	public CScene
{
private:
	explicit CScene_JUN(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, _uint iLevelIndex);
	virtual ~CScene_JUN() = default;
public:
	virtual HRESULT Initialize() override;
	virtual _uint Update(_double TimeDelta)  override;
	virtual HRESULT Render() override;
public:
	HRESULT ReadyScript();

public:
	static CScene_JUN* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, _uint iLevelIndex);
	virtual void Free() override;
};

END