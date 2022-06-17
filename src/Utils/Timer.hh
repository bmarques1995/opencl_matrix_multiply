#ifndef TIMER_HH
#define TIMER_HH

#include <chrono>

class Timer
{
public:
	static void Start();
	static void End();

	static void Log();
private:
	static bool s_Running;
	static std::chrono::steady_clock::time_point s_Start;
	static std::chrono::steady_clock::time_point s_End;
};

#endif