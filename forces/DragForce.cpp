//
// Created by Ruud Andriessen on 06/05/2017.
//

#include "DragForce.h"

DragForce::DragForce(std::vector<Particle*> particles, float amount) : amount(amount)
{
    this->setTarget(particles);
}

void DragForce::setTarget(std::vector<Particle*> particles)
{
    this->particles = particles;
}

void DragForce::apply()
{
    for (Particle* p : particles) {
        p->force -= p->velocity * amount;
    }
}

void DragForce::draw()
{

}