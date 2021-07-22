// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include <cstdint>
#include <vector>
#include <string>
#include <array>
#include "../HexCoding.h"

namespace TW {
inline void serial_data(std::string& data, const std::string& input) {
    uint32_t len = (uint32_t)input.size();
    data += std::string((char*)&len, sizeof(len));
    data += input;
}
inline void serial_data(std::string& data, const char* input) {
    uint32_t len = (uint32_t)strlen(input);
    data += std::string((char*)&len, sizeof(len));
    data += std::string(input);
}
inline void serial_data_hex(std::string& data, const char* input) {
    Data input2 = parse_hex(std::string(input));
    uint32_t len = (uint32_t)input2.size();
    data += std::string((char*)&len, sizeof(len));
    data += std::string(input2.begin(), input2.end());
}
inline void serial_data(std::string& data, const uint32_t input) {
    data += std::string((char*)&input, sizeof(input));
}
inline void serial_data(std::string& data, const uint16_t input) {
    data += std::string((char*)&input, sizeof(input));
}
inline void serial_data(std::string& data, const uint8_t input) {
    data += std::string((char*)&input, sizeof(input));
}
inline void serial_data(std::string& data, const uint64_t input) {
    data += std::string((char*)&input, sizeof(input));
}
inline void serial_data(std::string& data, const int32_t input) {
    data += std::string((char*)&input, sizeof(input));
}
inline void serial_data(std::string& data, const int16_t input) {
    data += std::string((char*)&input, sizeof(input));
}
inline void serial_data(std::string& data, const int8_t input) {
    data += std::string((char*)&input, sizeof(input));
}
inline void serial_data(std::string& data, const int64_t input) {
    data += std::string((char*)&input, sizeof(input));
}
inline int hex_to_dec(char c) {
    if ('0' <= c && c <= '9') {
        return (c - '0');
    } else if ('a' <= c && c <= 'f') {
        return (c - 'a' + 10);
    } else if ('A' <= c && c <= 'F') {
        return (c - 'A' + 10);
    } else {
        return -1;
    }
}
inline uint64_t hex_to_uint64(std::string& str) {
    if (str.size() <= 2 || str[0] != '0' || str[1] != 'x') {
        return {};
    }
    if (1 == str.size() % 2) {
        str.insert(2, "0");
    }
    uint64_t ret = 0;
    uint32_t first_index = (uint32_t)str.size() - 1;
    for (size_t i = first_index; i >= 2; --i) {
        uint64_t n = hex_to_dec(str[i]);
        ret += n * (uint64_t)(pow(16,first_index - i));
    }
    return ret;
}
} // namespace TW
