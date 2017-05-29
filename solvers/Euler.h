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
    enum TYPE {
        EXPLICIT,
        IMPLICIT,
        SEMI
    };
    TYPE type;

    Euler(TYPE type);

private:
    void implicit(System* sys, float h);

};


#endif //PARTICLETOY_EULER_H
