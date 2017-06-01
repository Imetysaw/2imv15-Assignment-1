//
// Created by Ruud Andriessen on 03/05/2017.
//

#include "DirectionalForce.h"

using namespace Eigen;

DirectionalForce::DirectionalForce(std::vector<Particle*> particles, Vec3f direction) : direction(direction)
{
    this->setTarget(particles);
}

void DirectionalForce::setTarget(std::vector<Particle*> particles)
{
    this->particles = particles;
}

void DirectionalForce::apply(bool springsCanBreak)
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

map<int, map<int, float>> DirectionalForce::jx() {
    map<int, map<int, float>> values = map<int, map<int, float>>();
    return values;
}

MatrixXf DirectionalForce::jv() {
    return MatrixXf();
}
