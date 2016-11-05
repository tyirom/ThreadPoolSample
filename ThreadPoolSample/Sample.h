#pragma once
#include "ThreadPool.h"

class Sample : public Task
{
	int m_count;

public:
	Sample();
	void Action() override;
};
