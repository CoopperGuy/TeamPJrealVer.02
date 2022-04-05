#pragma once
#include "Scene.h"

BEGIN(Client)

class CScene_Stage4 :
	public CScene
{
private:
	explicit CScene_Stage4(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, _uint iLevelIndex);
	virtual ~CScene_Stage4() = default;
public:
	virtual HRESULT Initialize() override;
	virtual _uint Update(_double TimeDelta)  override;
	virtual HRESULT Render() override;
public:
	HRESULT ReadyScript();
public:
	static CScene_Stage4* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, _uint iLevelIndex);
	virtual void Free() override;

private:
	_double m_dMakeDropRock = 0;

	_uint makeRockNum = 0;
};

END