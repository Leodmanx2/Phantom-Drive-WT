#include "Light.hpp"

Light::Light(const glm::vec3& pos,
             const glm::vec3& dir,
             const glm::vec3& clr,
             float            its,
             float            agl,
             float            rad)
  : position(pos)
  , direction(dir)
  , color(clr)
  , intensity(its)
  , angle(agl)
  , radius(rad) {}
