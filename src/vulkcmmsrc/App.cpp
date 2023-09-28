#include "App.h"
#include "Simple_render_system.h"

namespace lve {

    FirstApp::FirstApp() { loadGameObjects(); }

    void FirstApp::run() {
        SimpleRenderSystem simpleRenderSystem{lveDevice, lveRenderer.getSwapChainRenderPass()};

        while(!lveWindow.shouldClose()) {
            glfwPollEvents();

            if(auto commandBuffer = lveRenderer.beginFrame()) {
                lveRenderer.beginSwapChainRenderPass(commandBuffer);
                simpleRenderSystem.renderGameObjects(commandBuffer, gameObjects);
                lveRenderer.endSwapChainRenderPass(commandBuffer);
                lveRenderer.endFrame();
            }
        }

        vkDeviceWaitIdle(lveDevice.device());
    }

    void FirstApp::loadGameObjects() {
        const std::vector<LveModel::Vertex> vertices{
            {{0.0f, -0.5f}, {1.0f, 0.0f, 0.0f}}, {{0.5f, 0.5f}, {0.0f, 1.0f, 0.0f}}, {{-0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}}};
        auto lveModel = std::make_shared<LveModel>(lveDevice, vertices);

        auto triangle = LveGameObject::createGameObject();
        triangle.model = lveModel;
        triangle.color = {.1f, .8f, .1f};
        triangle.transform2d.translation.x = .2f;
        triangle.transform2d.scale = {2.f, .5f};
        triangle.transform2d.rotation = .25f * glm::two_pi<float>();

        gameObjects.push_back(std::move(triangle));
    }

}  // namespace lve
