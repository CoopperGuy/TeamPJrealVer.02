#include "EnginePCH.h"
#include "..\Public\EmptyUI.h"
#include "Engine.h"
#include "Component.h"
#include "GameObject.h"
USING(Engine)

CEmptyUI::CEmptyUI(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
	: CGameObject(pDevice, pDeviceContext)
{
	m_Layer = "UI";
}

CEmptyUI::CEmptyUI(const CEmptyUI & rhs)
	: CGameObject(rhs)
{
}

CEmptyUI * CEmptyUI::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
{
	CEmptyUI*		pInstance = new CEmptyUI(pDevice, pDeviceContext);

	if (FAILED(pInstance->InitializePrototype()))
	{
		MSG_BOX("Failed to Create CEmptyUI");
		SafeRelease(pInstance);
	}

	return pInstance;
}

CGameObject * CEmptyUI::Clone(void * pArg)
{
	CEmptyUI*		pInstance = new CEmptyUI(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Create CEmptyUI");
		SafeRelease(pInstance);
	}
	

	return pInstance;
}

CGameObject * CEmptyUI::PrefabClone(void * pArg)
{
	return nullptr;
}

CGameObject * CEmptyUI::ChildrenPrefabClone(CGameObject * Parent, void * pArg)
{
	return nullptr;
}

void CEmptyUI::Free()
{
	__super::Free();
}

HRESULT CEmptyUI::InitializePrototype()
{
	if (FAILED(__super::InitializePrototype()))
		return E_FAIL;


	return S_OK;
}

HRESULT CEmptyUI::Initialize(void * pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(SetUpComponents()))
		return E_FAIL;

	return S_OK;
}

_uint CEmptyUI::Update(_double TimeDelta)
{
	if (__super::Update(TimeDelta))
		return E_FAIL;

	if (m_pParent)
	{
		if (dynamic_cast<CEmptyUI*>(m_pParent)) {
			CRectTransform* pParentTransform = dynamic_cast<CRectTransform*>(m_pParent->GetComponent("Com_Transform"));
			CRectTransform::RECTTRANSFORMDESC parentDesc = pParentTransform->GetTransformDesc();
			CRectTransform::RECTTRANSFORMDESC desc = m_pRectTransformCom->GetTransformDesc();
			CRectTransform::RECTTRANSFORMDESC newDesc = {
				parentDesc.posX + m_vTransformOffSet.x,
				parentDesc.posY + m_vTransformOffSet.y,
				desc.sizeX, desc.sizeY,
				desc.correctX,desc.correctY
			};

			m_pRectTransformCom->SetTransformMat(newDesc);
		}
	}
	if (!m_bIsActive || !m_bisRender)
		return S_OK;
	InteractMouse();
	if (m_bHover) {
		m_fTime += (_float)TimeDelta;
		if (m_fTime > 3000.f) {
			m_fTime = 0.f;
		}
	}
	else {
		m_fTime = 0.f;
	}
	if (m_bHover || m_bSelect) {
		m_fSelectHoverTime += (_float)TimeDelta;
		if (m_fSelectHoverTime > 300.f) {
			m_fSelectHoverTime = 0.f;
		}
	}
	else {
		m_fSelectHoverTime = 0.f;
	}

	return S_OK;
}

_uint CEmptyUI::LateUpdate(_double TimeDelta)
{
	if (nullptr == m_pRendererCom)
		return -1;

	if (!m_bIsActive)
		return 1;
	if (m_bisShrink) {
		CRectTransform::RECTTRANSFORMDESC	desc = m_pRectTransformCom->GetTransformDesc();

		if (m_fShrinkSpd < 0) {
			if (m_bisXShrink && m_bisYShrink) {
				if (desc.sizeX * desc.correctX < m_fShrinkMax && (desc.sizeY * desc.correctY) < m_fShrinkMax) {
					desc.correctX -= m_fShrinkSpd*(_float)TimeDelta;
					desc.correctY -= m_fShrinkSpd*(_float)TimeDelta;
				}
				else {
					if (m_bisBreath) {
						m_fShrinkSpd = -m_fShrinkSpd;
					}
				}
			}
			else if (m_bisXShrink) {
				if (desc.sizeX * desc.correctX < m_fShrinkMax) {
					desc.correctX -= m_fShrinkSpd*(_float)TimeDelta;
				}
				else {
					if (m_bisBreath) {
						m_fShrinkSpd = -m_fShrinkSpd;
					}
				}
			}
			else if (m_bisYShrink) {
				if ((desc.sizeY * desc.correctY) < m_fShrinkMax) {
					desc.correctY -= m_fShrinkSpd*(_float)TimeDelta;
				}
				else {
					if (m_bisBreath) {
						m_fShrinkSpd = -m_fShrinkSpd;
					}
				}
			}
		}
		else {

			if (m_bisXShrink && m_bisYShrink) {
				if (desc.sizeX * desc.correctX > m_fShrinkMin && (desc.sizeY * desc.correctY) > m_fShrinkMin) {
					desc.correctX -= m_fShrinkSpd*(_float)TimeDelta;
					desc.correctY -= m_fShrinkSpd*(_float)TimeDelta;
				}
				else {
					if (m_bisBreath) {
						m_fShrinkSpd = -m_fShrinkSpd;
					}
				}
			}
			else if (m_bisXShrink) {
				if (desc.sizeX * desc.correctX > m_fShrinkMin) {
					desc.correctX -= m_fShrinkSpd*(_float)TimeDelta;
				}
				else {
					if (m_bisBreath) {
						m_fShrinkSpd = -m_fShrinkSpd;
					}
				}
			}
			else if (m_bisYShrink) {
				if ((desc.sizeY * desc.correctY) > m_fShrinkMin) {
					desc.correctY -= m_fShrinkSpd*(_float)TimeDelta;
				}
				else {
					if (m_bisBreath) {
						m_fShrinkSpd = -m_fShrinkSpd;
					}
				}
			}
		}

		m_pRectTransformCom->SetTransformMat(desc);
	}

	if (!m_bisRender) {

		return _uint();
	}
	return m_pRendererCom->AddRenderGroup(CRenderer::RENDER_UI, this);
}

