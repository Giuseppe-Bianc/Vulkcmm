#pragma once

#include "Device.h"

namespace lve {

    class LveSwapChain {
    public:
        static constexpr inline int MAX_FRAMES_IN_FLIGHT = 2;
#pragma optimize("gt", on)
        LveSwapChain(LveWindow &window, LveDevice &device) : window_{window}, device_{device} { init(); }
#pragma optimize("gt", on)
        ~LveSwapChain() {
            cleanupSwapChain();
            cleanupSyncObjects();
        }

        LveSwapChain(const LveSwapChain &) = delete;
        void operator=(const LveSwapChain &) = delete;

        inline VkFramebuffer getFrameBuffer(int index) { return swapChainFramebuffers[index]; }
        inline VkRenderPass getRenderPass() { return renderPass; }
        inline VkImageView getImageView(int index) { return swapChainImageViews[index]; }
        void recreateSwapChain();
        void cleanupSwapChain();
        inline size_t imageCount() const { return swapChainImages.size(); }
        inline VkFormat getSwapChainImageFormat() const { return swapChainImageFormat; }
        inline VkExtent2D getSwapChainExtent() const { return swapChainExtent; }
        inline uint32_t width() const { return swapChainExtent.width; }
        inline uint32_t height() const { return swapChainExtent.height; }

        inline float extentAspectRatio() const { return C_F(swapChainExtent.width) / C_F(swapChainExtent.height); }
        VkFormat findDepthFormat();

        VkResult acquireNextImage(uint32_t *imageIndex);
        VkResult submitCommandBuffers(const VkCommandBuffer *buffers, uint32_t *imageIndex);

    private:
        void init();
        void cleanupSyncObjects();
        void createSwapChain();
        void createImageViews();
        void createDepthResources();
        void createRenderPass();
        void createFramebuffers();
        void createSyncObjects();

        // Helper functions
        VkSurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR> &availableFormats) const;
        VkPresentModeKHR chooseSwapPresentMode(const std::vector<VkPresentModeKHR> &availablePresentModes) const;
        VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR &capabilities) const;

        VkFormat swapChainImageFormat;
        VkExtent2D swapChainExtent;

        std::vector<VkFramebuffer> swapChainFramebuffers;
        VkRenderPass renderPass;

        std::vector<VkImage> depthImages;
        std::vector<VkDeviceMemory> depthImageMemorys;
        std::vector<VkImageView> depthImageViews;
        std::vector<VkImage> swapChainImages;
        std::vector<VkImageView> swapChainImageViews;

        LveWindow &window_;
        LveDevice &device_;

        VkSwapchainKHR swapChain;

        std::vector<VkSemaphore> imageAvailableSemaphores;
        std::vector<VkSemaphore> renderFinishedSemaphores;
        std::vector<VkFence> inFlightFences;
        std::vector<VkFence> imagesInFlight;
        size_t currentFrame = 0;
    };

}  // namespace lve
