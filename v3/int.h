#pragma once

///////////////////////////////////////////////////////////////
//  Distributed under the Boost Software License, Version 1.0.
//  (See at http://www.boost.org/LICENSE_1_0.txt)
///////////////////////////////////////////////////////////////

/*  Divide and multiply
 *
 *
 * Copyright (c) 2008
 * Evan Teran
 *
 * Permission to use, copy, modify, and distribute this software and its
 * documentation for any purpose and without fee is hereby granted, provided
 * that the above copyright notice appears in all copies and that both the
 * copyright notice and this permission notice appear in supporting
 * documentation, and that the same name not be used in advertising or
 * publicity pertaining to distribution of the software without specific,
 * written prior permission. We make no representations about the
 * suitability this software for any purpose. It is provided "as is"
 * without express or implied warranty.
 */

#include <cmath>
#include <cstdint>
#include <iomanip>
#include <limits>
#include <ostream>
#include <sstream>
#include <stdexcept>
#include <stdint.h>
#include <system_error>
#include <type_traits>

struct _test;

namespace std {
enum class wide_integer_s {
    Unsigned = 0,
    Signed = 1
};

template <size_t MachineWords, wide_integer_s Signed>
class wide_integer;

template <size_t MachineWords, wide_integer_s Signed, size_t MachineWords2, wide_integer_s Signed2>
struct common_type<wide_integer<MachineWords, Signed>, wide_integer<MachineWords2, Signed2>>;

template <size_t MachineWords, wide_integer_s Signed, typename Arithmetic>
struct common_type<wide_integer<MachineWords, Signed>, Arithmetic>;

template <typename Arithmetic, size_t MachineWords, wide_integer_s Signed>
struct common_type<Arithmetic, wide_integer<MachineWords, Signed>>;

template <size_t MachineWords, wide_integer_s Signed>
class wide_integer {
public:
    using base_type = uint8_t;
    using signed_base_type = int8_t;

    // ctors
    wide_integer() = default;

    template <typename T>
    constexpr wide_integer(T rhs) noexcept;
    template <size_t MachineWords2, wide_integer_s Signed2>
    constexpr wide_integer(const wide_integer<MachineWords2, Signed2>& rhs) noexcept;

    // assignment
    template <size_t MachineWords2, wide_integer_s Signed2>
    constexpr wide_integer<MachineWords, Signed>& operator=(const wide_integer<MachineWords2, Signed2>& rhs) noexcept;

    template <typename Arithmetic>
    constexpr wide_integer<MachineWords, Signed>& operator=(Arithmetic rhs) noexcept;

    template <typename Arithmetic>
    constexpr wide_integer<MachineWords, Signed>& operator*=(const Arithmetic& rhs);

    template <typename Arithmetic>
    constexpr wide_integer<MachineWords, Signed>& operator/=(const Arithmetic& rhs);

    template <typename Arithmetic>
    constexpr wide_integer<MachineWords, Signed>& operator+=(const Arithmetic& rhs) noexcept(Signed == wide_integer_s::Unsigned);

    template <typename Arithmetic>
    constexpr wide_integer<MachineWords, Signed>& operator-=(const Arithmetic& rhs) noexcept(Signed == wide_integer_s::Unsigned);

    template <typename Integral>
    constexpr wide_integer<MachineWords, Signed>& operator%=(const Integral& rhs);

    template <typename Integral>
    constexpr wide_integer<MachineWords, Signed>& operator&=(const Integral& rhs) noexcept;

    template <typename Integral>
    constexpr wide_integer<MachineWords, Signed>& operator|=(const Integral& rhs) noexcept;

    template <typename Integral>
    constexpr wide_integer<MachineWords, Signed>& operator^=(const Integral& rhs) noexcept;

    constexpr wide_integer<MachineWords, Signed>& operator<<=(int n);
    constexpr wide_integer<MachineWords, Signed>& operator>>=(int n) noexcept;

    // observers

    constexpr explicit operator bool() const noexcept;

