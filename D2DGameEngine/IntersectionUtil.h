#pragma once

#include "Ray.h"
#include "Line.h"
#include "Box.h"
#include "Circle.h"
#include "Capsule.h"
#include "Polygon.h"

#include "HitResult.h"

namespace intersectionUtil {
	// With Result
	inline	// TODO: Test required.
		bool RayRayIntersectWithResult(const Ray& ray1, const Ray& ray2, HitResult& outHitResult) {
		// NOTE: ChatGPT 4 Generated Code
		Math::Vector2 intersection;
		if (ray1.IntersectionPoint(ray2, intersection)) {
			// Find the impact normal
			int side = ray2.WhichSide(ray1.origin);

			Math::Vector2 impactNormal;
			if (side > 0) {
				impactNormal = { ray2.direction.y, -ray2.direction.x };	// rotate -90 degree
			}
			else if (side < 0) {
				impactNormal = { -ray2.direction.y, ray2.direction.x };	// rotate 90 degree
			}
			else {
				int side2 = ray2.WhichSide(ray1.origin + ray1.direction);
				if (side2 >= 0) {
					impactNormal = { -ray2.direction.y, ray2.direction.x };	// rotate 90 degree
				}
				else {
					impactNormal = { ray2.direction.y, -ray2.direction.x };	// rotate -90 degree
				}
			}

			outHitResult.impactNormal = impactNormal;
			outHitResult.impactPoint = intersection;

			return true;
		}

		return false;
	}

	inline	// TODO: Test required.
		bool RayLineIntersectWithResult(const Ray& ray, const Line& line, HitResult& outHitResult) {
		// NOTE: ChatGPT 4 Generated Code
		const Math::Vector2& p = ray.origin;          // Ray origin
		const Math::Vector2& r = ray.direction;       // Ray direction
		const Math::Vector2& q = line.start;          // Line start point
		const Math::Vector2 s = line.end - line.start; // Line direction vector (end - start)

		// We solve for t and u in the equations:
		// p + t*r = q + u*s
		// Rearranging terms gives us:
		// t*r - u*s = q - p

		float r_cross_s = r.x * s.y - r.y * s.x;
		Math::Vector2 q_minus_p = q - p;

		// Check if r cross s is 0, which means the lines are parallel (or possibly collinear)
		if (fabs(r_cross_s) < EPSILON) {
			return false; // Lines are parallel and there's no intersection
		}

		// Solve the system:
		// t = (q - p) x s / (r x s)
		// u = (q - p) x r / (r x s)
		float t = (q_minus_p.x * s.y - q_minus_p.y * s.x) / r_cross_s;
		float u = (q_minus_p.x * r.y - q_minus_p.y * r.x) / r_cross_s;

		// Check if the scalar for line segment u is between 0 and 1, and t must be greater than 0 for a ray
		if (u >= 0 && u <= 1 && t >= 0) {
			Math::Vector2 intersection = p + t * r;

			// Find the impact normal
			Math::Vector2 dir = (line.end - line.start);
			dir.Normalize();

			int side = line.WhichSide(ray.origin);

			Math::Vector2 impactNormal;
			if (side > 0) {
				impactNormal = { dir.y, -dir.x };
			}
			else if (side < 0) {
				impactNormal = { -dir.y, dir.x };
			}
			else {
				int side2 = line.WhichSide(ray.origin + ray.direction);
				if (side2 >= 0) {
					impactNormal = { -dir.y, dir.x };
				}
				else {
					impactNormal = { dir.y, -dir.x };
				}
			}

			outHitResult.impactNormal = impactNormal;
			outHitResult.impactPoint = intersection;

			return true;
		}

		return false;
	}

	inline	// TODO: Test required.
		bool RayBoxIntersectWithResult(const Ray& ray, const Box& box, HitResult& outHitResult) {
		// NOTE: ChatGPT 4 Generated Code

		Math::Vector2 invDir = { 1.0f / ray.direction.x, 1.0f / ray.direction.y };
		Math::Vector2 tMin = (box.ul - ray.origin) * invDir;
		Math::Vector2 tMax = (box.lr - ray.origin) * invDir;

		if (invDir.x < 0.0f) std::swap(tMin.x, tMax.x);
		if (invDir.y < 0.0f) std::swap(tMin.y, tMax.y);

		float tEnter = (std::max)(tMin.x, tMin.y);
		float tExit = (std::min)(tMax.x, tMax.y);

		// Check if there is no intersection
		if (tEnter > tExit || tExit < 0) {
			return false;
		}

		// Check if the intersection point is within bounds of the ray (ray starts at t = 0)
		if (tEnter < 0) {
			tEnter = tExit; // Use tExit if tEnter is behind the ray's start
			if (tEnter < 0) return false;
		}

		// Calculate the intersection point
		Math::Vector2 intersectionPoint = ray.origin + ray.direction * tEnter;

		// Set hit result details
		outHitResult.impactPoint = intersectionPoint;

		// Determine impact normal based on the dominant axis of intersection
		if (tMin.x > tMin.y) {
			outHitResult.impactNormal = invDir.x < 0 ? Math::Vector2(1, 0) : Math::Vector2(-1, 0);
		}
		else {
			outHitResult.impactNormal = invDir.y < 0 ? Math::Vector2(0, 1) : Math::Vector2(0, -1);
		}

		return true;
	}

	inline	// TODO: Test required.
		bool RayCircleIntersectWithResult(const Ray& ray, const Circle& circle, HitResult& outHitResult) {
		// NOTE: ChatGPT 4 Generated Code
		Math::Vector2 m = ray.origin - circle.center;
		float b = m.Dot(ray.direction);
		float c = m.Dot(m) - circle.radius * circle.radius;

		// If the ray's origin is outside the circle (c > 0) and ray pointing away from circle (b > 0), no intersection
		if (c > 0.0f && b > 0.0f) {
			return false;
		}

		float discriminant = b * b - c;

		// A negative discriminant corresponds to ray missing the circle
		if (discriminant < 0.0f) {
			return false;
		}

		// Ray now found to intersect circle, compute smallest t value of intersection
		float t = -b - sqrt(discriminant);

		// If t is negative, ray started inside the sphere, adjust t to be at the closest point
		if (t < 0.0f) {
			t = 0.0f;
		}

		Math::Vector2 intersection = ray.origin + ray.direction * t;  // Calculate the actual intersection point
		Math::Vector2 normal = (intersection - circle.center);
		normal.Normalize();

		outHitResult.impactPoint = intersection;
		outHitResult.impactNormal = normal;  // Normal at the point of intersection

		return true;
	}

	inline	// TODO: Test required.
		bool RayCapsuleIntersectWithResult(const Ray& ray, const Capsule& capsule, HitResult& outHitResult) {
		// NOTE: ChatGPT 4 Generated Code
		// Decompose the capsule into its mathematical representation
		Math::Vector2 normDir = capsule.direction; normDir.Normalize();
		Math::Vector2 capsuleStart = capsule.center - normDir * capsule.extent;
		Math::Vector2 capsuleEnd = capsule.center + normDir * capsule.extent;

		// Calculate parameters for the ray equation: p(t) = ray.origin + t * ray.direction
		// Line segment from capsuleStart to capsuleEnd represents the spine of the capsule
		Math::Vector2 d = capsuleEnd - capsuleStart; // Direction vector of the capsule segment
		Math::Vector2 m = ray.origin - capsuleStart;
		Math::Vector2 n = ray.origin - capsuleEnd;

		float md = m.Dot(d);
		float nd = n.Dot(d);
		float dd = d.Dot(d);

		// Check if the ray and capsule segment are parallel
		float rn = ray.direction.Dot(d);
		float a = dd - rn * rn;
		if (fabs(a) < EPSILON) {
			return false; // The ray and the capsule are parallel
		}

		// Solve quadratic equation for the ray parameter t
		float b = md - rn * m.Dot(ray.direction);
		float c = dd * m.Dot(m) - md * md - capsule.radius * capsule.radius * dd;
		float discr = b * b - a * c;

		if (discr < 0.0f) {
			return false; // No real roots, no intersection
		}

		// Compute the smallest non-negative root
		float t = (-b - sqrt(discr)) / a;
		if (t < 0) {
			t = 0; // If t is negative, ray starts inside the capsule
		}

		// Compute the intersection point
		Math::Vector2 intersection = ray.origin + ray.direction * t;

		// Compute normal at the intersection point
		Math::Vector2 base = capsuleStart + d * fmax(0.0f, fmin(1.0f, (intersection - capsuleStart).Dot(d) / dd));
		Math::Vector2 impactNormal = (intersection - base); impactNormal.Normalize();

		outHitResult.impactPoint = intersection;
		outHitResult.impactNormal = impactNormal;

		return true;
	}

