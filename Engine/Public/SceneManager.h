#pragma once

#include "Base.h"
#include "Scene.h"

/* ���� �Ҵ�Ǿ��ִ� ������ ���������Ѵ�. */
/* ƽ, ������ ȣ�����ش�. */
/* ���� ��ü�� �ÿ��� ���� ������ �����ϰ� ��ü�Ͽ� �����Ѵ�. */

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