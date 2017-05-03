#include "RodConstraint.h"

#ifdef __CYGWIN__
#include <GL/glut.h>
#else
#include <GLUT/glut.h>
#endif

RodConstraint::RodConstraint(Particle *p1, Particle * p2, double dist) :
  m_p1(p1), m_p2(p2), m_dist(dist) {}

void RodConstraint::draw()
{
  glBegin( GL_LINES );
  glColor3f(0.8, 0.7, 0.6);
  glVertex2f( m_p1->position[0], m_p1->position[1] );
  glColor3f(0.8, 0.7, 0.6);
  glVertex2f( m_p2->position[0], m_p2->position[1] );
  glEnd();

}
