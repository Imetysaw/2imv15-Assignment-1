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
    VectorXf qd = VectorXf::Zero(pSize);
    VectorXf Q = VectorXf::Zero(pSize);
    MatrixXf M = MatrixXf::Zero(pSize, pSize);
    MatrixXf W = MatrixXf::Zero(pSize, pSize);

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
    VectorXf C = VectorXf::Zero(cSize);
    VectorXf Cd = VectorXf::Zero(cSize);
    MatrixXf J = MatrixXf::Zero(cSize, pSize);
    MatrixXf Jt = MatrixXf::Zero(pSize, cSize);
    MatrixXf Jd = MatrixXf::Zero(cSize, pSize);

    // Compute the values for each constraint
    for (int i = 0; i < cSize; i++) {
        Constraint* c = constraints[i];

        // Compute C, Cd, j, jd for this constraint
        C[i] = c->C();
        Cd[i] = c->Cd();
        vector<Vec3f> j = c->j();
        vector<Vec3f> jd = c->jd();
        vector<Particle*> afParticles = c->affects();

        // Fill the matrices at the correct particle positions
        for (int k = 0; k < afParticles.size(); k++) {
            int pIndex = afParticles[k]->index * dimensions;
            for (int d = 0; d < dimensions; d++) {
                Jd(i, pIndex + d) = jd[k][d];
                J(i, pIndex + d) = j[k][d];
                Jt(pIndex + d, i) = j[k][d];
            }
        }
    }

    MatrixXf JW = J * W;
    MatrixXf JWJt = JW * Jt;
    VectorXf Jdqd = Jd * qd;
    VectorXf JWQ = JW * Q;

    VectorXf KsC = Ks * C;
    VectorXf KdCd = Kd * Cd;

    // Compute rhs
    VectorXf rhs = - Jdqd - JWQ - KsC - KdCd;

    // Initialize CG method
    ConjugateGradient<MatrixXf, Lower|Upper> cg;
//    cg.setTolerance(0.001f);

    // Compute lambda
    cg.compute(JWJt);
    VectorXf lambda = cg.solve(rhs);
//    printf("Iterations: %i\n", cg.iterations());
//    printf("Error: %d\n", cg.error());

    VectorXf Qh = J.transpose() * lambda;
    for (int i = 0; i < particles.size(); i++) {
        Particle *p = particles[i];
        int index = dimensions * i;
        for (int d = 0; d < dimensions; d++) {
            p->force[d] += Qh[index + d];
        }
    }
}
