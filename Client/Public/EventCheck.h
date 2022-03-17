#pragma once
#include "Base.h"
#include "Camera_Fly.h"
BEGIN(Client)
class CPlayer;
class CNpc_Shop;
class CAddQuickslot;
class CItem;
class CEventCheck :
	public CBase
{
	DECLARE_SINGLETON(CEventCheck);
public:
	CEventCheck();
	virtual ~CEventCheck() = default;
public:
	void SetPlayer(CPlayer* player);
	void SetAddQuick(CAddQuickslot* slot);
	void SetCameraFly(CCamera_Fly* camera);
public:
	void AddItemAtQuickSlot(CItem* item, _int idx);
public:
	void ContractShop(CNpc_Shop* _shop);
	void isOpeningShop(_bool right);
public:
	void OpenAddQuickSlot(_bool _open);
	void SetAddQuickIcon(string _path, _int idx);
public:
	void ShakeLook(_fvector look);
	void ShakePosition(_fvector _pos, _float _duration);
	void ShakeCamera(CCamera_Fly::SHAKE _shake, _int _cnt, _float _duration, _float _spd = 1.f);
private:
	CPlayer*	m_pPlayerScript = nullptr;
	CAddQuickslot*	m_pAddQuick = nullptr;
	CCamera_Fly*	m_pCameraFly = nullptr;
public:
	virtual void Free() override;
};

END