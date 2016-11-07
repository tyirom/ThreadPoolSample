// --------------------------------------------------------------------------------------------------------------------
// <copyright file="ThreadPool4.h" company="Grass Valley K.K.">
//   Copyright (C) 2016 Grass Valley K.K. All rights reserved.
// </copyright>
// --------------------------------------------------------------------------------------------------------------------
#pragma once
#include <condition_variable>
#include <deque>
#include <vector>
#include <winerror.h>

namespace ThreadPool4
{
	class Task
	{
	public:
		virtual ~Task()
		{
		}

		virtual void Action() = 0;
		virtual void Wait() = 0;
		virtual void Cancel() = 0;
		virtual HRESULT GetHResult() = 0;
	};

	template <class T>
	class TaskImpl : public Task
	{
		T& m_func;
		std::mutex m_mutex;
		std::condition_variable m_condition;
		bool m_isExecuted;
		bool m_isCanceled;
		HRESULT m_hr;

	public:
		explicit TaskImpl(const T& func);
		void Action() override;
		void Wait() override;
		void Cancel() override;
		HRESULT GetHResult() override;
	};

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
		std::deque<std::shared_ptr<Task>> m_tasks;
		std::mutex m_mutex;
		std::condition_variable m_condition;
		bool m_isDisposing;

	public:
		explicit ThreadPool(size_t nThreads);
		template <class T> std::shared_ptr<Task> Enqueue(const T& func);
		~ThreadPool();
	};

	template <class T>
	TaskImpl<T>::TaskImpl(const T& func) : m_func(func), m_isExecuted(false), m_isCanceled(false), m_hr(E_FAIL)
	{
	}

	template <class T>
	void TaskImpl<T>::Action()
	{
		std::lock_guard<std::mutex> lock(m_mutex);
		if (m_isCanceled)
		{
			return;
		}

		try
		{
			m_hr = m_func();
		}
		catch (...)
		{
			m_hr = E_FAIL;
		}

		m_isExecuted = true;
		m_condition.notify_all();
	}

	template <class T>
	void TaskImpl<T>::Wait()
	{
		std::unique_lock<std::mutex> lock(m_mutex);
		m_condition.wait(lock, [&] { return m_isExecuted || m_isCanceled; });
	}

	template <class T>
	void TaskImpl<T>::Cancel()
	{
		std::lock_guard<std::mutex> lock(m_mutex);
		m_isCanceled = true;
	}

	template <class T>
	HRESULT TaskImpl<T>::GetHResult()
	{
		std::lock_guard<std::mutex> lock(m_mutex);
		if (m_isCanceled)
		{
			return S_FALSE;
		}

		return m_hr;
	}

	template <class T>
	std::shared_ptr<Task> ThreadPool::Enqueue(const T& func)
	{
		std::shared_ptr<Task> pTask;
		{
			std::unique_lock<std::mutex> lock(m_mutex);
			pTask = std::make_shared<TaskImpl<decltype(func)>>(func);
			m_tasks.push_back(pTask);
		}
		m_condition.notify_one();
		return pTask;
	}
}
