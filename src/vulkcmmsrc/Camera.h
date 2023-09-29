#pragma once

#include "headers.h"

namespace lve {

    class LveCamera {
    public:
        void setOrthographicProjection(float left, float right, float top, float bottom, float near, float far);

        void setPerspectiveProjection(float fovy, float aspect, float near, float far);

        const glm::mat4 &getProjection() const noexcept { return projectionMatrix; }

    private:
        glm::mat4 projectionMatrix{1.f};
    };
}  // namespace lve
