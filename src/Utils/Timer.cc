#include "Timer.hh"
#include <iostream>

bool Timer::s_Running = false;
std::chrono::steady_clock::time_point Timer::s_Start;
std::chrono::steady_clock::time_point Timer::s_End;

void Timer::Start()
{
	if (!s_Running)
	{
		s_Start = std::chrono::steady_clock::now();
		s_Running = true;
	}
}

void Timer::End()
{
	if (s_Running)
	{
		s_End = std::chrono::steady_clock::now();
		s_Running = false;
	}
}

void Timer::Log()
{
	if (!s_Running)
		std::cout << "Elapsed: " << std::chrono::duration_cast<std::chrono::nanoseconds>(s_End - s_Start).count() << "ns\n";
}
