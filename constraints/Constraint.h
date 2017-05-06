//
// Created by Ruud Andriessen on 03/05/2017.
//

#ifndef PARTICLETOY_CONTRAINT_H
#define PARTICLETOY_CONTRAINT_H

#include <vector>

using namespace std;

class Constraint {
public:
    Constraint(vector<int> indices): pIndices(indices) {}
    virtual void draw() = 0;

    virtual float C() = 0;
    virtual float Cd() = 0;
    virtual vector<Vec3f> j() = 0;
    virtual vector<Vec3f> jd() = 0;

    vector<int> indices() { return pIndices; };
private:
    vector<int> pIndices;
};


#endif //PARTICLETOY_CONTRAINT_H
