#include "stdafx.h"
#include "..\Public\SoundHud.h"

USING(Client)

CSoundHud::CSoundHud()
{
}

HRESULT CSoundHud::Initailze(CGameObject * pArg)
{
	CEngine*	pEngine = CEngine::GetInstance();
	m_vecUIs.reserve(SOUNDS::SOUNDS_END);
	m_pThisUI =static_cast<CEmptyUI*>(pEngine->FindGameObjectWithName(0,"SoundUI"));
	list<CGameObject*>	child = m_pThisUI->GetChildren();
	for (auto& iter : child)
		m_vecUIs.emplace_back(static_cast<CEmptyUI*>(iter));
	m_Positions.first = static_cast<CEmptyUI*>(pEngine->FindGameObjectWithName(0, "SoundMinPos"))->GetPosition().x;
	m_Positions.second = static_cast<CEmptyUI*>(pEngine->FindGameObjectWithName(0, "SoundMaxPos"))->GetPosition().x;

	return S_OK;
}

void CSoundHud::Update(_double deltaTime)
{
	if(m_pThisUI->IsActive())
	{
		if (m_vecUIs[SOUNDS::SOUNDS_BGMBLOCK]->IsHovered()) {
			if (CEngine::GetInstance()->Get_MouseButtonState(CInput_Device::MOUSEBUTTONSTATE::MBS_LBUTTON))
			{
			}
		}
		if (CEngine::GetInstance()->Get_MouseButtonStateUp(CInput_Device::MOUSEBUTTONSTATE::MBS_LBUTTON)) 
		{
			m_bIsDraged = false;
		}
		if (m_bIsDraged)
		{
			if (CEngine::GetInstance()->Get_MouseButtonState(CInput_Device::MOUSEBUTTONSTATE::MBS_LBUTTON))
				m_bIsDraged = true;
			else 
				m_bIsDraged = false;
			POINT		ptMouse;
			GetCursorPos(&ptMouse);
			ScreenToClient(g_hWnd, &ptMouse);

			if (ptMouse.x > m_Positions.first && ptMouse.x < m_Positions.second) 
			{
				_float2 _blockOffset = m_vecUIs[SOUNDS::SOUNDS_BGMBLOCK]->GetTransformOffst();
				_float _center = _float(WINCX >> 1);
				_float	_gap = ptMouse.x - _center;
				m_vecUIs[SOUNDS::SOUNDS_BGMBLOCK]->SetTransformOffst(_gap, _blockOffset.y);

			}
		}
		
	}
	
	_float _bgmVol = CEngine::GetInstance()->GetBGMVolume();
	_float _blockPos = static_cast<CEmptyUI*>(m_vecUIs[SOUNDS::SOUNDS_BGMBLOCK])->GetPosition().x;
	_float _x0 = m_Positions.first;
	_float _x1 = m_Positions.second;
	_float _t = (_blockPos - _x0) / (_x1 - _x0);
	CEngine::GetInstance()->SetBGMVolume(_t);
}

void CSoundHud::LateUpdate(_double deltaTime)
{
}

void CSoundHud::Render()
{
}

CSoundHud * CSoundHud::Create(CGameObject * pTarget)
{
	CSoundHud*	obj = new CSoundHud();
	if (FAILED(obj->Initailze(pTarget))) {
		SAFE_RELEASE(obj);
		return nullptr;
	}
	return obj;
}

void CSoundHud::Free()
{
}
