#include "SpringForce.h"

#if defined(__CYGWIN__) || defined(WIN32)
  #include <GL/glut.h>
#else
  #include <GLUT/glut.h>
#endif
#include <assert.h>

SpringForce::SpringForce(Particle *p1, Particle * p2, double dist, double ks, double kd) :
       SpringForce({p1, p2}, dist, ks, kd) {}

SpringForce::SpringForce(std::vector<Particle*> particles, double dist, double ks, double kd) : dist(dist), ks(ks), kd(kd)
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
  glBegin( GL_LINES );
  glColor3f(0.8, 0.8, 0.8);
  glVertex3f( particles[0]->position[0], particles[0]->position[1], particles[0]->position[2] );
  glColor3f(0.6, 0.6, 0.6);
  glVertex3f( particles[1]->position[0], particles[1]->position[1], particles[1]->position[2] );
  glEnd();
}
