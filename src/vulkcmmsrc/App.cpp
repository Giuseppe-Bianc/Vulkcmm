#include "App.h"
#include "Simple_render_system.h"

namespace lve {
    static inline constexpr float val1 = .5f;
    static inline constexpr float val2 = .9f;
    static inline constexpr float val3 = .8f;
    static inline constexpr float val4 = .1f;
    static inline constexpr std::string_view titleBase = "Vulkan Tutorial. ";

    void FirstApp::updateFrameRate(FPSCounter &counter) {
        counter.update();
        auto fps = counter.getFPS();
        if(fps != 0) { lveWindow.setTitle(std::format("{} {} fps. ", titleBase, fps).c_str()); }
    }

    DISABLE_WARNINGS_PUSH(26455)
    FirstApp::FirstApp() { loadGameObjects(); }
    DISABLE_WARNINGS_POP()

    void FirstApp::run() {
        SimpleRenderSystem simpleRenderSystem{lveDevice, lveRenderer.getSwapChainRenderPass()};
        FPSCounter counter;

        while(!lveWindow.shouldClose()) {
            glfwPollEvents();
            updateFrameRate(counter);
            if(auto commandBuffer = lveRenderer.beginFrame()) {
                lveRenderer.beginSwapChainRenderPass(commandBuffer);
                simpleRenderSystem.renderGameObjects(commandBuffer, gameObjects);
                lveRenderer.endSwapChainRenderPass(commandBuffer);
                lveRenderer.endFrame();
            }
        }

        vkDeviceWaitIdle(lveDevice.device());
    }

    std::unique_ptr<LveModel> createCubeModel(LveDevice &device, glm::vec3 offset) {
        DISABLE_WARNINGS_PUSH(26496)
        Timer t;
        DISABLE_WARNINGS_POP()
        std::vector<LveModel::Vertex> vertices{
            // left face (white)
            {{-val1, -val1, -val1}, {val2, val2, val2}},
            {{-val1, val1, val1}, {val2, val2, val2}},
            {{-val1, -val1, val1}, {val2, val2, val2}},
            {{-val1, -val1, -val1}, {val2, val2, val2}},
            {{-val1, val1, -val1}, {val2, val2, val2}},
            {{-val1, val1, val1}, {val2, val2, val2}},

            // right face (yellow)
            {{val1, -val1, -val1}, {val3, val3, val4}},
            {{val1, val1, val1}, {val3, val3, val4}},
            {{val1, -val1, val1}, {val3, val3, val4}},
            {{val1, -val1, -val1}, {val3, val3, val4}},
            {{val1, val1, -val1}, {val3, val3, val4}},
            {{val1, val1, val1}, {val3, val3, val4}},

            // top face (orange, remember y axis points down)
            {{-val1, -val1, -val1}, {val2, .6f, val4}},
            {{val1, -val1, val1}, {val2, .6f, val4}},
            {{-val1, -val1, val1}, {val2, .6f, val4}},
            {{-val1, -val1, -val1}, {val2, .6f, val4}},
            {{val1, -val1, -val1}, {val2, .6f, val4}},
            {{val1, -val1, val1}, {val2, .6f, val4}},

            // bottom face (red)
            {{-val1, val1, -val1}, {val3, val4, val4}},
            {{val1, val1, val1}, {val3, val4, val4}},
            {{-val1, val1, val1}, {val3, val4, val4}},
            {{-val1, val1, -val1}, {val3, val4, val4}},
            {{val1, val1, -val1}, {val3, val4, val4}},
            {{val1, val1, val1}, {val3, val4, val4}},

            // nose face (blue)
            {{-val1, -val1, val1}, {val4, val4, val3}},
            {{val1, val1, val1}, {val4, val4, val3}},
            {{-val1, val1, val1}, {val4, val4, val3}},
            {{-val1, -val1, val1}, {val4, val4, val3}},
            {{val1, -val1, val1}, {val4, val4, val3}},
            {{val1, val1, val1}, {val4, val4, val3}},

            // tail face (green)
            {{-val1, -val1, -val1}, {val4, val3, val4}},
            {{val1, val1, -val1}, {val4, val3, val4}},
            {{-val1, val1, -val1}, {val4, val3, val4}},
            {{-val1, -val1, -val1}, {val4, val3, val4}},
            {{val1, -val1, -val1}, {val4, val3, val4}},
            {{val1, val1, -val1}, {val4, val3, val4}},

        };
        t.elapsedMcsToString("vertices setupp");
        for(auto &v : vertices) { v.position += offset; }
        return std::make_unique<LveModel>(device, vertices);
    }

    void FirstApp::loadGameObjects() {
        std::shared_ptr<LveModel> lveModel = createCubeModel(lveDevice, {.0f, .0f, .0f});
        auto cube = LveGameObject::createGameObject();
        cube.model = lveModel;
        cube.transform.translation = {.0f, .0f, val1};
        cube.transform.scale = {val1, val1, val1};
        gameObjects.emplace_back(std::move(cube));
    }

}  // namespace lve
