#pragma once
#include <winerror.h>
#if 0
#include "ThreadPool.h"

class Sample : public Task
{
	int m_count;

public:
	Sample();
	void Action() override;
};
#else
class Sample
{
	int m_count;

public:
	Sample();
	void Action();
};

class Sample2
{
	int m_count;

public:
	Sample2();
	HRESULT Action();
};

class Sample3
{
	int m_count;

public:
	Sample3();
	HRESULT Action();
	void Slot(HRESULT hr);
};
#endif
