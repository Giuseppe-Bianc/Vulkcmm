#pragma once

#include "Device.h"

namespace lve {

    class LveSwapChain {
    public:
        static constexpr inline int MAX_FRAMES_IN_FLIGHT = 2;
#pragma optimize("gt", on)
        LveSwapChain(LveDevice &deviceRef, VkExtent2D windowExtent);
        ~LveSwapChain();

        LveSwapChain(const LveSwapChain &) = delete;
        LveSwapChain &operator=(const LveSwapChain &) = delete;

        [[nodiscard]] inline VkFramebuffer getFrameBuffer(int index) { return swapChainFramebuffers[index]; }
        [[nodiscard]] inline VkRenderPass getRenderPass() { return renderPass; }
        [[nodiscard]] inline VkImageView getImageView(int index) { return swapChainImageViews[index]; }
        [[nodiscard]] inline size_t imageCount() const { return swapChainImages.size(); }
        [[nodiscard]] inline VkFormat getSwapChainImageFormat() const { return swapChainImageFormat; }
        [[nodiscard]] inline VkExtent2D getSwapChainExtent() const { return swapChainExtent; }
        [[nodiscard]] inline uint32_t width() const { return swapChainExtent.width; }
        [[nodiscard]] inline uint32_t height() const { return swapChainExtent.height; }

        [[nodiscard]] inline float extentAspectRatio() const { return C_F(swapChainExtent.width) / C_F(swapChainExtent.height); }
        VkFormat findDepthFormat();

        VkResult acquireNextImage(uint32_t *imageIndex);
        VkResult submitCommandBuffers(const VkCommandBuffer *buffers, const uint32_t *imageIndex);

    private:
        void createSwapChain();
        void createImageViews();
        void createDepthResources();
        void createRenderPass();
        void createFramebuffers();
        void createSyncObjects();

        // Helper functions
        [[nodiscard]] VkSurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR> &availableFormats) const;
        [[nodiscard]] VkPresentModeKHR chooseSwapPresentMode(const std::vector<VkPresentModeKHR> &availablePresentModes) const;
        [[nodiscard]] VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR &capabilities) const;

        VkFormat swapChainImageFormat;
        VkExtent2D swapChainExtent;

        std::vector<VkFramebuffer> swapChainFramebuffers;
        VkRenderPass renderPass;

        std::vector<VkImage> depthImages;
        std::vector<VkDeviceMemory> depthImageMemorys;
        std::vector<VkImageView> depthImageViews;
        std::vector<VkImage> swapChainImages;
        std::vector<VkImageView> swapChainImageViews;

        LveDevice &device;
        VkExtent2D windowExtent;

        VkSwapchainKHR swapChain;

        std::vector<VkSemaphore> imageAvailableSemaphores;
        std::vector<VkSemaphore> renderFinishedSemaphores;
        std::vector<VkFence> inFlightFences;
        std::vector<VkFence> imagesInFlight;
        size_t currentFrame = 0;
    };

}  // namespace lve
