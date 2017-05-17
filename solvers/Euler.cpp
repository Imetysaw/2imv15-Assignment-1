//
// Created by Ruud Andriessen on 03/05/2017.
//

#include "Euler.h"
#include <Eigen/Dense>
#include <Eigen/IterativeLinearSolvers>

using namespace Eigen;

void Euler::simulateStep(System* system, float h) {
    bool impl = false;

    // Get the old state
    VectorXf oldState = system->getState();

    // Evaluate derivative
    VectorXf deriv = system->derivEval();

    // Compute the new state
    VectorXf newState = oldState + h * deriv;

    // Set the new state
    system->setState(newState, system->getTime() + h);

    if (impl) {
        // Compute f'
        VectorXf derivp = system->derivEval();

        // Compute I 1/h
        MatrixXf I = MatrixXf::Identity(system->getDim(), system->getDim());
        MatrixXf Ih = (1.0f / h) * I;
        ConjugateGradient<MatrixXf, Lower|Upper> cg;

        // Solve (1/h I - f'(y0)) * dy = f(y0) for dy
        cg.compute(Ih - derivp);
        VectorXf dy = cg.solve(deriv);

        // Compute the new state
        newState = oldState + dy;
        system->setState(newState, system->getTime() + h);
     }
}