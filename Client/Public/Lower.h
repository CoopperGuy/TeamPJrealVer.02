#pragma once
#include "Equipment.h"

BEGIN(Client)

class CLower : public CEquipment
{
public:
	explicit CLower();
	explicit CLower(CGameObject* pObj);
	virtual ~CLower() = default;

public:
	virtual HRESULT Initialize();
	virtual void Update(_double deltaTime);
	virtual void LateUpdate(_double deltaTime);
	virtual void Render();

public:
	static CLower* Create();
	virtual void Free() override;

};

END