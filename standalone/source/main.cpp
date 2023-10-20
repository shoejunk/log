import stk.log;

#pragma warning(push)
#pragma warning(disable: 5050) // _M_FP_PRECISE is defined in current command line and not in module command line
import std.core;
#pragma warning(pop)
import <Windows.h>;

using namespace stk;

int main()
{
	logln("{0}, {1}!", "Hello", "World");
	logln("And hello to you too!");
	debug("This is a debug message\n");
	debugln("This is another debug message");
	debugln("And another");
	error("This is an error message\n");
	errorln("This is another error message");
	logln({ "boop"_h }, "Boop test 1!");
	logln.enable<"boop"_h>();
	logln({ "boop"_h }, "Boop test 2!");
	logln.disable<"boop"_h>();
	logln({ "boop"_h }, "Boop test 3!");
	return 0;
}
