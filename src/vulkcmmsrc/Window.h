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

        [[nodiscard]] bool shouldClose() { return (bool)(glfwWindowShouldClose(window)); }
        [[nodiscard]] inline VkExtent2D getExtent() const { return {C_UI32T(width), C_UI32T(height)}; }

        [[nodiscard]] inline bool wasWindowResized() const { return framebufferResized; }
        void resetWindowResizedFlag() { framebufferResized = false; }

        void createWindowSurface(VkInstance instance, VkSurfaceKHR *surface);

    private:
        static void framebufferResizeCallback(GLFWwindow *window, int width, int height);
        void initWindow();

        int width{};
        int height{};
        bool framebufferResized = false;

        std::string windowName;
        GLFWwindow *window = nullptr;
    };
}  // namespace lve
