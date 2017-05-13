//
// Created by Ruud Andriessen on 13/05/2017.
//

#include "Midpoint.h"

void Midpoint::simulateStep(System *system, float dt) {

    // Get the initial state
    std::vector<float> oldState;
    oldState = system->getState();

    // Evaluate a deriv step
    std::vector<float> deriv;
    system->derivEval(deriv);

    // Compute the halfway point
    std::vector<float> midPointState(system->getDim());
    for (int i = 0; i < system->getDim(); i++) {
        midPointState[i] = oldState[i] + deriv[i] * dt * 0.5f;
    }
    // Set the state to this midpoint
    system->setState(midPointState);

    // Evaluate derivative at the midpoint
    system->derivEval(deriv);

    // Update the state based on the computation from this midpoint
    std::vector<float> newState(system->getDim());
    for (int i = 0; i < system->getDim(); i++) {
        newState[i] = oldState[i] + deriv[i] * dt;
    }

    system->setState(newState);
}
