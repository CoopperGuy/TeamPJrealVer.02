#include "stdafx.h"
#include "..\Public\BasicEffect.h"

USING(Client)

CBasicEffect::CBasicEffect()
{
}

CBasicEffect::CBasicEffect(CGameObject * pObj)
	: m_pGameObject(pObj)
{
}

CBasicEffect * CBasicEffect::Create(CGameObject * pArg)
{
	return nullptr;
}


HRESULT CBasicEffect::Initialize(CGameObject * pArg)
{
	return S_OK;
}

void CBasicEffect::Update(_double deltaTime)
{
}

void CBasicEffect::LateUpdate(_double deltaTime)
{
}


void CBasicEffect::Free()
{
}
