#pragma once
#include "Base.h"

BEGIN(Engine)

class CTalk :
	public CBase
{
public:
	CTalk();
	virtual ~CTalk() = default;

public:
	virtual void Free() override;
};

END