	inline	// TODO: Test required.
		bool RayPolygonIntersectWithResult(const Ray& ray, const TPolygon& polygon, HitResult& outHitResult) {
		// NOTE: ChatGPT 4 Generated Code
		bool hit = false;
		float closestT = (std::numeric_limits<float>::max)();
		Math::Vector2 closestIntersection;
		Math::Vector2 closestNormal;

		// Iterate through each edge of the polygon
		for (size_t i = 0, j = polygon.points.size() - 1; i < polygon.points.size(); j = i++) {
			Math::Vector2 a = polygon.points[j];
			Math::Vector2 b = polygon.points[i];

			// Edge vector
			Math::Vector2 edge = b - a;

			// Ray direction
			Math::Vector2 dir = ray.direction;

			// Compute the determinant
			float det = edge.x * -dir.y - edge.y * -dir.x;

			// Avoid division by zero (parallel line and ray)
			if (fabs(det) < EPSILON) continue;

			// Difference vector
			Math::Vector2 d = ray.origin - a;
			float t = (d.x * -dir.y - d.y * -dir.x) / det;
			float u = (d.x * edge.y - d.y * edge.x) / det;

			// Check if the intersection is on the edge and ray is pointing towards the edge
			if (t >= 0.0f && t <= 1.0f && u >= 0.0f) {
				// Calculate the intersection point
				Math::Vector2 intersection = a + t * edge;

				if (u < closestT) { // Is this intersection closer?
					Math::Vector2 normal = Math::Vector2(-edge.y, edge.x); normal.Normalize();
					closestT = u;
					closestIntersection = intersection;
					closestNormal = normal; // Normal of the edge
					hit = true;
				}
			}
		}

		if (hit) {
			outHitResult.impactPoint = closestIntersection;
			outHitResult.impactNormal = closestNormal;
			return true;
		}

		return false;
	}

	inline	// TODO: Test required.
		bool LineRayIntersectWithResult(const Line& line, const Ray& ray, HitResult& outHitResult) {
		// NOTE: ChatGPT 4 Generated Code
		Math::Vector2 r = ray.direction;       // Ray direction
		Math::Vector2 s = line.end - line.start;  // Line segment vector
		Math::Vector2 o = line.start - ray.origin;  // Origin difference between line start and ray origin

		float rCrossS = r.x * s.y - r.y * s.x;
		float oCrossS = o.x * s.y - o.y * s.x;

		// If rCrossS is close to 0, ray and line are parallel (no intersection)
		if (std::fabs(rCrossS) < EPSILON) {
			return false;
		}

		float t = (o.x * r.y - o.y * r.x) / rCrossS; // Scalar on line segment
		float u = oCrossS / rCrossS; // Scalar on ray

		// Intersection condition: 0 <= t <= 1 for line segment, u >= 0 for ray
		if (t >= 0.0f && t <= 1.0f && u >= 0.0f) {
			Math::Vector2 normal = Math::Vector2(-s.y, s.x); normal.Normalize();

			outHitResult.impactPoint = ray.origin + u * r;
			outHitResult.impactNormal = normal; // Perpendicular to the line segment

			// Optional: check which side of the line the ray originates to flip normal if necessary
			if (ray.WhichSide(line.start) < 0) {
				outHitResult.impactNormal = -outHitResult.impactNormal;
			}

			return true;
		}

		return false;
	}

	inline	// TODO: Test required.
		bool LineLineIntersectWithResult(const Line& l1, const Line& l2, HitResult& outHitResult) {
		// NOTE: ChatGPT 4 Generated Code
		Math::Vector2 p1 = l1.start;
		Math::Vector2 p2 = l1.end;
		Math::Vector2 p3 = l2.start;
		Math::Vector2 p4 = l2.end;

		Math::Vector2 r = p2 - p1; // Direction vector of the first line
		Math::Vector2 s = p4 - p3; // Direction vector of the second line

		float rCrossS = r.x * s.y - r.y * s.x;
		float p1p3CrossS = (p3.x - p1.x) * s.y - (p3.y - p1.y) * s.x;
		float p1p3CrossR = (p3.x - p1.x) * r.y - (p3.y - p1.y) * r.x;

		// Check if the lines are parallel (rCrossS == 0)
		if (std::fabs(rCrossS) < EPSILON) {
			return false;  // Lines are parallel and there's no intersection
		}

		float t = p1p3CrossS / rCrossS;
		float u = p1p3CrossR / rCrossS;

		// Check if the scalar t and u are within the range of 0 and 1 to lie within the line segments
		if (t >= 0 && t <= 1 && u >= 0 && u <= 1) {
			outHitResult.impactPoint = p1 + t * r;  // Calculate the intersection point

			// The normal of the intersection can be calculated as perpendicular to the direction vector of either line
			outHitResult.impactNormal = Math::Vector2(-r.y, r.x);
			outHitResult.impactNormal.Normalize();

			// Optional adjustments based on which side the intersection occurs
			if (outHitResult.impactNormal.Dot(s) > 0) {
				outHitResult.impactNormal = -outHitResult.impactNormal;  // Ensure the normal points in a consistent direction
			}

			return true;
		}

		return false;  // No intersection within the bounds of the line segments
	}

	inline	// TODO: Test required.
		bool LineBoxIntersectWithResult(const Line& line, const Box& box, HitResult& outHitResult) {
		// NOTE: ChatGPT 4 Generated Code
		Math::Vector2 delta = line.end - line.start; // Direction vector of the line

		// These will store the parametric values of the intersections along the line
		float tmin = 0.0f;  // Start of the line
		float tmax = 1.0f;  // End of the line

		// Check against each axis (x and y)
		for (int i = 0; i < 2; ++i) {
			float p = (i == 0) ? line.start.x : line.start.y;
			float q = (i == 0) ? box.ul.x : box.lr.y;  // Use lower bounds for min
			float s = (i == 0) ? box.lr.x : box.ul.y;  // Use upper bounds for max
			float d = (i == 0) ? delta.x : delta.y;

			if (std::fabs(d) < EPSILON) {
				// Line is parallel to this axis
				if (p < q || p > s) {
					return false;  // Line is outside the slab
				}
			}
			else {
				float t1 = (q - p) / d; // Intersection with the near plane
				float t2 = (s - p) / d; // Intersection with the far plane

				if (t1 > t2) std::swap(t1, t2); // Ensure t1 is intersection with near plane

				tmin = (std::max)(tmin, t1);
				tmax = (std::min)(tmax, t2);

				if (tmin > tmax) {
					return false; // No intersection
				}
			}
		}

		// If we reach here, tmin and tmax define the points of intersection
		Math::Vector2 intersectionPoint = line.start + delta * tmin;
		Math::Vector2 exitPoint = line.start + delta * tmax;

		// Calculate normal based on the point of entry into the box
		Math::Vector2 normal;
		if (intersectionPoint.x == box.ul.x || intersectionPoint.x == box.lr.x) {
			normal = Math::Vector2((intersectionPoint.x == box.ul.x) ? -1.0f : 1.0f, 0.0f);
		}
		else {
			normal = Math::Vector2(0.0f, (intersectionPoint.y == box.ul.y) ? 1.0f : -1.0f);
		}

		outHitResult.impactPoint = intersectionPoint;
		outHitResult.impactNormal = normal;

		return true; // There was an intersection, and the hit result has been updated
	}

	inline	// TODO: Test required.
		bool LineCircleIntersectWithResult(const Line& line, const Circle& circle, HitResult& outHitResult) {
		// NOTE: ChatGPT 4 Generated Code

		Math::Vector2 ab = line.end - line.start;
		Math::Vector2 ac = circle.center - line.start;

		float ab_len = ab.Length();
		Math::Vector2 ab_norm = ab / ab_len;
		float proj_len = ac.Dot(ab_norm);
		Math::Vector2 closest = line.start + ab_norm * (std::max)(0.f, (std::min)(proj_len, ab_len));

		Math::Vector2 normal = closest - circle.center;
		float dist = normal.Length();
		normal.Normalize();

		if (dist < circle.radius) {
			outHitResult.impactNormal = normal;
			outHitResult.impactPoint = closest;

			outHitResult.normal = normal;
			outHitResult.penetrationDepth = circle.radius - dist;
			return true;
		}

		return false;
	}

