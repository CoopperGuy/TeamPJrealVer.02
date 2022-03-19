#pragma once

/* 다음레벨에 대한 리소스 원형, 객체 우넌형을 준비하낟. */
#include "Client_Defines.h"
#include "Base.h"

BEGIN(Client)
class CLoadingGauge;
class CLoader final : public CBase
{
private:
	static const _int	ThreadIndex = 6;
private:
	CLoader();
	virtual ~CLoader() = default;
public:
	CRITICAL_SECTION Get_CS() {
		return m_CS;
	}

	SCENE Get_NextScene() const {
		return m_eScene;
	}

	_bool Get_Finish() const {
		return m_isFinish;
	}
public:
	void SetFinish(_int idx) { m_isThreadFinish[idx] = true; }
public:
	HRESULT Initialize(SCENE eScene);
	HRESULT	UpdateGauge(_double deltaTime);
	HRESULT GamePlayLoader();
	HRESULT GameFlogasLoader();
	HRESULT	GameTestLoader();
	HRESULT	GameSceneLoader(string scene, SCENE sceneNum);
	HRESULT GameSceneStage01();
	HRESULT GameSceneStage02();
	HRESULT GameSceneLogo();
	HRESULT GameSceneKIM();
	HRESULT GameSceneLEE();
	HRESULT GameSceneJUN();
	HRESULT GameSceneSEO();

public:
	void	PlusFinishiCnt() { m_iFinishCnt++; }
private:
	SCENE				m_eScene = SCENE_END;
	HANDLE				m_hThread = 0;
	CRITICAL_SECTION	m_CS;
	_uint				m_iFinishCnt = 0;
	_bool				m_isFinish;
	_bool				m_isThreadFinish[ThreadIndex];
private:
	CLoadingGauge*		m_pLoadingGauge = nullptr;


public:
	static CLoader* Create(SCENE eScene);
	virtual void Free() override;
};

END