    template <class T>
    using __integral_not_wide_integer_class = typename std::enable_if<std::is_fundamental<T>::value, T>::type;

    template <class T, class = __integral_not_wide_integer_class<T>>
    constexpr operator T() const noexcept;

    constexpr operator long double() const noexcept;
    constexpr operator double() const noexcept;
    constexpr operator float() const noexcept;

    struct _impl;

private:
    friend struct ::_test;

    template <size_t MachineWords2, wide_integer_s Signed2>
    friend class wide_integer;

    friend class numeric_limits<wide_integer<MachineWords, wide_integer_s::Signed>>;
    friend class numeric_limits<wide_integer<MachineWords, wide_integer_s::Unsigned>>;

    base_type m_arr[_impl::arr_size];
};

// Unary operators
template <size_t MachineWords, wide_integer_s Signed>
constexpr wide_integer<MachineWords, Signed> operator~(const wide_integer<MachineWords, Signed>& lhs) noexcept;

template <size_t MachineWords, wide_integer_s Signed>
constexpr wide_integer<MachineWords, Signed> operator-(const wide_integer<MachineWords, Signed>& lhs) noexcept(Signed == wide_integer_s::Unsigned);

template <size_t MachineWords, wide_integer_s Signed>
constexpr wide_integer<MachineWords, Signed> operator+(const wide_integer<MachineWords, Signed>& lhs) noexcept(Signed == wide_integer_s::Unsigned);

// Binary operators
template <size_t MachineWords, wide_integer_s Signed, size_t MachineWords2, wide_integer_s Signed2>
std::common_type_t<wide_integer<MachineWords, Signed>, wide_integer<MachineWords2, Signed2>> constexpr operator*(const wide_integer<MachineWords, Signed>& lhs, const wide_integer<MachineWords2, Signed2>& rhs);
template <typename Arithmetic, typename Arithmetic2, class>
std::common_type_t<Arithmetic, Arithmetic2> constexpr operator*(const Arithmetic& rhs, const Arithmetic2& lhs);

template <size_t MachineWords, wide_integer_s Signed, size_t MachineWords2, wide_integer_s Signed2>
std::common_type_t<wide_integer<MachineWords, Signed>, wide_integer<MachineWords2, Signed2>> constexpr operator/(const wide_integer<MachineWords, Signed>& lhs, const wide_integer<MachineWords2, Signed2>& rhs);
template <typename Arithmetic, typename Arithmetic2, class>
std::common_type_t<Arithmetic, Arithmetic2> constexpr operator/(const Arithmetic& rhs, const Arithmetic2& lhs);

// TODO
template <size_t MachineWords, wide_integer_s Signed, typename Arithmetic>
std::common_type_t<wide_integer<MachineWords, Signed>, Arithmetic> constexpr operator+(const wide_integer<MachineWords, Signed>& lhs, const Arithmetic& rhs) noexcept(Signed == wide_integer_s::Unsigned);
template <size_t MachineWords, wide_integer_s Signed, typename Arithmetic, class>
std::common_type_t<wide_integer<MachineWords, Signed>, Arithmetic> constexpr operator+(const Arithmetic& rhs, const wide_integer<MachineWords, Signed>& lhs) noexcept(Signed == wide_integer_s::Unsigned);

template <size_t MachineWords, wide_integer_s Signed, size_t MachineWords2, wide_integer_s Signed2>
std::common_type_t<wide_integer<MachineWords, Signed>, wide_integer<MachineWords2, Signed2>> constexpr operator-(const wide_integer<MachineWords, Signed>& lhs, const wide_integer<MachineWords2, Signed2>& rhs);
template <typename Arithmetic, typename Arithmetic2, class>
std::common_type_t<Arithmetic, Arithmetic2> constexpr operator-(const Arithmetic& rhs, const Arithmetic2& lhs);

template <size_t MachineWords, wide_integer_s Signed, size_t MachineWords2, wide_integer_s Signed2>
std::common_type_t<wide_integer<MachineWords, Signed>, wide_integer<MachineWords2, Signed2>> constexpr operator%(const wide_integer<MachineWords, Signed>& lhs, const wide_integer<MachineWords2, Signed2>& rhs);
template <typename Integral, typename Integral2, class>
std::common_type_t<Integral, Integral2> constexpr operator%(const Integral& rhs, const Integral2& lhs);

template <size_t MachineWords, wide_integer_s Signed, size_t MachineWords2, wide_integer_s Signed2>
std::common_type_t<wide_integer<MachineWords, Signed>, wide_integer<MachineWords2, Signed2>> constexpr operator&(const wide_integer<MachineWords, Signed>& lhs, const wide_integer<MachineWords2, Signed2>& rhs);
template <typename Integral, typename Integral2, class>
std::common_type_t<Integral, Integral2> constexpr operator&(const Integral& rhs, const Integral2& lhs);

template <size_t MachineWords, wide_integer_s Signed, size_t MachineWords2, wide_integer_s Signed2>
std::common_type_t<wide_integer<MachineWords, Signed>, wide_integer<MachineWords2, Signed2>> constexpr operator|(const wide_integer<MachineWords, Signed>& lhs, const wide_integer<MachineWords2, Signed2>& rhs);
template <typename Integral, typename Integral2, class>
std::common_type_t<Integral, Integral2> constexpr operator|(const Integral& rhs, const Integral2& lhs);

template <size_t MachineWords, wide_integer_s Signed, size_t MachineWords2, wide_integer_s Signed2>
std::common_type_t<wide_integer<MachineWords, Signed>, wide_integer<MachineWords2, Signed2>> constexpr operator^(const wide_integer<MachineWords, Signed>& lhs, const wide_integer<MachineWords2, Signed2>& rhs);
template <typename Integral, typename Integral2, class>
std::common_type_t<Integral, Integral2> constexpr operator^(const Integral& rhs, const Integral2& lhs);

// TODO: Integral
template <size_t MachineWords, wide_integer_s Signed>
constexpr wide_integer<MachineWords, Signed> operator<<(const wide_integer<MachineWords, Signed>& lhs, int n) noexcept;
template <size_t MachineWords, wide_integer_s Signed>
constexpr wide_integer<MachineWords, Signed> operator>>(const wide_integer<MachineWords, Signed>& lhs, int n) noexcept;

template <size_t MachineWords, wide_integer_s Signed, typename Arithmetic>
constexpr bool operator<(const wide_integer<MachineWords, Signed>& lhs, const Arithmetic& rhs) noexcept;
template <size_t MachineWords, wide_integer_s Signed, typename Arithmetic, class>
constexpr bool operator<(const Arithmetic& rhs, const wide_integer<MachineWords, Signed>& lhs) noexcept;

template <size_t MachineWords, wide_integer_s Signed, typename Arithmetic>
constexpr bool operator>(const wide_integer<MachineWords, Signed>& lhs, const Arithmetic& rhs) noexcept;
template <size_t MachineWords, wide_integer_s Signed, typename Arithmetic, class>
constexpr bool operator>(const Arithmetic& rhs, const wide_integer<MachineWords, Signed>& lhs) noexcept;

template <size_t MachineWords, wide_integer_s Signed, typename Arithmetic>
constexpr bool operator<=(const wide_integer<MachineWords, Signed>& lhs, const Arithmetic& rhs) noexcept;
template <size_t MachineWords, wide_integer_s Signed, typename Arithmetic, class>
constexpr bool operator<=(const Arithmetic& rhs, const wide_integer<MachineWords, Signed>& lhs) noexcept;

template <size_t MachineWords, wide_integer_s Signed, typename Arithmetic>
constexpr bool operator>=(const wide_integer<MachineWords, Signed>& lhs, const Arithmetic& rhs) noexcept;
template <size_t MachineWords, wide_integer_s Signed, typename Arithmetic, class>
constexpr bool operator>=(const Arithmetic& rhs, const wide_integer<MachineWords, Signed>& lhs) noexcept;

template <size_t MachineWords, wide_integer_s Signed, typename Arithmetic>
constexpr bool operator==(const wide_integer<MachineWords, Signed>& lhs, const Arithmetic& rhs) noexcept;
template <size_t MachineWords, wide_integer_s Signed, typename Arithmetic, class>
constexpr bool operator==(const Arithmetic& rhs, const wide_integer<MachineWords, Signed>& lhs) noexcept;

template <size_t MachineWords, wide_integer_s Signed, typename Arithmetic>
constexpr bool operator!=(const wide_integer<MachineWords, Signed>& lhs, const Arithmetic& rhs) noexcept;
template <size_t MachineWords, wide_integer_s Signed, typename Arithmetic, class>
constexpr bool operator!=(const Arithmetic& rhs, const wide_integer<MachineWords, Signed>& lhs) noexcept;

template <size_t MachineWords, wide_integer_s Signed>
std::string to_string(const wide_integer<MachineWords, Signed>& n);

template <size_t MachineWords, wide_integer_s Signed>
std::wstring to_wstring(const wide_integer<MachineWords, Signed>& n);

template <size_t MachineWords, wide_integer_s Signed>
std::ostream& operator<<(std::ostream& out, const wide_integer<MachineWords, Signed>& n);

template <size_t MachineWords, wide_integer_s Signed>
std::wostream& operator<<(std::wostream& out, const wide_integer<MachineWords, Signed>& n);

template <size_t MachineWords, wide_integer_s Signed>
std::istream& operator>>(std::istream& in, wide_integer<MachineWords, Signed>& n);

template <size_t MachineWords, wide_integer_s Signed>
std::wistream& operator>>(std::wistream& in, wide_integer<MachineWords, Signed>& n);

//// Must be defined in another header
struct to_chars_result {
    char* ptr;
    std::error_code ec;
};

struct from_chars_result {
    const char* ptr;
    std::error_code ec;
};
////

template <size_t MachineWords, wide_integer_s Signed>
to_chars_result to_chars(char* first,
                         char* last,
                         const wide_integer<MachineWords, Signed>& value,
                         int base = 10);

template <size_t MachineWords, wide_integer_s Signed>
from_chars_result from_chars(const char* first,
                             const char* last,
                             wide_integer<MachineWords, Signed>& value,
                             int base = 10);

inline namespace literals {
inline namespace wide_integer_literals {
template <size_t MachineWords>
using wide_int = wide_integer<MachineWords, wide_integer_s::Signed>;

template <size_t MachineWords>
using wide_uint = wide_integer<MachineWords, wide_integer_s::Unsigned>;

using int128_t = wide_int<128 / CHAR_BIT / sizeof(long)>;
using uint128_t = wide_uint<128 / CHAR_BIT / sizeof(long)>;

using int256_t = wide_int<256 / CHAR_BIT / sizeof(long)>;
using uint256_t = wide_uint<256 / CHAR_BIT / sizeof(long)>;

using int512_t = wide_int<512 / CHAR_BIT / sizeof(long)>;
using uint512_t = wide_uint<512 / CHAR_BIT / sizeof(long)>;
} // namespace wide_integer_literals
} // namespace literals

constexpr int128_t operator"" _int128(const char* n);
constexpr int256_t operator"" _int256(const char* n);
constexpr int512_t operator"" _int512(const char* n);
constexpr uint128_t operator"" _uint128(const char* n);
constexpr uint256_t operator"" _uint256(const char* n);
constexpr uint512_t operator"" _uint512(const char* n);

// numeric limits
template <size_t MachineWords, wide_integer_s Signed>
class numeric_limits<wide_integer<MachineWords, Signed>>;

template <size_t MachineWords, wide_integer_s Signed>
struct hash<wide_integer<MachineWords, Signed>>;

} // namespace std

#include "impl.h"