	inline	// TODO: Test required.
		bool LineCapsuleIntersectWithResult(const Line& line, const Capsule& capsule, HitResult& outHitResult) {
		// NOTE: ChatGPT 4 Generated Code

		Math::Vector2 lineDir = line.end - line.start; // Direction vector of the line segment
		lineDir.Normalize();

		Math::Vector2 capsuleDir = capsule.Direction(); // Direction vector of the capsule
		capsuleDir.Normalize();

		// Calculate the nearest points on the capsule line segment to the line segment
		Math::Vector2 p1 = capsule.center - capsuleDir * capsule.extent; // One end of the capsule
		Math::Vector2 p2 = capsule.center + capsuleDir * capsule.extent; // Other end of the capsule

		Math::Vector2 nearestPoint;
		float minDist = (std::numeric_limits<float>::max)();

		// Check for nearest point on the capsule line to the start and end points of the line
		for (const auto& point : { line.start, line.end }) {
			float d = (point - p1).Dot(p2 - p1);
			float l = (p2 - p1).LengthSquared();
			float u = Clamp(d / l, 0.0f, 1.0f);
			Math::Vector2 closest = p1 + u * (p2 - p1);

			float dist = (closest - point).Length();
			if (dist < minDist) {
				minDist = dist;
				nearestPoint = closest;
			}
		}

		// Check if the minimum distance is within the radius of the capsule
		if (minDist <= capsule.radius) {
			Math::Vector2 normal = (nearestPoint - line.start); normal.Normalize();
			outHitResult.impactPoint = nearestPoint;
			outHitResult.impactNormal = normal; // Adjust normal calculation as needed
			return true;
		}

		return false;
	}

	inline	// TODO: Test required.
		bool LinePolygonIntersectWithResult(const Line& line, const TPolygon& polygon, HitResult& outHitResult) {
		// NOTE: ChatGPT 4 Generated Code

		Math::Vector2 lineVec = line.end - line.start;
		float minDist = (std::numeric_limits<float>::max)();
		bool hasIntersection = false;

		// Iterate over each edge of the polygon
		for (size_t i = 0; i < polygon.points.size(); ++i) {
			size_t j = (i + 1) % polygon.points.size();
			Math::Vector2 start = polygon.points[i];
			Math::Vector2 end = polygon.points[j];
			Math::Vector2 edge = end - start;
			Math::Vector2 edgeNormal(-edge.y, edge.x); edgeNormal.Normalize();

			// Line equation parameters
			Math::Vector2 dir = line.end - line.start;
			float denominator = edge.x * dir.y - edge.y * dir.x;

			if (std::fabs(denominator) < EPSILON) {
				continue; // Lines are parallel, no intersection
			}

			Math::Vector2 difference = line.start - start;
			float t = (difference.x * dir.y - difference.y * dir.x) / denominator;
			float u = (difference.x * edge.y - difference.y * edge.x) / denominator;

			if (t >= 0 && t <= 1 && u >= 0 && u <= 1) {
				Math::Vector2 intersectionPoint = start + t * edge;
				float dist = (intersectionPoint - line.start).Length();

				if (dist < minDist) {
					minDist = dist;
					hasIntersection = true;
					outHitResult.impactPoint = intersectionPoint;
					outHitResult.impactNormal = edgeNormal;
					// Assuming the line segment is quite small, we can approximate the collision normal directly from the edge
				}
			}
		}

		return hasIntersection;
	}

	inline	// TODO: Test required.
		bool BoxLineIntersectWithResult(const Box& box, const Line& line, HitResult& outHitResult) {
		// NOTE: ChatGPT 4 Generated Code

		Math::Vector2 p = line.start;
		Math::Vector2 d = line.end - line.start; // Direction vector of the line

		float tmin = 0.0; // Start of the normalized line
		float tmax = 1.0; // End of the normalized line

		// For each pair of parallel planes bounding the box
		for (int i = 0; i < 2; i++) {
			float q;
			float r;
			if (i == 0) { // Checking x-axis planes
				q = box.ul.x - p.x; // left side
				r = box.lr.x - p.x; // right side
			}
			else { // Checking y-axis planes
				q = box.lr.y - p.y; // bottom side
				r = box.ul.y - p.y; // top side
			}

			float dVal = (i == 0) ? d.x : d.y;

			if (std::abs(dVal) < EPSILON) { // Line is parallel to slab. No hit if origin not within slab
				if (q > 0 || r < 0) return false; // Line is parallel and outside the box
			}
			else {
				float t1 = q / dVal; // Entry intersection distance
				float t2 = r / dVal; // Exit intersection distance

				if (t1 > t2) std::swap(t1, t2); // Ensure t1 <= t2

				if (t1 > tmin) tmin = t1; // We want the furthest entry point
				if (t2 < tmax) tmax = t2; // We want the nearest exit point

				if (tmin > tmax) return false; // If the line exits before it enters, no intersection
			}
		}

		// Compute the actual intersection point
		if (tmin < 0 || tmin > 1) return false; // Ensure that the intersection point is within the line segment

		Math::Vector2 intersection = p + d * tmin;
		outHitResult.impactPoint = intersection;
		Math::Vector2 center = box.GetCenter();
		Math::Vector2 toCenter = center - intersection;
		toCenter.Normalize();

		// The normal is from the intersection point to the center of the box
		outHitResult.normal = toCenter;
		outHitResult.impactNormal = toCenter; // Impact normal is the opposite

		// Calculate penetration depth as the distance from the intersection to the nearest box edge along the line direction
		float distanceInside = (box.lr - box.ul).Length() / 2.0; // Approximate penetration using box size
		outHitResult.penetrationDepth = distanceInside - (intersection - center).Length();

		return true;
	}

	inline	// NOTE: VERIFIED
		bool BoxBoxIntersectWithResult(const Box& box1, const Box& box2, HitResult& outHitResult) {
		// NOTE: ChatGPT 4 Generated Code

		// Extract dimensions and positions from both boxes
		Math::Vector2 box1Min = box1.ul;
		Math::Vector2 box1Max = box1.lr;
		Math::Vector2 box2Min = box2.ul;
		Math::Vector2 box2Max = box2.lr;

		// Calculate overlap on the x-axis
		float xOverlap = (std::max)(0.0f, (std::min)(box1Max.x, box2Max.x) - (std::max)(box1Min.x, box2Min.x));
		// Calculate overlap on the y-axis
		float yOverlap = (std::max)(0.0f, (std::min)(box1Max.y, box2Max.y) - (std::max)(box1Min.y, box2Min.y));

		if (xOverlap > 0 && yOverlap > 0) {
			// Calculate penetration depth (smallest overlap dimension)
			float penetrationDepth = (std::min)(xOverlap, yOverlap);

			// Determine the direction of the normal
			Math::Vector2 normal;
			if (xOverlap < yOverlap) {
				// Penetration is less in the x direction
				normal = box2Min.x > box1Min.x ? Math::Vector2(-1, 0) : Math::Vector2(1, 0);
			}
			else {
				// Penetration is less in the y direction
				normal = box2Min.y > box1Min.y ? Math::Vector2(0, -1) : Math::Vector2(0, 1);
			}

			// Set impact point
			Math::Vector2 impactPoint = {
					(std::max)(box1Min.x, box2Min.x) + xOverlap / 2.0f,
					(std::max)(box1Min.y, box2Min.y) + yOverlap / 2.0f
			};

			// Fill outHitResult
			outHitResult.impactPoint = impactPoint;
			outHitResult.impactNormal = normal; // This is the outward normal from the surface of box2
			outHitResult.normal = normal; // Same as impactNormal since box2 is the impacted object
			outHitResult.penetrationDepth = penetrationDepth;

			return true;
		}

		return false;
	}

	inline	// NOTE: VERIFIED
		bool BoxCircleIntersectWithResult(const Box& box, const Circle& circle, HitResult& outHitResult) {
		// NOTE: ChatGPT 4 Generated Code

		Math::Vector2 closestPoint = {
				(std::max)(box.ul.x, (std::min)(circle.center.x, box.lr.x)),
				(std::max)(box.ul.y, (std::min)(circle.center.y, box.lr.y))
		};

		// Calculate the distance from the closest point on the box to the center of the circle
		Math::Vector2 delta = closestPoint - circle.center;
		float distanceSquared = delta.LengthSquared();
		float radiusSquared = circle.radius * circle.radius;

		if (distanceSquared <= radiusSquared) {
			// They intersect
			float distance = std::sqrt(distanceSquared);
			float penetrationDepth = circle.radius - distance;

			// The normal should be pointing from the circle to the box
			Math::Vector2 normal = delta;
			normal.Normalize();

			// Fill outHitResult
			outHitResult.impactPoint = closestPoint;
			outHitResult.impactNormal = normal; // This is the normal pointing from the circle to the box
			outHitResult.normal = normal; // This is the direction from the center of the circle to the impact point
			outHitResult.penetrationDepth = penetrationDepth;

			return true;
		}

		return false;
	}

