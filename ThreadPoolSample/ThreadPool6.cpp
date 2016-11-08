#include "stdafx.h"
#include "ThreadPool6.h"
#include <iostream>
#include <algorithm>

namespace ThreadPool6
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
			try
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
				HResultSlot::m_slot(task->GetHResult());
			}
			catch (const std::exception& ex)
			{
				ExceptionSlot::m_slot(ex);
			}
		}
	}

	void Worker::ErrorDetected(ExceptionSlot::Type slot)
	{
		ExceptionSlot::m_slot = slot;
	}

	void Worker::Executed(HResultSlot::Type slot)
	{
		HResultSlot::m_slot = slot;
	}

	void ThreadPool::OnError(const std::exception& ex) const
	{
		ExceptionSlot::m_slot(ex);
	}

	void ThreadPool::OnExecuted(HRESULT hr) const
	{
		HResultSlot::m_slot(hr);
	}

	ThreadPool::ThreadPool(unsigned int nThreads) : m_isDisposing(false)
	{
		for (size_t i = 0; i < std::min(nThreads, std::thread::hardware_concurrency()); ++i)
		{
			auto worker = Worker(*this);
			worker.ErrorDetected([this](const std::exception& ex) { OnError(ex); });
			worker.Executed([this](HRESULT hr) { OnExecuted(hr); });
			m_workers.push_back(std::thread(worker));
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

	void ThreadPool::ErrorDetected(ExceptionSlot::Type slot)
	{
		ExceptionSlot::m_slot = slot;
	}

	void ThreadPool::Executed(HResultSlot::Type slot)
	{
		HResultSlot::m_slot = slot;
	}
}
