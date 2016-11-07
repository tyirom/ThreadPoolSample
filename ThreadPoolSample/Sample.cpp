#include "stdafx.h"
#include "Sample.h"
#include <iostream>

Sample::Sample(): m_count(0)
{
}

void Sample::Action()
{
	std::wcout << L"Call Sample::Action (" << ++m_count << ")" << std::endl;
#if 0
	Task::Action();
	m_hr = S_OK;
#endif
}

Sample2::Sample2(): m_count(0)
{
}

HRESULT Sample2::Action()
{
	std::wcout << L"Call Sample2::Action (" << ++m_count << ")" << std::endl;
	if (m_count == 1)
	{
		return S_OK;
	}
	else if (m_count == 2)
	{
		return S_FALSE;
	}
	else if (m_count == 3)
	{
		return E_ABORT;
	}
	else if (m_count == 4)
	{
		return E_NOINTERFACE;
	}
	else if (m_count == 5)
	{
		return E_NOTIMPL;
	}

	return E_FAIL;
}

Sample3::Sample3(): m_count(0)
{
}

HRESULT Sample3::Action()
{
	std::wcout << L"Call Sample3::Action (" << ++m_count << ")" << std::endl;
	return S_OK;
}

void Sample3::Slot(HRESULT hr)
{
	std::wcout << L"Call Sample3::Slot : " << hr << std::endl;
}
