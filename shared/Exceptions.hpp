#pragma once
#include <exception>

namespace ServerBrowser {
    enum class Exceptions {
        NoMember,
        WrongType
    };

    class JsonException : public std::exception {
    private:
        const char* error;
        std::string message;
    public:
        explicit JsonException(Exceptions key, const std::string& message);
        const char* what() const noexcept override {
            return error;
        }
    };
}
