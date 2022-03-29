#include "stdafx.h"
#include "..\Public\SpriteFire.h"
#include "SpriteBomb.h"
#include "El_Flogas.h"

USING(Client)

CSpriteFire::CSpriteFire()
{
}

HRESULT CSpriteFire::Initialize(CEmptyEffect* pThis, CGameObject* pTarget, CEl_Flogas* pEl)
{
	m_pThis = pThis;
	m_pEl = pEl;
	CTransform* pTargetTrans = static_cast<CTransform*>(pTarget->GetComponent("Com_Transform"));
	m_pEffectTrans = static_cast<CTransform*>(m_pThis->GetComponent("Com_Transform"));

	_vector vTargetPos = pTargetTrans->GetState(CTransform::STATE_POSITION);
	vTargetPos = XMVectorSetY(vTargetPos, -0.1f);
	m_pEffectTrans->SetState(CTransform::STATE_POSITION, vTargetPos);
	m_fScale = m_pEffectTrans->GetScale(CTransform::STATE_RIGHT);

	return S_OK;
}

void CSpriteFire::Update(_double dDeltaTime)
{
	/*m_pEffectTrans->SetScale(_float3(m_fScale, m_fScale, 1.f));
	m_fScale += (_float)dDeltaTime * 0.4f;*/
}

void CSpriteFire::LateUpdate(_double dDeltaTime)
{
	_float Total = (_float)m_pThis->GetSpriteTotal() * 0.5f;
	_vector vPosition = XMVectorZero();
	if (m_pThis->GetSpriteNum() > Total)
	{	
		_int Z = 1;
		_int X = 1;

		if(m_dOnetime == 0)
		{
			/*for (_int i = 0; i < 5; ++i)
			{
				for (_int j = 0; j < 5; ++j)
				{
					CGameObject* pGameObject = CEngine::GetInstance()->AddGameObjectToPrefab(CEngine::GetInstance()->GetCurSceneNumber(), "Prototype_E_Bomb_Sprite", "E_Bomb_Sprite");
					if (m_pEl->Get_Right())
						X = -1;
					if (!m_pEl->Get_Front())
						Z = -1;
					vPosition = XMVectorSetX(vPosition, j * X);
					vPosition = XMVectorSetZ(vPosition, i * Z);
						CEngine::GetInstance()->AddScriptObject(CSpriteBomb::Create((CEmptyEffect*)pGameObject, m_pThis, vPosition), CEngine::GetInstance()->GetCurSceneNumber());
					
				}
			}*/

			CGameObject* pGameObject = CEngine::GetInstance()->AddGameObjectToPrefab(CEngine::GetInstance()->GetCurSceneNumber(), "Prototype_E_Bomb_Sprite", "E_Bomb_Sprite");
			CEngine::GetInstance()->AddScriptObject(CSpriteBomb::Create((CEmptyEffect*)pGameObject, m_pThis), CEngine::GetInstance()->GetCurSceneNumber());
		}
		m_dOnetime += dDeltaTime;
	}
	if (m_pThis->GetSpriteEnd()) 
	{
		this->SetDead();
		m_pThis->SetDead();
	}
}



CBasicEffect * CSpriteFire::Create(CEmptyEffect* pThis, CGameObject* pTarget, CEl_Flogas* pEl)
{
	CSpriteFire*		pInstance = new CSpriteFire();

	if (FAILED(pInstance->Initialize(pThis, pTarget, pEl)))
	{
		MSG_BOX("Failed to Create CSpriteFire");
		SafeRelease(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CSpriteFire::Free()
{
}
