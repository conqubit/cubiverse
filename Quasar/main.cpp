#include "stdafx.h"

#include "System.h"
#include "StaticInit.h"

int main(int argc, char* argv[]) {
    StaticInit();

    if (!System::Init()) {
        return EXIT_FAILURE;
    }

    System::Start();
    System::Shutdown();

    return EXIT_SUCCESS;
}