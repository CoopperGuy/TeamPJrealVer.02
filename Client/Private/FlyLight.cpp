#include "stdafx.h"
#include "Client_Struct.h"
#include "..\Public\FlyLight.h"
#include "EmptyEffect.h"

USING(Client)

CFlyLight::CFlyLight()
{
}

CFlyLight * CFlyLight::Create(void * pArg)
{
	CFlyLight*		pInstance = new CFlyLight();

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Create CFlyLight");
		SafeRelease(pInstance);
		return nullptr;
	}

	return pInstance;
}


HRESULT CFlyLight::Initialize(void* pArg)
{
	if (pArg != nullptr) {

		m_pGameObject = (CGameObject*)pArg;
		if (m_pGameObject == nullptr)
			return E_FAIL;

		m_pTransform = static_cast<CTransform*>(m_pGameObject->GetComponent("Com_Transform"));

		LIGHTDESC desc;
		desc.vDiffuse = _float4(1.f, 1.f, 1.f, 1.f);
		desc.vAmbient = _float4(1.0f, 0.293f, 0.043f, 1.f);
		desc.vSpecular = _float4(1.f, 1.f, 1.f, 1.f);
		desc.fLightRange = 10.f;
		desc.fLightAngle = 20.f;

		desc.eType = LIGHTDESC::LIGHT_POINT;
		CComponent* pLight = CLight::Create(CEngine::GetInstance()->GetDevice(), CEngine::GetInstance()->GetDeviceContext(), desc, m_pTransform);
		if (FAILED(m_pGameObject->AddComponent("Com_Light", pLight)))
			MSG_BOX("Failed to AddComponent");


		cout << XMVectorGetY(m_pTransform->GetState(CTransform::STATE_POSITION)) << endl;

	}
	return S_OK;
}

void CFlyLight::Update(_double deltaTime)
{
	if (m_bDead)
		return;

	if (!m_pGameObject)
		return;

}

void CFlyLight::LateUpdate(_double deltaTime)
{

	if (m_bDead)
	{
		this->SetDead();
		m_pGameObject->SetDead();
	}
}

void CFlyLight::Render()
{
}


void CFlyLight::Free()
{
	__super::Free();
}