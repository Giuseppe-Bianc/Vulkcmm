#include "Timer.h"
#include "VKRAppError.h"
#include "headers.h"

// This file will be generated automatically when cur_you run the CMake
// configuration step. It creates a namespace called `Vulkcmm`. You can modify
// the source template at `configured_files/config.hpp.in`.
#include "first_app.hpp"

// NOLINTNEXTLINE(bugprone-exception-escape)
int main([[maybe_unused]] int argc, [[maybe_unused]] const char **argv) {
    spdlog::set_pattern(R"(%^[%T] [%l] %v%$)");
    const auto console = spdlog::stdout_color_mt(R"(console)");
    spdlog::set_default_logger(console);
    lve::FirstApp app{};

    try {
        app.run();
    } catch(const std::exception &e) {
        LCRITICAL("Unhandled exception in main: {}", e.what());
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}
