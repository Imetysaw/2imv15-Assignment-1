//
// Created by Ruud Andriessen on 03/05/2017.
//

#ifndef PARTICLETOY_SYSTEM_H
#define PARTICLETOY_SYSTEM_H

#include "Particle.h"
#include "constraints/Constraint.h"
#include "forces/Force.h"

typedef struct {
    std::vector<Particle*> p;
    int n;
} State;

class Solver;
class System {
private:
    void drawParticles();
    void drawForces();
    void drawConstraints();
    void computeDerivative(vector<float> &dst);

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
    void derivEval(std::vector<float> &dst);
    std::vector<float> getState();
    float getTime();
    void setState(std::vector<float> &src, float time);
    unsigned long getDim();

    void step(float dt);
    void free();
    void reset();
    void draw();
};


#endif //PARTICLETOY_SYSTEM_H
