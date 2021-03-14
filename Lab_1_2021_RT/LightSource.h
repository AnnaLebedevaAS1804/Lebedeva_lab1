#pragma once
#include "LiteMath.h"
#include "RayTracer.h"

class LightSource {
public:
	explicit LightSource(const float3 l_position, const float3 l_color, const float l_intensity) : position(l_position), color(l_color), intensity(l_intensity) {};
	virtual ~LightSource() = default;

	float3 position;
	float3 color;
	float intensity;
};

class DirectlightSource : public LightSource 
{
public:
	DirectlightSource(const float3 l_position, const float3 l_color, const float l_intensity) : LightSource(l_position, l_color, l_intensity) {};
	~DirectlightSource() = default;
};