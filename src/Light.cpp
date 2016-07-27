#include "Light.hpp"

PointLight::PointLight(glm::vec3 pos, glm::vec3 clr, float its, float rad)
  : position(pos), color(clr), intensity(its), radius(rad) {}

SpotLight::SpotLight(
  glm::vec3 pos, glm::vec3 dir, glm::vec3 clr, float its, float agl, float rad)
  : position(pos)
  , direction(dir)
  , color(clr)
  , intensity(its)
  , angle(agl)
  , radius(rad) {}

DirectionLight::DirectionLight(glm::vec3 dir, glm::vec3 clr, float its)
  : direction(dir), color(clr), intensity(its) {}
