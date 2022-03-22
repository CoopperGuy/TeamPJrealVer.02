#include "stdafx.h"
#include "..\Public\EventCheck.h"
#include "Npc_Shop.h"
#include "Player.h"
#include "AddQuickslot.h"
#include "Camera_Fly.h"
USING(Client)

IMPLEMENT_SINGLETON(CEventCheck);

CEventCheck::CEventCheck()
{
}

void CEventCheck::SetPlayer(CPlayer * player)
{
	if(player)
		m_pPlayerScript = player;
}

void CEventCheck::SetAddQuick(CAddQuickslot * slot)
{
	if (slot)
		m_pAddQuick = slot;

}

void CEventCheck::SetCameraFly(CCamera_Fly * camera)
{
	if (camera)
		m_pCameraFly = camera;
}

void CEventCheck::SetBackPackHud(CBackPackHud * _backPack)
{
	if (_backPack)
		m_pBackPackHud = _backPack;
}

void CEventCheck::AddItemAtQuickSlot(CItem * item, _int idx)
{
	if (m_pPlayerScript)
		m_pPlayerScript->AddItemQuickSlot(item, idx);
}

void CEventCheck::ContractShop(CNpc_Shop* _shop)
{
	ITEMINFO item = _shop->GetBuyingItem();
	if (item.imageName == "")
		return;
	m_pPlayerScript->BuyItem(item.itemName,item);
}

void CEventCheck::isOpeningShop(_bool right)
{
	if (m_pPlayerScript) {
		m_pPlayerScript->Set_InvenRightLeft(right);
	}
}

void CEventCheck::OpenAddQuickSlot(_bool _open)
{
	if(m_pAddQuick)
		m_pAddQuick->SetIsOpen(_open);
}

void CEventCheck::SetAddQuickIcon(string _path, _int idx)
{
	if (m_pAddQuick)
		m_pAddQuick->UpdateIcon(_path, idx);
}

void CEventCheck::ShakeLook(_fvector look)
{
	if (m_pCameraFly)
		m_pCameraFly->ShakeLook(look,0.f);
}

void CEventCheck::ShakePosition(_fvector _pos, _float _duration)
{
	if (m_pCameraFly)
		m_pCameraFly->ShakePosition(_pos, _duration);

}

void CEventCheck::ShakeCamera(CCamera_Fly::SHAKE _shake, _int _cnt, _float _duration , _float _spd)
{
	if (m_pCameraFly)
		m_pCameraFly->ShakeCamera(_shake, _cnt, _duration, _spd);
}

void CEventCheck::SetUpEquip(string _equipName)
{
	if (m_pPlayerScript)
		m_pPlayerScript->SetUpEquip(_equipName);
}

_float3 CEventCheck::GetLockOnPos()
{
	if (m_pPlayerScript)
		return m_pPlayerScript->GetLockOnPosition();
	return _float3();
}



void CEventCheck::Free()
{
}

