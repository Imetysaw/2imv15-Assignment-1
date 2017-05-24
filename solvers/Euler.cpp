//
// Created by Ruud Andriessen on 03/05/2017.
//

#include "Euler.h"
#include <Eigen/Dense>
#include <Eigen/IterativeLinearSolvers>

using namespace Eigen;

void Euler::simulateStep(System* system, float h) {
    TYPE type = SEMI;


    if (type == IMPLICIT) {
        implicit(system, h);
    } else {
        // Get the old state
        VectorXf oldState = system->getState();

        // Evaluate derivative
        VectorXf deriv = system->derivEval();

        // Compute the new state
        VectorXf newState = oldState + h * deriv;

        if (type == SEMI) {
            // If we are running semi implicit euler, use the new velocity instead
            VectorXf semiImpl(system->getDim());
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
    }
}

void Euler::implicit(System *sys, float h) {
    MatrixXf A;
    VectorXf b;
    MatrixXf M = MatrixXf::Zero(sys->getDim()/2, sys->getDim()/2);

    for (int i = 0; i < sys->particles.size(); i++) {
        M(i,i) = sys->particles[i]->mass;
    }

    MatrixXf jx = MatrixXf::Zero(sys->getDim()/2, sys->getDim()/2);
    for (Force* f : sys->forces) {
        jx += f->jx();
//        jv += f->jv();
    }

    A = M - h*h*jx; //- h * jv;


    ConjugateGradient cg;

}
