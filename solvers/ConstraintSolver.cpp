//
// Created by Ruud Andriessen on 06/05/2017.
//

#include "ConstraintSolver.h"
#include <Eigen/Dense>
#include <Eigen/IterativeLinearSolvers>

using namespace Eigen;
using namespace std;

/**
 * Solves the constraints of the system
 * @param s The system to compute the constraints of
 * @param Ks
 * @param Kd
 */
void ConstraintSolver::solve(System *s, float Ks, float Kd) {
    vector<Particle*> particles = s->particles;
    vector<Constraint*> constraints = s->constraints;

    if (constraints.empty())
        return;

    int dimensions = 3;

    // calculate vector sizes containing particles
    unsigned long pSize = particles.size() * dimensions;

    // Define vectors and matrices with the correct sizes
    VectorXd qd = VectorXd::Zero(pSize);
    VectorXd Q = VectorXd::Zero(pSize);
    MatrixXd M = MatrixXd::Zero(pSize, pSize);
    MatrixXd W = MatrixXd::Zero(pSize, pSize);

    // Compute qd, Q, M and W based on the particles
    for (int i = 0; i < pSize; i += dimensions) {
        Particle *p = particles[i / dimensions];
        for (int d = 0; d < dimensions; d++) {
            M(i + d, i + d) = p->mass;          // M diagonals are the mass value of the particle
            W(i + d, i + d) = 1 / p->mass;      // W is the inverse of M
            Q[i + d] = p->force[d];             // Q contains the forces onto the particles
            qd[i + d] = p->velocity[d];         // q is the position of the particles, hence qd is their velocity
        }
    }

    unsigned long cSize = constraints.size();

    // Instantiate vector and matrices
    VectorXd C = VectorXd::Zero(cSize);
    VectorXd Cd = VectorXd::Zero(cSize);
    MatrixXd J = MatrixXd::Zero(cSize, pSize);
    MatrixXd Jt = MatrixXd::Zero(pSize, cSize);
    MatrixXd Jd = MatrixXd::Zero(cSize, pSize);

    // Compute the values for each constraint
    for (int i = 0; i < cSize; i++) {
        Constraint* c = constraints[i];

        // Compute C, Cd, j, jd for this constraint
        C[i] = c->C();
        Cd[i] = c->Cd();
        vector<Vec3f> j = c->j();
        vector<Vec3f> jd = c->jd();
        vector<int> pIndices = c->indices();

        // Fill the matrices at the correct particle positions
        for (int k = 0; k < pIndices.size(); k++) {
            int pIndex = pIndices[k] * dimensions;
            for (int d = 0; d < dimensions; d++) {
                Jd(i, pIndex + d) = jd[k][d];
                J(i, pIndex + d) = j[k][d];
                Jt(pIndex + d, i) = j[k][d];
            }
        }
    }

    MatrixXd JW = J * W;
    MatrixXd JWJt = JW * Jt;
    VectorXd Jdqd = Jd * qd;
    VectorXd JWQ = JW * Q;

    VectorXd KsC = Ks * C;
    VectorXd KdCd = Kd * Cd;

    // Compute rhs
    VectorXd rhs = - Jdqd - JWQ - KsC - KdCd;

    // Initialize CG method
    ConjugateGradient<MatrixXd, Lower|Upper> cg;

    // Compute lambda
    cg.compute(JWJt);
    VectorXd lambda = cg.solve(rhs);
//    printf("Iterations: %i\n", cg.iterations());
//    printf("Error: %d\n", cg.error());

    VectorXd Qh = J.transpose() * lambda;
    for (int i = 0; i < particles.size(); i++) {
        Particle *p = particles[i];
        int index = dimensions * i;
        for (int d = 0; d < dimensions; d++) {
            p->force[d] += Qh[index + d];
        }
    }
}
