export module stk.log;

#pragma warning(push)
#pragma warning(disable: 5050) // _M_FP_PRECISE is defined in current command line and not in module command line
import std.core;
#pragma warning(pop)

//import <cstdarg>;
//import <iostream>;
import <Windows.h>;

namespace stk
{
	export void log(char const* ksFormat, ...)
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