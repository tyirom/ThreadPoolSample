// --------------------------------------------------------------------------------------------------------------------
// <copyright file="StringProcessor.h" company="Grass Valley K.K.">
//   Copyright (C) 2016 Grass Valley K.K. All rights reserved.
// </copyright>
// --------------------------------------------------------------------------------------------------------------------
#pragma once
#include <fstream>
#include <string>

template <class FUNCTYPE>
class StringProcessorT
{
	const FUNCTYPE& m_func;

public:
	explicit StringProcessorT(const FUNCTYPE& func)
		: m_func(func)
	{
	}

	int Process(const std::string& buf) const
	{
		return m_func(buf);
	}

	int ProcessFile(const char* filename) const
	{
		std::ifstream ifs(filename);
		if (!ifs)
		{
			return 0;
		}

		std::string buf;
		int sum = 0;
		for (;;)
		{
			std::getline(ifs, buf);
			if (ifs.eof())
				break;
			sum += m_func(buf);
		}

		return sum;
	}
};

template <class FUNCTYPE>
StringProcessorT<FUNCTYPE> MakeStringProcessor(const FUNCTYPE& func)
{
	return StringProcessorT<FUNCTYPE>(func);
}
