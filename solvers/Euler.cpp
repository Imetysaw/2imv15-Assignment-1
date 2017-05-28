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

            //check collisions
            if(system->wallExists) {
                semiImpl = system->checkWallCollision(oldState, semiImpl);
            }
            system->setState(semiImpl, system->getTime() + h);
        } else {
            if(system->wallExists) {
                newState = system->checkWallCollision(oldState, newState);
            }
            system->setState(newState, system->getTime() + h);
        }
    }
}

void Euler::implicit(System *sys, float h) {
    // Get old state
    VectorXf oldState = sys->getState();

    // Fill mass matrix
    MatrixXf M = MatrixXf::Zero(sys->getDim()/2, sys->getDim()/2);

    for (int i = 0; i < sys->particles.size(); i++) {
        M(i,i) = sys->particles[i]->mass;
    }

    // Compute Jx
    MatrixXf jx = MatrixXf::Zero(sys->getDim()/2, sys->getDim()/2);
    for (Force* f : sys->forces) {
        jx += f->jx();
    }

    // Get fold and vold
    VectorXf fold = VectorXf::Zero(sys->getDim()/2);
    VectorXf vold = VectorXf::Zero(sys->getDim()/2);
    for (int i = 0; i < sys->particles.size() * 3; i+= 3) {
        Particle* p = sys->particles[i];
        fold[i + 0] = p->force[i + 0];
        fold[i + 1] = p->force[i + 1];
        fold[i + 2] = p->force[i + 2];
        vold[i + 0] = p->velocity[i + 0];
        vold[i + 1] = p->velocity[i + 1];
        vold[i + 2] = p->velocity[i + 2];
    }


    // Compute A
    MatrixXf A = M - h * h * jx; //- h * jv;
    VectorXf b = h * ( fold + h * jx * vold );

    // Solve for dy
    ConjugateGradient<MatrixXf, Lower|Upper> cg;
    cg.compute(A);
    VectorXf dy = cg.solve(b);

    // Set new state
    VectorXf newState = VectorXf(sys->getDim());
    for (int i = 0; i < dy.size(); i+=3) {
        newState[i + 0] = oldState[i + 0] + dy[3] * h;
        newState[i + 1] = oldState[i + 1] + dy[4] * h;
        newState[i + 2] = oldState[i + 2] + dy[5] * h;
        newState[i + 3] = dy[3];
        newState[i + 4] = dy[4];
        newState[i + 5] = dy[5];
    }
    if(sys->wallExists) {
        newState = sys->checkWallCollision(oldState, newState);
    }
    sys->setState(newState);
}
