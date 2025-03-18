#include "PreCompiledServer.h"

int main(int argc, char* argv[]) {
    try {
        Logger::debug("Starting Angle Shooter Server");
        // AngleShooterServer app;
        // app.run();
    } catch(std::runtime_error& e) {
        Logger::error(e.what());
    } catch(...) {
        Logger::error("An Error Occurred");
    }
    return 0;
}