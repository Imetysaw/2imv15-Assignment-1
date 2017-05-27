#include "SpringForce.h"
#include "AngularSpringForce.h"

#if defined(__CYGWIN__) || defined(WIN32)

#include <GL/glut.h>

#else
#include <GLUT/glut.h>
#endif

#include <assert.h>

AngularSpringForce::AngularSpringForce(Particle *p1, Particle *p2, Particle *p3, float dist, float ks, float kd) :
        AngularSpringForce({p1, p2, p3}, dist, ks, kd) {}

AngularSpringForce::AngularSpringForce(std::vector<Particle *> particles, float dist, float ks, float kd) : dist(dist),
                                                                                                            ks(ks),
                                                                                                            kd(kd) {
    setTarget(particles);
}


void AngularSpringForce::setTarget(std::vector<Particle *> particles) {
    assert(particles.size() == 3);
    this->particles = particles;
}

void AngularSpringForce::apply() {
    Vec3f l1 = particles[0]->position - particles[1]->position;
    Vec3f l2 = particles[1]->position - particles[2]->position;
    double cosAngle = (l1 * l2) / (norm(l1) * norm(l2));
    //deal with rounding errors
    if (cosAngle > 1.0) cosAngle = 1.0;
    if (cosAngle < -1.0) cosAngle = -1.0;
    double angle = acos(cosAngle);
    Vec3f ld = particles[0]->velocity - particles[1]->velocity;

    //Vector perpendicular to plane of the two vectors
    Vec3f crossVec = cross(l1, l2);
    crossVec /= norm(crossVec);
    //Vectors perpendicular to the two vectors in the plane
    Vec3f l1perp = cross(crossVec, l1);
    l1perp /= norm(l1perp);
    Vec3f l2perp = cross(crossVec, l2);
    l2perp /= norm(l2perp);

    // Compute spring force
    double result = ks * (angle - dist);
    particles[1]->force += result * (l1perp+l2perp);
}

void AngularSpringForce::draw() {
}

MatrixXf AngularSpringForce::jx() {
    MatrixXf jx = MatrixXf::Zero(particles.size(), particles.size());
    return Eigen::MatrixXf();
}
