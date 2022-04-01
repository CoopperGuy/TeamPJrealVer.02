#pragma once
#include "Equipment.h"

BEGIN(Client)

class CHelmet : public CEquipment
{
public:
	explicit CHelmet();
	explicit CHelmet(CGameObject* pObj);
	virtual ~CHelmet() = default;

public:
	virtual HRESULT Initialize();
	virtual void Update(_double deltaTime);
	virtual void LateUpdate(_double deltaTime);
	virtual void Render();

public:
	static CHelmet* Create();
	virtual void Free() override;

};

END