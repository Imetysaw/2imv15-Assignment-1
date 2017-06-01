#pragma once

#include <gfx/vec3.h>

class Particle
{
public:

	Particle(const Vec3f & startPosition, float mass, int index);
	virtual ~Particle(void);

	void reset();
	void draw(bool drawVelocity, bool drawForce);

	Vec3f startPos;
	Vec3f position;
	Vec3f force;
	Vec3f velocity;
    int index;
    float mass;
};
