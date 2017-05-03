//
// Created by Ruud Andriessen on 03/05/2017.
//

#ifndef PARTICLETOY_FORCE_H
#define PARTICLETOY_FORCE_H

#include "../Particle.h"
#include <vector>

class Force {
protected:
    std::vector<Particle*> particles;
    bool active = true;

public:
    virtual void setTarget(std::vector<Particle*> particles) = 0;
    virtual void apply() = 0;
    virtual void draw() = 0;

    void setActive(bool state);
    void toggle();

};


#endif //PARTICLETOY_FORCE_H
