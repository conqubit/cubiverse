#pragma once

#include "Logger.h"

class System {
public:
	static bool			 running;
	static FileLogger	 errorLog;

	static bool Init();
	static void Shutdown();

	static void Start();
	static void Update();
	static void Stop();

	static void DoTicks();
	static void Tick();
	static void SleepSeconds(double seconds);
	static void SleepMillis(int millisec);
};