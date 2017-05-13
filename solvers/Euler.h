//
// Created by Ruud Andriessen on 03/05/2017.
//

#ifndef PARTICLETOY_EULER_H
#define PARTICLETOY_EULER_H

#include "Solver.h"
#include "../System.h"

class Euler : public Solver {
public:
    void simulateStep(System* system, float h) override;
};


#endif //PARTICLETOY_EULER_H
