#pragma once

/* 다음레벨에 대한 리소스 원형, 객체 우넌형을 준비하낟. */
#include "Client_Defines.h"
#include "Base.h"
#include "ThreadLoader.h"
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
	_bool Get_IsEnd();
public:
	void SetFinish(_int idx) { m_isThreadFinish[idx] = true; }
	void SetCompleteBit(_uint _bits) { m_iCompleteBit |= (1<<_bits); }
public:
	HRESULT Initialize(SCENE eScene);
	HRESULT	UpdateGauge(_double deltaTime);
	HRESULT GamePlayLoader();
	HRESULT GameFlogasLoader();
	HRESULT GameUrsaLoader();
	HRESULT	GameTestLoader();
	HRESULT	GameSceneLoader(string scene, SCENE sceneNum);
	HRESULT GameSceneStage01();
	HRESULT GameSceneStage02();
	HRESULT	GameSceneStage03();
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
	_uint				m_iCompleteBit = 0;
private:
	CLoadingGauge*		m_pLoadingGauge = nullptr;
	unordered_map<std::string, std::thread>	Threads;
private:
	CThreadLoader*		m_ThreadLoader = nullptr;
	mutable	size_t		m_threadSize = 8;

public:
	static CLoader* Create(SCENE eScene);
	virtual void Free() override;
};

END