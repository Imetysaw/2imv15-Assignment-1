//
// Created by Ruud Andriessen on 03/05/2017.
//

#ifndef PARTICLETOY_DIRECTIONALFORCE_H
#define PARTICLETOY_DIRECTIONALFORCE_H

#include "Force.h"
#include "gfx/vec3.h"

using namespace Eigen;

class DirectionalForce : public Force {
public:
    DirectionalForce(std::vector<Particle*> particles, Vec3f direction);

    void setTarget(std::vector<Particle*> particles) override;
    void apply(bool springsCanBreak) override;
    void draw() override;

    map<int, map<int, float>> jx() override;
    MatrixXf jv() override;
    Vec3f direction;
private:
};


#endif //PARTICLETOY_DIRECTIONALFORCE_H
