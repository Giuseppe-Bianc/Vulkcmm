#pragma once

#include "headers.h"

namespace lve {

    class LveCamera {
    public:
        void setOrthographicProjection(float left, float right, float top, float bottom, float nearp, float farp);

        void setPerspectiveProjection(float fovy, float aspect, float nearp, float farp);

        void setViewDirection(glm::vec3 position, glm::vec3 direction, glm::vec3 up = glm::vec3{0.f, -1.f, 0.f});
        void setViewTarget(glm::vec3 position, glm::vec3 target, glm::vec3 up = glm::vec3{0.f, -1.f, 0.f});
        void setViewYXZ(glm::vec3 position, glm::vec3 rotation);

        [[nodiscard]] inline const glm::mat4 &getProjection() const noexcept { return projectionMatrix; }
        [[nodiscard]] inline const glm::mat4 &getView() const noexcept { return viewMatrix; }

    private:
        glm::mat4 projectionMatrix{1.f};
        glm::mat4 viewMatrix{1.f};
    };
}  // namespace lve
