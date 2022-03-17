#pragma once
#include "Scene.h"

BEGIN(Client)

class CScene_SEO :
	public CScene
{
private:
	explicit CScene_SEO(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, _uint iLevelIndex);
	virtual ~CScene_SEO() = default;
public:
	virtual HRESULT Initialize() override;
	virtual _uint Update(_double TimeDelta)  override;
	virtual HRESULT Render() override;
public:
	HRESULT ReadyScript();
public:
	static CScene_SEO* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, _uint iLevelIndex);
	virtual void Free() override;
};

END