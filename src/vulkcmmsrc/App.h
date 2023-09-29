#pragma once

#include "Device.h"
#include "FPSCounter.h"
#include "Game_object.h"
#include "Pipeline.h"
#include "Renderer.h"
#include "Window.h"

namespace lve {
    class FirstApp {
    public:
        static constexpr int WIDTH = 800;
        static constexpr int HEIGHT = 600;
        DISABLE_WARNINGS_PUSH(26455)
        FirstApp();
        ~FirstApp() noexcept = default;

        FirstApp(const FirstApp &) = delete;
        FirstApp &operator=(const FirstApp &) = delete;
        FirstApp(FirstApp &&) = delete;
        FirstApp &operator=(FirstApp &&) = delete;
        DISABLE_WARNINGS_POP()

        void run();

    private:
        void loadGameObjects();
        void updateFrameRate(FPSCounter &counter);

        LveWindow lveWindow{w, h, "Vulkan Tutorial"};
        LveDevice lveDevice{lveWindow};
        LveRenderer lveRenderer{lveWindow, lveDevice};

        std::vector<LveGameObject> gameObjects;
    };
}  // namespace lve
