#pragma once
#include "Base.h"
#include "Camera_Fly.h"
BEGIN(Client)
class CPlayer;
class CNpc_Shop;
class CAddQuickslot;
class CItem;
class CBackPackHud;
class CMenuHud;
class CEventCheck :
	public CBase
{
public:
	enum BACKPACKSTATE{BACK_EQUIP,BACK_CONSUM,BACK_MATERIAL,BACK_END};
private:
	DECLARE_SINGLETON(CEventCheck);
public:
	CEventCheck();
	virtual ~CEventCheck() = default;
public:
	void SetPlayer(CPlayer* player);
	void SetAddQuick(CAddQuickslot* slot);
	void SetCameraFly(CCamera_Fly* camera);
	void SetBackPackHud(CBackPackHud* _backPack);
	void SetMenus(CMenuHud* _menu);
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
	void ShakeUpDown(_int _cnt, _float _duration, _float _spd = 1.f);
	void ZoomFov(_float _duration, _float _destFov, _float _spd);
public:
	void SetUpEquip(string _equipName);
public:
	void OffAllMenus();
public:
	void SetCurBackPack(BACKPACKSTATE _state) { m_eBackState = _state; }
	BACKPACKSTATE GetBackPackState() { return m_eBackState; }
	void RemoveItem(ITEMTYPE _type, _int _idx);
	void SetChangeScene(_bool _changeScene) { m_bChangeScene = _changeScene; }
	void SetSceneNumber(SCENE _scene) { m_eSceneNum = _scene; }
public:
	_float3 GetLockOnPos();
	_bool	GetChangeScene() { return m_bChangeScene; }
	SCENE	GetSceneNumber() { return m_eSceneNum; }
private:
	CPlayer*		m_pPlayerScript = nullptr;
	CAddQuickslot*	m_pAddQuick = nullptr;
	CCamera_Fly*	m_pCameraFly = nullptr;
	CBackPackHud*	m_pBackPackHud = nullptr;
	CMenuHud*		m_pMenus = nullptr;
private:
	BACKPACKSTATE	m_eBackState = BACKPACKSTATE::BACK_END;
	_bool			m_bChangeScene = false;
	SCENE			m_eSceneNum = SCENE_END;
public:
	virtual void Free() override;
};

END