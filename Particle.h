#pragma once

#include <gfx/vec3.h>

class Particle
{
public:

	Particle(const Vec3f & startPosition, float mass);
	virtual ~Particle(void);

	void reset();
	void draw();

	Vec3f startPos;
	Vec3f position;
	Vec3f force;
	Vec3f velocity;
    float mass;
};
