#include "SpringForce.h"

#if defined(__CYGWIN__) || defined(WIN32)
  #include <GL/glut.h>
#else
  #include <GLUT/glut.h>
#endif
#include <assert.h>

SpringForce::SpringForce(Particle *p1, Particle * p2, float dist, float ks, float kd) :
       SpringForce({p1, p2}, dist, ks, kd) {}

SpringForce::SpringForce(std::vector<Particle*> particles, float dist, float ks, float kd) : dist(dist), ks(ks), kd(kd)
{
    setTarget(particles);
}


void SpringForce::setTarget(std::vector<Particle*> particles)
{
    assert(particles.size() == 2);
    this->particles = particles;
}

void SpringForce::apply()
{
    Vec3f l = particles[0]->position - particles[1]->position;
    Vec3f ld = particles[0]->velocity - particles[1]->velocity;

    // Compute spring force
    // f_a = -[ ks * ( |l| - r ) + kd * ld * l /|l| ] * l / |l|
    Vec3f result = -(ks * ( norm(l) - dist ) + kd * ((l * ld) / norm(l)) ) * (l / norm(l));

    particles[0]->force += result;
    particles[1]->force -= result;
}

void SpringForce::draw()
{
    float deltaDist = norm(particles[0]->position - particles[1]->position) - dist;
    glBegin( GL_LINES );
    glColor3f(0.8f, 0.8f - deltaDist, 0.8f - deltaDist);
    glVertex3f( particles[0]->position[0], particles[0]->position[1], particles[0]->position[2] );
    glVertex3f( particles[1]->position[0], particles[1]->position[1], particles[1]->position[2] );
    glEnd();
}

MatrixXf SpringForce::jx() {
    MatrixXf jx = MatrixXf::Zero(particles.size(), particles.size());
    return jx;
}
