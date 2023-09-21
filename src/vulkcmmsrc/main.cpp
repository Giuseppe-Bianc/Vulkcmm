#include "Timer.h"
#include "VKRAppError.h"
#include "headers.h"

#include <CLI/CLI.hpp>

// This file will be generated automatically when cur_you run the CMake
// configuration step. It creates a namespace called `Vulkcmm`. You can modify
// the source template at `configured_files/config.hpp.in`.
#include "App.h"
#include <internal_use_only/config.hpp>

// NOLINTNEXTLINE(bugprone-exception-escape)
int main([[maybe_unused]] int argc, [[maybe_unused]] const char **argv) {
    spdlog::set_pattern(R"(%^[%T] [%l] %v%$)");
    const auto console = spdlog::stdout_color_mt(R"(console)");
    spdlog::set_default_logger(console);
    try {
        CLI::App app{fmt::format("{} version {}", Vulkcmm::cmake::project_name, Vulkcmm::cmake::project_version)};
        LINFO("{} version {}", Vulkcmm::cmake::project_name, Vulkcmm::cmake::project_version);
        /*
        std::optional<std::string> message;
        app.add_option("-m,--message", message, "A message to print back out");
        bool show_version = false;
        app.add_flag("--version", show_version, "Show version information");

        bool is_turn_based = false;
        auto *turn_based = app.add_flag("--turn_based", is_turn_based);

        bool is_loop_based = false;
        auto *loop_based = app.add_flag("--loop_based", is_loop_based);

        turn_based->excludes(loop_based);
        loop_based->excludes(turn_based);


        CLI11_PARSE(app, argc, argv);
        */

    } catch(const std::exception &e) { LCRITICAL("Unhandled exception in main: {}", e.what()); }
    lve::FirstApp app{};

    try {
        app.run();
    } catch(const std::exception &e) {
        LCRITICAL("Unhandled exception in main: {}", e.what());
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}
