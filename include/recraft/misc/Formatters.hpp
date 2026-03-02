#pragma once
#include <mpack.h>

#include <format>
#include <string_view>

// clang-format off
template <>
struct std::formatter<mpack_error_t> : std::formatter<std::string_view> {
    auto format(mpack_error_t err, std::format_context& ctx) const {
        std::string_view text;

        switch (err) {
            case mpack_ok: text = "ok"; break;
            case mpack_error_io: text = "io error"; break;
            case mpack_error_invalid: text = "invalid messagepack"; break;
            case mpack_error_type: text = "type mismatch"; break;
            case mpack_error_too_big: text = "too big"; break;
            case mpack_error_memory: text = "out of memory"; break;
            case mpack_error_bug: text = "mpack api misuse"; break;
            case mpack_error_data: text = "invalid data"; break;
            default: text = "unknown error"; break;
        }
        return std::formatter<std::string_view>::format(text, ctx);
    }
};
// clang-format on
