#pragma once

#include <string>

namespace NStk
{
	class CLogger
	{
	public:
		void Log(std::string const&& ksMessage);
	};
}