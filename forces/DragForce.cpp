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

void DragForce::apply(bool springsCanBreak)
{
    if (!active)
        return;
    for (Particle* p : particles) {
        p->force -= p->velocity * amount;
    }
}

void DragForce::draw()
{

}

map<int, map<int, float>>  DragForce::jx() {
    return map<int, map<int, float>>();
}

MatrixXf DragForce::jv() {
    return MatrixXf();
}
