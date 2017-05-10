//
// Created by Ruud Andriessen on 03/05/2017.
//

#include "Euler.h"
#include "../System.h"
#include "ConstraintSolver.h"

void Euler::simulateStep(System* system, float dt)
{
    std::vector<float> delta;

    // 1. Clear the system of accumulated forces
    system->clearForces();

    // 2. Compute the forces
    system->computeForces();

    // 3. Compute constraint forces
    ConstraintSolver::solve(system, 60.0f, 50.0f);

    // 4. Compute the derivative and scale it based on dt based on the current state
    State s = system->getState();
    computeDerivative(s, delta);
    scale(delta, dt);

    // Update the new state with the computed change
    updateState(s, delta);

    system->setState(s);
}