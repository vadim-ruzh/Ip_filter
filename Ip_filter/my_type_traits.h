#pragma once

#include <list>
#include <vector>

template <typename T>
struct is_container;

template <typename T>
struct is_container : std::false_type {};

template <typename T>
struct is_container<std::vector<T>> : std::true_type {};

template <typename T>
struct is_container<std::list<T>> : std::true_type {};

template <typename T>
constexpr bool is_container_v = is_container<T>::value;
