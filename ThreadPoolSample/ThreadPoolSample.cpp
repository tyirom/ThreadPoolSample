// ThreadPoolSample.cpp : コンソール アプリケーションのエントリ ポイントを定義します。
//

#include "stdafx.h"
#include "ThreadPool.h"
#include "Sample.h"
#include <iostream>
#include "ThreadPool2.h"
#include "ThreadPool3.h"
#include "ThreadPool4.h"
#include "ThreadPool5.h"
#include "ThreadPool6.h"
#include "Slot.h"

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

void ThreadPoolSample5()
{
	auto pSample = std::make_unique<Sample3>();
	{
		std::unique_ptr<ThreadPool5::ThreadPool> pThreadPool(new ThreadPool5::ThreadPool(1));
		pThreadPool->Connect([](const std::exception& ex) { std::wcerr << L"Caught Exception : " << ex.what() << std::endl; });
		auto task1 = pThreadPool->Enqueue([&] { return pSample->Action(); }, [&](HRESULT hr) {pSample->Slot(hr); });
		auto task2 = pThreadPool->Enqueue([&] { return pSample->Action(); }, [&](HRESULT hr) {pSample->Slot(hr); });
		auto task3 = pThreadPool->Enqueue([&] { return pSample->Action(); }, [&](HRESULT hr) {pSample->Slot(hr); });
		auto task4 = pThreadPool->Enqueue([&] { return pSample->Action(); }, [&](HRESULT hr) {pSample->Slot(hr); });
		auto task5 = pThreadPool->Enqueue([&] { return pSample->Action(); }, [&](HRESULT hr) {pSample->Slot(hr); });
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

void ThreadPoolSample6()
{
	auto pSample = std::make_unique<Sample3>();
	{
		std::unique_ptr<ThreadPool6::ThreadPool> pThreadPool(new ThreadPool6::ThreadPool(1));
		pThreadPool->ErrorDetected([](const std::exception& ex) { std::wcerr << L"Caught Exception : " << ex.what() << std::endl; });
		pThreadPool->Executed([](HRESULT hr) { std::wcout << L"Executed : " << hr << std::endl; });
		auto task1 = pThreadPool->Enqueue([&] { return pSample->Action(); }, [&](HRESULT hr) {pSample->Slot(hr); });
		auto task2 = pThreadPool->Enqueue([&] { return pSample->Action(); }, [&](HRESULT hr) {pSample->Slot(hr); });
		auto task3 = pThreadPool->Enqueue([&] { return pSample->Action(); }, [&](HRESULT hr) {pSample->Slot(hr); });
		auto task4 = pThreadPool->Enqueue([&] { return pSample->Action(); }, [&](HRESULT hr) {pSample->Slot(hr); });
		auto task5 = pThreadPool->Enqueue([&] { return pSample->Action(); }, [&](HRESULT hr) {pSample->Slot(hr); });
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

void SlotSample()
{
#if 0
	Slot::Slot3<HRESULT, const std::exception&> slot;
	slot.Connect([](HRESULT hr) { std::wcout << L"HRESULT: " << hr << std::endl; });
	slot.Connect([](const std::exception& ex) { std::wcout << L"Exception: " << ex.what() << std::endl; });
	Slot::Slot<HRESULT, const std::exception&> slot;
	slot.Connect([](HRESULT hr) { std::wcout << L"HRESULT: " << hr << std::endl; });
#endif
	int twelf = 12.5;
	typedef std::reference_wrapper<int> rint;

	Slot::MyClass<float, rint> mc;
	std::vector<rint>& i = mc.access<rint>();
	i.push_back(twelf);

	mc.access<float>().push_back(10.5);

	std::cout << "Test:\n";
	std::cout << "floats: " << mc.access<float>()[0] << std::endl;
	std::cout << "ints: " << mc.access<rint>()[0] << std::endl;
}

int main()
{
	ThreadPoolSample1();
	ThreadPoolSample2();
	ThreadPoolSample3();
	ThreadPoolSample4();
	ThreadPoolSample5();
	ThreadPoolSample6();
	SlotSample();

	return 0;
}
