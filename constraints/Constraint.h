//
// Created by Ruud Andriessen on 03/05/2017.
//

#ifndef PARTICLETOY_CONTRAINT_H
#define PARTICLETOY_CONTRAINT_H

#include <vector>

using namespace std;

class Constraint {
public:
    Constraint(vector<Particle*> particles) : particles(particles) {}
    virtual void draw() = 0;

    virtual float C() = 0;
    virtual float Cd() = 0;
    virtual vector<Vec3f> j() = 0;
    virtual vector<Vec3f> jd() = 0;

    vector<Particle*> affects() { return particles; };
protected:
    vector<Particle*> particles;
};


#endif //PARTICLETOY_CONTRAINT_H
