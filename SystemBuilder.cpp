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
#include "forces/AngularSpringForce.h"

System* SystemBuilder::get(AvailableSystems s) {
    switch (s) {
        System* sys;
        case BASIC:
            sys = initBasic();
            sys->type = BASIC;
            return sys;
        case CLOTH:
            sys = initCloth();
            sys->type = CLOTH;
            return sys;
        case HAIR:
            sys = initHair();
            sys->type = HAIR;
            return sys;
    }
    return NULL;
}

System* SystemBuilder::initBasic()
{
    System* sys = new System(new Euler(Euler::SEMI));

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
    System* sys = new System(new Euler(Euler::SEMI));

    const int xSize = 4, ySize = 4;
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

    float r = 0.05f;
    sys->addConstraint(new CircularWireConstraint(sys->particles[0],
                                                  sys->particles[0]->startPos + Vec3f(0.f, 0.05f, 0.f),
                                                  r));
    sys->addConstraint(new CircularWireConstraint(sys->particles[ySize/2 * xSize],
                                                  sys->particles[ySize/2 * xSize]->startPos + Vec3f(-r, 0.f, 0.f),
                                                  r));
    sys->addConstraint(new CircularWireConstraint(sys->particles[xSize-1],
                                                  sys->particles[xSize-1]->startPos + Vec3f(0.f, r, 0.f),
                                                  r));
    return sys;
}


System* SystemBuilder::initHair() {
    System* sys = new System(new Euler(Euler::SEMI));

    const int ySize = 10;
    const float deltaY = 3.0f/ySize;
    const int numHairs = 8;


    for (int k = 0; k < numHairs; k++) {
        // Initialize particles
        for (int y = 0; y < ySize; y++) {
            sys->addParticle(new Particle(Vec3f(-0.5f + 0.03f*k, 0.5f - y * deltaY, deltaY * y), 0.2f, k * ySize + y));
        }

        float spr = 120.0f;
        float dmp = 1.5f;


        for (int y = 0; y < ySize - 1; y++) {
            sys->addForce(new SpringForce(sys->particles[k * ySize + y],
                                          sys->particles[k * ySize + y + 1],
                                          deltaY, spr, dmp));
        }

        for (int y = 2; y < ySize - 2; y++) {
            sys->addForce(new AngularSpringForce(sys->particles[k * ySize + y],
                                                 sys->particles[k * ySize + y + 1],
                                                 sys->particles[k * ySize + y + 2],
                                                 2.5f, 20.f, dmp));
        }

        float r = 0.05f;
        sys->addConstraint(new CircularWireConstraint(sys->particles[k * ySize],
                                                      sys->particles[k * ySize]->startPos + Vec3f(-r, 0.f, 0.f),
                                                      r));
    }


    // Add gravity and drag to all particles
    sys->addForce(new DirectionalForce(sys->particles, Vec3f(0, -9.81f, 0)));
    sys->addForce(new DragForce(sys->particles, 0.5f));

    return sys;
}