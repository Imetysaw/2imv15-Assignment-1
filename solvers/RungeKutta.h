//
// Created by s130604 on 21-5-2017.
//

#include "../System.h"

#ifndef PARTICLETOY_RUNGEKUTTA_H
#define PARTICLETOY_RUNGEKUTTA_H

#endif //PARTICLETOY_RUNGEKUTTA_H

#include "Solver.h"
#include "../System.h"

class RungeKutta : public Solver {
public:
    void simulateStep(System* system, float h) override;
};
