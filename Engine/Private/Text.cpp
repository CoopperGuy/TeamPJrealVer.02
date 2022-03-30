#include "EnginePCH.h"
#include "Text.h"
#include "Engine.h"

USING(Engine)

CText::CText(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
	: CComponent(pDevice, pDeviceContext)
{
	m_pSpriteBatch = CEngine::GetInstance()->GetSpriteBatch();
	m_pSpriteFont = CEngine::GetInstance()->GetSpriteFont();
	m_pSpriteKoreanFont = CEngine::GetInstance()->GetSpriteKoreanFont();
}

CText::CText(const CText & rhs)
	: CComponent(rhs)
	, m_pSpriteBatch(rhs.m_pSpriteBatch)
	, m_pSpriteFont(rhs.m_pSpriteFont)
	, m_pSpriteKoreanFont(rhs.m_pSpriteKoreanFont)
	, m_strText(rhs.m_strText)
	, m_vColor(rhs.m_vColor)
	, m_fLayerDepth(rhs.m_fLayerDepth)
	, m_bisShaderCut(rhs.m_bisShaderCut)
	, m_bIsKorean(rhs.m_bIsKorean)
{
}

CText * CText::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
{
	CText*	pInstance = new CText(pDevice, pDeviceContext);

	if (FAILED(pInstance->InitializePrototype()))
	{
		MSG_BOX("Failed To Create CText");
		SafeRelease(pInstance);
	}
	return pInstance;
}

CComponent * CText::Clone(void * pArg)
{
	CComponent*	pInstance = new CText(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed To Clone CText");
		SafeRelease(pInstance);
	}


	return pInstance;
}

void CText::Free()
{
	__super::Free();
}

HRESULT CText::InitializePrototype()
{
	return S_OK;
}

HRESULT CText::Initialize(void * pArg)
{
	if (nullptr != pArg)
		m_pTransform = (CRectTransform*)pArg;

	return S_OK;
}

HRESULT CText::Render()
{
	CRectTransform::RECTTRANSFORMDESC desc = m_pTransform->GetTransformDesc();
	_float2 winSize = CEngine::GetInstance()->GetCurrentWindowSize();
	float xFactor = winSize.x / 1280.f;
	float yFactor = winSize.y / 720.f;
	CD3D11_RECT rt(LONG((desc.posX - (desc.sizeX / 2.f)) * xFactor), LONG((desc.posY - (desc.sizeY / 2.f)) * yFactor),
		LONG((desc.posX + (desc.sizeX / 2.f)) * xFactor), LONG((desc.posY + (desc.sizeY / 2.f)) * yFactor));
	_float2 vPos = { (float)rt.left, (float)rt.top };
	if (m_bisShaderCut) {
		if (rt.top < 60 || rt.bottom > 660) {
			rt.top = rt.bottom;
		}
	}
	ComRef<ID3D11RasterizerState> scissorState = nullptr;
	CD3D11_RASTERIZER_DESC rsDesc(D3D11_FILL_SOLID, D3D11_CULL_BACK, FALSE,
		0, 0.f, 0.f, TRUE, TRUE, TRUE, FALSE);
	if (FAILED(CEngine::GetInstance()->GetDevice()->CreateRasterizerState(&rsDesc, &scissorState)))
		return E_FAIL;
	
	m_pSpriteBatch->Begin(SpriteSortMode_Deferred,
		nullptr, nullptr, nullptr, scissorState.Get(),
		[&]()
	{
		m_pDeviceContext->RSSetScissorRects(1, &rt);
	});


	/*_float padding = desc.sizeX / 5.f;
	_float textWidth = (desc.sizeX - padding) * xFactor;

	string newStr = "";
	string strCheck = "";
	_float4 size, charSize;
	XMStoreFloat4(&size, m_pSpriteFont->MeasureString(m_strText.c_str()));
	
	for (int i = 0; i < m_strText.length(); ++i)
	{
		string charToAdd = m_strText.substr(i, 1);
		newStr += charToAdd;
		strCheck += charToAdd;

		if (*charToAdd.begin() == '\n')
			strCheck = "";

		_float4 curStrSize;
		XMStoreFloat4(&curStrSize, m_pSpriteFont->MeasureString((strCheck + "S").c_str()));
		_float curStrWidth = curStrSize.x * m_vScale.x * xFactor;

		if (curStrWidth >= textWidth)
		{
			newStr += '\n';
			strCheck = "";
		}
	}*/

	//wstring test;
	//wchar_t* unicode;
	//if(MultiByteToWideChar(437,MB_PRECOMPOSED,newStr.c_str(),newStr.length(), unicode,newStr.length())){
	//}
	if (!m_bisCenter) {
		if (m_bIsKorean) {
			vPos = { desc.posX * xFactor ,desc.posY * yFactor };
			m_pSpriteKoreanFont->DrawString(m_pSpriteBatch, m_strText.c_str(), vPos, XMLoadFloat4(&m_vColor), 0.f, XMFLOAT2(0.f, 0.f), _float2{ m_vScale.x * xFactor, m_vScale.y * yFactor }, DirectX::SpriteEffects_None, m_fLayerDepth);

		}
		else {
			vPos = { desc.posX * xFactor ,desc.posY * yFactor };
			m_pSpriteFont->DrawString(m_pSpriteBatch, m_strText.c_str(), vPos, XMLoadFloat4(&m_vColor), 0.f, XMFLOAT2(0.f, 0.f), _float2{ m_vScale.x * xFactor, m_vScale.y * yFactor }, DirectX::SpriteEffects_None, m_fLayerDepth);

		}
	}
	else {
		if (m_bIsKorean) {
			m_pSpriteKoreanFont->DrawString(m_pSpriteBatch, m_strText.c_str(), vPos, XMLoadFloat4(&m_vColor), 0.f, XMFLOAT2(0.f, 0.f), _float2{ m_vScale.x * xFactor, m_vScale.y * yFactor }, DirectX::SpriteEffects_None, m_fLayerDepth);

		}
		else {
			m_pSpriteFont->DrawString(m_pSpriteBatch, m_strText.c_str(), vPos, XMLoadFloat4(&m_vColor), 0.f, XMFLOAT2(0.f, 0.f), _float2{ m_vScale.x * xFactor, m_vScale.y * yFactor }, DirectX::SpriteEffects_None, m_fLayerDepth);
		}
	}
	m_pSpriteBatch->End();

	return S_OK;
}

void CText::SetTextInfo(string text, _float layerDepth, _float4 color, _float2 scale)
{
	m_strText = text;
	m_fLayerDepth = layerDepth;
	m_vColor = color;
	m_vScale = scale;
}
