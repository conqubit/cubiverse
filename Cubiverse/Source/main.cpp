#include "stdafx.h"

#include "System.h"

int main(int argc, char* argv[]) {
	if (!System::Init()) {
		return 0;
	}

	System::Start();
	System::Shutdown();
	
	return 0;
}