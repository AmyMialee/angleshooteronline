#include "main/cpp/angleshooter/PreCompiledHeaders.h"
#include "main/cpp/angleshooter/util/Util.h"

#include <iostream>

int main(int argc, char* argv[]) {
    std::cout << "Common Start" << '\n';
    std::cout << Util::getCommon() << '\n';
    return 0;
}