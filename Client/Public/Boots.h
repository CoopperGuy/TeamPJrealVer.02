#pragma once
#include "Equipment.h"

BEGIN(Client)

class CBoots : public CEquipment
{
public:
	explicit CBoots();
	explicit CBoots(CGameObject* pObj);
	virtual ~CBoots() = default;

public:
	virtual HRESULT Initialize();
	virtual void Update(_double deltaTime);
	virtual void LateUpdate(_double deltaTime);
	virtual void Render();

public:
	static CBoots* Create();
	virtual void Free() override;

};

END