#pragma once
#include "Base.h"
#include "IScriptObject.h"
BEGIN(Engine)

class CScriptObjectManager final : public CBase
{
	DECLARE_SINGLETON(CScriptObjectManager)
private:
	CScriptObjectManager();
	virtual ~CScriptObjectManager() = default;
	virtual void Free() override;
public:
	HRESULT ReserveManager(_uint iNumScenes);
	void Update(_double deltaTime);
	void LateUpdate(_double deltaTime);
	void ClearScriptObject(_uint iSceneNum);
public:
	IScriptObject * AddObject(IScriptObject* pObj, _uint iNumScenes);
private:
	list<IScriptObject*>*	m_ScriptObjects;
private:
	_uint m_iNumScenes = 0;

};

END