#pragma once

#include "Device.h"
#include "Game_object.h"
#include "Pipeline.h"
#include "Renderer.h"
#include "Window.h"

namespace lve {
    class FirstApp {
    public:
        static constexpr int WIDTH = 800;
        static constexpr int HEIGHT = 600;

        FirstApp();
        ~FirstApp() noexcept;

        FirstApp(const FirstApp &) = delete;
        FirstApp &operator=(const FirstApp &) = delete;
        FirstApp(FirstApp &&) = delete;
        FirstApp &operator=(FirstApp &&) = delete;

        void run();

    private:
        void loadGameObjects();

        LveWindow lveWindow{w, h, "Vulkan Tutorial"};
        LveDevice lveDevice{lveWindow};
        LveRenderer lveRenderer{lveWindow, lveDevice};

        std::vector<LveGameObject> gameObjects;
    };
}  // namespace lve
