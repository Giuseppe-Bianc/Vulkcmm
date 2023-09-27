#pragma once

#include "Device.h"

namespace lve {
    class LveModel {
    public:
        struct Vertex {
            glm::vec2 position{};
            glm::vec3 color{};

            static std::vector<VkVertexInputBindingDescription> getBindingDescriptions();
            static std::vector<VkVertexInputAttributeDescription> getAttributeDescriptions();
        };

        LveModel(LveDevice &device, const std::vector<Vertex> &vertices);
        ~LveModel();

        LveModel(const LveModel &) = delete;
        LveModel &operator=(const LveModel &) = delete;

        void draw(VkCommandBuffer commandBuffer) const noexcept;
        void bind(VkCommandBuffer commandBuffer) noexcept;

    private:
        void createVertexBuffers(const std::vector<Vertex> &vertices);

        LveDevice &lveDevice;
        VkBuffer vertexBuffer;
        VkDeviceMemory vertexBufferMemory;
        uint32_t vertexCount;
    };
}  // namespace lve
