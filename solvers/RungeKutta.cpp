#include "RungeKutta.h"
#include <Eigen/IterativeLinearSolvers>

using namespace Eigen;

void RungeKutta::simulateStep(System *system, float h) {

    // Get the old state
    VectorXf oldState = system->getState();

    // Evaluate derivative
    VectorXf deriv = system->derivEval();

    //Calculate k1
    VectorXf k1 = h * deriv;
    //Calculate state at midpoint
    VectorXf newState = oldState + k1 / 2;
    system->setState(newState, system->getTime() + h / 2);

    //Get derivative at midpoint for k2
    deriv = system->derivEval();
    VectorXf k2 = h * deriv;
    newState = oldState + k2 / 2;
    system->setState(newState, system->getTime() + h / 2);

    deriv = system->derivEval();
    VectorXf k3 = h * deriv;
    newState = oldState + k3;
    system->setState(newState, system->getTime() + h);

    deriv = system->derivEval();
    VectorXf k4 = h * deriv;

    //Final state
    newState = oldState + 1 / 6 * k1 + 1 / 3 * k2 + 1 / 3 * k3 + 1 / 6 * k4;

    // Set the final state
    system->setState(newState, system->getTime() + h);
}