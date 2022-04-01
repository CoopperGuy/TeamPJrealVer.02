#pragma once
#include "IScriptObject.h"
#include "Engine.h"
BEGIN(Client)
class CItem;
class CReinforceHud :
	public IScriptObject
{
	enum REINFORCE{ REINFORCE_SLOT, REINFORCE_ITEMIMG, REINFORCE_MATE, REINFORCE_MATEIMG, REINFORCE_BUTTON, REINFORCE_NAME,REINFORCE_CLOSE, REINFORCE_EFFECT, REINFORCE_MATENAME,
		REINFORCE_END};
private:
	explicit CReinforceHud();
	virtual ~CReinforceHud() = default;
public:
	virtual HRESULT Initailze(CGameObject * pArg);
	virtual void Update(_double deltaTime) override;
	virtual void LateUpdate(_double deltaTime) override;
	virtual void Render();
public:
	void UpdateImage();
public:
	void SetUpReinforceItem(CItem*	_item);
	void SetUpReinforceMaterial(CItem*	_item);
public:
	void SetIsOnOff(_bool _isOnOff) { m_bIsOnOff = _isOnOff; }
public:
	_bool GetIsOnOff() {return m_bIsOnOff;}
private:
	void StartReinforce(_float _deltaTime);
private:
	vector<CEmptyUI*>	m_vecReinforceUIs;
	CEmptyUI*			m_pThisUI = nullptr;
	CItem*				m_pCurReinforceItem = nullptr;
	CItem*				m_pCurMaterial = nullptr;
private:
	_float				m_fReinforceDelta = 0.f;
	_float				m_fReinforceTime = 3.f;
private:
	_bool				m_bIsReinforceWhile = false;
	_bool				m_bIsOnOff = false;
public:
	static CReinforceHud* Create(CGameObject*	pTarget = nullptr);
	virtual void Free() override;
};

END