#include <log/logger.h>

#include <iostream>

namespace NStk
{
	void CLogger::Log(std::string const&& ksMessage)
	{
		std::cout << ksMessage << std::endl;
	}
}