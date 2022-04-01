#pragma once

#include "Base.h"

BEGIN(Engine)

class CAnimation final : public CBase
{
private:
	CAnimation();
	virtual ~CAnimation() = default;
public:
	HRESULT NativeConstruct(const char* pAnimationName, _double Duration, _double TickPerSecond);
	HRESULT Add_Channel(class CChannel* pChannel);
	_bool Update_TransformationMatrices(_double TimeDelta);
	HRESULT Blend_Animation(CAnimation* prevAnim, _float ratio, _bool bRoot);
	HRESULT Blend_Animation(list<_matrix> prevAnimMatrix, _float ratio);
	list<_matrix> Get_Blend_Animation_Matrix(CAnimation* prevAnim, _float ratio);
	HRESULT ResetCurrentTime();
	_double GetDuratuin() { return m_Duration; }
	_double GetTotalTime() { return fmod(m_TickPerSecond, m_Duration); }
	_uint	GetCurrentKeyFrame() { return (_uint)m_CurrrentTime; };
	void	Set_CurrentKeyFrame(_double CurrentKeyFrame);
	vector<class CChannel*> GetChannels() { return m_Channels; }
	_bool Get_isFinished() { return m_isFinished; }
	char* GetName() { return m_AnimationName; }
	void	SetLoop(_bool result) { m_isLoop = result; }
	void	SetRoot(_bool result) { m_bRoot = result; }
	void	SetForceLoop(_bool result) { m_bForceLoop = result; }
	_bool	GetLoop() { return m_isLoop; }
	_bool	GetRoot() { return m_bRoot; }

private:
	char				m_AnimationName[MAX_PATH];
	_double				m_Duration = 0.0; 
	_double				m_TickPerSecond = 0.0;
	_double				m_CurrrentTime = 0.0;
	_bool				m_isFinished = false;
	_bool				m_isLoop = true;
	_bool				m_bRoot = false;
	_bool				m_bForceLoop = false;
	_bool				m_bForceFinished = false;
	vector<class CChannel*>	m_Channels;
public:
	static CAnimation* Create(const char* pAnimationName, _double Duration, _double TickPerSecond);
	virtual void Free() override;
};

END