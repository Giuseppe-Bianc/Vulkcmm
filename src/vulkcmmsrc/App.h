#pragma once

#include "Model.h"
#include "Pipeline.h"
#include "Swap_chain.h"
#include "Window.h"

namespace lve {
    class FirstApp {
    public:
        static constexpr int WIDTH = 800;
        static constexpr int HEIGHT = 600;

        FirstApp();
        ~FirstApp();

        FirstApp(const FirstApp &) = delete;
        void operator=(const FirstApp &) = delete;

        void run();

    private:
        void createPipelineLayout();
        void createPipeline();
        void createCommandBuffers();
        void drawFrame();

        LveWindow lveWindow{WIDTH, HEIGHT, "Hello Vulkan!"};
        LveDevice lveDevice{lveWindow};
        LveSwapChain lveSwapChain{lveWindow, lveDevice};
        VkPipelineLayout pipelineLayout;
        std::unique_ptr<LvePipeline> simplePipeline;
        std::vector<VkCommandBuffer> commandBuffers;
        LveModel lveModel{lveDevice};
    };
}  // namespace lve
