#include "EnginePCH.h"
#include "..\Public\BossMovie.h"

USING(Engine)

static string _path = "../../Assets/UITexture/Movie/";

CBossMovie::CBossMovie()
{
}

HRESULT CBossMovie::Initailze(_int _type, CGameObject * pArg)
{
	CEngine*	pEngine = CEngine::GetInstance();

	m_pThisUI = static_cast<CEmptyUI*>(pEngine->SpawnPrefab("BossMovie"));
	pEngine->AddScriptObject(this,pEngine->GetCurSceneNumber());
	list<CGameObject*> _child = m_pThisUI->GetChildren();
	auto& iter = _child.front();
	m_pName = static_cast<CVIBuffer_RectUI*>(iter->GetComponent("Com_VIBuffer"));
	iter++;
	m_pSubName = static_cast<CVIBuffer_RectUI*>(iter->GetComponent("Com_VIBuffer"));
	if (_type == 0)
	{
		string _name = "Flogas.dds";
		string _subName = "LOP.dds";
		string _pullPath = _path + _name;
		m_pName->UpdateTexture(_pullPath, CVIBuffer_RectUI::TEXTUREID::TEXTURE_DIFFUSE);
		_pullPath = _path + _subName;
		m_pSubName->UpdateTexture(_pullPath, CVIBuffer_RectUI::TEXTUREID::TEXTURE_DIFFUSE);
	}
	else if(_type == 1)
	{
		string _name = "Ursa.dds";
		string _subName = "Varr.dds";
		string _pullPath = _path + _name;
		m_pName->UpdateTexture(_pullPath, CVIBuffer_RectUI::TEXTUREID::TEXTURE_DIFFUSE);
		_pullPath = _path + _subName;
		m_pSubName->UpdateTexture(_pullPath, CVIBuffer_RectUI::TEXTUREID::TEXTURE_DIFFUSE);
	}
	return S_OK;
}

void CBossMovie::Update(_double deltaTime)
{
}

void CBossMovie::LateUpdate(_double deltaTime)
{
	m_LifeDelta += deltaTime;
	if (m_LifeDelta > m_LifeTime) {
		this->SetDead();
		m_pThisUI->SetDead();
	}
}

void CBossMovie::Render()
{
}

CBossMovie * CBossMovie::Create(_int _type, CGameObject * pTarget)
{
	CBossMovie*	obj = new CBossMovie();
	if (FAILED(obj->Initailze(_type,pTarget))) {
		SAFE_RELEASE(obj);
		return nullptr;
	}
	return obj;
}

void CBossMovie::Free()
{
}

