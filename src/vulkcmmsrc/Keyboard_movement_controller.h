#pragma once

#include "Game_object.h"
#include "Window.h"

namespace lve {
    class KeyboardMovementController {
    public:
        struct KeyMappings {
            int moveLeft = GLFW_KEY_A;
            int moveRight = GLFW_KEY_D;
            int moveForward = GLFW_KEY_W;
            int moveBackward = GLFW_KEY_S;
            int moveUp = GLFW_KEY_E;
            int moveDown = GLFW_KEY_Q;
            int lookLeft = GLFW_KEY_LEFT;
            int lookRight = GLFW_KEY_RIGHT;
            int lookUp = GLFW_KEY_UP;
            int lookDown = GLFW_KEY_DOWN;
        };

        void moveInPlaneXZ(GLFWwindow *window, long double dt, LveGameObject &gameObject) const;

        KeyMappings keys{};
        double moveSpeed{3.0};
        double lookSpeed{1.5};
    };
}  // namespace lve
