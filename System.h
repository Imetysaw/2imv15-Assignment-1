//
// Created by Ruud Andriessen on 03/05/2017.
//

#ifndef PARTICLETOY_SYSTEM_H
#define PARTICLETOY_SYSTEM_H

#include "Eigen/Dense"

#include "Particle.h"
#include "constraints/Constraint.h"
#include "forces/Force.h"

using namespace Eigen;

class Solver;
class System {
private:
    void drawParticles();
    void drawForces();
    void drawConstraints();
    VectorXf computeDerivative();

    void computeForces();
    void clearForces();

    Solver* solver;
    float time;
public:
    System(Solver* solver);

    std::vector<Particle*> particles;
    std::vector<Force*> forces;
    std::vector<Constraint*> constraints;

    void addParticle(Particle* p);
    void addForce(Force* f);
    void addConstraint(Constraint* c);

    // ODE interface
    VectorXf derivEval();
    VectorXf getState();
    float getTime();
    void setState(VectorXf newState, float time);
    unsigned long getDim();

    void step(float dt);
    void free();
    void reset();
    void draw();
};


#endif //PARTICLETOY_SYSTEM_H
