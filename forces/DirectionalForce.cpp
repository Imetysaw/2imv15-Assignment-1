//
// Created by Ruud Andriessen on 03/05/2017.
//

#include "DirectionalForce.h"

#if defined(__CYGWIN__) || defined(WIN32)
    #include <GL/glut.h>
#else
    #include <GLUT/glut.h>
#endif
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
    if (active) {
        glColor3f(.8f, .5f, 0.f);
        glBegin(GL_LINES);
        Vec3f normDirection = direction / norm(direction);
        for (Particle *p : particles) {
            glVertex3f(p->position[0], p->position[1], p->position[2]);
            glVertex3f(p->position[0] + normDirection[0], p->position[1] + normDirection[1], p->position[2] + normDirection[2]);
        }
        glEnd();
    }
}

map<int, map<int, float>> DirectionalForce::jx() {
    map<int, map<int, float>> values = map<int, map<int, float>>();
    return values;
}

MatrixXf DirectionalForce::jv() {
    return MatrixXf();
}
