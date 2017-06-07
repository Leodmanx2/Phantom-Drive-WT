#include "Light.hpp"

PointLight::PointLight(const glm::vec3& pos,
                       const glm::vec3& clr,
                       float            its,
                       float            rad)
  : position(pos), color(clr), intensity(its), radius(rad) {}

SpotLight::SpotLight(const glm::vec3& pos,
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

DirectionLight::DirectionLight(const glm::vec3& dir,
                               const glm::vec3& clr,
                               float            its)
  : direction(dir), color(clr), intensity(its) {}
