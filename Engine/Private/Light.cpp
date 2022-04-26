#include "EnginePCH.h"
#include "..\public\Light.h"
#include "VIBuffer_Rect_Viewport.h"
#include "TargetManager.h"
#include "Engine.h"
#include "RenderTarget.h"


CLight::CLight(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
	: CComponent(pDevice, pDeviceContext)
{
}
CLight::CLight(const CLight & rhs)
	: CComponent(rhs)
	, m_LightDesc(rhs.m_LightDesc)
{
}
HRESULT CLight::Initialize(const LIGHTDESC & LightDesc, CTransform* pTransform, _bool _tempLight)
{
	m_pTransform = pTransform;
	m_bTempLight = _tempLight;
	
	if (LightDesc.eType == tagLightDesc::LIGHT_END)
	{
		m_LightDesc.eType = LIGHTDESC::LIGHT_POINT;
		m_LightDesc.vLightPos = _float4(0.f, 0.f, 0.f, 1.f);
		m_LightDesc.fLightRange = 10.f;
		m_LightDesc.fLightAngle = 0.f;
		m_LightDesc.vDiffuse = _float4(1.f, 1.f, 1.f, 1.f);
		m_LightDesc.vAmbient = _float4(0.4f, 0.4f, 0.4f, 1.f);
		m_LightDesc.vSpecular = _float4(1.f, 1.f, 1.f, 1.f);
	}
	else
		m_LightDesc = LightDesc;

	if (m_bTempLight) {
		m_fDestColor = m_LightDesc.vDiffuse;
		XMStoreFloat4(&m_LightDesc.vDiffuse, XMVectorZero());
	}

	_uint		iNumViewports = 1;

	D3D11_VIEWPORT		ViewportDesc;
	ViewportDesc.Width = WINCX;
	ViewportDesc.Height = WINCY;

	//m_pDeviceContext->RSGetViewports(&iNumViewports, &ViewportDesc);
	
	if (m_LightDesc.eType == LIGHTDESC::LIGHT_POINT) {
		CLight* light = CLightManager::GetInstance()->GetLightFront();
		m_pVIBuffer = light->GetVIBuffer();
		m_bIsClone = true;

	}
	else {
		m_pVIBuffer = CVIBuffer_Rect_Viewport::Create(m_pDevice, m_pDeviceContext, 0.f, 0.f, WINCX, WINCY, "../../Assets/Shader/Shader_Rect_Viewport.fx");
		m_bIsClone = false;

	}
	if (nullptr == m_pVIBuffer)
		return E_FAIL;

	if (m_LightDesc.eType != LIGHTDESC::LIGHT_POINT)
	{
		CTargetManager* m_pTargetManager = CTargetManager::GetInstance();
		/*_uint numLights = CLightManager::GetInstance()->GetNumLights();
		m_targetName = "Target_Shadow" + to_string(numLights);
		if (FAILED(m_pTargetManager->Add_RenderTarget(m_pDevice, m_pDeviceContext, m_targetName, WINCX, WINCY, DXGI_FORMAT_R32G32B32A32_FLOAT, _float4(0.f, 0.f, 1.f, 0.f))))
			return E_FAIL;

		if (FAILED(m_pTargetManager->Ready_DebugBuffer(m_targetName, 0.f + (numLights * 200.f), 600.f, 200.f, 200.f)))
			return E_FAIL;*/		
		
		//m_pRenderTarget = m_pTargetManager->Find_RenderTarget(m_targetName);
		m_pRenderTarget = m_pTargetManager->Find_RenderTarget("Target_ShadowDepth");
		//cout << "Direction Created \n";
	}

	CLightManager::GetInstance()->AddLight(this);

	return S_OK;
}



HRESULT CLight::Render_Light()
{
	_uint iPassIndex = 1;
	if (LIGHTDESC::LIGHT_DIRECTION == m_LightDesc.eType)
	{
		iPassIndex = 1;
		// m_pTransform->GetState(CTransform::STATE_LOOK);
		m_pVIBuffer->GetShader()->SetUp_ValueOnShader("g_vLightDir", &m_pTransform->GetState(CTransform::STATE_LOOK), sizeof(_float4));
	}
	else if (LIGHTDESC::LIGHT_POINT == m_LightDesc.eType)
	{
		iPassIndex = 2;
		m_pVIBuffer->GetShader()->SetUp_ValueOnShader("g_vLightPos", &m_pTransform->GetState(CTransform::STATE_POSITION), sizeof(_float4));
		m_pVIBuffer->GetShader()->SetUp_ValueOnShader("g_fLightRange", &m_LightDesc.fLightRange, sizeof(_float));
	}
	else if (LIGHTDESC::LIGHT_SPOT == m_LightDesc.eType)
	{
		iPassIndex = 3;
		m_pVIBuffer->GetShader()->SetUp_ValueOnShader("g_vLightDir", &m_pTransform->GetState(CTransform::STATE_LOOK), sizeof(_float4));
		m_pVIBuffer->GetShader()->SetUp_ValueOnShader("g_vLightPos", &m_pTransform->GetState(CTransform::STATE_POSITION), sizeof(_float4));
		m_pVIBuffer->GetShader()->SetUp_ValueOnShader("g_fLightRange", &m_LightDesc.fLightRange, sizeof(_float));
		m_pVIBuffer->GetShader()->SetUp_ValueOnShader("g_fLightAngle", &m_LightDesc.fLightAngle, sizeof(_float));
	}


	m_pVIBuffer->GetShader()->SetUp_ValueOnShader("g_vLightAmbient", &m_LightDesc.vAmbient, sizeof(_float4));
	m_pVIBuffer->GetShader()->SetUp_ValueOnShader("g_vLightDiffuse", &m_LightDesc.vDiffuse, sizeof(_float4));

	_matrix viewMatrix = CEngine::GetInstance()->GetTransform(CPipeline::D3DTS_VIEW);
	viewMatrix = XMMatrixInverse(nullptr, viewMatrix);
	// viewMatrix = XMMatrixIdentity();
	m_pVIBuffer->GetShader()->SetUp_ValueOnShader("g_ViewMatrixInv", &XMMatrixTranspose(viewMatrix), sizeof(_float4x4));

	_matrix projMatrix = CEngine::GetInstance()->GetTransform(CPipeline::D3DTS_PROJ);
	projMatrix = XMMatrixInverse(nullptr, projMatrix);
	m_pVIBuffer->GetShader()->SetUp_ValueOnShader("g_ProjMatrixInv", &XMMatrixTranspose(projMatrix), sizeof(_float4x4));

	_vector camPos = CEngine::GetInstance()->GetCamPosition();
	m_pVIBuffer->GetShader()->SetUp_ValueOnShader("g_vCamPosition", &CEngine::GetInstance()->GetCamPosition(), sizeof(_float4));

	CTargetManager*		pTargetManager = GET_INSTANCE(CTargetManager);

	ID3D11ShaderResourceView*	pNormalSRV = pTargetManager->GetShaderResourceView("Target_Normal");
	if (nullptr == pNormalSRV)
		return E_FAIL;

	m_pVIBuffer->GetShader()->SetUp_TextureOnShader("g_NormalTexture", pNormalSRV);

	ID3D11ShaderResourceView*	pDepthSRV = pTargetManager->GetShaderResourceView("Target_Depth");
	if (nullptr == pDepthSRV)
		return E_FAIL;

	m_pVIBuffer->GetShader()->SetUp_TextureOnShader("g_DepthTexture", pDepthSRV);

	ID3D11ShaderResourceView*	pSpecular = pTargetManager->GetShaderResourceView("Target_Specular2");
	if (nullptr == pSpecular)
		return E_FAIL;

	m_pVIBuffer->GetShader()->SetUp_TextureOnShader("g_SpecularTexture", pSpecular);

	m_pVIBuffer->Render(iPassIndex);

	RELEASE_INSTANCE(CTargetManager);


	return S_OK;
}

HRESULT CLight::Render_DebugBuffer()
{
	if (m_pRenderTarget)
		m_pRenderTarget->Render_DebugBuffer();
	return S_OK;
}

_uint CLight::Update(_double deltaTime)
{
	if (m_bTempLight) {
		m_DeltaTime += deltaTime;
		_double lerpTime = 2.2f * (4 * m_DeltaTime / m_LifeTime) *(-m_DeltaTime / m_LifeTime + 1);
		XMStoreFloat4(&m_LightDesc.vDiffuse, XMVectorLerp(XMVectorZero(), XMLoadFloat4(&m_fDestColor), (_float)lerpTime));
	}
	return _uint();
}

_matrix CLight::GetViewMatrix()
{
	return XMMatrixInverse(nullptr, m_pTransform->GetWorldMatrix());
}

_matrix CLight::GetProjMatrix()
{
	_float2 winSize = CEngine::GetInstance()->GetCurrentWindowSize();
	_float angle = 60.f;
	return XMMatrixPerspectiveFovLH(XMConvertToRadians(angle), winSize.x / winSize.y, 0.2f, 300.f);
}

_vector CLight::GetPosition()
{
	return m_pTransform->GetState(CTransform::STATE_POSITION);
}

_vector CLight::GetDirection()
{
	return XMVector3Normalize(XMVectorSetW(m_pTransform->GetState(CTransform::STATE_LOOK), 0.f));
}

_float CLight::GetAngle()
{
	return m_LightDesc.fLightAngle;
}

ID3D11ShaderResourceView * CLight::GetShaderResourceView()
{
	if (m_pRenderTarget)
		return m_pRenderTarget->GetShaderResourceView();

	return nullptr;
}

CLight * CLight::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext, const LIGHTDESC & LightDesc, CTransform* pTransform, _bool _tempLight)
{
	CLight*	pInstance = new CLight(pDevice, pDeviceContext);

	if (FAILED(pInstance->Initialize(LightDesc, pTransform, _tempLight)))
	{
		MSG_BOX("Failed To Creating CLight");
		SafeRelease(pInstance);
	}
	return pInstance;
}

CComponent * CLight::Clone(void * pArg)
{
	return nullptr;
}

void CLight::Free()
{
	CLightManager::GetInstance()->RemoveLight(this);
	if(!m_bIsClone)
		SafeRelease(m_pVIBuffer);
	//SafeRelease(m_pDeviceContext);
	//SafeRelease(m_pDevice);
}