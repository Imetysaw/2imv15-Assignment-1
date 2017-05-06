#include "Particle.h"
#if defined(_WIN32) || defined(WIN32)
    #include <GL/glut.h>
#else
    #include <GLUT/glut.h>
#endif

Particle::Particle(const Vec3f & startPosition) :
	startPos(startPosition), position(Vec3f(0.0, 0.0, 0.0)), velocity(Vec3f(0.0, 0.0, 0.0)), force(Vec3f(0.0,0.0,0.0)), mass(1)
{
}

Particle::~Particle(void)
{
}

void Particle::reset()
{
	position = startPos;
    velocity = Vec3f(0.0, 0.0, 0.0);
    force = Vec3f(0.0, 0.0, 0.0);
}
void Particle::draw()
{
	const double h = 0.03;
	glColor3f(1.f, 1.f, 1.f); 
	glBegin(GL_QUADS);
	glVertex2f(position[0]-h/2.0, position[1]-h/2.0);
	glVertex2f(position[0]+h/2.0, position[1]-h/2.0);
	glVertex2f(position[0]+h/2.0, position[1]+h/2.0);
	glVertex2f(position[0]-h/2.0, position[1]+h/2.0);
	glEnd();


    glColor3f(0.0, 0.7, 0.0);
    glBegin(GL_LINES);
    glVertex3f(position[0], position[1], position[2]);
    glVertex3f(position[0] + velocity[0], position[1] + velocity[1], position[2] + velocity[2]);
    glEnd();

    glColor3f(0.0, 0.6, 0.6);
    glBegin(GL_LINES);
    glVertex3f(position[0], position[1], position[2]);
    glVertex3f(position[0] + force[0], position[1] + force[1], position[2] + force[2]);
    glEnd();
}
