#include <log/log.h>

#include <cstdarg>
#include <iostream>
#include <Windows.h>

namespace NStk::NLog
{
	void Log(char const* ksFormat, ...)
	{
		va_list aList;
		va_start(aList, ksFormat);
		size_t uSize = _vscprintf(ksFormat, aList);
		char* aBuffer = new char[uSize + 1];
		vsprintf_s(aBuffer, uSize + 1, ksFormat, aList);
		OutputDebugStringA(aBuffer);
		std::cout << aBuffer;
		delete[] aBuffer;
		va_end(aList);
	}
}