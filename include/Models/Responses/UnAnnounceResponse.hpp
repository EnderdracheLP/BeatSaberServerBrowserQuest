#pragma once

#include "rapidjson-macros/shared/macros.hpp"

namespace ServerBrowser::Models::Responses {
    DECLARE_JSON_STRUCT(UnAnnounceResponse) {
        VALUE(std::string, Result);
        VALUE(bool, CanRetry);

        bool get_IsOk() {
            return Result == "ok";
        }
        __declspec(property(get = get_IsOk)) std::string IsOk;
    };
}