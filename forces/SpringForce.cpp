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

map<int, map<int, float>>  SpringForce::jx() {
    map<int, map<int, float>> values = map<int, map<int, float>>();
    MatrixXf I = MatrixXf::Identity(3, 3);

    Vec3f dxij = particles[0]->position - particles[1]->position;

    Vector3f xij = Vector3f(dxij[0], dxij[1], dxij[2]);
    float xijn = xij.norm();

    MatrixXf force = ks * I - ks * dist / xijn * I - ks * dist / (xijn * xijn * xijn) * xij * xij.transpose();

//    printf("Forces:\n");
    for (int i = 0; i < force.rows(); i++) {
        for (int j = 0; j < force.cols(); j++) {
//            printf("%.2f ", force(i, j));
            values[particles[0]->index * 3 + i][particles[1]->index * 3 + j] = force(i,j);
            values[particles[1]->index * 3 + i][particles[0]->index * 3 + j] = -force(i,j);
        }
//        printf("\n");
    }
//    printf("\n");
    return values;
}


MatrixXf SpringForce::jv() {
    return ks * MatrixXf::Identity(3, 3);
}