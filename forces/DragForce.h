//
// Created by Ruud Andriessen on 06/05/2017.
//

#ifndef PARTICLETOY_DRAGFORCE_H
#define PARTICLETOY_DRAGFORCE_H


#include "Force.h"

class DragForce : public Force {
public:
    DragForce(std::vector<Particle*> particles, float amount);

    void setTarget(std::vector<Particle*> particles) override;
    void apply() override;
    void draw() override;

    MatrixXf jx() override;
private:
    float amount;
};


#endif //PARTICLETOY_DRAGFORCE_H
