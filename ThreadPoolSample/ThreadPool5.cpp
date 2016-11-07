#include "stdafx.h"
#include "ThreadPool5.h"
#include <iostream>

namespace ThreadPool5
{
	Worker::Worker(ThreadPool& owner)
		: m_owner(owner)
	{
	}

	void Worker::operator()()
	{
		std::shared_ptr<Task> task;
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
			task->Action();
			task->Slot();
		}
	}

	ThreadPool::ThreadPool(size_t nThreads) : m_isDisposing(false)
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
}
