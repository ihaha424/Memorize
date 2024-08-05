#pragma once

#include <functional>

// 각각의 효과
// https://easings.net/ko
enum EasingEffect
{
	// Linear
	Linear,

	// Sine
	InSine,
	OutSine,
	InOutSine,

	// Quad
	InQuad,
	OutQuad,
	InOutQuad,

	// Cubic
	InCubic,
	OutCubic,
	InOutCubic,

	// Quart
	InQuart,
	OutQuart,
	InOutQuart,

	// Quint
	InQuint,
	OutQuint,
	InOutQuint,

	// Expo
	InExpo,
	OutExpo,
	InOutExpo,

	// InCirc
	InCirc,
	OutCirc,
	InOutCirc,

	// Back
	InBack,
	OutBack,
	InOutBack,

	// Elastic
	InElastic,
	OutElastic,
	InOutElastic,

	// Bounce
	InBounce,
	OutBounce,
	InOutBounce,

	EasingEffectEnd
};

#pragma region _EasingFunction

// Linear
float EaseLinear(float x);

// Sine
float EaseInSine(float x);
float EaseOutSine(float x);
float EaseInOutSine(float x);

// Quad
float EaseInQuad(float x);
float EaseOutQuad(float x);
float EaseInOutQuad(float x);

// Cubic
float EaseInCubic(float x);
float EaseOutCubic(float x);
float EaseInOutCubic(float x);

// Quart
float EaseInQuart(float x);
float EaseOutQuart(float x);
float EaseInOutQuart(float x);

// Quint
float EaseInQuint(float x);
float EaseOutQuint(float x);
float EaseInOutQuint(float x);

// Expo
float EaseInExpo(float x);
float EaseOutExpo(float x);
float EaseInOutExpo(float x);

// InCirc
float EaseInCirc(float x);
float EaseOutCirc(float x);
float EaseInOutCirc(float x);

// Back
float EaseInBack(float x);
float EaseOutBack(float x);
float EaseInOutBack(float x);

// Elastic
float EaseInElastic(float x);
float EaseOutElastic(float x);
float EaseInOutElastic(float x);

// Bounce
float EaseInBounce(float x);
float EaseOutBounce(float x);
float EaseInOutBounce(float x);

#pragma endregion

static std::function<float(float)> EasingFunction[EasingEffect::EasingEffectEnd] =
{
	// Linear
	EaseLinear,

	// Sine
	EaseInSine,
	EaseOutSine,
	EaseInOutSine,

	// Quad
	EaseInQuad,
	EaseOutQuad,
	EaseInOutQuad,

	// Cubic
	EaseInCubic,
	EaseOutCubic,
	EaseInOutCubic,

	// Quart
	EaseInQuart,
	EaseOutQuart,
	EaseInOutQuart,

	// Quint
	EaseInQuint,
	EaseOutQuint,
	EaseInOutQuint,

	// Expo
	EaseInExpo,
	EaseOutExpo,
	EaseInOutExpo,

	// InCirc
	EaseInCirc,
	EaseOutCirc,
	EaseInOutCirc,

	// Back
	EaseInBack,
	EaseOutBack,
	EaseInOutBack,

	// Elastic
	EaseInElastic,
	EaseOutElastic,
	EaseInOutElastic,

	// Bounce
	EaseInBounce,
	EaseOutBounce,
	EaseInOutBounce,
};