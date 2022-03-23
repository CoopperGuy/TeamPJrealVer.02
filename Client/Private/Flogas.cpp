#include "stdafx.h"
#include "..\Public\Flogas.h"
#include "StateMachine.h"
#include "BasicCollider.h"
#include "Transform.h"
#include "MonHp.h"
#include "InstanceEffectFire.h"
#include "El_Flogas.h"


#pragma region Effect
#include "EffectFly.h"
#include "EffectGrab.h"
#include "EffectPajang.h"
#include "EffectFlyLaser.h"
#include "EffectFireTwist.h"
#include "EffectSwordRing.h"
#include "EffectBlackhole.h"
#include "SlashWave.h"
#include "EffectMeteoFire.h"
#include "EffectMeteoTrail.h"
#include "EffectMagic.h"
#include "MeteoFireBall.h"
#include "Fire_explosion.h"
#pragma endregion

#include "Obb.h"
#include "WaterEA.h"

USING(Client)

CFlogas::CFlogas(CGameObject* pObj)
	: CEnemy(pObj)
{
}


CFlogas * CFlogas::Create(CGameObject * pObj, _float3 position)
{
	CFlogas*		pInstance = new CFlogas(pObj);

	if (FAILED(pInstance->Initialize(position)))
	{
		MSG_BOX("Failed to Create CFlogas");
		SafeRelease(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CFlogas::Free()
{
	__super::Free();
}

HRESULT CFlogas::Initialize(_float3 position)
{
	m_pGameObject = CEngine::GetInstance()->FindGameObjectWithName(CEngine::GetInstance()->GetCurSceneNumber(), "Flogas");
	if (m_pGameObject == nullptr)
		return E_FAIL;

	m_pTransform = static_cast<CTransform*>(m_pGameObject->GetComponent("Com_Transform"));
	m_pModel = static_cast<CModel*>(m_pGameObject->GetComponent("Com_Model"));
	m_pCollider = static_cast<CCollider*>(m_pGameObject->GetComponent("Com_Collider"));
	if (m_pCollider)
		m_pController = m_pCollider->GetController();
	m_pStat = static_cast<CStat*>(m_pGameObject->GetComponent("Com_Stat"));

	CGameObject* pTargetObj = CEngine::GetInstance()->FindGameObjectWithName(SCENE_STATIC, "Player");
	m_pTargetTransform = static_cast<CTransform*>(pTargetObj->GetComponent("Com_Transform"));

	m_pModel->SetAnimationLoop((_uint)Flogas::R_Slash, false);
	m_pModel->SetAnimationLoop((_uint)Flogas::L_Slash, false);
	m_pModel->SetAnimationLoop((_uint)Flogas::THRUST, false);
	m_pModel->SetAnimationLoop((_uint)Flogas::SWORDTHROWING_START, false);
	m_pModel->SetAnimationLoop((_uint)Flogas::SWORDTHROWING_END, false);
	m_pModel->SetAnimationLoop((_uint)Flogas::FIREWAVE, false);
	m_pModel->SetAnimationLoop((_uint)Flogas::FIREFIST, false);
	m_pModel->SetAnimationLoop((_uint)Flogas::FLYING, false);
	m_pModel->SetAnimationLoop((_uint)Flogas::FOOTHAMMER, false);
	m_pModel->SetAnimationLoop((_uint)Flogas::STUN, false);
	m_pModel->SetAnimationLoop((_uint)Flogas::DMG_F, false);
	m_pModel->SetAnimationLoop((_uint)Flogas::DMG_B, false);
	m_pModel->SetAnimationLoop((_uint)Flogas::DIE, false);
	m_pModel->SetAnimationLoop((_uint)Flogas::DEADBODY, false);
	m_pModel->SetAnimationLoop((_uint)Flogas::FLYING_START, false);
	m_pModel->SetAnimationLoop((_uint)Flogas::FLYING_END, false);
	m_pModel->SetAnimationLoop((_uint)Flogas::FLYING_END2, false);
	m_pModel->Set_ForceLoop((_uint)Flogas::FLYING_ING, true);
	m_eState = IDLE;
	m_pModel->SetUp_AnimationIndex(0);
	m_pMonHp = CMonHp::Create(m_pGameObject);
	XMStoreFloat3(&m_vCenterPos, m_pTransform->GetState(CTransform::STATE_POSITION));

	m_pElement[0] = CEl_Flogas::Create("El_Flogas01");
	m_pElement[1] = CEl_Flogas::Create("El_Flogas02");
	m_pElement[2] = CEl_Flogas::Create("El_Flogas03");
	m_pElement[3] = CEl_Flogas::Create("El_Flogas04");
	CEngine::GetInstance()->AddScriptObject(m_pElement[0], CEngine::GetInstance()->GetCurSceneNumber());
	CEngine::GetInstance()->AddScriptObject(m_pElement[1], CEngine::GetInstance()->GetCurSceneNumber());
	CEngine::GetInstance()->AddScriptObject(m_pElement[2], CEngine::GetInstance()->GetCurSceneNumber());
	CEngine::GetInstance()->AddScriptObject(m_pElement[3], CEngine::GetInstance()->GetCurSceneNumber());

	Create_Trail();

	return S_OK;
}

void CFlogas::Update(_double dDeltaTime)
{
	if (!m_pGameObject)
		return;

	if (!m_pGameObject->IsActive())
		return;
	//if (m_pStat->GetStatInfo().hp <= 0)
	//	m_pGameObject->SetDead();

	__super::Update(dDeltaTime);
	if (m_pStat->GetStatInfo().hp < m_pStat->GetStatInfo().maxHp)
	{

		m_bStartBattle = true;
		if (m_pStat->GetStatInfo().hp < m_pStat->GetStatInfo().maxHp * 0.5f) {
			m_bPhaseSecond = true;
			static bool makeEA = true;
			if (makeEA) {
				_float3 Leftpos = { -6.5f,0.3f,-1.f };
				_float3 Rightpos = { 6.5f,0.3f,-1.f };

				m_pWaterEA.emplace_back(CWaterEA::Create(nullptr, Leftpos));
				m_pWaterEA.emplace_back(CWaterEA::Create(nullptr, Rightpos));
				makeEA = false;
			}
		}
		
		if (m_pStat->GetStatInfo().hp <= 0)
		{
			m_bStartBattle = false;
			m_bDeadMotion = true;
		}
	}


	m_fDist = SetDistance();

	if (m_bStartBattle)
	{
		if (!m_bPhaseSecond)
			InCombat(dDeltaTime);
		else
			SecondCombat(dDeltaTime);
	}
	else
	{
		if (!m_bDeadMotion)
			m_pModel->SetUp_AnimationIndex(IDLE);
		else
		{
			Empty_queue();
			if (m_pMonHp)
			{
				m_pMonHp->SetRelease();
				m_pMonHp = nullptr;
			}
			m_eState = DIE;
			if (m_pModel->Get_isFinished(DIE))
				m_eState = DEADBODY;
		}

	}


	if (CEngine::GetInstance()->Get_DIKDown(DIK_P))
	{
		m_bStartBattle = true;
	}

	//if (CEngine::GetInstance()->Get_DIKDown(DIK_O))
	//{
	//	m_bPhaseSecond = true;
	//}
	//if (CEngine::GetInstance()->Get_DIKDown(DIK_I))
	//{
	//	m_bDeadMotion = true;
	//}


	//if (CEngine::GetInstance()->Get_DIKDown(DIK_NUMPAD9))
	//{
	//	m_eState = FIREFIST;
	//	m_bMakeEffect = true;
	//}



	if (CEngine::GetInstance()->Get_DIKDown(DIK_U))
	{
		CGameObject* pGameObject = CEngine::GetInstance()->AddGameObjectToPrefab(CEngine::GetInstance()->GetCurSceneNumber(), "Prototype_Effect_Fire_explosion", "Fire_explosion");
		CEngine::GetInstance()->AddScriptObject(CFire_explosion::Create((CEmptyEffect*)pGameObject, m_pGameObject), CEngine::GetInstance()->GetCurSceneNumber());
	}

	if (m_pCollider) {
		PxExtendedVec3 footpos = m_pCollider->GetController()->getFootPosition();
	}


	m_pModel->SetUp_AnimationIndex((_uint)m_eState);
	m_pStat->SetSTATE(m_eCurSTATES);

	//fall down
	/*PxControllerFilters filters;
	m_pController->move(PxVec3(0.0f, -0.1f, 0.f), 0.01f, PxF32(1.f / dDeltaTime), filters);*/

	if (m_pTrailBuffer)
	{
		//m_pTrailBuffer->SetIsActive(true);
		m_pTrailBuffer->Update(dDeltaTime, XMLoadFloat4x4(&m_wpBoneMatrix) * XMLoadFloat4x4(&m_pTransform->GetMatrix()));
	}
}

void CFlogas::LateUpdate(_double dDeltaTime)
{
	__super::LateUpdate(dDeltaTime);

	m_pModel->Play_Animation(dDeltaTime * m_dAniSpeed);

	OrganizeEffect(m_eState);

	if (m_bDeadMotion)
	{
		for (auto WaterEA : m_pWaterEA)
		{
			WaterEA->SetDead();
		}
	}
}

void CFlogas::Render()
{
}

void CFlogas::Create_Trail()
{
	m_pTrail = CEngine::GetInstance()->AddGameObject(SCENE_STATIC, "Prototype_EmptyEffect", "Flogas_Trail");
	if (m_pTrail == nullptr)
		return;

	CEmptyEffect* pEffect = static_cast<CEmptyEffect*>(m_pTrail);

	pEffect->SetPassIndex(3);
	pEffect->SetTexture("../../Assets/Textures/Effect/Diffuse/LV_ElRano_Object_SpermaPropB_E_LBR.dds", CEmptyEffect::TEXTURE_DIFFUSE);
	pEffect->SetTexture("../../Assets/Textures/Effect/Mask/Trun_FX_Trail02_Tex_HKJ.jpg", CEmptyEffect::TEXTURE_MASK);
	pEffect->SetTexture("../../Assets/Textures/Effect/Noise/Trail.dds", CEmptyEffect::TEXTURE_NOISE);

	pEffect->SetScrollSpeedX(_float3(0.5f, 0.5f, 0.f));
	pEffect->SetScrollSpeedY(_float3(0.f, 0.f, 0.f));
	pEffect->setDistortion(0, _float2(0.1f, 0.2f));
	pEffect->setDistortion(1, _float2(0.1f, 0.3f));
	pEffect->setDistortion(2, _float2(0.1f, 0.1f));
	pEffect->SetDistortionScale(4.f);
	pEffect->SetDistortionBias(1.f);
	XMStoreFloat4x4(&m_wpBoneMatrix, m_pModel->Get_BoneWithoutOffset("BN_WP_R"));
	_matrix WeaponTrans = XMLoadFloat4x4(&m_wpBoneMatrix) * XMLoadFloat4x4(&m_pTransform->GetMatrix());
	m_pTrail->AddComponent(0, "Prototype_VIBuffer_Trail", "Com_Trail", &WeaponTrans);
	m_pTrailBuffer = static_cast<CVIBuffer_Trail*>(m_pTrail->GetComponent("Com_Trail"));
}

void CFlogas::Set_TrailOnOff()
{
	switch (m_eState)
	{
	case Client::CFlogas::IDLE:
		break;
	case Client::CFlogas::WALK:
		break;
	case Client::CFlogas::RUN:
		break;
	case Client::CFlogas::R_Slash:
		break;
	case Client::CFlogas::L_Slash:
		break;
	case Client::CFlogas::THRUST:
		break;
	case Client::CFlogas::SWORDTHROWING_START:
		break;
	case Client::CFlogas::SWORDTHROWING_LOOP:
		break;
	case Client::CFlogas::SWORDTHROWING_END:
		break;
	case Client::CFlogas::FIREWAVE:
		break;
	case Client::CFlogas::FIREFIST:
		break;
	case Client::CFlogas::FLYING_END2:
		break;
	case Client::CFlogas::FLYING:
		break;
	case Client::CFlogas::FOOTHAMMER:
		break;
	case Client::CFlogas::STICKSWORD1:
		break;
	case Client::CFlogas::STICKSWORD2:
		break;
	case Client::CFlogas::STICKSWORD3:
		break;
	case Client::CFlogas::STICKSWORDCutScene:
		break;
	case Client::CFlogas::STUN:
		break;
	case Client::CFlogas::DMG_F:
		break;
	case Client::CFlogas::DMG_B:
		break;
	case Client::CFlogas::DIE:
		break;
	case Client::CFlogas::DEADBODY:
		break;
	case Client::CFlogas::FLYING_START:
		break;
	case Client::CFlogas::FLYING_ING:
		break;
	case Client::CFlogas::FLYING_END:
		break;
	case Client::CFlogas::Flogas_END:
		break;

	}
}

void CFlogas::Empty_queue()
{
	if (!m_QueState.empty())
	{
		_uint size = (_uint)m_QueState.size();
		for (_uint i = 0; i < size; ++i)
			m_QueState.pop();
	}
}

void CFlogas::InCombat(_double dDeltaTime)
{
	if (!m_bAtt && !m_bSpecialAtt && !m_bOverChase && m_iOnePatternCount < 3)
		Adjust_Dist(dDeltaTime);
	//if (m_bOverChase)
	//	OverChase(dDeltaTime);
	if (m_iOnePatternCount > 2)
	{
		if (OriginShift(dDeltaTime) && !m_bMeteor)
		{
			Empty_queue();
			if (m_QueState.empty())
			{
				m_QueState.push(FIREFIST);
				m_QueState.push(FLYING_END2);
				m_eState = m_QueState.front();
				m_QueState.pop();
				m_bMeteor = true;
			}
		}
		else if (m_bMeteor)
		{
			m_bMakeEffect = true;

			if (m_bDelay)
				m_dDelayTime += dDeltaTime;

			if (m_pModel->Get_isFinished(FIREFIST))
			{
				if (m_dDelayTime <= 0)
				{
					m_eState = m_QueState.front();
					m_QueState.pop();
				}
				m_bDelay = true;
			}

			if (m_dDelayTime > 3.5f)
			{
				m_bDelay = false;
				m_bCenter = false;
				m_bMeteor = false;
				m_dDelayTime = 0.f;
				m_iOnePatternCount = 0;
			}
		}
	}
	else if (m_bClose)
	{
		if (m_QueState.empty())
		{
			RandomPattern();
		}

		if (!m_bAtt)
		{
			m_eState = IDLE;

			m_pTransform->SetLook(XMVector3Normalize(m_pTargetTransform->GetState(CTransform::STATE_POSITION) - m_pTransform->GetState(CTransform::STATE_POSITION)));
			m_eState = m_QueState.front();
			m_QueState.pop();
			m_bAtt = true;
			m_dDelayTime = 0.f;
		}
		else
		{
			//공격 끝났을때
			if (m_pModel->Get_isFinished())
			{
				m_bAtt = false;
			}
		}
	}
	else if (m_bSpecialAtt)
		SpecialPattern(dDeltaTime);
	//if (m_bDelay)
	//{
	//	m_dDelayTime += dDeltaTime;
	//}
	if (!m_bSpecialAtt && m_iOnePatternCount < 3)
		EndState(m_eState, dDeltaTime);
}

void CFlogas::SecondCombat(_double dDeltaTime)
{
	_int Drawing = rand() % 100;
	if (m_bFly)
	{
		Flying(dDeltaTime);

		if (m_pModel->Get_isFinished())
		{
			if (m_pModel->Get_AnimIndex() == FLYING_END)
			{
				m_eState = FLYING_END2;
				m_bFly = false;
				m_dFlyTime = 0.f;
			}
		}
	}
	else
	{
		m_dAniSpeed = 1.f;
		if (m_iFlyingCount > 3)
		{
			m_bFly = true;
			m_iFlyingCount = 0;
			for (_uint i = 0; i < 4; ++i)
				m_pElement[i]->SetActive(true);
		}
		else if (m_QueState.empty())
		{
			if (m_eState != IDLE)
			{
				if (!m_pModel->Get_isFinished())
					return;
			}
			++m_iFlyingCount;

			m_eState = IDLE;

			if (Drawing > 50)
			{
				m_QueState.push(R_Slash);
				m_QueState.push(L_Slash);
			}
			else
			{
				m_QueState.push(L_Slash);
				m_QueState.push(R_Slash);
			}

			while (m_QueState.size() < 4)
			{
				_int iSecondDrawing = rand() % 100;

				/*	if (iSecondDrawing < 20)
					{
						if (m_QueState.back() != FIREFIST)
							m_QueState.push(FIREFIST);
						else
							continue;
					}*/

				if (iSecondDrawing < 40)
				{
					if (m_QueState.back() != FOOTHAMMER)
						m_QueState.push(FOOTHAMMER);
					else
						continue;
				}
				else if (iSecondDrawing < 90)
				{
					if (m_QueState.back() != FIREWAVE)
						m_QueState.push(FIREWAVE);
					else
						continue;
				}
				else if (iSecondDrawing < 95)
				{
					if (m_QueState.size() >= 3)
					{
						m_QueState.push(SWORDTHROWING_START);
						m_QueState.push(SWORDTHROWING_END);
					}
					else
						continue;
				}
			}
			m_bSecondAtt = true;
			m_dDelayTime = 0.f;
			m_dSpecialDelay = 0.f;
		}
		else if (m_bSecondAtt)
		{
			m_dDelayTime += dDeltaTime;
			if (m_dDelayTime > 1.5f + rand() % 5 * 0.5f)
			{
				if (m_dSpecialDelay <= 0 && m_pModel->Get_AnimIndex() != SWORDTHROWING_START)
				{
					m_pTransform->SetLook(XMVectorSetY(XMVector3Normalize(XMLoadFloat3(&m_vTargetToLook)), 0.f));
					m_eState = m_QueState.front();
					m_QueState.pop();
				}
				m_dSpecialDelay += dDeltaTime;

			}

			if (m_QueState.empty())
			{
				m_bSecondAtt = false;
				m_dDelayTime = 0.f;
			}
			if (m_pModel->Get_isFinished())
			{
				m_dSpecialDelay = 0.f;
				m_dDelayTime = 0.f;
			}

			EndState(m_eState, dDeltaTime);
		}
	}
}

void CFlogas::EndState(Flogas eState, _double dDeltaTime)
{

	if (m_pModel->Get_isFinished())
		m_bChanged = true;

	else
	{
		m_bChanged = false;
		m_dChangeTime = 0.f;
	}
	if (m_bChanged)
	{
		if (m_dChangeTime <= 0)
		{
			switch (eState)
			{
			case SWORDTHROWING_START:
				m_eState = m_QueState.front();
				m_QueState.pop();
				break;
			case SWORDTHROWING_END:
				if (!m_bPhaseSecond)
				{
					m_bAtt = false;
					m_eState = IDLE;
				}
				break;
			case FLYING_END:
				m_eState = FLYING_END2;
				break;
			case DIE:
				m_eState = DEADBODY;
				break;
			case DEADBODY:
				m_eState = DEADBODY;
				break;
			default:
				m_eState = IDLE;
				break;
			}
		}
		m_dChangeTime += dDeltaTime;
	}
}

void CFlogas::RandomPattern()
{
	_int Drawing = rand() % 100;

	if (m_QueState.empty())
	{
		++m_iOnePatternCount;

		if (Drawing > 50)
		{
			m_QueState.push(R_Slash);
			m_QueState.push(L_Slash);
		}
		else
		{

			m_QueState.push(R_Slash);
			m_QueState.push(THRUST);
		}
		/*if (Drawing <= 10)
		{
			m_QueState.push(FIREWAVE);
		}
		else
		{*/

		if (m_QueState.back() == THRUST)
			m_QueState.push(L_Slash);
		else
			m_QueState.push(THRUST);

	}
}


void CFlogas::Adjust_Dist(_double dDeltaTime)
{
	_int Drawing = rand() % 100;

	if (m_fDist >= 6.5f)
	{
		if (Drawing < 5)
		{
			m_pTransform->SetLook(XMVector3Normalize(m_pTargetTransform->GetState(CTransform::STATE_POSITION) - m_pTransform->GetState(CTransform::STATE_POSITION)));
			m_bSpecialAtt = true;
			m_bSpecialStart = true;
			m_bClose = false;
			m_bMove = false;
		}
		else
			m_bMove = true;
	}
	else if (m_fDist >= 1.f)
	{
		m_bClose = false;
		m_bMove = true;
	}
	else
	{
		m_bClose = true;
		m_bMove = false;
	}

	_vector vLook, vTargetLook;
	vLook = m_pTransform->GetState(CTransform::STATE_LOOK);
	vTargetLook = XMLoadFloat3(&m_vTargetToLook);
	PxVec3 vDir = PxVec3(0.f, 0.f, 0.f);
	PxControllerFilters filters;
	if (m_bMove)
	{

		/*	m_dChaseTime += dDeltaTime;
			if (m_dChaseTime > 3.f)
			{
				Empty_queue();
				m_QueState.push(FIREWAVE);
				m_bOverChase = true;
				m_bClose = false;
				m_bMove = false;
				m_dChaseTime = 0.f;

			}*/
		m_eState = RUN;
		/*if (m_fDist >= 1.3f)
			  m_eState = RUN;
		else
			m_eState = WALK;*/
		vLook = XMVectorLerp(vLook, vTargetLook, 0.5f);
		vLook = XMVectorSetY(vLook, 0.f);
		m_pTransform->SetLook(vLook);
		memcpy(&vDir, &vLook, sizeof(_float3));
		m_pController->move(vDir * 0.8f * dDeltaTime, 0.f, (_float)dDeltaTime, nullptr);
	}
}

void CFlogas::SpecialPattern(_double dDeltaTime)
{
	if (m_bSpecialStart)
	{
		Empty_queue();

		m_QueState.push(SWORDTHROWING_START);
		m_QueState.push(SWORDTHROWING_END);
		m_eState = m_QueState.front();
		m_QueState.pop();
		m_bSpecialStart = false;
	}


	if (m_pModel->Get_isFinished())
	{
		if (m_QueState.empty())
		{
			m_bSpecialAtt = false;
			return;
		}
		m_eState = m_QueState.front();
		m_QueState.pop();
	}
}

void CFlogas::OverChase(_double dDeltaTime)
{
	if (m_pModel->Get_isFinished())
	{
		if (m_QueState.empty())
		{
			m_bOverChase = false;
			m_dChangeTime = 0.f;
			return;
		}
	}
	if (m_dChangeTime <= 0.f)
	{
		if (!m_QueState.empty())
		{
			m_eState = m_QueState.front();
			m_QueState.pop();
		}
	}

	m_dChangeTime += dDeltaTime;

}

void CFlogas::Flying(_double dDeltaTime)
{
	if (OriginShift(dDeltaTime))
	{
		Empty_queue();


		_uint iCheck = m_pElement[0]->Get_Destination() + m_pElement[1]->Get_Destination() + m_pElement[2]->Get_Destination()
			+ m_pElement[3]->Get_Destination();

		if (iCheck >= 3)
		{
			m_eState = FLYING_END;
			for (_uint i = 0; i < 4; ++i)
				m_pElement[i]->SetExplosion(true);
			//m_bFly = false;
		}
		else
		{
			if (m_dFlyTime <= 0)
			{
				m_eState = FLYING_START;
				for (_uint i = 0; i < 4; ++i)
					m_pElement[i]->SetActive(true);
				m_bMakeEffect = true;
			}
			else if (m_pModel->Get_isFinished())
			{
				m_eState = FLYING_ING;
			}
			m_dFlyTime += dDeltaTime;
			if (m_eState == FLYING_ING)
				m_dAniSpeed = 0.5f;
		}
	}
}

_float CFlogas::SetDistance()
{
	_vector vTargetPos, vPos;
	vTargetPos = m_pTargetTransform->GetState(CTransform::STATE_POSITION);
	vPos = m_pTransform->GetState(CTransform::STATE_POSITION);
	XMStoreFloat3(&m_vTargetToLook, vTargetPos - vPos);
	return XMVectorGetX(XMVector3Length(vTargetPos - vPos));
}

_bool CFlogas::OriginShift(_double dDeltaTime)
{
	_vector vCenter;
	PxVec3 vDir = PxVec3(0.f, 0.f, 0.f);
	PxControllerFilters filters;
	vCenter = (XMLoadFloat3(&m_vCenterPos) - m_pTransform->GetState(CTransform::STATE_POSITION));
	_float Length = XMVectorGetX(XMVector3Length(vCenter));

	if (Length > 0.05f)
	{
		if (!m_bCenter)
		{
			m_bAtt = false;
			m_eState = RUN;
			vCenter = XMVectorSetY(vCenter, 0.f);
			m_pTransform->SetLook(vCenter);
			memcpy(&vDir, &XMVector3Normalize(vCenter), sizeof(_float3));
			m_pController->move(vDir * 1.f * (_float)dDeltaTime, 0.01f, (_float)dDeltaTime, nullptr);
			return false;
		}
	}
	else
	{
		m_pTransform->SetLook(XMLoadFloat3(&m_vTargetToLook));
		m_bCenter = true;
	}
	return true;
}

void CFlogas::OrganizeEffect(Flogas eState)
{
	//키프레임으로 이펙트
	_uint keyFrame = m_pModel->GetCurrentKeyFrame();
	_vector pos = m_pTransform->GetState(CTransform::STATE_POSITION);
	//m_pTrail->SetIsActive(false);
	m_DrawTrail = false;
	switch (eState)
	{
	case IDLE:
		m_iMakeMeteo = 0;
		break;
	case WALK:
		break;
	case RUN:
		break;
	case R_Slash:
	{
		if (keyFrame >= 30 && keyFrame <= 35) {
			/*CGameObject* pGameObject = CEngine::GetInstance()->AddGameObjectToPrefab(CEngine::GetInstance()->GetCurSceneNumber(), "Prototype_Effect_FireSlash", "E_FireSlash");
			CEngine::GetInstance()->AddScriptObject(CSlashWave::Create((CEmptyEffect*)pGameObject, m_pGameObject), CEngine::GetInstance()->GetCurSceneNumber());*/
			m_eCurSTATES = CStat::STATES_ATK;
			m_DrawTrail = true;
		}
		else
			m_eCurSTATES = CStat::STATES_IDEL;
		if (keyFrame == 28.f) {
			auto EffectRing = CEngine::GetInstance()->AddGameObjectToPrefab(CEngine::GetInstance()->GetCurSceneNumber(), "Prototype_Effect_SwordRing", "Effect_SwordRing");
			CEngine::GetInstance()->AddScriptObject(m_pEffSwordRing = CEffectSwordRing::Create(EffectRing), CEngine::GetInstance()->GetCurSceneNumber());
			m_pEffSwordRing->SetSlashR(false);
		}
	}
	break;
	case L_Slash:
	{
		if (keyFrame == 27)
		{
			CGameObject* pGameObject = CEngine::GetInstance()->AddGameObjectToPrefab(CEngine::GetInstance()->GetCurSceneNumber(), "Prototype_Effect_FireSlash", "E_FireSlash");
			CEngine::GetInstance()->AddScriptObject(CSlashWave::Create((CEmptyEffect*)pGameObject, m_pGameObject), CEngine::GetInstance()->GetCurSceneNumber());
		}
		if (keyFrame >= 29 && keyFrame <= 36) {
			m_eCurSTATES = CStat::STATES_ATK;
			m_DrawTrail = true;
		}
		else
			m_eCurSTATES = CStat::STATES_IDEL;
		/*if (keyFrame == 28.f) {
			auto EffectRing = CEngine::GetInstance()->AddGameObjectToPrefab(CEngine::GetInstance()->GetCurSceneNumber(), "Prototype_Effect_SwordRing", "Effect_SwordRing");
			CEngine::GetInstance()->AddScriptObject(m_pEffSwordRing = CEffectSwordRing::Create(EffectRing), CEngine::GetInstance()->GetCurSceneNumber());
			m_pEffSwordRing->SetSlashR(true);
		}*/
		break;
	}
	case THRUST:
		if (keyFrame >= 38 && keyFrame <= 60) {
			m_eCurSTATES = CStat::STATES_ATK;
			m_DrawTrail = true;
		}
		else
			m_eCurSTATES = CStat::STATES_IDEL;
		break;
	case SWORDTHROWING_START:
		if (keyFrame >= 82 && keyFrame <= 116) {
			m_eCurSTATES = CStat::STATES_ATK;
			m_DrawTrail = true;
		}
		break;
	case SWORDTHROWING_LOOP:
		m_eCurSTATES = CStat::STATES_IDEL;
		break;
	case SWORDTHROWING_END:
		if (keyFrame >= 64 && keyFrame <= 79) {
			m_eCurSTATES = CStat::STATES_ATK;
			m_DrawTrail = true;
		}
		else
			m_eCurSTATES = CStat::STATES_IDEL;
		break;
	case FIREWAVE: {
		_int keyFrame = m_pModel->GetCurrentKeyFrame();
		static bool make = true;
		if (keyFrame >= 25 && keyFrame <= 26) {
			if (make) {
				auto EffectBlackhole = CEngine::GetInstance()->AddGameObjectToPrefab(CEngine::GetInstance()->GetCurSceneNumber(), "Prototype_Effect_Blackhole", "Effect_Blackhole");
				CEngine::GetInstance()->AddScriptObject(m_pEffBlackhole = CEffectBlackhole::Create(EffectBlackhole), CEngine::GetInstance()->GetCurSceneNumber());
				make = false;
			}
		}

		if (keyFrame == 124.f)
		{
			CGameObject* EffectPajang = CEngine::GetInstance()->AddGameObjectToPrefab(CEngine::GetInstance()->GetCurSceneNumber(), "Prototype_Effect_Pajang", "Effect_Pajang");
			CEngine::GetInstance()->AddScriptObject(m_pEffPajang = CEffectPajang::Create(EffectPajang), CEngine::GetInstance()->GetCurSceneNumber());
			make = true;
		}
		break;
	}
	case FIREFIST: {
		if (keyFrame == 50) {
			//내려찍기 
			if (m_bMakeEffect) {
				auto EffectTrail = CEngine::GetInstance()->AddGameObjectToPrefab(CEngine::GetInstance()->GetCurSceneNumber(), "Prototype_Effect_MeteoTrail", "E_MeteoTrail");
				CEngine::GetInstance()->AddScriptObject(CEffectMeteoTrail::Create(EffectTrail), CEngine::GetInstance()->GetCurSceneNumber());
				auto EffectFire = CEngine::GetInstance()->AddGameObjectToPrefab(CEngine::GetInstance()->GetCurSceneNumber(), "Prototype_Effect_MeteoFire", "E_MeteoFire");
				CEngine::GetInstance()->AddScriptObject(CEffectMeteoFire::Create(EffectFire, pos), CEngine::GetInstance()->GetCurSceneNumber());
				m_bMakeEffect = false;
			}
		}
		if (keyFrame >= 51 && keyFrame <= 70)
		{

			if (m_iMakeMeteo <= 8)
			{
				auto EffectMagic = CEngine::GetInstance()->AddGameObjectToPrefab(CEngine::GetInstance()->GetCurSceneNumber(), "Prototype_Effect_MeteoDropArea", "E_MeteoDropArea");
				CEngine::GetInstance()->AddScriptObject(CEffectMagic::Create(EffectMagic, pos), CEngine::GetInstance()->GetCurSceneNumber());

				/*auto Meteo = CEngine::GetInstance()->AddGameObjectToPrefab(CEngine::GetInstance()->GetCurSceneNumber(), "Prototype_GameObecjt_MeteoOBB", "O_MeteoOBB");
				CEngine::GetInstance()->AddScriptObject(CMeteoFireBall::Create(Meteo, pos), CEngine::GetInstance()->GetCurSceneNumber());*/
				m_iMakeMeteo += 1;
				/*auto EffectDropArea = CEngine::GetInstance()->AddGameObjectToPrefab(CEngine::GetInstance()->GetCurSceneNumber(), "Prototype_Effect_MeteoDropArea", "E_MeteoDropArea");
				CEngine::GetInstance()->AddScriptObject(CEffectMagic::Create(EffectDropArea, pos), CEngine::GetInstance()->GetCurSceneNumber());*/
			}
		}
	}
				   break;
	case FOOTHAMMER:
		m_eCurSTATES = CStat::STATES_IDEL;
		break;
	case STUN:
		break;
	case DMG_F:
		break;
	case DMG_B:
		break;
	case FLYING_START: {
		if (keyFrame == 30.f) {
			if (m_bMakeEffect) {
				auto EffectFly = CEngine::GetInstance()->AddGameObjectToPrefab(CEngine::GetInstance()->GetCurSceneNumber(), "Prototype_Effect_BossFly", "Effect_BossFly");
				CEngine::GetInstance()->AddScriptObject(m_pEffFly = CEffectFly::Create(EffectFly), CEngine::GetInstance()->GetCurSceneNumber());
				auto EffectFlyLaser = CEngine::GetInstance()->AddGameObjectToPrefab(CEngine::GetInstance()->GetCurSceneNumber(), "Prototype_Effect_BossFlyLaser", "Effect_BossFlyLaser");
				CEngine::GetInstance()->AddScriptObject(m_pEffFlyLaser = CEffectFlyLaser::Create(EffectFlyLaser), CEngine::GetInstance()->GetCurSceneNumber());
				m_bMakeEffect = false;
			}

		}
		break;
	}
	case FLYING_ING:
		break;
	case FLYING_END:
		if (m_pEffFly)
			m_pEffFly->SetAnimationEnd();
		if (m_pEffFlyLaser)
			m_pEffFlyLaser->SetDead();
		break;
	case FLYING_END2:
		break;
	}
	m_pTrailBuffer->SetIsActive(m_DrawTrail);
}



