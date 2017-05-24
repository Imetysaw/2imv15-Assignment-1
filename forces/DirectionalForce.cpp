//
// Created by Ruud Andriessen on 03/05/2017.
//

#include "DirectionalForce.h"

DirectionalForce::DirectionalForce(std::vector<Particle*> particles, Vec3f direction) : direction(direction)
{
    this->setTarget(particles);
}

void DirectionalForce::setTarget(std::vector<Particle*> particles)
{
    this->particles = particles;
}

void DirectionalForce::apply()
{
    if (!active)
        return;
    for (Particle* p : particles) {
        p->force += p->mass * direction;
    }
}

void DirectionalForce::draw()
{

}

MatrixXf DirectionalForce::jx() {
    return MatrixXf::Zero(particles.size(), particles.size());
}
