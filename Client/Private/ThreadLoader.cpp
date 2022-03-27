#include "stdafx.h"
#include "..\Public\ThreadLoader.h"

USING(Client)

CThreadLoader::CThreadLoader(size_t _numThreads)
	:m_iNum_Threads(_numThreads)
	,m_bStop_All(false)
{
	m_worker_Threads.reserve(m_iNum_Threads);
	for (size_t i = 0; i < m_iNum_Threads; i++) {
		m_worker_Threads.emplace_back([this]() {this->WorkerThread(); });
	}
}

void CThreadLoader::WorkerThread()
{
	while (true) {
		std::unique_lock<std::mutex> lock(m_job_Mutex);
		m_cv_Job_Queue.wait(lock, [this]() {return !this->m_jobs.empty() || m_bStop_All; });
		if (m_bStop_All && this->m_jobs.empty()) {
			return;
		}

		std::function<void()> job = std::move(m_jobs.front());
		m_jobs.pop();
		lock.unlock();

		job();
	}
}

void CThreadLoader::Free()
{
	m_bStop_All = true;
	m_cv_Job_Queue.notify_all();

	for (auto& iter : m_worker_Threads) {
		iter.join();
	}
}
