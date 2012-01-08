#include "stdafx.h"

#include "System.h"

#pragma comment (linker, "/subsystem:windows /entry:mainCRTStartup") 

int main(int argc, char* argv[]) {
	if (!System::Init()) {
		return 0;
	}

	System::Start();
	System::Shutdown();
	
	return 0;
}