#include "SpringForce.h"
#include "AngularSpringForce.h"

#if defined(__CYGWIN__) || defined(WIN32)
#include <GL/glut.h>
#else
#include <GLUT/glut.h>
#endif
#include <assert.h>

AngularSpringForce::AngularSpringForce(Particle *p1, Particle * p2, float dist, float ks, float kd) :
        AngularSpringForce({p1, p2}, dist, ks, kd) {}

AngularSpringForce::AngularSpringForce(std::vector<Particle*> particles, float dist, float ks, float kd) : dist(dist), ks(ks), kd(kd)
{
    setTarget(particles);
}


void AngularSpringForce::setTarget(std::vector<Particle*> particles)
{
    assert(particles.size() == 3);
    this->particles = particles;
}

void AngularSpringForce::apply()
{
    Vec3f l1 = particles[0]->position - particles[1]->position;
    Vec3f l2 = particles[1]->position - particles[2]->position;
    double angle = acos((l1 * l2) / (norm(l1) * norm(l2)));
    Vec3f ld = particles[0]->velocity - particles[1]->velocity;

    //Vector perpendicular to plane of the two vectors
    Vec3f crossVec = cross(l1, l2);

    //Vectors perpendicular to the two vectors in the plane
    Vec3f l1perp = cross(crossVec, l1);
    Vec3f l2perp = cross(crossVec, l2);

    // Compute spring force
    double result = ks * ( angle - dist );

    particles[0]->force += result * l1perp;
    particles[2]->force += result * l2perp;
}

void AngularSpringForce::draw()
{
    float deltaDist = norm(particles[0]->position - particles[1]->position) - dist;
    glBegin( GL_LINES );
    glColor3f(0.8f, 0.8f - deltaDist, 0.8f - deltaDist);
    glVertex3f( particles[0]->position[0], particles[0]->position[1], particles[0]->position[2] );
    glVertex3f( particles[1]->position[0], particles[1]->position[1], particles[1]->position[2] );
    glEnd();
}

MatrixXf AngularSpringForce::jx() {
    MatrixXf jx = MatrixXf::Zero(particles.size(), particles.size());
    return Eigen::MatrixXf();
}
