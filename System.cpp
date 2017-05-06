//
// Created by Ruud Andriessen on 03/05/2017.
//

#include "System.h"
#include "solvers/Solver.h"

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
    time += dt;
}

/**
 * Clears all forces from the particles in the system
 */
void System::clearForces()
{
    for (Particle* p : particles) {
        p->force = Vec3f(0.0, 0.0, 0.0);
    }
}

/**
 * Constructs a state given the current system
 * @return A copy of the current state of the system
 */
State System::getState()
{
    State s;
    std::vector<Particle*> particles;

    for (Particle* p : this->particles) {
        particles.push_back(p);
    }

    s.p = particles;
    s.n = particles.size();
    return s;
}

void System::setState(State s)
{
    particles = s.p;
}

void System::computeForces()
{
    for (Force* f : forces) {
        f->apply();
    }
}

/// Private ///

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