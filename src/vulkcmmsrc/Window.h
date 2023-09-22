#pragma once
#include "Timer.h"
#include "VKRAppError.h"
#include "headers.h"

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

namespace lve {

    class LveWindow {
    public:
        LveWindow() = default;
        LveWindow(int w, int h, const std::string &name);
        ~LveWindow();

        LveWindow(const LveWindow &) = delete;
        LveWindow &operator=(const LveWindow &) = delete;

        bool shouldClose() { return (bool)(glfwWindowShouldClose(window)); }
        void createWindowSurface(VkInstance instance, VkSurfaceKHR *surface);

        [[nodiscard]] inline int getWidth() const { return width; }
        [[nodiscard]] inline int getHeight() const { return height; }

    private:
        void initWindow();

        const int width;
        const int height;

        std::string windowName;
        GLFWwindow *window;
    };
}  // namespace lve
