#include "Timer.h"
#include "VKRAppError.h"
#include "headers.h"

#include <CLI/CLI.hpp>

// This file will be generated automatically when cur_you run the CMake
// configuration step. It creates a namespace called `Vulkcmm`. You can modify
// the source template at `configured_files/config.hpp.in`.
#include <internal_use_only/config.hpp>

#pragma optimize("gt", on)
static inline void errorCallback(int error, const char *description) { GLWFERR(error, description) }

#pragma optimize("gt", on)
static inline void keyCallback(GLFWwindow *window, int key, [[maybe_unused]] int scancode, int action,
                               [[maybe_unused]] int mods) noexcept {
    switch(key) {
    case GLFW_KEY_ESCAPE:
        if(action == GLFW_PRESS) {
            glfwSetWindowShouldClose(window, GLFW_TRUE);
            LINFO("escape close");
        }
        break;
    // Add more cases for other keys if needed
    [[likely]] default:
        // Handle other keys here
        break;
    }
}

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

    } catch(const std::exception &e) { spdlog::error("Unhandled exception in main: {}", e.what()); }
    Timer t;
    if(!glfwInit()) [[unlikely]] {
        LCRITICAL("Failed to initialize GLFW.");
        throw VKRAppError("Failed to initialize GLFW");
    }
    if(!glfwVulkanSupported()) {
        glfwTerminate();
        LCRITICAL("Failed to initialize GLFW. vulkan not supported");
        throw VKRAppError("Failed to initialize GLFW. vulkan not supported");
    }
    glfwSetErrorCallback(errorCallback);
    t.stop();
    const auto glfwsetuptime = t.elapsedMilliseconds();
    Timer tt;
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
    glfwWindowHint(GLFW_DOUBLEBUFFER, GLFW_TRUE);
    glfwWindowHint(GLFW_SCALE_TO_MONITOR, GLFW_TRUE);
    glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);
    tt.stop();
    const auto glfwhintstime = tt.elapsedMicroseconds();
    Timer ttt;
    GLFWwindow *window = glfwCreateWindow(w, h, windowTitle.data(), nullptr, nullptr);
    if(window == nullptr) [[unlikely]] {
        LCRITICAL("Failed to create GLFW window.");
        glfwTerminate();
        throw VKRAppError("Failed to create GLFW window");
    }
    glfwSetKeyCallback(window, keyCallback);
    ttt.stop();
    const auto wcreationtime = ttt.elapsedMilliseconds();
    // Ottenimento delle dimensioni del monitor primario
    Timer tttt;
    GLFWmonitor *primaryMonitor = glfwGetPrimaryMonitor();
    if(primaryMonitor == nullptr) [[unlikely]] {
        LCRITICAL("Failed to get the primary monitor.");
        glfwDestroyWindow(window);
        glfwTerminate();
        throw VKRAppError("Failed to get primary monitor");
    }
    tttt.stop();
    const auto primaryMonitortime = tttt.elapsedMicroseconds();
    Timer ttttt;
    const GLFWvidmode *mode = glfwGetVideoMode(primaryMonitor);
    if(mode == nullptr) [[unlikely]] {
        LCRITICAL("Failed to get the video mode of the primary monitor.");
        glfwDestroyWindow(window);
        glfwTerminate();
        throw VKRAppError("Failed to get video mode");
    }
    ttttt.stop();
    const auto modetime = ttttt.elapsedMicroseconds();
    Timer tttttt;
    const int monitorWidth = mode->width;
    const int monitorHeight = mode->height;

    // Calcolo delle coordinate per centrare la finestra
    int windowWidth;
    int windowHeight;
    glfwGetWindowSize(window, &windowWidth, &windowHeight);
    const int centerX = CALC_CENTRO(monitorWidth, windowWidth);
    const int centerY = CALC_CENTRO(monitorHeight, windowHeight);
    tttttt.stop();
    const auto repossetuptime = tttttt.elapsedMicroseconds();
    Timer ttttttt;

    // Posizionamento della finestra al centro del monitor
    glfwSetWindowPos(window, centerX, centerY);

    int posX = 0;
    int posY = 0;
    glfwGetWindowPos(window, &posX, &posY);
    if(posX != centerX || posY != centerY) [[unlikely]] {
        LCRITICAL("Failed to position the window at the center.");
        glfwDestroyWindow(window);
        glfwTerminate();
        throw VKRAppError("Failed to set window position centered");
    }
    const auto wrepostime = ttttttt.elapsedMicroseconds();

    glfwShowWindow(window);
    static const auto totalsetuptime = glfwsetuptime + (glfwhintstime / C_LD(1000.0)) + wcreationtime +
                                       (primaryMonitortime / C_LD(1000.0)) + (modetime / C_LD(1000.0)) +
                                       (repossetuptime / C_LD(1000.0)) + (wrepostime / C_LD(1000.0));
    LINFO("setup time for glfw = {0} ms", glfwsetuptime);
    LINFO("Time spent on GLFW Hints Setup = {0} us", glfwhintstime);
    LINFO("time for creating the window {0}: (w: {1}, h: {2}, pos:(x:{3}, y:{4})) = {5} ms", windowTitle.data(), w, h, centerX,
          centerY, wcreationtime);
    LINFO("Time taken to retrieve the primary monitor = {0} us", primaryMonitortime);
    LINFO("Time taken to retrieve the primary monitor mode = {0} us", modetime);
    LINFO("Time taken for the setup of window repositioning = {0} us", repossetuptime);
    LINFO("time for repositionig the window{0} = {1} us", windowTitle.data(), wrepostime);
    LINFO("toltal setup time = {0} ms", totalsetuptime);
    while(!glfwWindowShouldClose(window)) { glfwPollEvents(); }
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
