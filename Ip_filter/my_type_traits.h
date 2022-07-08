#pragma once

#include <forward_list>
#include <list>
#include <vector>
#include <array>


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



//
//template<typename T>
//struct is_array : std::false_type {};
//
//template<typename T, typename Item>
//struct is_array : std::true_type
//{
//	std::is_same<typename T<Item,0>,std::array<Item,0>>
//};
//


//
//template<typename T>
//struct is_sequence_container;
//
//
//template <typename T>
//struct is_sequence_container : std::false_type {};
//
//
//template<typename T,size_t N>
//struct is_sequence_container : std::bool_constant<is_array<T,N>> {};
//
//
//

//
//template<typename T>
//struct is_sequence_container <std::deque<T>> : std::true_type {};
//
//template<typename T>
//struct is_sequence_container <std::forward_list<T>> : std::true_type {};
//
//template<typename T>
//struct is_sequence_container <std::list<T>> : std::true_type {};
//
//template<typename T>
//struct is_sequence_container <std::vector<T>> : std::true_type {};
//
