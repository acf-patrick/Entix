#include <iostream>

#include "entry.h"
#include "../application/application.h"

extern Application* createApp();

int main(int argc, char** argv)
{
    std::cout << "Creating main application" << std::endl;
    Application* app = createApp();

    if (!app)
    {
        std::cerr << "Failed to create main application" << std::endl;
        exit(1);
    }

    app->run();

    std::cout << "Deleting application" << std::endl;
    delete app;

    return 0;
}