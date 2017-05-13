//
// Created by Ruud Andriessen on 10/05/2017.
//

#include "SystemBuilder.h"
#include "solvers/Euler.h"
#include "solvers/Midpoint.h"
#include "forces/SpringForce.h"
#include "forces/DirectionalForce.h"
#include "forces/DragForce.h"
#include "constraints/CircularWireConstraint.h"
#include "constraints/RodConstraint.h"

System* SystemBuilder::get(AvailableSystems s) {
    switch (s) {
        case BASIC:
            return initBasic();
        case CLOTH:
            return initCloth();
    }
    return NULL;
}

System* SystemBuilder::initBasic()
{
    System* sys = new System(new Midpoint());

    const float dist = 0.2f;
    const Vec3f center(0.0, 0.0, 0.0);
    const Vec3f offset(dist, 0.0, 0.0);

    // Create three particles, attach them to each other, then add a
    // circular wire constraint to the first.

    sys->addParticle(new Particle(center + offset, 1.0f));
    sys->addParticle(new Particle(center + offset + offset, 1.0f));
    sys->addParticle(new Particle(center + offset + offset + offset, 1.0f));

    sys->addForce(new DragForce(sys->particles, 0.1f));
    sys->addForce(new SpringForce(sys->particles[0], sys->particles[1], dist, 1.0, 1.0));
    sys->addForce(new DirectionalForce(sys->particles, Vec3f(0, -0.0981f, 0)));

	sys->addConstraint(new RodConstraint(sys->particles[1], sys->particles[2], dist, {1, 2}));
    sys->addConstraint(new CircularWireConstraint(sys->particles[0], center, dist, {0}));
    return sys;
}

System* SystemBuilder::initCloth() {
    System* sys = new System(new Midpoint());

    const int gSize = 12;

    // Initialize particles
    for (float x = -0.5f; x < 0.5f; x += 1.0f / gSize) {
        for (float y = 0.5f; y > -0.5f; y -= 1.0f / gSize) {
            sys->addParticle(new Particle(Vec3f(x, y, 0.0f), 0.5f));
        }
    }

    // Add gravity and drag to all particles
    sys->addForce(new DirectionalForce(sys->particles, Vec3f(0, -0.0098f, 0)));
    sys->addForce(new DirectionalForce(sys->particles, Vec3f(0, 0, -0.01f)));
    sys->addForce(new DragForce(sys->particles, 0.5f));

    for (int x = 0; x < gSize - 1; x++) {
        for (int y = 0; y < gSize; y++) {
            sys->addForce(new SpringForce(sys->particles[x + y * gSize],
                                          sys->particles[x + 1 + y * gSize],
                                          1.0f / gSize, 0.6, 0.5));
        }
    }

    for (int x = 0; x < gSize; x++) {
        for (int y = 0; y < gSize - 1; y++) {
            sys->addForce(new SpringForce(sys->particles[x + y * gSize],
                                          sys->particles[x + (y + 1) * gSize],
                                          1.0f / gSize, 0.6, 0.5));
        }
    }

    for (int x = 0; x < gSize - 1; x++) {
        for (int y = 0; y < gSize - 1; y++) {
            sys->addForce(new SpringForce(sys->particles[x + y * gSize],
                                          sys->particles[x + 1 + (y + 1) * gSize],
                                          sqrt(2 * pow(1.0f / gSize, 2)), 1.0, 0.5));
        }
    }



    for (int x = 0; x < gSize - 1; x++) {
        for (int y = 1; y < gSize; y++) {
            sys->addForce(new SpringForce(sys->particles[x + y * gSize],
                                          sys->particles[x + 1 + (y - 1) * gSize],
                                          sqrt(2 * pow(1.0f / gSize, 2)), 1.0, 0.5));
        }
    }

    sys->addConstraint(new CircularWireConstraint(sys->particles[0], Vec3f(-0.5f, 0.6f, 0), 0.1f, {0}));
    sys->addConstraint(new CircularWireConstraint(sys->particles[gSize*gSize - gSize], Vec3f(0.45f, 0.6f, 0), 0.1f, {gSize*gSize-gSize}));
    return sys;
}