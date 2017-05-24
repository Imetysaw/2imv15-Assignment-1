//
// Created by Ruud Andriessen on 03/05/2017.
//

#ifndef PARTICLETOY_DIRECTIONALFORCE_H
#define PARTICLETOY_DIRECTIONALFORCE_H

#include "Force.h"
#include "gfx/vec3.h"

class DirectionalForce : public Force {
public:
    DirectionalForce(std::vector<Particle*> particles, Vec3f direction);

    void setTarget(std::vector<Particle*> particles) override;
    void apply() override;
    void draw() override;

    MatrixXf jx() override;
    Vec3f direction;
private:
};


#endif //PARTICLETOY_DIRECTIONALFORCE_H
