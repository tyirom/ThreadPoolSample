#pragma once

#include <vector>
#include <thread>
#include <deque>
#include <mutex>
#include <winerror.h>

class ThreadPool;
class Worker
{
	ThreadPool& m_owner;

public:
	explicit Worker(ThreadPool& owner);
	void operator()();
};

class ThreadPool
{
	friend Worker;
	std::vector<std::thread> m_workers;
	std::deque<std::function<void()>> m_tasks;
	std::mutex m_mutex;
	std::condition_variable m_condition;
	bool m_isDisposing;

public:
	explicit ThreadPool(size_t nThreads);
	template<class T> void Enqueue(T func);
	~ThreadPool();
};

template <class T>
void ThreadPool::Enqueue(T func)
{
	{
		std::unique_lock<std::mutex> lock(m_mutex);
		m_tasks.push_back(std::function<void()>(func));
	}
	m_condition.notify_one();
}

class Task
{
	std::condition_variable m_condition;
	std::mutex m_mutex;
	bool m_isExecuted;
	bool m_isCanceled;

protected:
	HRESULT m_hr;

public:
	Task();
	virtual ~Task();

	virtual void Action();
	virtual void Wait();
	virtual HRESULT GetHResult();
	virtual void Cancel();
};