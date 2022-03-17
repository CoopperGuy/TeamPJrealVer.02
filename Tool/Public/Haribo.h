#pragma once
#include "ToolCharacter.h"
//#include "StateMachine.h"

BEGIN(Tool)

class CHaribo : public CToolCharacter
{
private:
	explicit CHaribo();
	explicit CHaribo(CGameObject* pObj);
	virtual ~CHaribo() = default;

public:
	static CHaribo* Create(CGameObject* pObj);
	virtual void Free() override;

public:
	virtual void GetDamage(_vector sourceLocation) {}


private:
	void Set_Attach(_fmatrix WorldMatrix);
	_fmatrix Remove_ScaleRotation(_fmatrix TransformMatrix);
	_fmatrix Remove_OnlyScale(_fmatrix TransformMatrix);
	_fmatrix Remove_Scale(_fmatrix TransformMatrix);

public:
	HRESULT Initialize();
	virtual void Update(_double dDeltaTime);
	virtual void LateUpdate(_double dDeltaTime);
	virtual void Render();



};
END
