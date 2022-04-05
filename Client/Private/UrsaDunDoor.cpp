#include "stdafx.h"
#include "..\Public\UrsaDunDoor.h"
#include "PortalUI.h"
#include "Stat.h"
#include "Ursa.h"
#include "Wolf.h"
USING(Client)

CUrsaDunDoor::CUrsaDunDoor()
{
}

HRESULT CUrsaDunDoor::Initailze(CGameObject * pArg)
{
	m_pUrsaDunDoor = CEngine::GetInstance()->FindGameObjectWithName(CEngine::GetInstance()->GetCurSceneNumber(), "UrsaDoor");
	m_pPlayer = CEngine::GetInstance()->FindGameObjectWithName(SCENE_STATIC, "Player");
	m_pUrsa = CEngine::GetInstance()->FindGameObjectWithName(CEngine::GetInstance()->GetCurSceneNumber(), "Ursa");


	m_pTransform = static_cast<CTransform*>(m_pUrsaDunDoor->GetComponent("Com_Transform"));
	if (m_pUrsaDunDoor == nullptr || m_pPlayer == nullptr)
		return E_FAIL;

	pUrsaDunDoor = m_pTransform->GetState(CTransform::STATE_POSITION);

	doorY = XMVectorGetY(pUrsaDunDoor);

	UrsaStat = static_cast<CStat*>(m_pUrsa->GetComponent("Com_Stat"));

	return S_OK;
}

void CUrsaDunDoor::Update(_double deltaTime)
{

	if (m_bDead)
		return;

	static bool first = false;
	static bool second = false;

	if (UrsaStat->GetStatInfo().hp <= UrsaStat->GetStatInfo().maxHp / 1.5)
		first = true;

	if (UrsaStat->GetStatInfo().hp <= UrsaStat->GetStatInfo().maxHp / 3)
		second = true;

	if (CEngine::GetInstance()->Get_DIKDown(DIK_6))
		first = true;



	if (first && make == 0 || second&& make == 1)
	{
		make += 1;
		_float3 one = { -0.2f,2.f,7.3f };
		_float3 two = { 0.7f,2.f,7.3f };
		_float3 three = { 1.5f,2.f,7.3f };

		m_pWolf.emplace_back(CWolf::Create(nullptr, one));
		m_pWolf.emplace_back(CWolf::Create(nullptr, two));
		m_pWolf.emplace_back(CWolf::Create(nullptr, three));

		m_bOpenDoor = true;
		if (m_bCloseDoor)
			m_bCloseDoor = false;


		first = false;
		second = false;

	}
	first = false;
	second = false;

	if (m_bOpenDoor)
	{
		if (MaxHight >= XMVectorGetY(m_pTransform->GetState(CTransform::STATE_POSITION))) {
			CEngine::GetInstance()->PlaySoundW("DGDoor.mp3", CHANNELID::MAPOBJ);
			CEngine::GetInstance()->SetVolume(0.2f, CHANNELID::MAPOBJ);

			//cout << doorY << endl;

			doorY += 0.2f * (_float)deltaTime;
			m_pTransform->SetState(CTransform::STATE_POSITION, _vector{ XMVectorGetX(pUrsaDunDoor),doorY,XMVectorGetZ(pUrsaDunDoor) });

		}
		if (MaxHight <= XMVectorGetY(m_pTransform->GetState(CTransform::STATE_POSITION)))
		{
			if (m_pWolf.size() > 0) {
				for (auto Wolf : m_pWolf)
				{
					if (Wolf->dead())

						if (Wolf)
							Wolf->SetAtt();
				}
			}

			m_bCloseDoor = true;
		}
		closedoordt = 0;

	}

	if (m_bCloseDoor)
	{
		CEngine::GetInstance()->StopSound(CHANNELID::MAPOBJ);
		m_bOpenDoor = false;
		closedoordt += deltaTime;

		if (closedoordt >= 3.f)
		{
			if (0.f <= XMVectorGetY(m_pTransform->GetState(CTransform::STATE_POSITION))) {
				doorY -= 0.2f * (_float)deltaTime;
				m_pTransform->SetState(CTransform::STATE_POSITION, _vector{ XMVectorGetX(pUrsaDunDoor),doorY,XMVectorGetZ(pUrsaDunDoor) });

			}
		}
		if (m_pWolf.size() > 0) {
			for (auto Wolf : m_pWolf)
			{
				if (Wolf)
					Wolf->SetAtt();
			}
		}
	}
}

void CUrsaDunDoor::LateUpdate(_double deltaTime)
{

	auto iter = m_pWolf.begin();
	for (; iter != m_pWolf.end(); )
	{
		bool iEvent = (*iter)->IsDead();

		if (iEvent)
			iter = m_pWolf.erase(iter);

		else
			++iter;
	}


	if (UrsaStat->GetStatInfo().hp <= 0) {
		if (m_pWolf.size() > 0) {
			for (auto Wolf : m_pWolf)
			{
				if (Wolf)
				{
					Wolf->SetDead();
				}
				else
					Wolf->SetDead();
			}
		}
	}
	if (m_bDead)
	{
		this->SetDead();
		m_pUrsaDunDoor->SetDead();
	}
}


CUrsaDunDoor * CUrsaDunDoor::Create(CGameObject * pTarget)
{
	CUrsaDunDoor*	obj = new CUrsaDunDoor();
	if (FAILED(obj->Initailze(pTarget))) {
		SAFE_RELEASE(obj);
		return nullptr;
	}
	return obj;
}

void CUrsaDunDoor::Free()
{
	if (m_pWolf.size() > 0) {
	/*	for (auto& pair : m_pWolf)
		{
			if(pair)
				SafeRelease(pair);
		}*/
		m_pWolf.clear();
	}
}
