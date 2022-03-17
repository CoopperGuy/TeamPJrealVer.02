#include "stdafx.h"
#include "..\Public\ElfNPC.h"
#include "QuestData.h"
BEGIN(Client)
void CreateTalkDest(CElfNPC* thisUI) {
	thisUI->SetTalkDesk(static_cast<CEmptyUI*>(CEngine::GetInstance()->SpawnPrefab("U_TalkDesk")));
}
void CreateTalkBox(CElfNPC* thisUI) {
	thisUI->SetTalkBox(static_cast<CEmptyUI*>(CEngine::GetInstance()->SpawnPrefab("U_TalkBox")));
}
END

USING(Client)

CElfNPC::CElfNPC()
{
}

CElfNPC::CElfNPC(CGameObject * pObj)
	:CNPC(pObj)
{
}

HRESULT CElfNPC::Initailze(CGameObject * pArg)
{
	if (FAILED(__super::Initailze(pArg))) {
		return E_FAIL;
	}
	if (pArg) {/*
		m_pTarget = static_cast<CEmptyGameObject*>(pArg);
		SafeAddRef(m_pTarget);*/
		m_pTarget = static_cast<CEmptyGameObject*>(CEngine::GetInstance()->FindGameObjectWithName(CEngine::GetInstance()->GetCurSceneNumber(),"Npc_Elf"));
		m_pModel =static_cast<CModel*>(m_pTarget->GetComponent("Com_Model"));
		m_pThisTransform = static_cast<CTransform*>(m_pTarget->GetComponent("Com_Transform"));

		m_tNPCTag = static_cast<CNPCInfo*>(m_pTarget->GetComponent("Com_NPCInfo"))->GetNPCInfo();

		m_pAlretUI = static_cast<CEmptyGameObject*>(CEngine::GetInstance()->SpawnPrefab("O_NPC_F"));
		_vector pos = XMVector3TransformCoord(XMVectorSet(0.f, 0.5f, 0.f, 0.f), m_pThisTransform->GetWorldMatrix());
		_float3 float3Pos;	
		XMStoreFloat3(&float3Pos, pos);
		m_pAlretUI->SetPosition(float3Pos);
	}
	CEngine::GetInstance()->AddScriptObject(this, CEngine::GetInstance()->GetCurSceneNumber());
	return S_OK;
}

void CElfNPC::Update(_double deltaTime)
{
	__super::Update(deltaTime);

	this->CheckAlret();
	this->MakeTalkDesk(deltaTime);
}

void CElfNPC::LateUpdate(_double deltaTime)
{
	__super::LateUpdate(deltaTime);
	this->BoxCheck();
	if (m_bisEndTalk) {
		if(m_pTalkDesk)
			m_pTalkDesk->SetDead();
		if(m_pTalkText)
			m_pTalkText->SetDead();
		if (m_pTalkBox)
			m_pTalkBox->SetDead();

		m_pTalkText = nullptr;
		m_pText = nullptr;
		m_pTalkBox = nullptr;
		m_pBoxText = nullptr;

		m_bisTalkBoxCreate = false;
		m_bisTalkBoxCreate = false;
		m_bisTalkCreate = false;
		m_bisEndTalk = false;
	}
}

void CElfNPC::SetTalkDesk(CEmptyUI * m_pUI)
{
	m_pUI->SetActive(false);
	m_pTalkDesk = m_pUI;
	list<CGameObject*> child = m_pTalkDesk->GetChildren();
	m_pTalkText = static_cast<CEmptyUI*>(child.front());
	m_pText = static_cast<CText*>(m_pTalkText->GetComponent("Com_Text"));
	m_bisTalkCreate = true;
}

void CElfNPC::SetTalkBox(CEmptyUI * m_pUI)
{
	m_pUI->SetActive(false);
	m_pTalkBox = m_pUI;
	list<CGameObject*> child = m_pTalkBox->GetChildren();
	m_pBoxText = static_cast<CText*>(static_cast<CEmptyUI*>(child.front())->GetComponent("Com_Text"));
	m_bisTalkBoxCreate = true;
}

void CElfNPC::CheckAlret()
{
	//Press F
	_float distance;
	if (m_pThisTransform) {
		distance = __super::CalculatePlayerDistance(m_pThisTransform);
	}

	if (distance < m_fAlretMakeDistance) {
		if (m_pAlretUI) {
			m_pAlretUI->SetActive(true);
		}
		if (CEngine::GetInstance()->IsKeyDown('F')) {
			if (!m_bisTalkCreate) {
				std::thread createDesk(CreateTalkDest, this);
				createDesk.detach();
				m_iCurTalkProgress = 0;
			}
		}

	}
	else {
		if (m_pAlretUI) {
			m_pAlretUI->SetActive(false);
		}
	}
}

