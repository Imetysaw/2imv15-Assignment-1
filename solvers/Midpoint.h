//
// Created by Ruud Andriessen on 13/05/2017.
//

#ifndef PARTICLETOY_MIDPOINT_H
#define PARTICLETOY_MIDPOINT_H


#include "Solver.h"

class Midpoint : public Solver {
public:
    void simulateStep(System* system, float dt) override;
};




#endif //PARTICLETOY_MIDPOINT_H
