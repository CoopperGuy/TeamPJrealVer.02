#include "stdafx.h"
#include "..\Public\ComboIcon.h"
static string leftPath = "../../Assets/UITexture/Alret/leftClick.png";
static string rightPath = "../../Assets/UITexture/Alret/RightClick.png";

BEGIN(Client)
void prefabCreateThread(CGameObject** thisUI, CComboIcon*	script, CRectTransform**	pTransform, _float2	pDst, _bool leftRight, _bool isEffect) {

	*thisUI = CEngine::GetInstance()->SpawnPrefab("U_ComboPrefab");

	*pTransform = dynamic_cast<CRectTransform*>((*thisUI)->GetComponent("Com_Transform"));
	(*pTransform)->SetPosition(pDst.x, pDst.y);

	script->SetIsCreated();

	list<CGameObject*> child = (*thisUI)->GetChildren();
	int i = 0;
	for (auto& iter : child) {
		CVIBuffer_RectUI* buf = dynamic_cast<CVIBuffer_RectUI*>((iter)->GetComponent("Com_VIBuffer"));
		CRectTransform* trans = dynamic_cast<CRectTransform*>((iter)->GetComponent("Com_Transform"));

		script->SetVIBuffer(buf, i);
		script->SetUI(dynamic_cast<CEmptyUI*>(iter), i);
		if (i == CComboIcon::COMBOHUD_MOUSE) {
			if (!leftRight) {
				buf->UpdateTexture(leftPath, CVIBuffer_RectUI::TEXTURE_DIFFUSE);
			}
			else {
				buf->UpdateTexture(rightPath, CVIBuffer_RectUI::TEXTURE_DIFFUSE);
			}
		}
		if (i == CComboIcon::COMBOHUD_EFFECT) {
			iter->SetActive(isEffect);
		}
		i++;
	}
	CEngine::GetInstance()->AddScriptObject(script, CEngine::GetInstance()->GetCurSceneNumber());
}
END

USING(Client)

CComboIcon::CComboIcon()
{
}

HRESULT CComboIcon::Initailze(CRectTransform*	pSrc, CRectTransform*	pDst, _bool leftRight, _bool isEffect)
{
	m_fDst = { pDst->GetPosition().x, pDst->GetPosition().y };
	m_fSrc = { pSrc->GetPosition().x + 300.f, pSrc->GetPosition().y };
	m_isEffect = isEffect;
	/*std::thread createPrefab(prefabCreateThread, &m_thisUI, this, &m_pTransform, m_fDst, leftRight, isEffect);
	createPrefab.detach();*/

	m_thisUI = CEngine::GetInstance()->SpawnPrefab("U_ComboPrefab");

	m_pTransform = dynamic_cast<CRectTransform*>((m_thisUI)->GetComponent("Com_Transform"));
	(m_pTransform)->SetPosition(m_fDst.x, m_fDst.y);

	this->SetIsCreated();

	list<CGameObject*> child = (m_thisUI)->GetChildren();
	int i = 0;
	for (auto& iter : child) {
		CVIBuffer_RectUI* buf = dynamic_cast<CVIBuffer_RectUI*>((iter)->GetComponent("Com_VIBuffer"));
		CRectTransform* trans = dynamic_cast<CRectTransform*>((iter)->GetComponent("Com_Transform"));

		this->SetVIBuffer(buf, i);
		this->SetUI(dynamic_cast<CEmptyUI*>(iter), i);
		if (i == CComboIcon::COMBOHUD_MOUSE) {
			if (!leftRight) {
				buf->UpdateTexture(leftPath, CVIBuffer_RectUI::TEXTURE_DIFFUSE);
			}
			else {
				buf->UpdateTexture(rightPath, CVIBuffer_RectUI::TEXTURE_DIFFUSE);
			}
		}
		if (i == CComboIcon::COMBOHUD_EFFECT) {
			iter->SetActive(isEffect);
		}
		i++;
	}
	CEngine::GetInstance()->AddScriptObject(this, CEngine::GetInstance()->GetCurSceneNumber());

	return S_OK;
}

void CComboIcon::Update(_double deltaTime)
{
	if (isCreated) {
		if (m_isGoing) {
			_float t = m_lifeDelta / m_lifeTime;
			_vector pos = XMVectorLerp(XMLoadFloat2(&m_fDst), XMLoadFloat2(&m_fSrc), (_float)t);
			_float2 movePos;
			XMStoreFloat2(&movePos, pos);
			movePos.y = m_fDst.y;
			m_pTransform->SetPosition(movePos.x, movePos.y);
			if (movePos.x <= m_fSrc.x)
				m_isGoing = false;
		}
	}
}

void CComboIcon::LateUpdate(_double deltaTime)
{
	if (isCreated) {
		if (!m_isGoing) {
			m_lifeDelta += deltaTime;
			if (m_lifeDelta > m_lifeTime) {
				m_thisUI->SetDead();
				__super::SetDead();
				m_lifeTime = 0;
				return;

			}
		}
		if (m_isDestroy) {
			m_thisUI->SetDead();
			__super::SetDead();
			return;
		}
		if (m_isEffect) {
			if(m_lifeDelta > 0.1f)
				m_pEmptyUI[COMBOHUD_EFFECT]->SetShrinkInfo(1000.f, 0.f, 10.f);
			//CRectTransform::RECTTRANSFORMDESC desc = m_pUITransform[COMBOHUD_EFFECT]->GetTransformDesc();
			//desc.sizeX -= 100.f*deltaTime;
			//desc.sizeY -= 100.f*deltaTime;
			//m_pUITransform[COMBOHUD_EFFECT]->SetTransformMat(desc);
			
		}
	}
}

void CComboIcon::Render()
{
}

CComboIcon * CComboIcon::Create(CRectTransform*	pSrc, CRectTransform*	pDst, _bool leftRight, _bool isEffect)
{
	CComboIcon*	obj = new CComboIcon();
	if (FAILED(obj->Initailze(pSrc, pDst, leftRight, isEffect))) {
		SAFE_RELEASE(obj);
		return nullptr;
	}
	return obj;
}

void CComboIcon::Free()
{
}


