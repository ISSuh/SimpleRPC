/**
 * 
 *  Copyright:  Copyright (c) 2020, ISSuh
 * 
 */

#ifndef SRPC_RPC_TRAIT_TYPETRAITS_HPP_
#define SRPC_RPC_TRAIT_TYPETRAITS_HPP_

#include <iostream>
#include <string>
#include <cstring>
#include <vector>
#include <functional>
#include <utility>
#include <tuple>

namespace srpc {
namespace trait {

/**
 * Type Traits
 */

template <typename T, typename ENABLE = void>
struct TypeTraits {
  static constexpr bool valid = false;
  using Type = T;
};

// Support for void type
template <typename T>
struct TypeTraits<T,
                   typename std::enable_if<std::is_void<T>::value>::type
                  > {
  static constexpr bool valid = true;
  using Type = T;
};

// Support for any arithmetic type
template <typename T>
struct TypeTraits<T,
                   typename std::enable_if<std::is_arithmetic<T>::value>::type
                  > {
  static constexpr bool valid = true;
  using Type = T;

  template <typename S>
  static void write(S& s, T v) { s.write(&v, sizeof(v)); }

  template <typename S>
  static void read(S& s, Type& v) { s.read(&v, sizeof(v)); }

  static Type get(Type v) { return v; }
};

// Generic support for T&, for any valid T
template <typename T> struct TypeTraits<T&> : TypeTraits<T> {
  static_assert(TypeTraits<T>::valid, "T& - Unsupported Type");
};

// Generic support for const T&, for any valid T
template <typename T> struct TypeTraits<const T&> : TypeTraits<T> {
  static_assert(TypeTraits<T>::valid, "const T& - Unsupported Type");
};

// Generic support for T*, for any valid T
template <typename T> struct TypeTraits<T*> : TypeTraits<T> {
  static_assert(TypeTraits<T>::valid, "T* - Unsupported Type");
};

// Generic support for const const T*, for any valid T
template <typename T>
struct TypeTraits<const T*> : TypeTraits<T> {
  static_assert(TypeTraits<T>::valid, "const T* - Unsupported Type");
};

// Generic support for const T* const, for any valid T
template <typename T> struct TypeTraits<T* const> : TypeTraits<T> {
  static_assert(TypeTraits<T>::valid, "T* const - Unsupported Type");
};

template <typename T>
struct TypeTraits<std::vector<T>> {
  using Type = std::vector<T>;
  static constexpr bool valid = TypeTraits<T>::valid;

  static_assert(TypeTraits<T>::valid, "vector<T> - Unsupported Type");

  // Write the vector size, followed by  each element
  template <typename S>
  static void write(S& s, const std::vector<T>& v) {
    unsigned len = static_cast<unsigned>(v.size());
    s.write(&len, sizeof(len));
    for (auto&& i : v){
      TypeTraits<T>::write(s, i);
    }
  }

  template <typename S>
  static void read(S& s, std::vector<T>& v) {
    unsigned len;
    s.read(&len, sizeof(len));
    v.clear();
    while (len--) {
      T i;
      TypeTraits<T>::read(s, i);
      v.push_back(std::move(i)); 
    }
  }

  static std::vector<T>&& get(std::vector<T>&& v) { return std::move(v); }
};

struct StringTraits {
  template <typename S>
  static void write(S& s, const char* v) {
    int len = static_cast<int>(std::strlen(v));
    s.write(&len, sizeof(len));
    s.write(v, len);
  }

  template <typename S>
  static void write(S& s, const std::string& v) {
    int len = static_cast<int>(v.size());
    s.write(&len, sizeof(len));
    s.write(v.c_str(), len);
  }

  template <typename S>
  static void read(S& s, std::string& v) {
    int len;
    s.read(&len, sizeof(len));
    v.clear();

    if (len) {
      v.reserve(len);
      v.append(len, 0);
      s.read(&v[0], len);
    }
  }
};

// Barebones for const char* support
template <>
struct TypeTraits<const char*> {
  static constexpr bool valid = true;
  using Type = std::string;

  template <typename S>
  static void write(S& s, const char* v) { 
    StringTraits::write(s, v);
  }

  template <typename S>
  static void read(S& s, Type& v) {
    StringTraits::read(s, v);
  }

  static const char* get(const Type& v) { return v.c_str(); }
};

template <int N>
struct TypeTraits<char[N]> : TypeTraits<const char*> {
  static_assert(TypeTraits<char*>::valid, "char[N] -  Unsupported Type");
};
template <int N>
struct TypeTraits<const char[N]> : TypeTraits<const char*> {
  static_assert(TypeTraits<char*>::valid, "const char[N] -  Unsupported Type");
};

template <int N>
struct TypeTraits<const char (&)[N]> : TypeTraits<const char*> {
  static_assert(TypeTraits<char*>::valid, "char (&)[N] -  Unsupported Type");
};

template <>
struct TypeTraits<std::string> : TypeTraits<const char*> {
  static constexpr bool valid = true;

  template <typename S>
  static void write(S& s, const char* v) { 
    StringTraits::write(s, v);
  }

  template <typename S>
  static void write(S& s, const std::string& v) { 
    StringTraits::write(s, v);
  }

  template <typename S>
  static void read(S& s, std::string& v) {
    StringTraits::read(s, v);
  }

  static const std::string& get(const Type& v) { return v; }
};

}  // namespace trait
}  // namespace srpc

#endif  // SRPC_RPC_TRAIT_TYPETRAITS_HPP_
