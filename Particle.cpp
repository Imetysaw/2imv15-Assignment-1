#include "Particle.h"
#if defined(_WIN32) || defined(WIN32)
    #include <GL/glut.h>
#else
    #include <GLUT/glut.h>
#endif

Particle::Particle(const Vec3f & startPosition, float mass) :
	startPos(startPosition), position(Vec3f(0.0, 0.0, 0.0)), velocity(Vec3f(0.0, 0.0, 0.0)), force(Vec3f(0.0,0.0,0.0)), mass(mass)
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
void Particle::draw(bool drawUtil)
{
//	const float h = 2.f;
//	glColor3f(1.f, 1.f, 1.f);
//    glPointSize(h);
//	glBegin(GL_POINTS);
//    glVertex3f(position[0], position[1], position[2]);
//	glEnd();

    if (drawUtil) {
        glColor3f(0.0, 0.6, 0.0);
        glBegin(GL_LINES);
        glVertex3f(position[0], position[1], position[2]);
        glVertex3f(position[0] + velocity[0] * 0.2f, position[1] + velocity[1] * 0.2f, position[2] + velocity[2] * 0.2f);
        glEnd();

        glColor3f(0.0, 0.6, 0.6);
        glBegin(GL_LINES);
        glVertex3f(position[0], position[1], position[2]);
        glVertex3f(position[0] + force[0] * 0.2f, position[1] + force[1] * 0.2f, position[2] + force[2] * 0.2f);
        glEnd();
    }
}
