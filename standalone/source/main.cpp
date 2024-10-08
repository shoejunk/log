import std;
import stk.log;
import <Windows.h>;

using namespace stk;

int main()
{
	// log, debug, and error
	logln("{1}, {0}!", "World", "Hello");
	logln("And hello to you too!");
	debug("This is a debug message\n");
	debugln("This is another debug message");
	debugln("And another");
	error("This is an error message\n");
	errorln("This is another error message");

	// tags
	logln({ "boop"_h }, "Boop test 1!");
	logln.enable<"boop"_h>();
	logln({ "boop"_h }, "Boop test 2!");
	logln.disable<"boop"_h>();
	logln({ "boop"_h }, "Boop test 3!");

	return 0;
}
