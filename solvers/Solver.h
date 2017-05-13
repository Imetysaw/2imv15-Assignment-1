//
// Created by Ruud Andriessen on 03/05/2017.
//

#ifndef PARTICLETOY_SOLVER_H
#define PARTICLETOY_SOLVER_H

#include <vector>
#include "../System.h"

using namespace std;

class System;
class Solver {
public:
    virtual void simulateStep(System* sys, float dt) = 0;
};


#endif //PARTICLETOY_SOLVER_H