	inline	// NOTE: VERIFIED
		bool BoxCapsuleIntersectWithResult(const Box& box, const Capsule& capsule, HitResult& outHitResult) {
		// NOTE: ChatGPT 4 Generated Code

		// Capsule endpoints
		Math::Vector2 capsuleDirectionNormalized = capsule.direction;
		capsuleDirectionNormalized.Normalize();
		Math::Vector2 A = capsule.center - capsuleDirectionNormalized * capsule.extent;
		Math::Vector2 B = capsule.center + capsuleDirectionNormalized * capsule.extent;

		// Find the closest point on the capsule line segment to the nearest point on the box
		Math::Vector2 nearestBoxPoint = {
				(std::max)(box.ul.x, (std::min)((A.x + B.x) / 2, box.lr.x)),
				(std::max)(box.ul.y, (std::min)((A.y + B.y) / 2, box.lr.y))
		};

		// Closest point on capsule segment to nearestBoxPoint
		Math::Vector2 AP = nearestBoxPoint - A;
		Math::Vector2 AB = B - A;
		float AB_mag_squared = AB.Dot(AB);
		float AP_dot_AB = AP.Dot(AB);
		float t = std::clamp(AP_dot_AB / AB_mag_squared, 0.0f, 1.0f);
		Math::Vector2 closest = A + AB * t;

		// Distance from closest point on capsule to box point
		Math::Vector2 distanceVec = nearestBoxPoint - closest;
		float distance = distanceVec.Length();

		// Check if the closest point is within the capsule's radius
		if (distance <= capsule.radius) {
			Math::Vector2 normal = distanceVec / distance;
			outHitResult.impactPoint = closest;
			outHitResult.impactNormal = normal; // Normalize distance vector
			outHitResult.normal = normal;     // Normal pointing from the capsule to the box
			outHitResult.penetrationDepth = capsule.radius - distance;

			return true;
		}

		return false;
	}

	inline	// TODO: Test required.
		bool BoxPolygonIntersectWithResult(const Box& box, const TPolygon& polygon, HitResult& outHitResult) {
		// NOTE: ChatGPT 4 Generated Code

		// Collect all relevant axes for the SAT (Separating Axis Theorem)
		std::vector<Math::Vector2> axes = {
				{1, 0}, // Horizontal normal
				{0, 1}  // Vertical normal
		};

		// Add polygon's edge normals
		for (size_t i = 0; i < polygon.points.size(); i++) {
			size_t next = (i + 1) % polygon.points.size();
			Math::Vector2 edge = polygon.points[next] - polygon.points[i];
			Math::Vector2 normal = { -edge.y, edge.x }; // Perpendicular to the edge
			normal.Normalize();
			axes.push_back(normal);
		}

		float minOverlap = std::numeric_limits<float>::infinity();
		Math::Vector2 smallestAxis;
		bool overlapFound = true;

		// Check overlap for each axis
		for (const auto& axis : axes) {
			float boxMin, boxMax, polyMin, polyMax;

			// Project box onto the axis
			boxMin = boxMax = axis.Dot(box.ul);
			for (const auto& corner : box.GetVertices()) {
				float projection = axis.Dot(corner);
				boxMin = (std::min)(boxMin, projection);
				boxMax = (std::max)(boxMax, projection);
			}

			// Project polygon onto the axis
			polyMin = polyMax = axis.Dot(polygon.points[0]);
			for (const auto& point : polygon.points) {
				float projection = axis.Dot(point);
				polyMin = (std::min)(polyMin, projection);
				polyMax = (std::max)(polyMax, projection);
			}

			// Check for no overlap
			float overlap = (std::min)(boxMax, polyMax) - (std::max)(boxMin, polyMin);
			if (overlap < 0) {
				overlapFound = false; // Found a separating axis
				break;
			}

			// Find the axis with the minimum overlap
			if (overlap < minOverlap) {
				minOverlap = overlap;
				smallestAxis = axis;
			}
		}

		if (overlapFound) {
			// Intersection detected, fill in the HitResult
			outHitResult.impactNormal = smallestAxis;
			outHitResult.normal = smallestAxis;
			outHitResult.penetrationDepth = minOverlap;

			// Calculate an approximate impact point as the midpoint of overlap
			Math::Vector2 center = box.GetCenter();
			outHitResult.impactPoint = center + smallestAxis * (minOverlap / 2.0f);
			return true;
		}

		return false;
	}

	inline	// TODO: Test required.
		bool CircleLineIntersectWithResult(const Circle& circle, const Line& line, HitResult& outHitResult) {
		// NOTE: ChatGPT 4 Generated Code

		Math::Vector2 ab = line.end - line.start;
		Math::Vector2 ac = circle.center - line.start;
		Math::Vector2 ab_normalized = ab; ab_normalized.Normalize();

		float projection_length = ac.Dot(ab_normalized);
		Math::Vector2 nearest_point = line.start + ab_normalized * (std::max)(0.0f, (std::min)(projection_length, ab.Length()));

		Math::Vector2 circle_to_nearest = nearest_point - circle.center;
		float dist = circle_to_nearest.Length();
		circle_to_nearest.Normalize();

		if (dist <= circle.radius) {
			outHitResult.impactPoint = nearest_point;
			outHitResult.impactNormal = -circle_to_nearest;
			outHitResult.normal = -circle_to_nearest;
			outHitResult.penetrationDepth = circle.radius - dist;

			return true;
		}

		return false;
	}

	inline	// NOTE: VERIFIED
		bool CircleBoxIntersectWithResult(const Circle& circle, const Box& box, HitResult& outHitResult) {
		// NOTE: ChatGPT 4 Generated Code

		// Clamping the circle center to the box bounds to find the closest point
		float clampedX = (std::max)(box.ul.x, (std::min)(circle.center.x, box.lr.x));
		float clampedY = (std::max)(box.ul.y, (std::min)(circle.center.y, box.lr.y));

		Math::Vector2 closestPoint(clampedX, clampedY);
		Math::Vector2 circleToClosest = circle.center - closestPoint;
		float distanceSquared = circleToClosest.LengthSquared();
		circleToClosest.Normalize();

		if (distanceSquared <= (circle.radius * circle.radius)) {
			// There's an intersection
			outHitResult.impactPoint = closestPoint;
			outHitResult.impactNormal = circleToClosest;
			outHitResult.normal = circleToClosest;
			outHitResult.penetrationDepth = circle.radius - sqrt(distanceSquared);  // Compute how deep the circle penetrates into the box

			return true;
		}

		return false;
	}

	inline	// NOTE: VERIFIED
		bool CircleCircleIntersectWithResult(const Circle& circle1, const Circle& circle2, HitResult& outHitResult) {
		// NOTE: ChatGPT 4 Generated Code

		Math::Vector2 distVector = circle1.center - circle2.center;
		float distance = distVector.Length();
		float radiusSum = circle1.radius + circle2.radius;

		if (distance < radiusSum) {
			// Calculate penetration depth
			outHitResult.penetrationDepth = radiusSum - distance;

			// Impact normal points from the center of the impacted circle (circle2) to the collider (circle1)
			Math::Vector2 impactNormal = distVector / distance; // Normalize the distance vector

			// Impact point on the surface of the impacted circle (circle2)
			outHitResult.impactPoint = circle2.center + impactNormal * circle2.radius;

			// Normal is the same as the impact normal in this symmetrical collision
			outHitResult.normal = impactNormal;

			// The impactNormal is as described, pointing outwards from circle2 towards circle1
			outHitResult.impactNormal = impactNormal;

			return true;
		}

		return false;
	}

