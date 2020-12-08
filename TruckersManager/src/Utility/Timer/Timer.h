#pragma once

class Timer
{
private:
	//Variables
	std::chrono::time_point<std::chrono::high_resolution_clock> startTimePoint;
	std::string message;
	bool scopeBased;

	double timeUs;
	double timeMs;
	double timeS;

public:
	//Constructors/Destructors
	Timer(std::string message, bool scopeBased = true);
	virtual ~Timer();

	//Functions
	void Start();
	void Stop();

	//Getter/Setter
	float GetTimeUs() { return this->timeUs; }
	float GetTimeMs() { return this->timeMs; }
	float GetTimeS() {  return this->timeS; }
};