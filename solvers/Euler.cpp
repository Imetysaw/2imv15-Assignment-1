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
    std::vector<float> oldState;
    oldState = system->getState();

    // Evaluate derivative
    std::vector<float> deriv;
    system->derivEval(deriv);

    // Compute the new state
    std::vector<float> newState(system->getDim());
    for (int i = 0; i < system->getDim(); i++) {
        newState[i] = oldState[i] + deriv[i] * h;
    }

    // Set the new state
    system->setState(newState, system->getTime() + h);

    if (impl) {
        // Compute f'
        std::vector<float> derivp;
        system->derivEval(derivp);

        VectorXf fpy = VectorXf::Map(&derivp[0], derivp.size());
        VectorXf fy = VectorXf::Map(&deriv[0], deriv.size());

        // Compute I 1/h
        MatrixXf I = MatrixXf::Identity(system->getDim(), system->getDim());
        MatrixXf Ih = (1.0f / h) * I;
        ConjugateGradient<MatrixXf, Lower|Upper> cg;

        // Solve (1/h I - f'(y0)) * dy = f(y0) for dy
        cg.compute(Ih - fpy);
        VectorXf dy = cg.solve(fy);

        for (int i = 0; i < system->getDim(); i++) {
            newState[i] = oldState[i] + dy[i];
        }
        system->setState(newState, system->getTime() + h);
     }
}