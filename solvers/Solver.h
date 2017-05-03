//
// Created by Ruud Andriessen on 03/05/2017.
//

#ifndef PARTICLETOY_SOLVER_H
#define PARTICLETOY_SOLVER_H

#include <vector>

class System;
class Solver {
public:
    virtual void simulation_step(System* sys, float dt) = 0;
};


#endif //PARTICLETOY_SOLVER_H
