#include "CircularWireConstraint.h"

#if defined(__CYGWIN__) || defined(WIN32)
#include <GL/glut.h>
#else
#include <GLUT/glut.h>
#endif


CircularWireConstraint::CircularWireConstraint(Particle *p, const Vec3f & center,
                                               const float radius, const vector<int> indices) :
        Constraint(indices), particle(p), center(center), radius(radius) {}


void CircularWireConstraint::draw()
{
    glBegin(GL_LINE_LOOP);
    glColor3f(0.0,1.0,0.0);
    for (int i=0; i < 360; i=i+18)
    {
        float degInRad = i * M_PI / 180.0f;
        glVertex3f(center[0]+cos(degInRad)*radius,center[1]+sin(degInRad)*radius, 0);
    }
    glEnd();

    glBegin(GL_LINES);
    glColor3f(1.0,1.0,0.0);
        glVertex3f(particle->position[0], particle->position[1], particle->position[2]);
        glVertex3f(center[0], center[1], center[2]);
    glEnd();
}

/**
 * Compute C of this constraint
 * @return x * x - r * r
 */
float CircularWireConstraint::C() {
    Vec3f delta = particle->position - center;
    return delta * delta - radius * radius;
}

/**
 * Computes Cd of this constraint
 * @return x * xd
 */
float CircularWireConstraint::Cd() {
    Vec3f pVector = (particle->position - center) * 2;
    Vec3f vVector = particle->velocity;

    return pVector * vVector;
}

/**
 * Computes j of this constraint
 * @return
 */
vector<Vec3f> CircularWireConstraint::j() {
    vector<Vec3f> j;
    j.push_back((particle->position - center) * 2);
    return j;
}

/**
 * Computes jd of this constraint
 * @return
 */
vector<Vec3f> CircularWireConstraint::jd() {
    vector<Vec3f> jd;
    jd.push_back(particle->velocity * 2);
    return jd;
}
