// ThreadPoolSample.cpp : コンソール アプリケーションのエントリ ポイントを定義します。
//

#include "stdafx.h"
#include "ThreadPool.h"
#include "Sample.h"
#include <iostream>
#include "ThreadPool2.h"
#include "ThreadPool3.h"
#include "ThreadPool4.h"

void ThreadPoolSample1()
{
	std::unique_ptr<Sample> pSample(new Sample);
	{
		std::unique_ptr<ThreadPool> pThreadPool(new ThreadPool(1));
		pThreadPool->Enqueue([&] { pSample->Action(); });
		pThreadPool->Enqueue([&] { pSample->Action(); });
		pThreadPool->Enqueue([&] { pSample->Action(); });
		pThreadPool->Enqueue([&] { pSample->Action(); });
		pThreadPool->Enqueue([&] { pSample->Action(); });
#if 0
		pSample->Wait();
		pSample->Wait();
		pSample->Wait();
		pSample->Wait();
		pSample->Wait();
		std::wcout << L"Result : " << pSample->GetHResult() << std::endl;
#endif
	}
	// pSampleはpThreadPoolがデストラクトされるまで存在しなければいけない。	
}

void ThreadPoolSample2()
{
	std::unique_ptr<Sample> pSample(new Sample);
	{
		std::unique_ptr<ThreadPool2::ThreadPool> pThreadPool(new ThreadPool2::ThreadPool(1));
		pThreadPool->Enqueue([&] { pSample->Action(); });
		pThreadPool->Enqueue([&] { pSample->Action(); });
		pThreadPool->Enqueue([&] { pSample->Action(); });
		pThreadPool->Enqueue([&] { pSample->Action(); });
		pThreadPool->Enqueue([&] { pSample->Action(); });
#if 0
		pSample->Wait();
		pSample->Wait();
		pSample->Wait();
		pSample->Wait();
		pSample->Wait();
		std::wcout << L"Result2 : " << pSample->GetHResult() << std::endl;
#endif
	}
}

void ThreadPoolSample3()
{
	std::unique_ptr<Sample> pSample(new Sample);
	{
		std::unique_ptr<ThreadPool3::ThreadPool> pThreadPool(new ThreadPool3::ThreadPool(1));
		auto task1 = pThreadPool->Enqueue([&] { pSample->Action(); });
		auto task2 = pThreadPool->Enqueue([&] { pSample->Action(); });
		auto task3 = pThreadPool->Enqueue([&] { pSample->Action(); });
		auto task4 = pThreadPool->Enqueue([&] { pSample->Action(); });
		auto task5 = pThreadPool->Enqueue([&] { pSample->Action(); });
		task1->Wait();
		task2->Wait();
		task3->Wait();
		task4->Wait();
		task5->Wait();
	}
}

void ThreadPoolSample4()
{
	std::unique_ptr<Sample2> pSample(new Sample2);
	{
		std::unique_ptr<ThreadPool4::ThreadPool> pThreadPool(new ThreadPool4::ThreadPool(1));
		auto task1 = pThreadPool->Enqueue([&] { return pSample->Action(); });
		auto task2 = pThreadPool->Enqueue([&] { return pSample->Action(); });
		auto task3 = pThreadPool->Enqueue([&] { return pSample->Action(); });
		auto task4 = pThreadPool->Enqueue([&] { return pSample->Action(); });
		auto task5 = pThreadPool->Enqueue([&] { return pSample->Action(); });
		task1->Wait();
		std::wcout << L"task1 : " << task1->GetHResult() << std::endl;
		task2->Wait();
		std::wcout << L"task2 : " << task2->GetHResult() << std::endl;
		task3->Wait();
		std::wcout << L"task3 : " << task3->GetHResult() << std::endl;
		task4->Wait();
		std::wcout << L"task4 : " << task4->GetHResult() << std::endl;
		task5->Wait();
		std::wcout << L"task5 : " << task5->GetHResult() << std::endl;
	}
}

int main()
{
	ThreadPoolSample1();
	ThreadPoolSample2();
	ThreadPoolSample3();
	ThreadPoolSample4();

	return 0;
}
