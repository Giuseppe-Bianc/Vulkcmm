#include "App.h"
#include "Camera.h"
#include "Keyboard_movement_controller.h"
#include "Simple_render_system.h"

namespace lve {
    static inline constexpr float val1 = .5f;
    static inline constexpr float val2 = .9f;
    static inline constexpr float val3 = .8f;
    static inline constexpr float val4 = .1f;

    void FirstApp::updateFrameRate(const long double &frametime) {
        frameCount++;
        totalTime += frametime;

        if(totalTime >= 1.0L) {
            long double fps = (frameCount / totalTime);
            lveWindow.setTitle(std::format("{} FPS: {} frametime {}", titleBase, fps, (frametime * 1000)).c_str());

            // Reset counters
            frameCount = 0;
            totalTime = 0.0;
        }
    }

    DISABLE_WARNINGS_PUSH(26455)
    FirstApp::FirstApp() { loadGameObjects(); }
    DISABLE_WARNINGS_POP()

    void FirstApp::run() {
        SimpleRenderSystem simpleRenderSystem{lveDevice, lveRenderer.getSwapChainRenderPass()};
        LveCamera camera{};
        auto viewerObject = LveGameObject::createGameObject();
        const KeyboardMovementController cameraController{};

        auto currentTime = std::chrono::high_resolution_clock::now();
        while(!lveWindow.shouldClose()) {
            glfwPollEvents();

            const auto newTime = std::chrono::high_resolution_clock::now();
            const auto frameTime = std::chrono::duration<long double, std::chrono::seconds::period>(newTime - currentTime)
                                       .count();
            currentTime = newTime;
            updateFrameRate(frameTime);

            cameraController.moveInPlaneXZ(lveWindow.getGLFWwindow(), frameTime, viewerObject);
            camera.setViewYXZ(viewerObject.transform.translation, viewerObject.transform.rotation);
            const float aspect = lveRenderer.getAspectRatio();
            // camera.setOrthographicProjection(-aspect, aspect, -1, 1, -1, 1);
            camera.setPerspectiveProjection(glm::radians(50.f), aspect, 0.1f, 10.f);
            if(auto commandBuffer = lveRenderer.beginFrame()) {
                lveRenderer.beginSwapChainRenderPass(commandBuffer);
                simpleRenderSystem.renderGameObjects(commandBuffer, gameObjects, camera);
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
        cube.transform.translation = {.0f, .0f, 2.5f};
        cube.transform.scale = {val1, val1, val1};
        gameObjects.emplace_back(std::move(cube));
    }

}  // namespace lve