	inline	// NOTE: VERIFIED
		bool CircleCapsuleIntersectWithResult(const Circle& circle, const Capsule& capsule, HitResult& outHitResult) {
		// NOTE: ChatGPT 4 Generated Code
		// Normalize direction vector of the capsule
		Math::Vector2 capsuleDir = capsule.Direction();
		capsuleDir.Normalize();

		// Calculate the endpoints of the capsule segment
		Math::Vector2 capsuleStart = capsule.Center() - capsuleDir * capsule.Extent();
		Math::Vector2 capsuleEnd = capsule.Center() + capsuleDir * capsule.Extent();

		// Calculate the closest point on the capsule segment to the circle center
		Math::Vector2 circleCenter = circle.center;
		Math::Vector2 closestPointOnCapsule = Line{ capsuleStart, capsuleEnd }.NearestPoint(circleCenter);

		// Calculate the vector from the closest point on the capsule to the circle center
		Math::Vector2 closestVector = circleCenter - closestPointOnCapsule;
		float distance = closestVector.Length();
		closestVector.Normalize();

		// Check if the distance is less than the sum of their radii
		float combinedRadii = circle.radius + capsule.radius;
		if (distance <= combinedRadii) {
			outHitResult.impactPoint = closestPointOnCapsule;
			outHitResult.impactNormal = closestVector;
			outHitResult.normal = closestVector;
			outHitResult.penetrationDepth = combinedRadii - distance;
			return true;
		}

		return false;
	}


	inline	// TODO: Test required.
		bool CirclePolygonIntersectWithResult(const Circle& circle, const TPolygon& polygon, HitResult& outHitResult) {
		// NOTE: ChatGPT 4 Generated Code
		float minDist = (std::numeric_limits<float>::max)();
		Math::Vector2 closestPoint;
		Math::Vector2 closestNormal;

		for (size_t i = 0; i < polygon.points.size(); i++) {
			size_t next = (i + 1) % polygon.points.size();
			Math::Vector2 start = polygon.points[i];
			Math::Vector2 end = polygon.points[next];

			Math::Vector2 edge = end - start;
			Math::Vector2 toCircle = circle.center - start;
			float edgeLengthSquared = edge.LengthSquared();
			float t = (std::max)(0.0f, (std::min)(1.0f, toCircle.Dot(edge) / edgeLengthSquared));
			Math::Vector2 projection = start + t * edge;
			Math::Vector2 normal = (circle.center - projection);
			float dist = normal.Length();

			if (dist < minDist) {
				minDist = dist;
				closestPoint = projection;
				closestNormal = normal / dist;  // Normalize the vector
			}
		}

		if (minDist < circle.radius) {
			outHitResult.impactNormal = closestNormal;
			outHitResult.impactPoint = closestPoint;
			outHitResult.normal = closestNormal;
			outHitResult.penetrationDepth = circle.radius - minDist;
			return true;
		}

		return false;
	}

	inline	// TODO: Test required.
		bool CapsuleLineIntersectWithResult(const Capsule& capsule, const Line& line, HitResult& outHitResult) {
		// NOTE: ChatGPT 4 Generated Code
// Define capsule endpoints
		Math::Vector2 capsuleDirection = capsule.Direction();
		capsuleDirection.Normalize();
		Math::Vector2 A = capsule.Center() - capsuleDirection * capsule.Extent(); // Start of the capsule
		Math::Vector2 B = capsule.Center() + capsuleDirection * capsule.Extent(); // End of the capsule

		// Line segment details
		Math::Vector2 C = line.start;
		Math::Vector2 D = line.end;

		// Calculate parameters
		Math::Vector2 AB = B - A;
		Math::Vector2 CD = D - C;
		Math::Vector2 AC = C - A;

		float a = AB.Dot(AB); // Always >= 0
		float b = AB.Dot(CD);
		float c = CD.Dot(CD); // Always >= 0
		float d = AB.Dot(AC);
		float e = CD.Dot(AC);

		float denominator = a * c - b * b; // Always >= 0

		// Initialize parameters to be used
		float s, t;
		Math::Vector2 closestPointOnCapsule, closestPointOnLine;

		// Check lines' relation to each other (parallel or not)
		if (denominator < EPSILON) {
			// The lines are parallel, use the start of the capsule as base
			s = 0.0;
			t = (b > c ? d / b : e / c); // Use the largest denominator
		}
		else {
			// Lines are not parallel
			s = (b * e - c * d) / denominator;
			t = (a * e - b * d) / denominator;
		}

		// Restrict s and t to the boundaries of the segments
		s = (std::max)(0.0f, (std::min)(1.0f, s));
		t = (std::max)(0.0f, (std::min)(1.0f, t));

		// Calculate the closest points
		closestPointOnCapsule = A + s * AB;
		closestPointOnLine = C + t * CD;

		// Calculate the vector between the closest points
		Math::Vector2 closestVec = closestPointOnLine - closestPointOnCapsule;
		float distanceSquared = closestVec.LengthSquared();
		closestVec.Normalize();

		if (distanceSquared <= capsule.radius * capsule.radius) {
			// There is an intersection
			outHitResult.impactPoint = closestPointOnLine;
			Math::Vector2 normal = closestVec;
			outHitResult.impactNormal = normal; // Normal from capsule to line
			outHitResult.normal = normal; // Normal from capsule's surface to the line
			outHitResult.penetrationDepth = capsule.radius - std::sqrt(distanceSquared); // Penetration depth

			return true;
		}

		return false;
	}

	inline	// NOTE: VERIFIED
		bool CapsuleBoxIntersectWithResult(const Capsule& capsule, const Box& box, HitResult& outHitResult) {
		// NOTE: ChatGPT 4 Generated Code
		// 
		// Capsule endpoints
		Math::Vector2 capsuleDirectionNormalized = capsule.direction;
		capsuleDirectionNormalized.Normalize();
		Math::Vector2 A = capsule.center - capsuleDirectionNormalized * capsule.extent;
		Math::Vector2 B = capsule.center + capsuleDirectionNormalized * capsule.extent;

		// Find the closest point on the capsule line segment to the nearest point on the box
		Math::Vector2 nearestBoxPoint = {
				(std::max)(box.ul.x, (std::min)((A.x + B.x) / 2, box.lr.x)),
				(std::max)(box.ul.y, (std::min)((A.y + B.y) / 2, box.lr.y))
		};

		// Closest point on capsule segment to nearestBoxPoint
		Math::Vector2 AP = nearestBoxPoint - A;
		Math::Vector2 AB = B - A;
		float AB_mag_squared = AB.Dot(AB);
		float AP_dot_AB = AP.Dot(AB);
		float t = std::clamp(AP_dot_AB / AB_mag_squared, 0.0f, 1.0f);
		Math::Vector2 closest = A + AB * t;

		// Distance from closest point on capsule to box point
		Math::Vector2 distanceVec = closest  - nearestBoxPoint;
		float distance = distanceVec.Length();

		// Check if the closest point is within the capsule's radius
		if (distance <= capsule.radius) {
			Math::Vector2 normal = distanceVec / distance;
			outHitResult.impactPoint = closest;
			outHitResult.impactNormal = normal; // Normalize distance vector
			outHitResult.normal = normal;     // Normal pointing from the capsule to the box
			outHitResult.penetrationDepth = capsule.radius - distance;

			return true;
		}

		return false;
	}

	inline	// NOTE: VERIFIED
		bool CapsuleCircleIntersectWithResult(const Capsule& capsule, const Circle& circle, HitResult& outHitResult) {
		// NOTE: ChatGPT 4 Generated Code
// Calculate normalized direction of the capsule
		Math::Vector2 capsuleDir = capsule.Direction();
		capsuleDir.Normalize();

		// Get the start and end points of the capsule
		Math::Vector2 capsuleStart = capsule.Center() - capsuleDir * capsule.Extent();
		Math::Vector2 capsuleEnd = capsule.Center() + capsuleDir * capsule.Extent();

		// Find the closest point on the capsule line segment to the circle's center
		Math::Vector2 ac = circle.center - capsuleStart;
		Math::Vector2 ab = capsuleEnd - capsuleStart;
		float ab_magnitude = ab.Length();
		float ac_dot_ab = ac.Dot(ab);
		float t = ac_dot_ab / (ab_magnitude * ab_magnitude);
		t = (std::max)(0.0f, (std::min)(1.0f, t));  // Clamp t to the segment
		Math::Vector2 closestPoint = capsuleStart + ab * t;

		// Calculate the distance from the closest point to the circle's center
		Math::Vector2 difference = closestPoint - circle.center;
		float distance = difference.Length();
		difference.Normalize();

		// Check for intersection
		if (distance <= (capsule.radius + circle.radius)) {
			// There is an intersection
			outHitResult.impactPoint = closestPoint;
			outHitResult.impactNormal = difference;  // Normal points from the circle to the capsule

			// Compute the penetration depth
			if (distance != 0) {
				outHitResult.penetrationDepth = (capsule.radius + circle.radius) - distance;
			}
			else {
				outHitResult.penetrationDepth = capsule.radius + circle.radius;  // Fully embedded
			}

			// Set the normal from the impacted circle's center to the impact point
			outHitResult.normal = difference;

			return true;
		}

		return false;
	}

