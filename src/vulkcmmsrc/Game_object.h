#pragma once

#include "Model.h"

namespace lve {

    struct Transform2dComponent {
        glm::vec2 translation{};  // (position offset)
        glm::vec2 scale{1.f, 1.f};
        float rotation;

        glm::mat2 mat2() {
            const float s = glm::sin(rotation);
            const float c = glm::cos(rotation);
            const glm::mat2 rotMatrix{{c, s}, {-s, c}};

            const glm::mat2 scaleMat{{scale.x, .0f}, {.0f, scale.y}};
            return rotMatrix * scaleMat;
        }
    };

    class LveGameObject {
    public:
        using id_t = unsigned int;
#pragma optimize("gt", on)
        static LveGameObject createGameObject() {
            static id_t currentId = 0;
            return LveGameObject{currentId++};
        }

        LveGameObject(const LveGameObject &) = delete;
        LveGameObject &operator=(const LveGameObject &) = delete;
        LveGameObject(LveGameObject &&) = default;
        LveGameObject &operator=(LveGameObject &&) = default;
#pragma optimize("gt", on)
        [[nodiscard]] inline id_t getId() const noexcept { return id; }

        std::shared_ptr<LveModel> model{};
        glm::vec3 color{};
        Transform2dComponent transform2d{};

    private:
#pragma optimize("gt", on)
        explicit LveGameObject(id_t objId) : id{objId} {}

        id_t id;
    };
}  // namespace lve
