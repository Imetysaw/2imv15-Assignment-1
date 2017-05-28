//
// Created by Ruud Andriessen on 03/05/2017.
//

#include "Euler.h"
#include <Eigen/Dense>
#include <Eigen/IterativeLinearSolvers>

using namespace Eigen;

void Euler::simulateStep(System* system, float h) {
    TYPE type = IMPLICIT;


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

    for (int i = 0; i < sys->particles.size() * 3; i+=3) {
        M(i + 0, i + 0) = sys->particles[i/3]->mass;
        M(i + 1, i + 1) = sys->particles[i/3]->mass;
        M(i + 2, i + 2) = sys->particles[i/3]->mass;
    }

    std::map<int, std::map<int, float>> indexMap;

    // Initialize empty map to compute jx
    map<int, map<int, float>> jxm = map<int, map<int, float>>();

    for (Force* f : sys->forces) {
        // Compute map for every force and update jxm appropriately
        map<int, map<int, float>> jx = f->jx();
        for (auto const &i1 : jx) {
            for (auto const &i2 : i1.second) {
                if (jxm.count(i1.first) && jxm[i1.first].count(i2.first)) {
                    // i1 and i2 exist
                    // Hence, we update the already existing value
                    jxm[i1.first][i2.first] += i2.second;
                } else {
                    // No value yet exists, since i1 or i2 does not exist
                    // Hence we set a new value
                    jxm[i1.first][i2.first] = i2.second;
                }
            }
        }
    }

    // Fill jx matrix based on the map jxm
    MatrixXf jx(sys->getDim()/2, sys->getDim()/2);
    for (int i = 0; i < sys->getDim()/2; i++) {
        for (int j = 0; j < sys->getDim()/2; j++) {
            jx(i, j) = jxm[i][j];
        }
    }

    // Get fold and vold
    VectorXf der = sys->derivEval();
    VectorXf fold = VectorXf::Zero(sys->getDim()/2);
    VectorXf vold = VectorXf::Zero(sys->getDim()/2);
    for (int i = 0; i < sys->particles.size(); i++) {
        Particle* p = sys->particles[i];
        vold[i * 3 + 0] = der[i * 6 + 0];//p->velocity[0];
        vold[i * 3 + 1] = der[i * 6 + 1];//p->velocity[1];
        vold[i * 3 + 2] = der[i * 6 + 2];//p->velocity[2];
        fold[i * 3 + 0] = der[i * 6 + 3];
        fold[i * 3 + 1] = der[i * 6 + 4];
        fold[i * 3 + 2] = der[i * 6 + 5];
    }

    // Compute A
    MatrixXf A = M - h * h * jx; //- h * jv;
    VectorXf b = h * ( fold + h * jx * vold );

    // Solve for dy
    ConjugateGradient<MatrixXf, Lower|Upper> cg;
    cg.compute(A);
    VectorXf dy = cg.solve(b);
    std::cout << "#iterations:     " << cg.iterations() << std::endl;
    std::cout << "estimated error: " << cg.error()      << std::endl;

    // Set new state
    VectorXf newState(sys->getDim());

    for (int i = 0; i < dy.size(); i+=3) {
        int si = i * 2; // State index
        newState[si + 0] = oldState[si + 0] + dy[i + 0] * h;    // Update position based on velocity * timestep
        newState[si + 1] = oldState[si + 1] + dy[i + 1] * h;
        newState[si + 2] = oldState[si + 2] + dy[i + 2] * h;
        newState[si + 3] = dy[i + 0];                           // Set new velocity
        newState[si + 4] = dy[i + 1];
        newState[si + 5] = dy[i + 2];
    }

    sys->setState(newState);
}