	/**
	 * @brief Capsule and capsule intersection with result.
	 * @param capsule1 Impacting capsule
	 * @param capsule2 Impacted capsule
	 * @param outHitResult Normal should poing from capsule 2 to capsule 1.
	 * @return
	 */
	inline	// NOTE: VERIFIED
		bool CapsuleCapsuleIntersectWithResult(const Capsule& capsule1, const Capsule& capsule2, HitResult& outHitResult) {
		// https://wickedengine.net/2020/04/capsule-collision-detection/
		// Normalize direction vectors considering the coordinate system
		
		Math::Vector2 normal1 = capsule1.Direction();
		float radius1 = capsule1.Radius();
		Math::Vector2 normal2 = capsule2.direction;
		float radius2 = capsule2.Radius();

		Math::Vector2 start1 = capsule1.Center() + normal1 * capsule1.HalfLength();
		Math::Vector2 end1 = capsule1.Center() - normal1 * capsule1.HalfLength();
		Math::Vector2 start2 = capsule2.Center() + normal2 * capsule2.HalfLength();
		Math::Vector2 end2 = capsule2.Center() - normal2 * capsule2.HalfLength();

		Math::Vector2 lineEndOffset1 = normal1 * radius1;
		Math::Vector2 a1 = end1 + lineEndOffset1;
		Math::Vector2 b1 = start1 - lineEndOffset1;

		Math::Vector2 lineEndOffset2 = normal2 * radius2;
		Math::Vector2 a2 = end2 + lineEndOffset2;
		Math::Vector2 b2 = start2 - lineEndOffset2;

		// Vectors between line endpoints;
		Math::Vector2 v0 = a2 - a1;
		Math::Vector2 v1 = b2 - a1;
		Math::Vector2 v2 = a2 - b1;
		Math::Vector2 v3 = b2 - b1;

		// Squared distances;
		float d0 = v0.Dot(v0);
		float d1 = v1.Dot(v1);
		float d2 = v2.Dot(v2);
		float d3 = v3.Dot(v3);

		// Select best potential endpoint on capsule 1
		Math::Vector2 best1;
		if (d2 < d0 || d2 < d1 || d3 < d0 || d3 < d1) {
			best1 = b1;
		}
		else {
			best1 = a1;
		}

		// Select point on capsule 2 line segment nearest to best potential endpoint on capsule 1
		Math::Vector2 best2 = Line{ a2, b2 }.NearestPoint(best1);
		// Do the same thing for capsule 1
		best1 = Line{ a1, b1 }.NearestPoint(best2);

		// Penetration normal
		Math::Vector2 penetrationNormal = best1 - best2;
		float penetrationDepth = (radius1 + radius2) - penetrationNormal.Length();
		penetrationNormal.Normalize();
		
		// Check if the distance is less than the sum of their radii
		if (penetrationDepth > 0) {
			outHitResult.impactPoint = best2; // Midpoint of the closest points as impact point
			outHitResult.impactNormal = penetrationNormal; // Normal from capsule1 to capsule2 at the impact point
			outHitResult.normal = penetrationNormal; // Normal from the impacted capsule to the collider
			outHitResult.penetrationDepth = penetrationDepth; // Penetration depth
			return true;
		}

		return false;
	}

	inline	// TODO: Test required.
		bool CapsulePolygonIntersectWithResult(const Capsule& capsule, const TPolygon& polygon, HitResult& outHitResult) {
		// NOTE: ChatGPT 4 Generated Code
// First, get capsule's line segment
		Math::Vector2 capsuleDir = capsule.Direction();
		capsuleDir.Normalize();
		Math::Vector2 capsuleStart = capsule.Center() - capsuleDir * capsule.Extent();
		Math::Vector2 capsuleEnd = capsule.Center() + capsuleDir * capsule.Extent();
		float minDistance = (std::numeric_limits<float>::max)();
		bool intersected = false;

		// Check intersection with each edge of the polygon
		for (size_t i = 0; i < polygon.points.size(); ++i) {
			Math::Vector2 point1 = polygon.points[i];
			Math::Vector2 point2 = polygon.points[(i + 1) % polygon.points.size()];

			// Line equation from point1 to point2
			Math::Vector2 lineDir = point2 - point1;
			Math::Vector2 lineStart = point1;
			Math::Vector2 lineEnd = point2;

			// Closest point calculation on both the capsule segment and polygon edge
			Math::Vector2 d1 = capsuleEnd - capsuleStart;
			Math::Vector2 d2 = lineEnd - lineStart;
			Math::Vector2 r = capsuleStart - lineStart;
			float a = d1.Dot(d1);
			float e = d2.Dot(d2);
			float f = d2.Dot(r);

			// Check if the lines are nearly parallel
			float c = d1.Dot(r);
			float b = d1.Dot(d2);
			float denom = a * e - b * b;

			float s, t;
			if (std::fabs(denom) < EPSILON) {
				s = 0.0;
				t = (b > c ? 0.0f : 1.0f); // Use extreme points if parallel
			}
			else {
				s = std::clamp((b * f - c * e) / denom, 0.0f, 1.0f);
				t = std::clamp((a * f - b * c) / denom, 0.0f, 1.0f);
			}

			// Closest points on both segments
			Math::Vector2 closestOnCapsule = capsuleStart + d1 * s;
			Math::Vector2 closestOnLine = lineStart + d2 * t;
			Math::Vector2 closestVector = closestOnLine - closestOnCapsule;
			float distance = closestVector.Length();

			// Check if the minimum distance is less than the radius of the capsule
			if (distance <= capsule.radius && distance < minDistance) {
				minDistance = distance;
				intersected = true;
				outHitResult.impactPoint = closestOnLine;
				closestVector.Normalize();
				outHitResult.impactNormal = closestVector; // Normal points from polygon edge to capsule
				outHitResult.normal = closestVector;
				outHitResult.penetrationDepth = capsule.radius - distance;
			}
		}

		return intersected;
	}

	inline	// TODO: Test required.
		bool PolygonLineIntersectWithResult(const TPolygon& polygon, const Line& line, HitResult& outHitResult) {
		// NOTE: ChatGPT 4 Generated Code
		Math::Vector2 lineDir = line.end - line.start;
		lineDir.Normalize();
		float closestDist = (std::numeric_limits<float>::max)();
		bool hasIntersection = false;

		// Iterate through each edge of the polygon
		for (size_t i = 0; i < polygon.numPoints; ++i) {
			Math::Vector2 point1 = polygon.points[i];
			Math::Vector2 point2 = polygon.points[(i + 1) % polygon.numPoints];

			// Calculate parameters for the line intersection formula
			Math::Vector2 edgeDir = point2 - point1;
			Math::Vector2 edgeNormal(-edgeDir.y, edgeDir.x); // Perpendicular to the edge
			edgeNormal.Normalize();
			Math::Vector2 edgeStart = point1;
			Math::Vector2 r = line.start - edgeStart;
			float a = edgeDir.Dot(edgeDir);  // Dot product of edge direction
			float b = edgeDir.Dot(lineDir);  // Cross direction dot product
			float e = lineDir.Dot(lineDir);  // Dot product of line direction
			float d = r.Dot(edgeDir);        // r dot edge direction
			float c = r.Dot(lineDir);        // r dot line direction
			float denominator = a * e - b * b;

			if (fabs(denominator) < EPSILON) {
				continue;  // Lines are parallel
			}

			// Calculate the intersection t for the line
			float t = (b * d - a * c) / denominator;
			float u = (e * d - b * c) / denominator;

			if (t >= 0.0 && t <= 1.0 && u >= 0.0 && u <= 1.0) {
				// Calculate the intersection point
				Math::Vector2 intersection = line.start + t * lineDir;
				float dist = (intersection - line.start).Length();

				if (dist < closestDist) {
					closestDist = dist;
					hasIntersection = true;

					// Update HitResult
					outHitResult.impactPoint = intersection;
					outHitResult.impactNormal = -edgeNormal;  // Normal from the edge of the polygon
					outHitResult.normal = -edgeNormal;  // Normal should be towards the center of the polygon from the line

					// Calculate penetration depth
					// Penetration depth is the distance the line's end penetrates past the intersection along the line's direction
					Math::Vector2 lineEndRelativeToIntersection = line.end - intersection;
					float penetrationAlongNormal = lineEndRelativeToIntersection.Dot(-edgeNormal);
					outHitResult.penetrationDepth = (penetrationAlongNormal > 0) ? penetrationAlongNormal : 0;
				}
			}
		}

		return hasIntersection;
	}

