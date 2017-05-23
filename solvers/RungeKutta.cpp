#include "RungeKutta.h"
#include <Eigen/IterativeLinearSolvers>

using namespace Eigen;

void RungeKutta::simulateStep(System *system, float h) {

    // Get the old state
    VectorXf oldState = system->getState();
    float oldTime = system->getTime();

    // Evaluate derivative
    VectorXf deriv = system->derivEval();

    //Calculate k1
    VectorXf k1 = h * deriv;
    //Calculate state at midpoint
    VectorXf newState = oldState + k1 / 2;
    system->setState(newState, oldTime + h / 2);

    //Get derivative at midpoint for k2
    deriv = system->computeDerivative();
    VectorXf k2 = h * deriv;
    newState = oldState + k2 / 2;
    system->setState(newState, oldTime + h / 2);

    deriv = system->computeDerivative();
    VectorXf k3 = h * deriv;
    newState = oldState + k3;
    system->setState(newState, oldTime + h);

    deriv = system->computeDerivative();
    VectorXf k4 = h * deriv;

    //Final state
    newState = oldState + 1.0f / 6.0f * k1 + 1.0f / 3.0f * k2 + 1.0f / 3.0f * k3 + 1.0f / 6.0f * k4;

    // Set the final state
    system->setState(newState, oldTime + h);
}