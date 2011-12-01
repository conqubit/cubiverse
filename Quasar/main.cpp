#include "stdafx.h"

#include "System.h"

int main(int argc, char** argv) {
    if (!System::Init()) {
        return EXIT_FAILURE;
    }

    System::Start();
    System::Shutdown();

    return EXIT_SUCCESS;
}
