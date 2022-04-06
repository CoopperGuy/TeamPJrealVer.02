#include "stdafx.h"
#include "..\Public\EventCheck.h"
#include "Npc_Shop.h"
#include "Player.h"
#include "AddQuickslot.h"
#include "Camera_Fly.h"
#include "BackPackHud.h"
#include "ReinforceHud.h"
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

void CEventCheck::SetMenus(CMenuHud * _menu)
{
	if (_menu)
		m_pMenus = _menu;
}

void CEventCheck::SetReinforceHud(CReinforceHud * _reinforce)
{
	if (_reinforce)
		m_pReinforceHud = _reinforce;
}

void CEventCheck::AddItemAtQuickSlot(CItem * item, _int idx)
{
	if (m_pPlayerScript)
		m_pPlayerScript->AddItemQuickSlot(item, idx);
}

void CEventCheck::AddDropItem()
{
	if (m_pPlayerScript)
	{
		_int _rand = rand() % 10;
		_int _idx = 0;
		if (_rand < 1)
			_idx = 2;
		else if (_rand < 7)
			_idx = 0;
		else
			_idx = 1;
		m_pPlayerScript->AddItem(_idx);

	}

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

void CEventCheck::ShakeUpDown(_int _cnt, _float _duration, _float _spd)
{
	if (m_pCameraFly)
		m_pCameraFly->ShakeUpDown(_cnt, _duration, _spd);
}

void CEventCheck::ZoomFov(_float _duration, _float _destFov, _float _spd)
{
	if (m_pCameraFly)
		m_pCameraFly->ZoomFov(_duration, _destFov, _spd);

}

void CEventCheck::SetUpEquip(string _equipName)
{
	if (m_pPlayerScript)
		m_pPlayerScript->SetUpEquip(_equipName);
}

void CEventCheck::SlowAttack()
{
	if (m_pPlayerScript)
		m_pPlayerScript->Set_SlowAttack(true);

}

void CEventCheck::OffAllMenus()
{
	if (m_pMenus)
		m_pMenus->OffAllMenus();
}

_bool CEventCheck::CameraEventCheck(_float3 _pos)
{
	if (m_pPlayerScript)
	{
		_float3 _playerPos = m_pPlayerScript->Get_CenterPosition();
		if (_playerPos.z > _pos.z)
			return true;
	}
	return false;
}

_bool CEventCheck::CameraEventCheckReverse(_float3 _pos)
{
	if (m_pPlayerScript)
	{
		_float3 _playerPos = m_pPlayerScript->Get_CenterPosition();
		if (_playerPos.z < _pos.z)
			return true;
	}
	return false;
}

void CEventCheck::SetUpReinforceItem(CItem * _item)
{
	if (m_pReinforceHud)
		m_pReinforceHud->SetUpReinforceItem(_item);
}

void CEventCheck::SetUpReinforceMaterial(CItem * _item)
{
	if (m_pReinforceHud)
		m_pReinforceHud->SetUpReinforceMaterial(_item);

}

void CEventCheck::RemoveItem(ITEMTYPE _type, _int _idx)
{
	if (m_pBackPackHud)
		m_pBackPackHud->RemoveItem(_type, _idx);
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

