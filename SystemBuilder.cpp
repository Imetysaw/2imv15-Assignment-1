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
#include "solvers/RungeKutta.h"

System* SystemBuilder::get(AvailableSystems s) {
    switch (s) {
        case BASIC:
            return initBasic();
        case CLOTH:
            return initCloth();
        case HAIR:
            return initHair();
    }
    return NULL;
}

System* SystemBuilder::initBasic()
{
    System* sys = new System(new Euler());

    const float dist = 0.5f;
    const Vec3f center(0.0, 0.0, 0.0);
    const Vec3f offset(dist, 0.0, 0.0);

    sys->addParticle(new Particle(center + offset, 1.0f, 0));
    sys->addParticle(new Particle(center + offset * 2, 1.0f, 1));
    sys->addParticle(new Particle(center + offset * 3, 1.0f, 2));
    sys->addParticle(new Particle(center + offset * 3, 1.0f, 3));
    sys->addParticle(new Particle(center + offset * 4, 1.0f, 4));
    sys->addParticle(new Particle(center + offset * 4, 1.0f, 5));

    sys->addForce(new DragForce(sys->particles, 0.5f));
    sys->addForce(new SpringForce(sys->particles[0], sys->particles[1], dist, 150.f, 1.5f));
    sys->addForce(new SpringForce(sys->particles[2], sys->particles[4], dist, 150.f, 1.5f));
    sys->addForce(new SpringForce(sys->particles[3], sys->particles[5], dist, 50.f, 1.5f));
    sys->addForce(new DirectionalForce(sys->particles, Vec3f(0, -9.81f, 0)));

    sys->addConstraint(new RodConstraint(sys->particles[1], sys->particles[2], dist));
    sys->addConstraint(new RodConstraint(sys->particles[1], sys->particles[3], dist));
    sys->addConstraint(new CircularWireConstraint(sys->particles[0], center, dist));

    return sys;
}

System* SystemBuilder::initCloth() {
    System* sys = new System(new Euler());

    const int xSize = 10, ySize = 26;
    const float deltaX = 2.0f/xSize, deltaY = 3.0f/ySize;
    int pindex = 0;
    // Initialize particles
    for (int y = 0; y < ySize; y++) {
        for (int x = 0; x < xSize; x++) {
            sys->addParticle(new Particle(Vec3f(-0.5f + x * deltaX, 0.5f - y * deltaY, deltaY * y), 0.2f, pindex));
            pindex++;
        }
    }

    // Add gravity and drag to all particles
    sys->addForce(new DirectionalForce(sys->particles, Vec3f(0, -9.81f, 0)));
    sys->addForce(new DragForce(sys->particles, 0.5f));

    float spr = 120.0f;
    float dmp = 1.5f;

    for (int y = 0; y < ySize; y++) {
        for (int x = 0; x < xSize - 1; x++) {
            sys->addForce(new SpringForce(sys->particles[x + y * xSize],
                                          sys->particles[x + 1 + y * xSize],
                                          deltaX, spr, dmp));
        }
    }

    for (int y = 0; y < ySize - 1; y++) {
        for (int x = 0; x < xSize; x++) {
            sys->addForce(new SpringForce(sys->particles[x + y * xSize],
                                          sys->particles[x + (y + 1) * xSize],
                                          deltaY, spr, dmp));
        }
    }

    for (int y = 0; y < ySize - 1; y++) {
        for (int x = 0; x < xSize - 1; x++) {
            sys->addForce(new SpringForce(sys->particles[x + y * xSize],
                                          sys->particles[x + 1 + (y + 1) * xSize],
                                          sqrt(pow(deltaX, 2) + pow(deltaY, 2)), spr, dmp));
        }
    }



    for (int y = 0; y < ySize - 1; y++) {
        for (int x = 1; x < xSize; x++) {
            sys->addForce(new SpringForce(sys->particles[x + y * xSize],
                                          sys->particles[x - 1 + (y + 1) * xSize],
                                          sqrt(pow(deltaX, 2) + pow(deltaY, 2)), spr, dmp));
        }
    }

//    sys->addConstraint(new CircularWireConstraint(sys->particles[0],
//                                                  sys->particles[0]->startPos + Vec3f(0.f, 0.05f, 0.f),
//                                                  0.05f, {0}));
    float r = 0.05f;
    sys->addConstraint(new CircularWireConstraint(sys->particles[0], //ySize/2 * xSize],
                                                  sys->particles[0]->startPos + Vec3f(-r, 0.f, 0.f),
                                                  r));
    sys->addConstraint(new CircularWireConstraint(sys->particles[xSize-1],
                                                  sys->particles[xSize-1]->startPos + Vec3f(0.f, r, 0.f),
                                                  r));
    return sys;
}


System* SystemBuilder::initHair() {
    System* sys = new System(new Euler());

    float particleDist = 0.01;
    int parCount = 50;

    for (int i = 0; i < parCount; i++) {
        sys->addParticle(new Particle(Vec3f(0.0f, -i * particleDist / 2, 0.0f), 0.2f, i));
    }

    for (int i = 0; i < parCount - 1; i++) {
        sys->addForce(new SpringForce(
                sys->particles[i],
                sys->particles[i + 1],
                particleDist, 0.1f, 0.1f
        ));
    }


    return sys;
}