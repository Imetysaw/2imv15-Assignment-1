//
// Created by Ruud Andriessen on 03/05/2017.
//

#ifndef PARTICLETOY_SYSTEM_H
#define PARTICLETOY_SYSTEM_H

#include "Particle.h"
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

    Solver* solver;
    float time;
public:
    System(Solver* solver);

    std::vector<Particle*> particles;
    std::vector<Force*> forces;

    void addParticle(Particle* p);
    void addForce(Force* f);
    State getState();
    void setState(State s);

    void computeForces();
    void clearForces();

    void step(float dt);
    void free();
    void reset();
    void draw();
};


#endif //PARTICLETOY_SYSTEM_H
