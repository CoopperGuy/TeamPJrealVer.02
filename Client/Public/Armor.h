#pragma once
#include "Equipment.h"

BEGIN(Client)

class CArmor : public CEquipment
{
public:
	explicit CArmor();
	explicit CArmor(CGameObject* pObj);
	virtual ~CArmor() = default;

public:
	virtual HRESULT Initialize();
	virtual void Update(_double deltaTime);
	virtual void LateUpdate(_double deltaTime);
	virtual void Render();

public:
	static CArmor* Create();
	virtual void Free() override;

};

END