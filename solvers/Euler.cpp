//
// Created by Ruud Andriessen on 03/05/2017.
//

#include "Euler.h"
#include "../System.h"

void Euler::simulation_step(System* system, float dt)
{
    std::vector<float> delta;

    // Clear the system of accumulated forces
    system->clearForces();
    system->computeForces();

    // Get the current system state
    State s = system->getState();

    // Compute the derivative and scale it based on dt
    computeDerivative(s, delta);
    scale(delta, dt);

    // Update the new state with the computed change
    updateState(s, delta);

    system->setState(s);
}

void Euler::computeDerivative(State s, std::vector<float> &dst)
{
    for(int i=0; i < s.n; i++){
        dst.push_back(s.p[i]->velocity[0]);             /* xdot = v */
        dst.push_back(s.p[i]->velocity[1]);
        dst.push_back(s.p[i]->velocity[2]);
        dst.push_back(s.p[i]->force[0] / s.p[i]->mass); /* vdot = f/m */
        dst.push_back(s.p[i]->force[1] / s.p[i]->mass);
        dst.push_back(s.p[i]->force[2] / s.p[i]->mass);
    }
}

void Euler::scale(std::vector<float> &src, float dt)
{
    for (int i = 0; i < src.size(); i++) {
        src[i] *= dt;
    }
}

void Euler::updateState(State s, std::vector<float> &src)
{
    for(int i=0; i < s.n; i++){
        s.p[i]->position[0] += src[i * 6 + 0];
        s.p[i]->position[1] += src[i * 6 + 1];
        s.p[i]->position[2] += src[i * 6 + 2];
        s.p[i]->velocity[0] += src[i * 6 + 3];
        s.p[i]->velocity[1] += src[i * 6 + 4];
        s.p[i]->velocity[2] += src[i * 6 + 5];
    }
}