#pragma once

#include "D2DGameEngine/framework.h"

#include "array"

inline const wchar_t* TITLE = L"";

constexpr int SCREEN_WIDTH = 1920;
constexpr int SCREEN_HEIGHT = 1080;

constexpr Math::Vector2 MAP_VERTEX_0{ 0, -1780 };
constexpr Math::Vector2 MAP_VERTEX_1{ 3120, 100 };
constexpr Math::Vector2 MAP_VERTEX_2{ 3120, 260 };
constexpr Math::Vector2 MAP_VERTEX_3{ 0, 2140 };
constexpr Math::Vector2 MAP_VERTEX_4{ -3120, 260 };
constexpr Math::Vector2 MAP_VERTEX_5{ -3120, 100 };

constexpr std::array<Math::Vector2, 6> MAP_VERTICES {
	MAP_VERTEX_0, 
	MAP_VERTEX_1, 
	MAP_VERTEX_2, 
	MAP_VERTEX_3, 
	MAP_VERTEX_4, 
	MAP_VERTEX_5
};
constexpr std::size_t NUM_MAP_VERTICES = MAP_VERTICES.size();

inline constexpr
Math::Vector2 GET_MAP_CENTER()
{
	Math::Vector2 v{ 0.f, 0.f };
	for (uint i = 0; i < NUM_MAP_VERTICES; ++i)
	{
		v.x += MAP_VERTICES[i].x;
		v.y += MAP_VERTICES[i].y;
	}
	return { v.x / NUM_MAP_VERTICES, v.y / NUM_MAP_VERTICES };
}
