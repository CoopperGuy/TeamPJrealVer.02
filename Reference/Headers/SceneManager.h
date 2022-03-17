#pragma once

#include "Base.h"
#include "Scene.h"

/* 현재 할당되어있는 레벨을 보관ㅇㄴ한다. */
/* 틱, 렌덜르 호출해준다. */
/* 씬이 교체될 시에는 이전 레벨을 삭제하고 교체하여 보관한다. */

BEGIN(Engine)

class CSceneManager final : public CBase
{
	DECLARE_SINGLETON(CSceneManager)
public:
	CSceneManager();
	virtual ~CSceneManager() = default;

public:
	HRESULT		SetUpCurrentScene(CScene* pCurrentScene, _uint scnenNum);
	_uint		UpdateScene(_double TimeDelta);
	HRESULT		RenderScene();
public:
	void	SetCurSceneNumber(_uint idx) { m_curScene = idx; }
public:
	_uint	GetCurSceneNumber() { return m_curScene; }

private:
	CScene*				m_pCurrentScene = nullptr;
	_uint	m_curScene = 0;
	_uint	m_preScene = 0;
public:
	virtual void Free() override;
};

END