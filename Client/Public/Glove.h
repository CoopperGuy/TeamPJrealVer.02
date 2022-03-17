#pragma once
#include "Equipment.h"

BEGIN(Client)

class CGlove : public CEquipment
{
public:
	explicit CGlove();
	explicit CGlove(CGameObject* pObj);
	virtual ~CGlove() = default;

public:
	virtual HRESULT Initialize();
	virtual void Update(_double deltaTime);
	virtual void LateUpdate(_double deltaTime);
	virtual void Render();

public:
	static CGlove* Create();
	virtual void Free() override;

};

END