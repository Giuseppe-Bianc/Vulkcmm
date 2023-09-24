#include "Model.h"

namespace lve {
#pragma optimize("gt", on)
    LveModel::LveModel(LveDevice &device) : device{device} { createVertexBuffer(); }

    void LveModel::createVertexBuffer() {
        vertexCount = static_cast<uint32_t>(vertices.size());
        assert(vertexCount >= 3 && "Vertex count must be at least 3");
        VkDeviceSize bufferSize = sizeof(vertices[0]) * vertexCount;
        device.createBuffer(bufferSize, VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
                            VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, vertexBuffer,
                            vertexBufferMemory);

        void *data;
        vkMapMemory(device.device(), vertexBufferMemory, 0, bufferSize, 0, &data);
        memcpy(data, vertices.data(), C_ST(bufferSize));
        vkUnmapMemory(device.device(), vertexBufferMemory);
    }
#pragma optimize("gt", on)
    void LveModel::cleanup() {
        vkDestroyBuffer(device.device(), vertexBuffer, nullptr);
        vkFreeMemory(device.device(), vertexBufferMemory, nullptr);
    }
#pragma optimize("gt", on)
    void LveModel::draw(VkCommandBuffer commandBuffer) const { vkCmdDraw(commandBuffer, vertexCount, 1, 0, 0); }

    void LveModel::bind(VkCommandBuffer commandBuffer) {
        static const std::vector<VkBuffer> buffers = {vertexBuffer};
        static const std::vector<VkDeviceSize> offsets = {0};
        vkCmdBindVertexBuffers(commandBuffer, 0, 1, buffers.data(), offsets.data());
    }
#pragma optimize("gt", on)
    std::vector<VkVertexInputAttributeDescription> LveModel::Vertex::getAttributeDescriptions() {
        std::vector<VkVertexInputAttributeDescription> attributeDescriptions(1, VkVertexInputAttributeDescription{});

        attributeDescriptions[0].binding = 0;
        attributeDescriptions[0].location = 0;
        attributeDescriptions[0].format = VK_FORMAT_R32G32_SFLOAT;
        attributeDescriptions[0].offset = 0;

        return attributeDescriptions;
    }

}  // namespace lve
