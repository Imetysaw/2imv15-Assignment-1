//
// Created by Ruud Andriessen on 03/05/2017.
//

#ifndef PARTICLETOY_SYSTEM_H
#define PARTICLETOY_SYSTEM_H

#include "Eigen/Dense"

#include "Particle.h"
#include "constraints/Constraint.h"
#include "forces/Force.h"
#include "SystemBuilder.h"

using namespace Eigen;

class Solver;
class System {
private:
    void drawParticles(bool drawUtil);
    void drawForces();
    void drawConstraints();

    void computeForces();
    void clearForces();

    float time;
public:
    System(Solver* solver);

    std::vector<Particle*> particles;
    std::vector<Force*> forces;
    std::vector<Constraint*> constraints;

    bool wallExists;
    float dt;
    SystemBuilder::AvailableSystems type;
    Solver* solver;

    void addParticle(Particle* p);
    void addForce(Force* f);
    void addConstraint(Constraint* c);
    VectorXf checkWallCollision(VectorXf oldState, VectorXf newState);

    // ODE interface
    VectorXf derivEval();
    VectorXf computeDerivative();
    VectorXf getState();
    float getTime();
    void setState(VectorXf src);
    void setState(VectorXf newState, float time);
    unsigned long getDim();

    void step(bool adaptive);
    void free();
    void reset();
    void draw(bool drawUtil, bool drawOthers);
};


#endif //PARTICLETOY_SYSTEM_H
