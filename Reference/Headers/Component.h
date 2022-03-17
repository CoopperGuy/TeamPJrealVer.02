#pragma once

#include "Base.h"
BEGIN(Engine)
class CGameObject;

class ENGINE_DLL CComponent abstract : public CBase
{
protected:
	explicit CComponent(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	explicit CComponent(const CComponent& rhs);
	virtual ~CComponent() = default;

public:
	virtual HRESULT InitializePrototype();
	virtual HRESULT Initialize(void* pArg);
public:
	virtual void SetMaster(CGameObject* master) { m_pMaster = master; }

protected:
	ID3D11Device*			m_pDevice = nullptr;
	ID3D11DeviceContext*	m_pDeviceContext = nullptr;

protected:
	_bool			m_isCloned = false;
	CGameObject*	m_pMaster = nullptr;
public:
	virtual CComponent* Clone(void* pArg) = 0;
	virtual void Free() override;
};

END