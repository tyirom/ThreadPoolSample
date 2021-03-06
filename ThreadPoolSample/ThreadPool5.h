﻿// --------------------------------------------------------------------------------------------------------------------
// <copyright file="ThreadPool5.h" company="Grass Valley K.K.">
//   Copyright (C) 2016 pichio. All rights reserved.
// </copyright>
// --------------------------------------------------------------------------------------------------------------------
#pragma once
#include <condition_variable>
#include <deque>
#include <vector>
#include <winerror.h>

namespace ThreadPool5
{
	template <class T>
	class Slot
	{
		using Type = std::function<T>;
	protected:
		Type m_slot;

	public:
		void Connect(Type slot)
		{
			m_slot = slot;
		}
	};
	using ExceptionSlot = Slot<void(const std::exception&)>;

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
		virtual void Slot() = 0;
	};

	template <class ActionT, class SlotT>
	class TaskImpl : public Task
	{
		ActionT& m_func;
		SlotT& m_slot;
		std::mutex m_mutex;
		std::condition_variable m_condition;
		bool m_isExecuted;
		bool m_isCanceled;
		HRESULT m_hr;

	public:
		explicit TaskImpl(const ActionT& func, const SlotT& slot);
		void Action() override;
		void Wait() override;
		void Cancel() override;
		HRESULT GetHResult() override;
		void Slot() override;
	};

	class ThreadPool;
	class Worker : public ExceptionSlot
	{
		ThreadPool& m_owner;

	public:
		explicit Worker(ThreadPool& owner);
		void operator()();
	};

	class ThreadPool : public ExceptionSlot
	{
		friend Worker;
		std::vector<std::thread> m_workers;
		std::deque<std::shared_ptr<Task>> m_tasks;
		std::mutex m_mutex;
		std::condition_variable m_condition;
		bool m_isDisposing;

		void OnError(const std::exception& ex) const;

	public:
		explicit ThreadPool(size_t nThreads);
		template <class T, class SlotT> std::shared_ptr<Task> Enqueue(const T& func, const SlotT& slot);
		~ThreadPool();
	};

	template <class ActionT, class SlotT>
	TaskImpl<ActionT, SlotT>::TaskImpl(const ActionT& func, const SlotT& slot) : m_func(func), m_slot(slot), m_isExecuted(false), m_isCanceled(false), m_hr(E_FAIL)
	{
	}

	template <class ActionT, class SlotT>
	void TaskImpl<ActionT, SlotT>::Action()
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

	template <class ActionT, class SlotT>
	void TaskImpl<ActionT, SlotT>::Wait()
	{
		std::unique_lock<std::mutex> lock(m_mutex);
		m_condition.wait(lock, [&] { return m_isExecuted || m_isCanceled; });
	}

	template <class ActionT, class SlotT>
	void TaskImpl<ActionT, SlotT>::Cancel()
	{
		std::lock_guard<std::mutex> lock(m_mutex);
		m_isCanceled = true;
	}

	template <class ActionT, class SlotT>
	HRESULT TaskImpl<ActionT, SlotT>::GetHResult()
	{
		std::lock_guard<std::mutex> lock(m_mutex);
		if (m_isCanceled)
		{
			return S_FALSE;
		}

		return m_hr;
	}

	template <class ActionT, class SlotT>
	void TaskImpl<ActionT, SlotT>::Slot()
	{
		try
		{
			m_slot(m_hr);
		}
		catch (...)
		{
			// no throw
		}
	}

	template <class ActionT, class SlotT>
	std::shared_ptr<Task> ThreadPool::Enqueue(const ActionT& func, const SlotT& slot)
	{
		std::shared_ptr<Task> pTask;
		{
			std::unique_lock<std::mutex> lock(m_mutex);
			pTask = std::make_shared<TaskImpl<decltype(func), decltype(slot)>>(func, slot);
			m_tasks.push_back(pTask);
		}
		m_condition.notify_one();
		return pTask;
	}
}
