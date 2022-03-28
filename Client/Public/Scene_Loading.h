#pragma once

#include "Client_Defines.h"
#include "Scene.h"

BEGIN(Client)

class CScene_Loading final : public CScene
{
private:
	explicit CScene_Loading(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, _uint iLevelIndex);
	virtual ~CScene_Loading() = default;
public:
	virtual HRESULT Initialize(SCENE eScene);
	virtual _uint Update(_double TimeDelta)  override;
	virtual HRESULT Render() override;
	virtual void StartLoader();
private:
	class CLoader*			m_pLoader = nullptr;
private:
	SCENE					m_eNextSCENE;
public:
	static CScene_Loading* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, SCENE eScene, _uint iSceneIndex);
	virtual void Free() override;
};

END