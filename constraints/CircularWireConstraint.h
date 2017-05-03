#pragma once

#include "../Particle.h"
#include "Constraint.h"

class CircularWireConstraint : public Constraint {
 public:
  CircularWireConstraint(Particle *p, const Vec3f & center, const double radius);

  void draw() override;

 private:

  Particle * const m_p;
  Vec3f const m_center;
  double const m_radius;
};