HRESULT CEmptyUI::Render(_uint iPassIndex)
{
	
	CVIBuffer* buffer = static_cast<CVIBuffer*>(GetComponent("Com_VIBuffer"));
	if (buffer) {
		(buffer)->GetShader()->SetUp_ValueOnShader("g_isSelected", &m_bSelect, sizeof(_bool));
		(buffer)->GetShader()->SetUp_ValueOnShader("g_isHover", &m_bHover, sizeof(_bool));
		(buffer)->GetShader()->SetUp_ValueOnShader("g_Time", &m_fTime, sizeof(_float));
		(buffer)->GetShader()->SetUp_ValueOnShader("g_SHTime", &m_fSelectHoverTime, sizeof(_float));

		if (dynamic_cast<CVIBuffer_RectUI*>(buffer)) {
			static_cast<CVIBuffer_RectUI*>(buffer)->GetShader()->SetUp_ValueOnShader("g_Percentage", &m_fPercentage, sizeof(_float));
			static_cast<CVIBuffer_RectUI*>(buffer)->GetShader()->SetUp_ValueOnShader("g_Back", &m_fBackPercentage, sizeof(_float));
			static_cast<CVIBuffer_RectUI*>(buffer)->GetShader()->SetUp_ValueOnShader("g_degree", &m_fDegree, sizeof(_float));
		}

		(buffer)->Render(m_iPassIndex);
	}
	CComponent* text = GetComponent("Com_Text");
	if (text)
		dynamic_cast<CText*>(text)->Render();

	return S_OK;
}

void CEmptyUI::SetRectTransform(CRectTransform::RECTTRANSFORMDESC _desc)
{
	m_pRectTransformCom->SetTransformMat(_desc);
}

void CEmptyUI::SetPosition(_float x, _float y)
{
	m_pRectTransformCom->SetPosition(x, y);
}

void CEmptyUI::SetClientPosition(_float x, _float y)
{
	m_pRectTransformCom->SetClientPosition(x, y);
}

void CEmptyUI::LinkTranformWithParent()
{
	if (m_pParent || dynamic_cast<CEmptyUI*>(m_pParent))
	{
		CRectTransform* pParentTransform = dynamic_cast<CRectTransform*>(m_pParent->GetComponent("Com_Transform"));
		CRectTransform::RECTTRANSFORMDESC parentDesc = pParentTransform->GetTransformDesc();
		CRectTransform::RECTTRANSFORMDESC desc = m_pRectTransformCom->GetTransformDesc();

		m_vTransformOffSet.x = desc.posX - parentDesc.posX;
		m_vTransformOffSet.y = desc.posY - parentDesc.posY;
	}
}

void CEmptyUI::SetSize(_float x, _float y)
{
	CRectTransform::RECTTRANSFORMDESC _desc;
	_desc = m_pRectTransformCom->GetTransformDesc();
	_desc.sizeX = x;
	_desc.sizeY = y;
	m_pRectTransformCom->SetTransformMat(_desc);
}

void CEmptyUI::InteractMouse()
{
	if (m_isHovering) {
		if (m_pRectTransformCom->IsMouseInRect())
		{
			m_bHover = true;
			if (m_pEngine->IsMouseUp(0))
				m_bSelect = !m_bSelect;
		}
		else
		{
			m_bHover = false;
		}
	}
}

void CEmptyUI::SetCorrectSize(_float x, _float y)
{
	CRectTransform::RECTTRANSFORMDESC	desc = m_pRectTransformCom->GetTransformDesc();
	desc.correctX = x;
	desc.correctY = y;
	m_pRectTransformCom->SetTransformMat(desc);
}

void CEmptyUI::SetCorrectYSize(_float y)
{
	CRectTransform::RECTTRANSFORMDESC	desc = m_pRectTransformCom->GetTransformDesc();
	desc.correctY = y;
	m_pRectTransformCom->SetTransformMat(desc);
}

void CEmptyUI::SetisRender(_bool tf)
{
	m_bisRender = tf;
	for (auto& iter : m_listChildren) {
		iter->SetisRender(m_bisRender);
	}
}

_float2 CEmptyUI::GetPosition()
{
	return m_pRectTransformCom->GetPosition();
}

_float2 CEmptyUI::GetUISize()
{
	return m_pRectTransformCom->GetUISize();
}

HRESULT CEmptyUI::SetUpComponents()
{
	if (FAILED(__super::SetUpComponents(0, "Prototype_RectTransform", "Com_Transform", (CComponent**)&m_pRectTransformCom)))
		return E_FAIL;

	if (FAILED(__super::SetUpComponents(0, "Prototype_Renderer", "Com_Renderer", (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	return S_OK;
}
