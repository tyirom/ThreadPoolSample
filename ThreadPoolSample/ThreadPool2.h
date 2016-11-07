// --------------------------------------------------------------------------------------------------------------------
// <copyright file="ThreadPool2.h" company="Grass Valley K.K.">
//   Copyright (C) 2016 Grass Valley K.K. All rights reserved.
// </copyright>
// --------------------------------------------------------------------------------------------------------------------
#pragma once
#include <condition_variable>
#include <deque>
#include <vector>

namespace ThreadPool2
{
	// Thread pool implementation by Type Erasure.
	class Holder
	{
	public:
		virtual ~Holder()
		{
		}

		virtual void Action() = 0;
	};

	template <class T>
	class Function : public Holder
	{
		T& m_func;

	public:
		explicit Function(const T& func)
			: m_func(func)
		{
		}

		void Action() override
		{
			m_func();
		}
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
		std::deque<std::shared_ptr<Holder>> m_tasks;
		std::mutex m_mutex;
		std::condition_variable m_condition;
		bool m_isDisposing;

	public:
		explicit ThreadPool(size_t nThreads);
		template <class T> void Enqueue(const T& func);
		~ThreadPool();
	};

	template <class T>
	void ThreadPool::Enqueue(const T& func)
	{
		{
			std::unique_lock<std::mutex> lock(m_mutex);
			m_tasks.push_back(std::make_shared<Function<decltype(func)>>(func));
		}
		m_condition.notify_one();
	}
}
