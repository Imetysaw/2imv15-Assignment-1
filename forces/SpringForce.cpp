#include "SpringForce.h"

#ifdef __CYGWIN__
  #include <GL/glut.h>
#else
  #include <GLUT/glut.h>
#endif
#include <assert.h>

SpringForce::SpringForce(Particle *p1, Particle * p2, double dist, double ks, double kd) :
       SpringForce({p1, p2}, dist, ks, kd) {}

SpringForce::SpringForce(std::vector<Particle*> particles, double dist, double ks, double kd) : m_dist(dist), m_ks(ks), m_kd(kd)
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
}

void SpringForce::draw()
{
  glBegin( GL_LINES );
  glColor3f(0.6, 0.7, 0.8);
  glVertex2f( particles[0]->position[0], particles[0]->position[1] );
  glColor3f(0.6, 0.7, 0.8);
  glVertex2f( particles[1]->position[0], particles[1]->position[1] );
  glEnd();
}
