#include "tmpch.h"
#include "Timer.h"

//Constructors/Destructors
Utility::Timer::Timer(std::string message, bool scopeBased)
	: message(message), scopeBased(scopeBased), timeUs(-1.0f), timeMs(-1.0f), timeS(-1.0f)
{
	if (this->scopeBased)
		this->startTimePoint = std::chrono::high_resolution_clock::now();
}

Utility::Timer::~Timer()
{
	if (this->scopeBased)
		Stop();
}

void Utility::Timer::Start()
{
	this->startTimePoint = std::chrono::high_resolution_clock::now();
}

//Functions
void Utility::Timer::Stop()
{
	auto endTimepoint = std::chrono::high_resolution_clock::now();

	auto start = std::chrono::time_point_cast<std::chrono::microseconds>(startTimePoint).time_since_epoch().count();
	auto end = std::chrono::time_point_cast<std::chrono::microseconds>(endTimepoint).time_since_epoch().count();

	auto duration = end - start;
	double ms = duration * 0.001;
	double s = ms * 0.001;

	this->timeUs = duration;
	this->timeMs = ms;
	this->timeS  = s;

	if (this->message != "")
		Utility::AddLog(this->message + std::to_string(duration) + "us (" + std::to_string(ms) + "ms, " + std::to_string(s) + "s)");
}