	inline	// TODO: Test required.
		bool PolygonBoxIntersectWithResult(const TPolygon& polygon, const Box& box, HitResult& outHitResult) {
		// NOTE: ChatGPT 4 Generated Code
		bool hasIntersection = false;
		float minPenetrationDepth = (std::numeric_limits<float>::max)();
		Math::Vector2 minPenetrationNormal;
		Math::Vector2 contactPoint;

		// Iterate through each edge of the box
		std::vector<Math::Vector2> boxVertices = box.GetVertices();
		std::vector<Math::Vector2> normals = {
				{0, 1},  // Normal for top and bottom
				{1, 0}   // Normal for left and right
		};

		for (int i = 0; i < 4; ++i) {
			Math::Vector2 edgeStart = boxVertices[i];
			Math::Vector2 edgeEnd = boxVertices[(i + 1) % 4];
			Math::Vector2 edgeDir = edgeEnd - edgeStart;
			Math::Vector2 edgeNormal(-edgeDir.y, edgeDir.x);
			edgeNormal.Normalize();

			float polygonMin = (std::numeric_limits<float>::max)();
			float polygonMax = -(std::numeric_limits<float>::max)();
			Math::Vector2 closestPoint;

			for (const auto& vertex : polygon.points) {
				float projection = vertex.Dot(edgeNormal);
				polygonMin = (std::min)(polygonMin, projection);
				polygonMax = (std::max)(polygonMax, projection);

				// Checking for closest point along the normal
				if (fabs(projection - polygonMin) < EPSILON) {
					closestPoint = vertex;
				}
			}

			float boxMin = (std::numeric_limits<float>::max)(), boxMax = -(std::numeric_limits<float>::max)();
			for (const auto& vertex : boxVertices) {
				float projection = vertex.Dot(edgeNormal);
				boxMin = (std::min)(boxMin, projection);
				boxMax = (std::max)(boxMax, projection);
			}

			// Check for overlap
			if (polygonMax < boxMin || polygonMin > boxMax) {
				return false;  // No overlap, thus no intersection
			}

			float overlapMin = (std::max)(polygonMin, boxMin);
			float overlapMax = (std::min)(polygonMax, boxMax);
			float overlap = overlapMax - overlapMin;

			if (overlap < minPenetrationDepth) {
				minPenetrationDepth = overlap;
				minPenetrationNormal = (polygonMax > boxMax) ? edgeNormal : -edgeNormal;
				contactPoint = closestPoint; // Set contact point to the closest polygon vertex along current normal
				hasIntersection = true;
			}
		}

		if (hasIntersection) {
			outHitResult.impactNormal = -minPenetrationNormal;
			outHitResult.normal = -minPenetrationNormal; // Normal points from box to polygon
			outHitResult.impactPoint = contactPoint;
			outHitResult.penetrationDepth = minPenetrationDepth;
			return true;
		}

		return false;
	}

	inline	// TODO: Test required.
		bool PolygonCircleIntersectWithResult(const TPolygon& polygon, const Circle& circle, HitResult& outHitResult) {
		// NOTE: ChatGPT 4 Generated Code
		bool collisionDetected = false;
		float minPenetrationDepth = (std::numeric_limits<float>::max)();
		Math::Vector2 minAxis;

		// Check for separation and find minimal penetration depth along polygon axes
		std::vector<Math::Vector2> normals = polygon.GetAxisNormals();
		for (const auto& normal : normals) {
			float circleProjection = circle.center.Dot(normal) - circle.radius;
			float polygonMin = (std::numeric_limits<float>::max)();
			float polygonMax = -(std::numeric_limits<float>::max)();

			for (const auto& vertex : polygon.points) {
				float projection = vertex.Dot(normal);
				polygonMin = (std::min)(polygonMin, projection);
				polygonMax = (std::max)(polygonMax, projection);
			}

			// No overlap if circle's projection is outside the polygon's projection
			if (circleProjection > polygonMax) {
				return false; // Separation found
			}

			float overlap = polygonMax - circleProjection;
			if (overlap < minPenetrationDepth) {
				minPenetrationDepth = overlap;
				minAxis = normal;
				collisionDetected = true;
			}
		}

		// Check for separation and find minimal penetration depth along circle's normal towards each vertex
		for (const auto& vertex : polygon.points) {
			Math::Vector2 toVertex = vertex - circle.center;
			toVertex.Normalize();
			float vertexProjection = vertex.Dot(toVertex);
			float circleProjection = circle.center.Dot(toVertex) + circle.radius;

			float polygonMin = (std::numeric_limits<float>::max)();
			float polygonMax = -(std::numeric_limits<float>::max)();

			for (const auto& p : polygon.points) {
				float projection = p.Dot(toVertex);
				polygonMin = (std::min)(polygonMin, projection);
				polygonMax = (std::max)(polygonMax, projection);
			}

			if (circleProjection < polygonMin) {
				return false; // Separation found
			}

			float overlap = circleProjection - polygonMin;
			if (overlap < minPenetrationDepth) {
				minPenetrationDepth = overlap;
				minAxis = -toVertex; // Normal should point from circle to polygon
				collisionDetected = true;
			}
		}

		if (collisionDetected) {
			// Calculate impact point as the closest point on the polygon to the circle's center
			float minDistance = (std::numeric_limits<float>::max)();
			Math::Vector2 closestPoint;
			for (const auto& vertex : polygon.points) {
				float distance = (vertex - circle.center).Length();
				if (distance < minDistance) {
					minDistance = distance;
					closestPoint = vertex;
				}
			}

			outHitResult.impactNormal = -minAxis; // Direction to resolve the penetration, polygon to circle
			outHitResult.impactPoint = closestPoint; // Closest point on polygon to circle center
			outHitResult.penetrationDepth = minPenetrationDepth; // Depth to move the circle out of collision
			outHitResult.normal = -minAxis; // Normal direction from circle to polygon
			return true;
		}

		return false;
	}

	inline	// TODO: Test required.
		bool PolygonCapsuleIntersectWithResult(const TPolygon& polygon, const Capsule& capsule, HitResult& outHitResult) {
		// NOTE: ChatGPT 4 Generated Code
		float minPenetrationDepth = (std::numeric_limits<float>::max)();
		Math::Vector2 minAxis;
		bool collisionDetected = false;

		// Capsule properties
		Math::Vector2 capsuleDirection = capsule.direction;
		capsuleDirection.Normalize();
		Math::Vector2 capsuleStart = capsule.center - capsuleDirection * capsule.extent;
		Math::Vector2 capsuleEnd = capsule.center + capsuleDirection * capsule.extent;

		// Test polygon edges against capsule
		for (int i = 0; i < polygon.numPoints; ++i) {
			Math::Vector2 currentVertex = polygon.points[i];
			Math::Vector2 nextVertex = polygon.points[(i + 1) % polygon.numPoints];
			Math::Vector2 edge = nextVertex - currentVertex;
			Math::Vector2 edgeNormal = Math::Vector2(-edge.y, edge.x);
			edgeNormal.Normalize();

			// Project capsule onto the edge normal
			float capsuleMin, capsuleMax;
			float dotStart = capsuleStart.Dot(edgeNormal);
			float dotEnd = capsuleEnd.Dot(edgeNormal);
			capsuleMin = (std::min)(dotStart, dotEnd) - capsule.radius;
			capsuleMax = (std::max)(dotStart, dotEnd) + capsule.radius;

			// Project polygon onto the edge normal
			float polygonMin = (std::numeric_limits<float>::max)();
			float polygonMax = -(std::numeric_limits<float>::max)();
			for (const auto& point : polygon.points) {
				float dot = point.Dot(edgeNormal);
				polygonMin = (std::min)(polygonMin, dot);
				polygonMax = (std::max)(polygonMax, dot);
			}

			if (capsuleMax < polygonMin || capsuleMin > polygonMax) {
				return false;  // No overlap, no collision
			}

			// Calculate overlap
			float overlap = (std::min)(capsuleMax - polygonMin, polygonMax - capsuleMin);
			if (overlap < minPenetrationDepth) {
				minPenetrationDepth = overlap;
				minAxis = (capsuleMax - polygonMin < polygonMax - capsuleMin) ? edgeNormal : -edgeNormal;
				collisionDetected = true;
			}
		}

		// Finally, if a collision is detected
		if (collisionDetected) {
			outHitResult.impactNormal = -minAxis;
			outHitResult.penetrationDepth = minPenetrationDepth;
			outHitResult.normal = -minAxis;  // Direction from polygon to capsule
			// Compute impact point: the closest point on capsule to polygon center
			Math::Vector2 polygonCenter = polygon.GetCenter();
			Math::Vector2 closestPoint = capsuleStart + capsuleDirection * ((polygonCenter - capsuleStart).Dot(capsuleDirection));
			outHitResult.impactPoint = closestPoint;
			return true;
		}

		return false;
	}

