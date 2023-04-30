#pragma once
#include <cstdint>
#include <string_view>

constexpr uint32_t FNVHash(std::string_view input) noexcept {
        const unsigned int fnv_prime = 0x811C9DC5;
        unsigned int hash = 0;
        
        for (auto& ch : input){
                hash *= fnv_prime;
                hash ^= ch;
        }
        
        return hash;
}

constexpr inline uint32_t operator "" _(char const *p, size_t) {
        return FNVHash(p);
}