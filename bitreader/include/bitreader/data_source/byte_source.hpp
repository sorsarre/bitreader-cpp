#pragma once
#include <cstdint>
#include <cstddef>
#include <concepts>
#include <memory>

namespace brcpp
{

template<typename T>
concept byte_source = requires(T r, uint64_t& buf, uint64_t pos, size_t count)
{
    { r.get_n(buf, count) } -> std::same_as<size_t>;
    { r.depleted() } -> std::same_as<bool>;
    { r.available() } -> std::same_as<uint64_t>;
    { r.position() } -> std::same_as<uint64_t>;
    { r.seek(pos) } -> std::same_as<void>;
    { r.skip(count) } -> std::same_as<void>;
    { r.clone() } -> std::same_as<std::shared_ptr<T>>;
};

}
