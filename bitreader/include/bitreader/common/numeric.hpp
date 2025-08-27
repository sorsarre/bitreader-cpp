#pragma once
#include <type_traits>

namespace brcpp
{

template<typename T>
concept arithmetic = std::is_arithmetic_v<T>;

template<arithmetic T>
constexpr const T one = T{1};

template<arithmetic T>
constexpr const T zero = T{0};

}