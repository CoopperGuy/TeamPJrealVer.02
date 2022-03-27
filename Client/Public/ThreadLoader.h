#pragma once
#include "Base.h"
#include <future>
BEGIN(Client)

class CThreadLoader :
	public CBase
{
public:
	CThreadLoader(size_t _numThreads);
	virtual ~CThreadLoader() = default;

	template<class F, class... Args>
	std::future<typename std::result_of<F(Args...)>::type> EnqueueJob(F&& f, Args&&... args);

private:
	void WorkerThread();

private:
	size_t m_iNum_Threads;
	std::vector<std::thread>	m_worker_Threads;
	std::queue<std::function<void()>> m_jobs;
	std::condition_variable	m_cv_Job_Queue;
	std::mutex	m_job_Mutex;
private:
	_bool m_bStop_All;

public:
	virtual void Free() override;
};

END

template<class F, class ...Args>
inline std::future<typename std::result_of<F(Args...)>::type> CThreadLoader::EnqueueJob(F && f, Args && ...args)
{
	if (m_bStop_All)
		throw std::runtime_error("ThreadPool was stoped");
	using return_type = typename std::result_of<F(Args...)>::type;
	auto job = std::make_shared<std::packaged_task<return_type()>>(
		std::bind(std::forward<F>(f), std::forward<Args>(args)...));
	std::future<return_type> job_result_future = job->get_future();

	std::lock_guard<std::mutex> lock(m_job_Mutex);
	m_jobs.push([job]() {(*job)(); });

	m_cv_Job_Queue.notify_one();

	return job_result_future;
		
}
