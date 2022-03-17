#pragma once
#include "GameObject.h"

BEGIN(Engine)
class CTransform;
END

BEGIN(Client)

class CTestCar :
	public CGameObject
{
protected:
	explicit CTestCar(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	explicit CTestCar(const CTestCar& rhs);
	virtual ~CTestCar() = default;
public:
	virtual HRESULT InitializePrototype()override;
	virtual HRESULT Initialize(void* pArg)override;
	virtual _uint	Update(_double dDeltaTime)override;
	virtual _uint	LateUpdate(_double dDeltaTime)override;
	virtual HRESULT Render(_uint iPassIndex = 0)override;
private:
	CTransform*	m_pTransform = nullptr;
public:
	static CTestCar*	Create(CTestCar* obj);
	virtual CGameObject * Clone(void * pArg) override;
};

END