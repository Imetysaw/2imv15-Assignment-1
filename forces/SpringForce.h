#pragma once

#include "../Particle.h"
#include "Force.h"

class SpringForce : Force {

public:
    SpringForce(Particle *p1, Particle * p2, double dist, double ks, double kd);
    SpringForce(std::vector<Particle*> particles, double dist, double ks, double kd);

    void draw();
    void applyTo(std::vector<Particle*> particles) override;

private:
    double const m_dist;     // rest length
    double const m_ks, m_kd; // spring strength constants
};
