#pragma once
#include "IScriptObject.h"
#include "Engine.h"

BEGIN(Client)

class CItem;
class CPlayer;
class CQuickSlot :
	public IScriptObject
{
private:
	enum QUICK {QUICK_ICON, QUICK_BORDER,QUICK_END};
private:
	explicit CQuickSlot();
	virtual ~CQuickSlot() = default;
public:
	virtual HRESULT Initailze(CGameObject * pArg);
	virtual void Update(_double deltaTime) override;
	virtual void LateUpdate(_double deltaTime) override;
	virtual void Render();
public:
	void RegistItem(CItem* _item, _int idx);
	void UseItem(CPlayer* player);
private:
	_int	m_iMyIdx = -1;
	_float	m_fCDDelta = 0.f;
	_float	m_fCDTime = 20.f;
private:
	CItem*	m_CurItem = nullptr;
private:
	CEmptyUI*	m_pThis = nullptr;
	vector<CVIBuffer_RectUI*>	m_childVIBuffer;
public:
	static CQuickSlot* Create(CGameObject*	pTarget = nullptr);
	virtual void Free() override;
};

END