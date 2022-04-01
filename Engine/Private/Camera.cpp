#include "EnginePCH.h"
#include "..\public\Camera.h"
#include "Pipeline.h"
#include "Transform.h"
#include "Engine.h"
#include <dsound.h>
CCamera::CCamera(ID3D11Device * pDevice, ID3D11DeviceContext * pDevice_Context)
	: CGameObject(pDevice, pDevice_Context)
	, m_pPipeline(CPipeline::GetInstance())
{
}

CCamera::CCamera(const CCamera & rhs)
	: CGameObject(rhs)
	, m_pPipeline(rhs.m_pPipeline)
{
}

HRESULT CCamera::InitializePrototype()
{
	__super::InitializePrototype();

	return S_OK;
}

HRESULT CCamera::Initialize(void * pArg)
{
	__super::Initialize(pArg);

	SetUpComponents(0, "Prototype_Transform", "Com_Transform", (CComponent**)&m_pTransformCom);

	if (nullptr != pArg)
		memcpy(&m_CameraDesc, pArg, sizeof(CAMERADESC));
	else
		m_pTransformCom->SetMatrix(XMMatrixIdentity());
	//AddComponent(0, "Prototype_Transform", "Com_Transform");

	//m_pTransformCom = dynamic_cast<CTransform*>(GetComponent("Com_Transform"));


	if (nullptr == m_pTransformCom)
		return E_FAIL;

	_vector	vLook = XMLoadFloat3(&m_CameraDesc.vAt) - XMLoadFloat3(&m_CameraDesc.vEye);
	m_pTransformCom->SetState(CTransform::STATE_LOOK, XMVector3Normalize(vLook));

	_vector	vRight = XMVector3Cross(XMLoadFloat3(&m_CameraDesc.vAxisY), vLook);
	m_pTransformCom->SetState(CTransform::STATE_RIGHT, XMVector3Normalize(vRight));

	_vector	vUp = XMVector3Cross(vLook, vRight);
	m_pTransformCom->SetState(CTransform::STATE_UP, XMVector3Normalize(vUp));

	m_pTransformCom->SetState(CTransform::STATE_POSITION, XMLoadFloat3(&m_CameraDesc.vEye));

	return S_OK;
}

_uint CCamera::Update(_double TimeDelta)
{
	__super::Update(TimeDelta);
	if (m_bIsActive)
	{
		_float2 winSize = m_pEngine->GetCurrentWindowSize();
		m_pPipeline->Set_Transform(CPipeline::D3DTS_VIEW, m_pTransformCom->GetWorldMatrixInverse());
		m_pPipeline->Set_Transform(CPipeline::D3DTS_PROJ, XMMatrixPerspectiveFovLH(XMConvertToRadians(m_fov), winSize.x / winSize.y, 0.01f, 300.f));
	}

	return _uint();
}

_uint CCamera::LateUpdate(_double TimeDelta)
{
	__super::LateUpdate(TimeDelta);

	return _uint();
}


void CCamera::Free()
{
	__super::Free();
}
