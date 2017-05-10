//
// Created by Ruud Andriessen on 07/05/2017.
//

#include "Solver.h"

void Solver::computeDerivative(State s, vector<float> &dst)
{
    for(int i=0; i < s.n; i++){
        dst.push_back(s.p[i]->velocity[0]);             /* xdot = v */
        dst.push_back(s.p[i]->velocity[1]);
        dst.push_back(s.p[i]->velocity[2]);
        dst.push_back(s.p[i]->force[0] / s.p[i]->mass); /* vdot = f/m */
        dst.push_back(s.p[i]->force[1] / s.p[i]->mass);
        dst.push_back(s.p[i]->force[2] / s.p[i]->mass);
    }
}

void Solver::scale(vector<float> &src, float dt)
{
    for (int i = 0; i < src.size(); i++) {
        src[i] *= dt;
    }
}

void Solver::updateState(State s, vector<float> &src)
{
    for(int i=0; i < s.n; i++){
        s.p[i]->position[0] += src[i * 6 + 0];
        s.p[i]->position[1] += src[i * 6 + 1];
        s.p[i]->position[2] += src[i * 6 + 2];
        s.p[i]->velocity[0] += src[i * 6 + 3];
        s.p[i]->velocity[1] += src[i * 6 + 4];
        s.p[i]->velocity[2] += src[i * 6 + 5];
    }
}