void CElfNPC::MakeTalkDesk(_double deltaTime)
{
	if (m_bisTalkCreate) {
		if (m_pModel) {
			m_pModel->SetAnimationLoop(1, true);
			m_pModel->Play_Animation(deltaTime);
		}
		CQuestData* questData = CEngine::GetInstance()->GetQuestData(m_tNPCTag.hasQuestID);
		_int talkID = questData->questProgress + m_tNPCTag.hasQuestID + m_tNPCTag.npcID;
		//이미 퀘스트 중이거나 클리어 했을 경우
		if (CEngine::GetInstance()->SearchAcceptQuest(m_tNPCTag.hasQuestID) || CEngine::GetInstance()->SearchClearQuest(m_tNPCTag.hasQuestID)) {
			if (questData->isQuestClear() || !questData->isQuestAcheive()) {
				talkID = m_tNPCTag.npcID;
				NormalTalk(talkID);
			}
			else {
				QuestTalk(talkID);
			}
		}
		else {
			QuestTalk(talkID);
		}
	}
}

void CElfNPC::MakeTalkBox(_double deltaTime)
{
	std::thread createBox(CreateTalkBox, this);
	createBox.detach();
}

void CElfNPC::BoxCheck()
{
	if (m_bisTalkBoxCreate) {
		m_pTalkBox->SetActive(true);
		_int boxIndex = m_tNPCTag.boxID + m_tNPCTag.npcID;
		BoxTalk(boxIndex);
		if (m_pTalkBox->IsHovered()) {
			if (CEngine::GetInstance()->IsMouseDown(0)) {
				AcceptQuest();
				CEngine::GetInstance()->AcceptQuest(m_tNPCTag.hasQuestID);
				m_pTalkBox->SetActive(false);
				m_pTalkBox->SetDead();
				g_Menu = false;
			}
		}
	}
}

void CElfNPC::AcceptQuest()
{
	CQuestData*	questData = CEngine::GetInstance()->GetQuestData(m_tNPCTag.hasQuestID);
	CEngine::GetInstance()->AddAcceptQuest(m_tNPCTag.hasQuestID, questData);
	m_bisEndTalk = true;
	m_pTalkDesk->SetActive(false);
	m_iCurTalkProgress = 0;
	questData->questProgress++;
}

void CElfNPC::NormalTalk(_int id)
{
	vector<string> talk = CEngine::GetInstance()->GetTalk(id);

	m_pText->SetString(talk[m_iCurTalkProgress]);
	m_pTalkDesk->SetActive(true);
	if (CEngine::GetInstance()->IsKeyDown(VK_SPACE)) {
		m_iCurTalkProgress = 0;
		m_bisEndTalk = true;
		m_pTalkDesk->SetActive(false);
	}
}

void CElfNPC::QuestTalk(_int talkID)
{
	vector<string> talk = CEngine::GetInstance()->GetTalk(talkID);

	CQuestData* questData = CEngine::GetInstance()->GetQuestData(m_tNPCTag.hasQuestID);
	size_t talkSize = CEngine::GetInstance()->GetTalkSize(talkID);

	if (m_iCurTalkProgress >= talkSize) {
		m_bisEndTalk = true;
		m_pTalkDesk->SetActive(false);
		m_iCurTalkProgress = 0;
		return;
	}
	

	m_pTalkDesk->SetActive(true);
	m_pText->SetString(talk[m_iCurTalkProgress]);


	if (CEngine::GetInstance()->IsKeyDown(VK_SPACE)) {
		m_iCurTalkProgress++;
		if (m_iCurTalkProgress >= talkSize - 1) {
			/*_int talkBoxID = m_tNPCTag.boxID + m_tNPCTag.npcID;
			vector<string> talkBox = CEngine::GetInstance()->GetTalk(talkBoxID);*/
			CQuestData* questData = CEngine::GetInstance()->GetQuestData(m_tNPCTag.hasQuestID);
			if (!questData->isQuestAcheive()) {
				MakeTalkBox(0);
				g_Menu = true;
			}
			else {
				m_bisEndTalk = true;
				m_pTalkDesk->SetActive(false);
				m_iCurTalkProgress = 0;
				CEngine::GetInstance()->QuestClear(m_tNPCTag.hasQuestID);
			}
		}
	}

}

void CElfNPC::BoxTalk(_int id)
{
	vector<string> talks = CEngine::GetInstance()->GetTalk(id);
	m_pBoxText->SetString(talks.front());
}

CElfNPC * CElfNPC::Create(CGameObject * pTartget)
{
	CElfNPC*	pObj = new CElfNPC();
	if (pObj->Initailze(pTartget)) {
		SafeRelease(pObj);
		return nullptr;
	}
	return pObj;
}

void CElfNPC::Free()
{
	__super::Free();
}

