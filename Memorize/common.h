#pragma once

#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
#include <windows.h>

// STL
#include <iostream>
#include <memory>
#include <utility>

// Config file
#include "config.h"

// 디버그 로거
#include "D2DGameEngine/Debug.h"

#include "D2DGameEngine/IntersectionUtil.h"

inline
bool IsInMap(const Math::Vector2& point)
{
	const std::size_t NUM_VERTICES = MAP_VERTICES.size();
	for (uint i = 0; i < NUM_VERTICES; ++i) 
	{
		Line line{ MAP_VERTICES[i], MAP_VERTICES[(i + 1) % NUM_VERTICES] };
		if (line.WhichSide(point) < 0) return false;
	}
	return true;
}

inline
bool CircleMapIntersect(const Circle& circle, Math::Vector2& outNormal)
{
	const std::size_t NUM_VERTICES = MAP_VERTICES.size();
	for (uint i = 0; i < NUM_VERTICES; ++i)
	{
		Line line{ MAP_VERTICES[i], MAP_VERTICES[(i + 1) % NUM_VERTICES] };
		
		HitResult hitResult;
		if (intersectionUtil::CircleLineIntersectWithResult(circle, line, hitResult))
		{
			outNormal = hitResult.normal;
			LOG_INFO(dbg::text(outNormal.x, ", ", outNormal.y));
			return true;
		}
	}
	return false;
}
