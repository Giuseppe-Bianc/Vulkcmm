#pragma once

#include "Device.h"
#include "FPSCounter.h"
#include "Game_object.h"
#include "Renderer.h"
#include "Window.h"

namespace lve {
    static inline constexpr std::string_view titleBase = "Vulkan Tutorial. ";
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
        void updateFrameRate(const long double &frametime);

        LveWindow lveWindow{w, h, titleBase.data()};
        LveDevice lveDevice{lveWindow};
        LveRenderer lveRenderer{lveWindow, lveDevice};

        std::vector<LveGameObject> gameObjects;
        int frameCount{};
        long double totalTime{};
    };
}  // namespace lve
