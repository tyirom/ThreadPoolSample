#include "stdafx.h"
#include "ThreadPool.h"
#include <iostream>

Worker::Worker(ThreadPool& owner): m_owner(owner)
{
}

void Worker::operator()()
{
	std::function<void()> task;
	while (true)
	{
		{
			std::unique_lock<std::mutex> lock(m_owner.m_mutex);
			m_owner.m_condition.wait(lock, [&] { return m_owner.m_isDisposing || !m_owner.m_tasks.empty(); });
			if (m_owner.m_isDisposing)
			{
				break;
			}

			task = m_owner.m_tasks.front();
			m_owner.m_tasks.pop_front();
		}
		task();
	}
}

ThreadPool::ThreadPool(size_t nThreads): m_isDisposing(false)
{
	for (size_t i = 0; i < nThreads; ++i)
	{
		m_workers.push_back(std::thread(Worker(*this)));
	}
}

ThreadPool::~ThreadPool()
{
	m_isDisposing = true;
	m_condition.notify_all();

	for (size_t i = 0; i < m_workers.size(); ++i)
	{
		m_workers[i].join();
	}

	std::wcout << L"Destruct ThreadPool (remain " << m_tasks.size() << L" tasks)" << std::endl;
}

#if 0
Task::Task(): m_isExecuted(false), m_isCanceled(false), m_hr(E_FAIL)
{
}

Task::~Task()
{
	m_condition.notify_all();
}

void Task::Action()
{
	{
		std::lock_guard<std::mutex> lock(m_mutex);
		if (m_isCanceled)
		{
			std::wcout << L"Task::Action Canceled" << std::endl;
			return;
		}
	}

	std::wcout << L"Call Task::Action" << std::endl;
	m_isExecuted = true;
	m_condition.notify_one();
}

void Task::Wait()
{
	std::unique_lock<std::mutex> lock(m_mutex);
	if (m_isCanceled)
	{
		return;
	}

	m_condition.wait(lock, [this] { return m_isExecuted || m_isCanceled; });
}

HRESULT Task::GetHResult()
{
	return m_hr;
}

void Task::Cancel()
{
	std::lock_guard<std::mutex> lock(m_mutex);
	m_isCanceled = true;
	std::wcout << L"Call Task::Cancel" << std::endl;
}
#endif
