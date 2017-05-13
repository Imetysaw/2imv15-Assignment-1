//
// Created by Ruud Andriessen on 03/05/2017.
//

#include "Euler.h"

void Euler::simulateStep(System* system, float dt)
{
    std::vector<float> oldState;
    oldState = system->getState();

    std::vector<float> deriv;
    system->derivEval(deriv);

    for (int i = 0; i < system->getDim(); i++) {
        oldState[i] += deriv[i] * dt;
    }

    system->setState(oldState);
}