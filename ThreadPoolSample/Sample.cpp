#include "stdafx.h"
#include "Sample.h"
#include <iostream>

Sample::Sample(): m_count(0)
{
}

void Sample::Action()
{
	std::wcout << L"Call Sample::Action (" << ++m_count << ")" << std::endl;
	Task::Action();
	m_hr = S_OK;
}
