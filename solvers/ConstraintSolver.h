//
// Created by Ruud Andriessen on 06/05/2017.
//

#ifndef PARTICLETOY_CONSTRAINTSOLVER_H
#define PARTICLETOY_CONSTRAINTSOLVER_H


#include "../System.h"

class ConstraintSolver {
public:
    static void solve(System* s, float Ks, float Kd);
};


#endif //PARTICLETOY_CONSTRAINTSOLVER_H
