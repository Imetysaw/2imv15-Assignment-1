//
// Created by Ruud Andriessen on 06/05/2017.
//

#include "ConstraintSolver.h"
#include "../linearSolver.h"
#include <assert.h>

using namespace std;

vector<vector<float>> mult(vector<vector<float>> m1,
                           vector<vector<float>> m2) {
    unsigned long r1 = m1.size(), r2 = m2.size(), c1 = m1[0].size(), c2 = m2[0].size();
    assert(c1 == r2);

    vector<vector<float>> r = vector<vector<float>>(r1, vector<float>(c2));

    for (int i = 0; i < r1; ++i) {
        for (int j = 0; j < c2; ++j) {
            for (int k = 0; k < c1; ++k) {
                r[i][j] += m1[i][k] * m2[k][j];
            }
        }
    }

    return r;
}

vector<float> mult(vector<float> v, float multiplier) {
    vector<float> r = vector<float>(v.size());

    for (int i = 0; i < v.size(); i++) {
        r[i] = v[i] * multiplier;
    }

    return r;
}

vector<float> mult(vector<vector<float>> m,
                           vector<float> v) {
    vector<float> r = vector<float>(m[0].size());
    for (int i = 0; i < m[0].size(); i++) {
        float result = 0;

        for (int j = 0; j < m.size(); j++) {
            result += m[j][i] * v[j];
        }

        r[i] = result;
    }

    return r;
}

vector<float> sub(vector< float > v1, vector< float > v2) {
    vector<float> r = vector< float >(v1.size(), 0);
    for (int i = 0; i < v1.size(); i++) {
        r[i] = v1[i] - v2[i];
    }

    return r;
}

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
    vector<float> qd = vector<float>(pSize);
    vector<float> Q = vector<float>(pSize);
    vector<vector<float>> M = vector<vector<float>>(pSize, vector<float>(pSize));
    vector<vector<float>> W = vector<vector<float>>(pSize, vector<float>(pSize));

    // Compute qd, Q, M and W based on the particles
    for (int i = 0; i < pSize; i += dimensions) {
        Particle *p = particles[i / dimensions];
        for (int d = 0; d < dimensions; d++) {
            M[i + d][i + d] = p->mass;          // M diagonals are the mass value of the particle
            W[i + d][i + d] = 1 / p->mass;      // W is the inverse of M
            Q[i + d] = p->force[d];             // Q contains the forces onto the particles
            qd[i + d] = p->velocity[d];         // q is the position of the particles, hence qd is their velocity
        }
    }

    unsigned long cSize = constraints.size();

    // Instantiate vector and matrices
    vector<float> C = vector<float>();
    vector<float> Cd = vector<float>();
    vector<vector<float>> J = vector<vector<float>>(cSize, vector<float>(pSize));
    vector<vector<float>> Jt = vector<vector<float>>(pSize, vector<float>(cSize));
    vector<vector<float>> Jd = vector<vector<float>>(cSize, vector<float>(pSize));

    // Compute the values for each constraint
    for (int i = 0; i < cSize; i++) {
        Constraint* c = constraints[i];

        // Compute C, Cd, j, jd for this constraint
        C.push_back(c->C());
        Cd.push_back(c->Cd());
        vector<Vec3f> j = c->j();
        vector<Vec3f> jd = c->jd();
        vector<int> pIndices = c->indices();

        // Fill the matrices at the correct particle positions
        for (int k = 0; k < pIndices.size(); k++) {
            int pIndex = pIndices[k] * dimensions;
            for (int d = 0; d < dimensions; d++) {
                Jd[i][pIndex + d] = jd[k][d];
                J[i][pIndex + d] = j[k][d];
                Jt[pIndex + d][i] = j[k][d];
            }
        }
    }

    vector<vector<float>> JW = vector<vector<float>>(cSize, vector<float>(pSize));
    vector<vector<float>> JWJt = vector<vector<float>>(cSize, vector<float>(pSize));

    JW = mult(J, W);
    JWJt = mult(JW, Jt);

    vector<float> Jdqd = vector<float>(qd.size());
    vector<float> JWQ = vector<float>(Q.size());
    Jdqd = mult(mult(Jd, qd), -1.0f);
    JWQ = mult(JW, Q);

    vector<float> KsC = mult(C, Ks);
    vector<float> KdCd = mult(Cd, Kd);

    // Compute JWJ^T lambda
    vector<float> JWJtL = sub(sub(sub(Jdqd, JWQ), KsC), KdCd);

    implicitMatrix *mtx = new implicitMatrix(&JWJt);

    float l[constraints.size()];

    int numSteps = 1000;

    ConjGrad(constraints.size(), mtx, l, &JWJtL[0], 0.00001f, &numSteps);
    printf("Used %i steps\n", numSteps);

    std::vector< float > lambda(l, l + sizeof l / sizeof l[0]);

    vector< float > Qh = mult(J, lambda);
    for (int i = 0; i < particles.size(); i++) {
        Particle *p = particles[i];
        int index = dimensions * i;
        for (int d = 0; d < dimensions; d++) {
            p->force[d] += Qh[index + d];
        }
    }
}
