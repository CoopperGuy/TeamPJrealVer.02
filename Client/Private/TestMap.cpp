#include "stdafx.h"
#include "..\Public\TestMap.h"

USING(Client)

CTestMap::CTestMap(CGameObject* pObj)
	: CCharacter(pObj)
{
}


CTestMap * CTestMap::Create(CGameObject * pObj)
{
	CTestMap*		pInstance = new CTestMap(pObj);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed to Create CTestMap");
		SafeRelease(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CTestMap::Free()
{
	__super::Free();
	SafeRelease(m_pTransformCom);
	SafeRelease(m_pRendererCom);
	SafeRelease(m_pModelCom);
	SafeRelease(m_pgameObject);
	SafeRelease(m_pCapsuleCollider);
}

HRESULT CTestMap::Initialize()
{

	list<class CGameObject*> objList = CEngine::GetInstance()->GetGameObjectInLayer(0, "LAYER_MAP");
	if (objList.size() <= 0)
		return E_FAIL;

	m_pgameObject = objList.front();
	m_pTransformCom = dynamic_cast<CTransform*>(m_pgameObject->GetComponent("Com_Transform"));
	m_pModelCom = dynamic_cast<CModel*>(m_pgameObject->GetComponent("Com_Model"));
	m_pRendererCom = dynamic_cast<CRenderer*>(m_pgameObject->GetComponent("Com_Renderer"));

	return S_OK;
}

void CTestMap::Update(_double dDeltaTime)
{
	if (!m_pgameObject)
		return;
}

void CTestMap::LateUpdate(_double dDeltaTime)
{
}

void CTestMap::Render()
{
}
