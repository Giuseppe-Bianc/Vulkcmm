#pragma once
#include "headers.h"

class VKRAppError : public std::runtime_error {
public:
    explicit VKRAppError(std::string_view message) : std::runtime_error(message.data()) {}
};
