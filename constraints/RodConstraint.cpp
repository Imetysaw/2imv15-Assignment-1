#include "RodConstraint.h"

#ifdef __CYGWIN__
#include <GL/glut.h>
#else
#include <GLUT/glut.h>
#endif

RodConstraint::RodConstraint(Particle *p1, Particle * p2, float dist, vector<int> indices) :
  Constraint(indices), p1(p1), p2(p2), dist(dist) {}


float RodConstraint::C() {
  Vec3f delta = p1->position - p2->position;

  return delta[0] * delta[0] + delta[1] * delta[1] + delta[2] * delta[2] - dist * dist;
}

float RodConstraint::Cd() {
    Vec3f pDiff = (p1->position - p2->position) * 2;
    Vec3f vDiff = (p1->velocity - p2->velocity) * 2;

    return pDiff[0] * vDiff[0] + pDiff[1] * vDiff[1] +  pDiff[2] * vDiff[2];
}

std::vector<Vec3f> RodConstraint::j() {
    std::vector <Vec3f> j;

    j.push_back((p1->position - p2->position) * 2);
    j.push_back((p2->position - p1->position) * 2);

    return j;
}

vector<Vec3f> RodConstraint::jd() {
    vector <Vec3f> jd;

    jd.push_back((p1->velocity - p2->velocity) * 2);
    jd.push_back((p2->velocity - p1->velocity) * 2);

    return jd;
}


void RodConstraint::draw()
{
  glBegin( GL_LINES );
  glColor3f(0.8, 0.7, 0.6);
  glVertex3f( p1->position[0], p1->position[1], p1->position[2] );
  glColor3f(0.8, 0.7, 0.6);
  glVertex3f( p2->position[0], p2->position[1], p2->position[2] );
  glEnd();
}