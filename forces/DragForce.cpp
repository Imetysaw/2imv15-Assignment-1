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
    map<int, map<int, float>> values = map<int, map<int, float>>();

    for (Particle* p : particles) {
        values[p->index + 0][p->index + 0]  = p->velocity[0] * -.5f;
        values[p->index + 1][p->index + 1] = p->velocity[1] * -.5f;
        values[p->index + 2][p->index + 2] = p->velocity[2] * -.5f;
    }

    return values;
}

MatrixXf DragForce::jv() {
    return MatrixXf();
}
