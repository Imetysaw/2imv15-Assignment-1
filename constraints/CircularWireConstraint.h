#pragma once

#include "../Particle.h"
#include "Constraint.h"
#include <gfx/vec3.h>
#include <vector>

using namespace std;

class CircularWireConstraint : public Constraint {
public:
    CircularWireConstraint(Particle *p, const Vec3f & center, const float radius, const vector<int> indices);

    void draw() override;

    float C() override;
    float Cd() override;
    vector<Vec3f> j() override;
    vector<Vec3f> jd() override;

private:
    Particle * const particle;
    Vec3f const center;
    float const radius;
};
