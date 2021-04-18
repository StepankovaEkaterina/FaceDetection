#include <iostream>

#include "app/app.h"

int main(int argc, char const *argv[])
{
    facedetection::App app(argc, argv);
    return app.run();
}
