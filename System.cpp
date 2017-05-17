//
// Created by Ruud Andriessen on 03/05/2017.
//

#include "System.h"
#include "solvers/Solver.h"
#include "solvers/ConstraintSolver.h"

System::System(Solver* solver) : solver(solver), time(0.0f) {}

/**
 * Adds a given particle to the system
 * @param p The particle to add
 */
void System::addParticle(Particle* p)
{
    particles.push_back(p);
}

/**
 * Adds a force to use in the system when advancing a time step
 * @param f The new force to use in the system
 */
void System::addForce(Force* f)
{
    forces.push_back(f);
}

/**
 * Adds a constraint to use in the system when advancing a time step
 * @param c The new constraint to use in the system
 */
void System::addConstraint(Constraint* c)
{
    constraints.push_back(c);
}

/**
 * Frees all system data
 */
void System::free()
{
    particles.clear();
    forces.clear();
}

/**
 * Resets all the system to it's initial state
 */
void System::reset()
{
    for (Particle * p : particles) {
        p->reset();
    }
}

/**
 * Draws the forces
 */
void System::draw()
{
    drawParticles();
    drawForces();
    drawConstraints();
}

/**
 * Runs the active solver on the system to progress it's state by dt time
 * @param dt the amount of time to advance the system
 */
void System::step(float dt)
{
    solver->simulateStep(this, dt);
}


unsigned long System::getDim() {
    return particles.size() * 3 * 2; // 3 dimensions, velocity and position
}

/**
 * Constructs a state given the current system
 * @return A copy of the current state of the system
 */
std::vector<float> System::getState()
{
    std::vector<float> r;

    for (Particle* p : this->particles) {
        r.push_back(p->position[0]);
        r.push_back(p->position[1]);
        r.push_back(p->position[2]);
        r.push_back(p->velocity[0]);
        r.push_back(p->velocity[1]);
        r.push_back(p->velocity[2]);
    }

    return r;
}

float System::getTime() {
    return time;
}

/**
 * Evaluates a derivative
 * @param dst The destination vector
 */
void System::derivEval(std::vector<float> &dst) {
    clearForces();
    computeForces();
    ConstraintSolver::solve(this, 60.0f, 50.0f);
    computeDerivative(dst);
}

void System::setState(vector<float> &src, float t)
{
    for(int i=0; i < particles.size(); i++){
        particles[i]->position[0] = src[i * 6 + 0];
        particles[i]->position[1] = src[i * 6 + 1];
        particles[i]->position[2] = src[i * 6 + 2];
        particles[i]->velocity[0] = src[i * 6 + 3];
        particles[i]->velocity[1] = src[i * 6 + 4];
        particles[i]->velocity[2] = src[i * 6 + 5];
    }
    this->time = t;
}

/// Private ///

void System::computeForces()
{
    for (Force* f : forces) {
        f->apply();
    }
}

void System::clearForces()
{
    for (Particle* p : particles) {
        p->force = Vec3f(0.0f, 0.0f, 0.0f);
    }
}

void System::computeDerivative(vector<float> &dst)
{
    for(int i=0; i < particles.size(); i++){
        dst.push_back(particles[i]->velocity[0]);             /* xdot = v */
        dst.push_back(particles[i]->velocity[1]);
        dst.push_back(particles[i]->velocity[2]);
        dst.push_back(particles[i]->force[0] / particles[i]->mass); /* vdot = f/m */
        dst.push_back(particles[i]->force[1] / particles[i]->mass);
        dst.push_back(particles[i]->force[2] / particles[i]->mass);
    }
}

void System::drawParticles()
{
    for(Particle* p : particles)
    {
        p->draw();
    }
}

void System::drawForces()
{
    for(Force* f : forces)
    {
        f->draw();
    }
}

void System::drawConstraints()
{
    for (Constraint* c : constraints) {
        c->draw();
    }
}