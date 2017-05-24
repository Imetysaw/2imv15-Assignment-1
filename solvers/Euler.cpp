//
// Created by Ruud Andriessen on 03/05/2017.
//

#include "Euler.h"
#include <Eigen/Dense>
#include <Eigen/IterativeLinearSolvers>

using namespace Eigen;

void Euler::simulateStep(System* system, float h) {
    TYPE type = SEMI;

    // Get the old state
    VectorXf oldState = system->getState();

    // Evaluate derivative
    VectorXf deriv = system->derivEval();

    // Compute the new state
    VectorXf newState = oldState + h * deriv;

    if (type == SEMI) {
        // If we are running semi implicit euler, use the new velocity instead
        VectorXf semiImpl;
        for (int i = 0; i < newState.size(); i += 6) {
            semiImpl[i + 0] = oldState[i + 0] + h * newState[i + 3];  // Xnew implicit, using Vnew
            semiImpl[i + 1] = oldState[i + 1] + h * newState[i + 4];  // Xold + h * Vnew
            semiImpl[i + 2] = oldState[i + 2] + h * newState[i + 5];
            semiImpl[i + 3] = newState[i + 3];  // Keep Vnew
            semiImpl[i + 4] = newState[i + 4];
            semiImpl[i + 5] = newState[i + 5];
        }

        // Set the new state, using semi implicit computation
        system->setState(semiImpl, system->getTime() + h);
    } else {
        system->setState(newState, system->getTime() + h);
    }

    if (type == IMPLICIT) {
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