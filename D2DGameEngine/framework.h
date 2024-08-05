// framework.h
// Description: 프로젝트 전반에 걸쳐서 범용적으로 사용되면서 안정적인 (내용이 바뀌지 않는) 헤더들의 모음

#pragma once

#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
#include <windows.h>

// 윈도우 라이브러리에서의 타입 Re-definitions
using AppWindow = HWND;
using WindowStyleFlags = DWORD;

// STL Libraries
#include <iostream>
#include <memory>
#include <utility>
#include <typeindex>
#include <typeinfo>
#include <any>
#include <optional>
#include <functional>
#include <concepts>
#include <numbers>
#include <limits>

#include <filesystem>
#include <fstream>
#include <sstream>
#include <string>
#include <string_view>
using namespace std::literals;
#include <format>

// Math Libraries
#include <cmath>

// stl containers
#include <vector>
#include <list>
#include <stack>
#include <queue>
#include <map>
#include <set>
#include <unordered_map>
#include <unordered_set>

#include <condition_variable>
#include <mutex>
#include <thread>

#include <type_traits>

/**
 * @brief Check if the type is completely defined.
 * @ref https://devblogs.microsoft.com/oldnewthing/20190708-00/?p=102664
 */
template<typename, typename = void>
struct is_type_complete
	: std::false_type {};

template<typename T>
struct is_type_complete<T, std::void_t<decltype(sizeof(T))>>
	: std::true_type {};


struct is_32bit_system : std::bool_constant<sizeof(void*) == 4ULL> {};
struct is_64bit_system : std::bool_constant<sizeof(void*) == 8ULL> {};
inline constexpr bool is_32bit_system_v{ is_32bit_system::value };
inline constexpr bool is_64bit_system_v{ is_64bit_system::value };

constexpr unsigned int MAX_CHAR_LENGTH = 256U;

consteval std::size_t KB(std::size_t kb) { return kb * 1024; }
consteval std::size_t MB(std::size_t mb) { return KB(mb * 1024); }
consteval std::size_t GB(std::size_t gb) { return MB(gb * 1024); }

using Flags = uint32_t;
