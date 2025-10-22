/*++
Copyright (C) 2025 3MF Consortium

All rights reserved.

Redistribution and use in source and binary forms, with or without modification,
are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this
list of conditions and the following disclaimer.
2. Redistributions in binary form must reproduce the above copyright notice,
this list of conditions and the following disclaimer in the documentation
and/or other materials provided with the distribution.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

Abstract: API-compatible replacement for cpp-base64 2.rc.08 implemented by
the 3MF Consortium to avoid bundling third-party sources.

--*/

#pragma once

#include <array>
#include <cstddef>
#include <cstdint>
#include <stdexcept>
#include <string>
#include <string_view>

struct lib3mf_base64_internal final {
    inline static constexpr std::array<const char*, 2> alphabets = {
        "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/",
        "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789-_",
    };

    inline static constexpr std::array<char, 2> padding = {'=', '.'};

    static inline bool is_padding(char ch) noexcept {
        return ch == '=' || ch == '.';
    }

    static inline int decode_char(unsigned char value) {
        if (value >= 'A' && value <= 'Z') {
            return value - 'A';
        }
        if (value >= 'a' && value <= 'z') {
            return value - 'a' + 26;
        }
        if (value >= '0' && value <= '9') {
            return value - '0' + 52;
        }
        if (value == '+' || value == '-') {
            return 62;
        }
        if (value == '/' || value == '_') {
            return 63;
        }
        throw std::runtime_error("Input is not valid base64-encoded data.");
    }

    static inline std::string insert_linebreaks(std::string text, std::size_t interval) {
        if (interval == 0) {
            return text;
        }
        std::size_t position = interval;
        while (position < text.size()) {
            text.insert(position, "\n");
            position += interval + 1;
        }
        return text;
    }

    static inline std::string encode_bytes(const unsigned char* data, std::size_t length, bool url_variant) {
        if (length == 0) {
            return {};
        }

        const char* alphabet = alphabets[url_variant];
        const char pad = padding[url_variant];
        std::string encoded;
        encoded.reserve(((length + 2) / 3) * 4);

        std::size_t index = 0;
        while (index + 3 <= length) {
            const std::uint32_t chunk = (static_cast<std::uint32_t>(data[index]) << 16) |
                                        (static_cast<std::uint32_t>(data[index + 1]) << 8) |
                                        (static_cast<std::uint32_t>(data[index + 2]));

            encoded.push_back(alphabet[(chunk >> 18) & 0x3F]);
            encoded.push_back(alphabet[(chunk >> 12) & 0x3F]);
            encoded.push_back(alphabet[(chunk >> 6) & 0x3F]);
            encoded.push_back(alphabet[chunk & 0x3F]);
            index += 3;
        }

        const std::size_t remaining = length - index;
        if (remaining == 1) {
            const std::uint32_t chunk = static_cast<std::uint32_t>(data[index]) << 16;
            encoded.push_back(alphabet[(chunk >> 18) & 0x3F]);
            encoded.push_back(alphabet[(chunk >> 12) & 0x3F]);
            encoded.push_back(pad);
            encoded.push_back(pad);
        } else if (remaining == 2) {
            const std::uint32_t chunk = (static_cast<std::uint32_t>(data[index]) << 16) |
                                        (static_cast<std::uint32_t>(data[index + 1]) << 8);
            encoded.push_back(alphabet[(chunk >> 18) & 0x3F]);
            encoded.push_back(alphabet[(chunk >> 12) & 0x3F]);
            encoded.push_back(alphabet[(chunk >> 6) & 0x3F]);
            encoded.push_back(pad);
        }

        return encoded;
    }

    static inline std::string encode_common(std::string_view input, bool url_variant) {
        const auto* bytes = reinterpret_cast<const unsigned char*>(input.data());
        return encode_bytes(bytes, input.size(), url_variant);
    }

    static inline std::string encode_with_breaks(std::string_view input, std::size_t interval) {
        return insert_linebreaks(encode_common(input, false), interval);
    }

    static inline std::string strip_linebreaks(std::string_view input) {
        std::string compact;
        compact.reserve(input.size());
        for (char ch : input) {
            if (ch != '\n') {
                compact.push_back(ch);
            }
        }
        return compact;
    }

    static inline std::string decode_impl(std::string_view input) {
        if (input.empty()) {
            return {};
        }
        if (input.size() % 4 == 1) {
            throw std::runtime_error("Input is not valid base64-encoded data.");
        }

        std::string decoded;
        decoded.reserve((input.size() / 4) * 3);

        std::size_t index = 0;
        while (index < input.size()) {
            if (index + 1 >= input.size()) {
                throw std::runtime_error("Input is not valid base64-encoded data.");
            }

            const int val0 = decode_char(static_cast<unsigned char>(input[index]));
            const int val1 = decode_char(static_cast<unsigned char>(input[index + 1]));

            const bool has_third = (index + 2) < input.size();
            const bool has_fourth = (index + 3) < input.size();

            const char third_char = has_third ? input[index + 2] : '=';
            const char fourth_char = has_fourth ? input[index + 3] : '=';

            const bool pad_third = !has_third || is_padding(third_char);
            const bool pad_fourth = !has_fourth || is_padding(fourth_char);

            int val2 = 0;
            int val3 = 0;
            if (!pad_third) {
                val2 = decode_char(static_cast<unsigned char>(third_char));
            }
            if (!pad_fourth) {
                val3 = decode_char(static_cast<unsigned char>(fourth_char));
            }

            decoded.push_back(static_cast<char>((val0 << 2) | (val1 >> 4)));

            if (!pad_third) {
                decoded.push_back(static_cast<char>(((val1 & 0x0F) << 4) | (val2 >> 2)));
                if (!pad_fourth) {
                    decoded.push_back(static_cast<char>(((val2 & 0x03) << 6) | val3));
                }
            } else if (!pad_fourth) {
                throw std::runtime_error("Input is not valid base64-encoded data.");
            }

            index += 4;
        }

        return decoded;
    }

    static inline std::string decode(std::string_view input, bool remove_linebreaks) {
        if (!remove_linebreaks) {
            return decode_impl(input);
        }
        std::string compact = strip_linebreaks(input);
        return decode_impl(compact);
    }
};

inline std::string base64_encode(const unsigned char* bytes_to_encode, std::size_t length, bool url = false) {
    return lib3mf_base64_internal::encode_bytes(bytes_to_encode, length, url);
}

inline std::string base64_encode(const std::string& input, bool url = false) {
    return lib3mf_base64_internal::encode_common(std::string_view{input}, url);
}

inline std::string base64_encode(std::string_view input, bool url = false) {
    return lib3mf_base64_internal::encode_common(input, url);
}

inline std::string base64_encode_pem(const std::string& input) {
    return lib3mf_base64_internal::encode_with_breaks(std::string_view{input}, 64);
}

inline std::string base64_encode_pem(std::string_view input) {
    return lib3mf_base64_internal::encode_with_breaks(input, 64);
}

inline std::string base64_encode_mime(const std::string& input) {
    return lib3mf_base64_internal::encode_with_breaks(std::string_view{input}, 76);
}

inline std::string base64_encode_mime(std::string_view input) {
    return lib3mf_base64_internal::encode_with_breaks(input, 76);
}

inline std::string base64_decode(const std::string& input, bool remove_linebreaks = false) {
    return lib3mf_base64_internal::decode(std::string_view{input}, remove_linebreaks);
}

inline std::string base64_decode(std::string_view input, bool remove_linebreaks = false) {
    return lib3mf_base64_internal::decode(input, remove_linebreaks);
}
