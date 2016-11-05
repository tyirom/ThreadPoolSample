// ThreadPoolSample.cpp : コンソール アプリケーションのエントリ ポイントを定義します。
//

#include "stdafx.h"
#include "ThreadPool.h"
#include "Sample.h"
#include <iostream>

int main()
{
	std::unique_ptr<ThreadPool> pThreadPool(new ThreadPool(1));
	std::unique_ptr<Sample> pSample(new Sample);
	pThreadPool->Enqueue([&] { pSample->Action(); });
	pThreadPool->Enqueue([&] { pSample->Action(); });
	pThreadPool->Enqueue([&] { pSample->Action(); });
	pThreadPool->Enqueue([&] { pSample->Action(); });
	pThreadPool->Enqueue([&] { pSample->Action(); });
	pSample->Wait();
	pSample->Wait();
	pSample->Wait();
	pSample->Wait();
	pSample->Wait();
	std::wcout << L"Result : " << pSample->GetHResult() << std::endl;

    return 0;
}

