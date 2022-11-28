#include "stdafx.h"
#include "..\Public\ThreadLoader.h"
#include <chrono>
USING(Client)

CThreadLoader::CThreadLoader(size_t _numThreads)
	:m_iNum_Threads(_numThreads)
	, m_bStop_All(false)
	, m_fCur_Jobs(0)
	, m_fMax_Jobs(0)
	, isEnd(false)
{
	m_worker_Threads.reserve(m_iNum_Threads);

}

_float CThreadLoader::loadingPercentage()
{
	if (m_fMax_Jobs == 0)
		return 0.f;
	return (_float)m_fCur_Jobs / (_float)m_fMax_Jobs;
}

void CThreadLoader::Start_Thread()
{
	for (size_t i = 0; i < m_iNum_Threads; i++) {
		m_worker_Threads.emplace_back([this]() {this->WorkerThread(); });
	}
}

_bool CThreadLoader::GetIsEnd()
{
	return isEnd;
}

_bool CThreadLoader::GetIsEnable()
{
	//size_t remain_Jobs = m_fMax_Jobs - m_fCur_Jobs;
	//if (remain_Jobs < 2)
	//	return true;
	return false;
}

void CThreadLoader::WorkerThread()
{
	while (true) {
		std::function<void()> job;
		{
			std::unique_lock<std::mutex> lock(m_job_Mutex);
			if (m_fCur_Jobs == m_fMax_Jobs) {
				m_bStop_All = true;
				isEnd = true;
			}
			m_cv_Job_Queue.wait(lock, [this]() {return !this->m_jobs.empty() || m_bStop_All; });

			if (m_bStop_All && this->m_jobs.empty()) {
				return;
			}

			job = std::move(m_jobs.front());
			m_jobs.pop();
		}

		job();
		m_fCur_Jobs++;
	}
}

void CThreadLoader::Free()
{
	m_bStop_All = true;
	m_cv_Job_Queue.notify_all();
	for (auto& iter : m_worker_Threads) {
		iter.join();
	}
	m_worker_Threads.clear();
}
