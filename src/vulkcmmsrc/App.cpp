#include "App.h"

namespace lve {

    FirstApp::FirstApp() {
        loadModels();
        createPipelineLayout();
        createPipeline();
        createCommandBuffers();
    }

    FirstApp::~FirstApp() { vkDestroyPipelineLayout(lveDevice.device(), pipelineLayout, nullptr); }

    void FirstApp::run() {
        while(!lveWindow.shouldClose()) {
            glfwPollEvents();
            drawFrame();
        }

        vkDeviceWaitIdle(lveDevice.device());
    }

    void FirstApp::loadModels() {
        const std::vector<LveModel::Vertex> vertices{
            {{0.0f, -0.5f}, {1.0f, 0.0f, 0.0f}}, {{0.5f, 0.5f}, {0.0f, 1.0f, 0.0f}}, {{-0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}}};
        lveModel = std::make_unique<LveModel>(lveDevice, vertices);
    }

    void FirstApp::createPipelineLayout() {
        VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
        pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
        pipelineLayoutInfo.setLayoutCount = 0;
        pipelineLayoutInfo.pSetLayouts = nullptr;
        pipelineLayoutInfo.pushConstantRangeCount = 0;
        pipelineLayoutInfo.pPushConstantRanges = nullptr;
        if(vkCreatePipelineLayout(lveDevice.device(), &pipelineLayoutInfo, nullptr, &pipelineLayout) != VK_SUCCESS) {
            throw VKRAppError("failed to create pipeline layout!");
        }
    }

    void FirstApp::createPipeline() {
        PipelineConfigInfo pipelineConfig{};
        LvePipeline::defaultPipelineConfigInfo(pipelineConfig, lveSwapChain->width(), lveSwapChain->height());
        pipelineConfig.renderPass = lveSwapChain->getRenderPass();
        pipelineConfig.pipelineLayout = pipelineLayout;
        lvePipeline = std::make_unique<LvePipeline>(lveDevice, "../../../shaders/vert.spv", "../../../shaders/frag.spv",
                                                    pipelineConfig);
    }

    void FirstApp::createCommandBuffers() {
        commandBuffers.resize(lveSwapChain->imageCount());

        VkCommandBufferAllocateInfo allocInfo{};
        allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
        allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
        allocInfo.commandPool = lveDevice.getCommandPool();
        allocInfo.commandBufferCount = static_cast<uint32_t>(commandBuffers.size());

        if(vkAllocateCommandBuffers(lveDevice.device(), &allocInfo, commandBuffers.data()) != VK_SUCCESS) {
            throw VKRAppError("failed to allocate command buffers!");
        }

        for(size_t i = 0; i < commandBuffers.size(); i++) {
            VkCommandBufferBeginInfo beginInfo{};
            beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

            if(vkBeginCommandBuffer(commandBuffers[i], &beginInfo) != VK_SUCCESS) {
                throw VKRAppError("failed to begin recording command buffer!");
            }

            VkRenderPassBeginInfo renderPassInfo{};
            renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
            renderPassInfo.renderPass = lveSwapChain->getRenderPass();
            renderPassInfo.framebuffer = lveSwapChain->getFrameBuffer(C_I(i));

            renderPassInfo.renderArea.offset = {0, 0};
            renderPassInfo.renderArea.extent = lveSwapChain->getSwapChainExtent();

            std::array<VkClearValue, 2> clearValues{};
            clearValues[0].color = {0.1f, 0.1f, 0.1f, 1.0f};
            clearValues[1].depthStencil = {1.0f, 0};
            renderPassInfo.clearValueCount = C_UI32T(clearValues.size());
            renderPassInfo.pClearValues = clearValues.data();

            vkCmdBeginRenderPass(commandBuffers[i], &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

            lvePipeline->bind(commandBuffers[i]);

            lveModel->bind(commandBuffers[i]);
            lveModel->draw(commandBuffers[i]);

            vkCmdEndRenderPass(commandBuffers[i]);
            VK_CHECK(vkEndCommandBuffer(commandBuffers[i]), VKRAppError("failed to record command buffer!"));
        }
    }

    void FirstApp::drawFrame() {
        // NOLINT
        uint32_t imageIndex{};
        auto result = lveSwapChain->acquireNextImage(&imageIndex);
        if(result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR) { throw VKRAppError("failed to acquire swap chain image"); }

        result = lveSwapChain->submitCommandBuffers(&commandBuffers[imageIndex], &imageIndex);
        VK_CHECK(result, VKRAppError("failed to present swap chain image!"));
    }
}  // namespace lve
