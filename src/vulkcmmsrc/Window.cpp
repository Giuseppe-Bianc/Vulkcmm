#include "Window.h"

#ifdef _DEBUG
static constexpr long double TIME_DIVISOR = C_LD(1000.0);
#else
static constexpr long double TIME_DIVISOR = C_LD(1'000'000.0);
#endif  // _DEBUG
namespace lve {

    LveWindow::LveWindow(int ww, int hh, const std::string &name) : width{ww}, height{hh}, windowName{name} { initWindow(); }

    LveWindow::~LveWindow() {
        glfwDestroyWindow(window);
        glfwTerminate();
    }

    void LveWindow::initWindow() {
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
        //        glfwWindowHint(GLFW_DOUBLEBUFFER, GLFW_TRUE);
        glfwWindowHint(GLFW_SCALE_TO_MONITOR, GLFW_TRUE);
        glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);
        tt.stop();
#ifdef _DEBUG
        const auto glfwhintstime = tt.elapsedMicroseconds();
#else
        const auto glfwhintstime = tt.elapsedNanoseconds();
#endif  // _DEBUG
        Timer ttt;
        window = glfwCreateWindow(width, height, windowName.c_str(), nullptr, nullptr);
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
#ifdef _DEBUG
        const auto primaryMonitortime = tttt.elapsedMicroseconds();
#else
        const auto primaryMonitortime = tttt.elapsedNanoseconds();
#endif  // _DEBUG

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

        static const auto totalsetuptime = glfwsetuptime + (glfwhintstime / TIME_DIVISOR) + wcreationtime +
                                           (primaryMonitortime / TIME_DIVISOR) + (modetime / TIME_DIVISOR) +
                                           (repossetuptime / C_LD(1000.0)) + (wrepostime / C_LD(1000.0));

        LINFO("setup time for glfw = {0} ms", glfwsetuptime);
        INFO_LOG_TIME("Time spent on GLFW Hints Setup", glfwhintstime);
        LINFO("time for creating the window {0}: (w: {1}, h: {2}, pos:(x:{3}, y:{4})) = {5} ms", windowTitle.data(), w, h,
              centerX, centerY, wcreationtime);
        INFO_LOG_TIME("Time taken to retrieve the primary monitor", primaryMonitortime);
        LINFO("Time taken to retrieve the primary monitor mode = {0} us", modetime);
        LINFO("Time taken for the setup of window repositioning = {0} us", repossetuptime);
        LINFO("time for repositionig the window{0} = {1} us", windowTitle.data(), wrepostime);
        LINFO("toltal setup time = {0} ms", totalsetuptime);
    }

    void LveWindow::createWindowSurface(VkInstance instance, VkSurfaceKHR *surface) {
        if(glfwCreateWindowSurface(instance, window, nullptr, surface) != VK_SUCCESS) {
            throw std::runtime_error("failed to create window surface!");
        }
    }
}  // namespace lve