	inline	// TODO: Test required.
		bool PolygonPolygonIntersectWithResult(const TPolygon& polygon1, const TPolygon& polygon2, HitResult& outHitResult) {
		// NOTE: ChatGPT 4 Generated Code
		float minPenetrationDepth = (std::numeric_limits<float>::max)();
		Math::Vector2 collisionNormal;
		bool collisionDetected = false;
		Math::Vector2 approximateImpactPoint;
		int countContacts = 0;

		// Combine the edges of both polygons for the potential separating axes
		std::vector<Math::Vector2> axes;
		for (int i = 0; i < polygon1.numPoints; i++) {
			Math::Vector2 edge = polygon1.points[(i + 1) % polygon1.numPoints] - polygon1.points[i];
			Math::Vector2 edgeRotated{ -edge.y, edge.x };
			edgeRotated.Normalize();
			axes.push_back(edgeRotated); // Normal of the edge
		}
		for (int i = 0; i < polygon2.numPoints; i++) {
			Math::Vector2 edge = polygon2.points[(i + 1) % polygon2.numPoints] - polygon2.points[i];
			Math::Vector2 edgeRotated{ -edge.y, edge.x };
			edgeRotated.Normalize();
			axes.push_back(edgeRotated); // Normal of the edge
		}

		// Test all potential separating axes for overlap
		for (const auto& axis : axes) {
			// Project both polygons onto the axis
			float min1, max1;
			float min2, max2;

			// Project polygon1
			min1 = max1 = polygon1.points[0].Dot(axis);
			for (const auto& point : polygon1.points) {
				float projection = point.Dot(axis);
				min1 = (std::min)(min1, projection);
				max1 = (std::max)(max1, projection);
			}

			// Project polygon2
			min2 = max2 = polygon2.points[0].Dot(axis);
			for (const auto& point : polygon2.points) {
				float projection = point.Dot(axis);
				min2 = (std::min)(min2, projection);
				max2 = (std::max)(max2, projection);
			}

			// Determine overlap
			float overlap = (std::min)(max1, max2) - (std::max)(min1, min2);
			if (overlap < 0) {
				return false;  // Separating axis found, no intersection
			}

			// Check for minimal penetration depth and calculate contact points
			if (overlap < minPenetrationDepth) {
				minPenetrationDepth = overlap;
				collisionNormal = axis;

				// Ensure the normal points from polygon1 to polygon2
				Math::Vector2 direction = polygon2.GetCenter() - polygon1.GetCenter();
				if (collisionNormal.Dot(direction) < 0) {
					collisionNormal = -collisionNormal;
				}

				// Estimate impact point by averaging edge vertices contributing to overlap
				approximateImpactPoint = (polygon1.GetCenter() + polygon2.GetCenter()) * 0.5f;
			}
		}

		// Collision detected, populate HitResult
		if (minPenetrationDepth < (std::numeric_limits<float>::max)()) {
			outHitResult.impactNormal = -collisionNormal;
			outHitResult.normal = -collisionNormal;  // Normal is used for moving polygon1 out of collision
			outHitResult.penetrationDepth = minPenetrationDepth;
			outHitResult.impactPoint = approximateImpactPoint; // Approximate point of impact
			collisionDetected = true;
		}

		return collisionDetected;
	}


	// No result
	inline
		bool RayRayIntersect(const Ray& ray1, const Ray& ray2) {
		Math::Vector2 intersection;
		return ray1.IntersectionPoint(ray2, intersection);
	}

	inline
		bool RayLineIntersect(const Ray& ray, const Line& line) {
		HitResult inHitResult;
		return RayLineIntersectWithResult(ray, line, inHitResult);
	}

	inline
		bool RayBoxIntersect(const Ray& ray, const Box& box) {
		HitResult inHitResult;
		return RayBoxIntersectWithResult(ray, box, inHitResult);
	}

	inline
		bool RayCircleIntersect(const Ray& ray, const Circle& circle) {
		HitResult inHitResult;
		return RayCircleIntersectWithResult(ray, circle, inHitResult);
	}

	inline
		bool RayCapsuleIntersect(const Ray& ray, const Capsule& capsule) {
		HitResult inHitResult;
		return RayCapsuleIntersectWithResult(ray, capsule, inHitResult);
	}

	inline
		bool RayPolygonIntersect(const Ray& ray, const TPolygon& capsule) {
		HitResult inHitResult;
		return RayPolygonIntersectWithResult(ray, capsule, inHitResult);
	}


	inline
		bool LineLineIntersect(const Line& l1, const Line& l2) {
		HitResult inHitResult;
		return LineLineIntersectWithResult(l1, l2, inHitResult);
	}

	inline
		bool LineBoxIntersect(const Line& line, const Box& box) {
		HitResult inHitResult;
		return LineBoxIntersectWithResult(line, box, inHitResult);
	}

	inline
		bool LineCircleIntersect(const Line& line, const Circle& circle) {
		HitResult inHitResult;
		return LineCircleIntersectWithResult(line, circle, inHitResult);
	}

	inline
		bool LineCapsuleIntersect(const Line& line, const Capsule& capsule) {
		HitResult inHitResult;
		return LineCapsuleIntersectWithResult(line, capsule, inHitResult);
	}

	inline
		bool LinePolygonIntersect(const Line& line, const TPolygon& polygon) {
		HitResult inHitResult;
		return LinePolygonIntersectWithResult(line, polygon, inHitResult);
	}

	inline
		bool BoxBoxIntersect(const Box& box1, const Box& box2) {
		HitResult inHitResult;
		return BoxBoxIntersectWithResult(box1, box2, inHitResult);
	}

	inline
		bool BoxCircleIntersect(const Box& box, const Circle& circle) {
		HitResult inHitResult;
		return BoxCircleIntersectWithResult(box, circle, inHitResult);
	}

	inline
		bool BoxCapsuleIntersect(const Box& box, const Capsule& capsule) {
		HitResult inHitResult;
		return BoxCapsuleIntersectWithResult(box, capsule, inHitResult);
	}

	inline
		bool BoxPolygonIntersect(const Box& box, const TPolygon& polygon) {
		HitResult inHitResult;
		return BoxPolygonIntersectWithResult(box, polygon, inHitResult);
	}

	inline
		bool CircleCircleIntersect(const Circle& circle1, const Circle& circle2) {
		HitResult inHitResult;
		return CircleCircleIntersectWithResult(circle1, circle2, inHitResult);
	}

	inline
		bool CircleCapsuleIntersect(const Circle& circle, const Capsule& capsule) {
		HitResult inHitResult;
		return CircleCapsuleIntersectWithResult(circle, capsule, inHitResult);
	}

	inline
		bool CirclePolygonIntersect(const Circle& circle, const TPolygon& polygon) {
		HitResult inHitResult;
		return CirclePolygonIntersectWithResult(circle, polygon, inHitResult);
	}

	inline
		bool CapsuleCapsuleIntersect(const Capsule& capsule1, const Capsule& capsule2) {
		HitResult inHitResult;
		return CapsuleCapsuleIntersectWithResult(capsule1, capsule2, inHitResult);
	}

	inline
		bool CapsulePolygonIntersect(const Capsule& capsule, const TPolygon& polygon) {
		HitResult inHitResult;
		return CapsulePolygonIntersectWithResult(capsule, polygon, inHitResult);
	}

	inline
		bool PolygonPolygonIntersect(const TPolygon polygon1, const TPolygon polygon2) {
		HitResult inHitResult;
		return PolygonPolygonIntersectWithResult(polygon1, polygon2, inHitResult);
	}


	inline
	bool BoundayCircleBoxIntersect(const Circle& circle, const Box& box) {
		Line line1{ box.ul, {box.lr.x, box.ul.y} };
		Line line2{ {box.lr.x, box.ul.y}, box.lr };
		Line line3{ box.lr, { box.ul.x, box.lr.y} };
		Line line4{ { box.ul.x, box.lr.y}, box.ul };

		std::initializer_list<Line> list{
			line1, line2, line3, line4
		};

		for (const Line& line : list) {
			HitResult hitResult;
			if (CircleLineIntersectWithResult(circle, line, hitResult))
			{
				return true;
			}
		}

		return false;
	}
}






