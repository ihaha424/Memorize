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
#include <algorithm>

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

// SimpleMath Library
#include "SimpleMath.h"

// D2D Types
#include "d2d_types.h"

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


template<class Interface>
inline void SafeRelease(Interface** ppInterface) {
	if (*ppInterface != nullptr) {
		(*ppInterface)->Release();
		(*ppInterface) = nullptr;
	}
}

template<class Interface>
inline void SafeRelease(std::unique_ptr<Interface>& upInterface) {
	if (upInterface) {
		upInterface.get()->Release();
		upInterface.release();
	}
}

inline constexpr 
float Clamp(float value, float min, float max) {
	if (value < min) return min;
	if (value > max) return max;
	return value;
}

constexpr float PI_F = std::numbers::pi_v<float>;
constexpr float PI = std::numbers::pi_v<double>;

namespace DirectX::SimpleMath {

	inline constexpr
	float DegreeToRadian(float degree) {
		return degree * PI_F / 180.f;
	}

	inline constexpr
	float RadianToDegree(float radian) {
		return radian * 180.f * PI_F;
	}

	inline
		Math::Vector3 ExtractTranslation(const Math::Matrix& transform) {
		return { transform._41, transform._42, transform._43 };
	}

	inline
		Math::Vector3 ExtractScale(const Math::Matrix& transform) {
		float sx = Math::Vector3(transform._11, transform._12, transform._13).Length();
		float sy = Math::Vector3(transform._21, transform._22, transform._23).Length();
		float sz = Math::Vector3(transform._31, transform._32, transform._33).Length();
		return { sx, sy, sz };
	}

	inline
		Math::Matrix ExtractRotation(const Math::Matrix& transform, Math::Vector3* outScale = nullptr) {
		Math::Vector3 scale = ExtractScale(transform);
		if (outScale) {
			*outScale = scale;
		}
		Math::Matrix rotation = transform;
		rotation._11 /= scale.x; rotation._12 /= scale.x; rotation._13 /= scale.x; rotation._14 = 0.f;
		rotation._21 /= scale.y; rotation._22 /= scale.y; rotation._23 /= scale.y; rotation._24 = 0.f;
		rotation._31 /= scale.z; rotation._32 /= scale.z; rotation._33 /= scale.z; rotation._34 = 0.f;
		rotation._41  =		  0.f; rotation._42  =		 0.f;	rotation._43  =			0.f; rotation._44 = 1.f;

		return rotation;
	}
}

inline constexpr
D2D_Vec2F ToD2DVec2F(const Math::Vector2& vec) {
	return { vec.x, vec.y };
}

inline constexpr
D2D_Vec2F ToD2DVec2F(const Math::Vector3& vec) {
	return { vec.x, vec.y };
}

inline constexpr
D2D_Point2F ToD2DP2F(const Math::Vector2& vec) {
	return { vec.x, vec.y };
}

inline constexpr
D2D_Point2F ToD2DP2F(const Math::Vector3& vec) {
	return { vec.x, vec.y };
}

inline
D2D_Mat3x2F ToD2DMat(const Math::Matrix& mat) {
	D2D_Mat3x2F m{};
	m._11 = mat._11;
	m._12 = mat._12;
	m._21 = mat._21;
	m._22 = mat._22;
	m._31 = mat._41;
	m._32 = mat._42;
	return m;
}

constexpr float EPSILON = 1e-8;


