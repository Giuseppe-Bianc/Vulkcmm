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

    void FirstApp::loadGameObjects() {
        std::shared_ptr<LveModel> lveModel = LveModel::createModelFromFile(lveDevice, "models/smooth_vase.obj");
        auto gameObj = LveGameObject::createGameObject();
        gameObj.model = lveModel;
        gameObj.transform.translation = {.0f, .0f, 2.5f};
        gameObj.transform.scale = glm::vec3(3.f);
        gameObjects.emplace_back(std::move(gameObj));
    }

}  // namespace lve
